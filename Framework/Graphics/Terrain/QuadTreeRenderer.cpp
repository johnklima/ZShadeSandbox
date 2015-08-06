#include "afxHeader.h"
#include "QuadTreeRenderer.h"
#include "CGlobal.h"
#include "TextureManager.h"
#include "MaterialManager.h"
using ZShadeSandboxTerrain::QuadTreeRenderer;
//================================================================================================================
ID3D11ShaderResourceView* QuadTreeRenderer::mBlendMapSRV = 0;
//================================================================================================================
QuadTreeRenderer::QuadTreeRenderer(D3D* d3d, ZShadeSandboxTerrain::QuadTreeMesh* qtm)
:	m_d3d(d3d)
,	m_quadtreeMesh(qtm)
,	bShowBoundingBox(false)
,	bShowRootBoundingBox(false)
,   bRenderFullMap(false)
,   bRenderExtraRenderDist(false)
,   bRenderSize(1)
,   bRootContainsCamera(true)
,   bWireframe(false)
{
	m_EngineOptions = d3d->GetEngineOptions();
	
	m_TerrainTessellationQuadShadowShader = new TerrainTessellationQuadShadowShader(m_d3d);
	m_TerrainTessellationTriShadowShader = new TerrainTessellationTriShadowShader(m_d3d);
	m_TerrainTessellationQuadShader = new TerrainTessellationQuadShader(m_d3d);
	m_TerrainTessellationTriShader = new TerrainTessellationTriShader(m_d3d);
	m_TextureShader = new MaterialShader(d3d);
	
	mShadowMapTarget = new RenderTarget2D(d3d);
	mShadowMapTarget->Initialize(qtm->GetQuadTree()->MapSize(), qtm->GetQuadTree()->MapSize(), DXGI_FORMAT_R32G32B32A32_FLOAT);

	mSSAOTarget = new RenderTarget2D(d3d);
	mSSAOTarget->Initialize(qtm->GetQuadTree()->MapSize(), qtm->GetQuadTree()->MapSize(), DXGI_FORMAT_R32G32B32A32_FLOAT);

	//Load the world matrix
	XMStoreFloat4x4(&mWorld, XMMatrixIdentity());

	bTextureUpdated = false;

	int size = m_quadtreeMesh->GetQuadTree()->MapSize();

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			mBlendMapValues.push_back(XMFLOAT4(0, 0, 0, 0));
		}
	}

	BuildBlendMapSRV();
}
//================================================================================================================
XMMATRIX QuadTreeRenderer::GetWorld()
{
	return XMLoadFloat4x4(&mWorld);
}
//================================================================================================================
void QuadTreeRenderer::RenderSSAO(Camera* pCamera, LightCamera* camera, ZShadeSandboxTerrain::TerrainShadingConst tsc)
{
	// Currently only running DirectX 11
	if (!m_EngineOptions->m_d3dVersion == DIRECTX11)
		return;

	mSSAOTarget->SetRenderTarget();
	{
		mSSAOTarget->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);

		camera->Update();// you probably should call camera.update once per frame in somewhere like GraphicsEngine.Update() ...

		RenderSSAO(m_quadtreeMesh->GetRootNode(), tsc, pCamera, camera);
	}
}
//================================================================================================================
void QuadTreeRenderer::RenderSSAO(ZShadeSandboxTerrain::QMeshNode* node, ZShadeSandboxTerrain::TerrainShadingConst tsc, Camera* pCamera, LightCamera* camera)
{
	if (node == 0) return;

	bool result = pCamera->ViewingFrustum()->ContainsAABB(node->boundary.Center(), node->boundary.Extents());

	// If it can't be seen then none of its children can either so don't continue down the tree, this is where the speed is gained.
	if (!result)
	{
		if (node->parent != NULL)
		{
			// If the camera is in its parent node then this node can be rendered if it is a leaf
			if (node->parent->bContainsCamera)
			{
				tsc.g_FrustumCull = 0;
				// The camera is inside this node
				node->bContainsCamera = true;
			}
			else
			{
				// The camera is not in this node
				node->bContainsCamera = false;
				tsc.g_FrustumCull = 1;
				return;
			}
		}
		else
		{
			// Camera is off the map, this is the root node
			tsc.g_FrustumCull = 0;
		}
	}
	else
	{
		tsc.g_FrustumCull = 0;

		// We want the first node that is not root to contain the camera so that we only render the node the camera is in
		if (node->parent != NULL)
		{
			// The camera is inside this node
			node->bContainsCamera = true;
		}
	}

	// Still want to see the terrain even though we are no longer on it
	if (node->parent == NULL)
	{
		if (!node->boundary.ContainsPoint3DOmitY(pCamera->Position()))
		{
			tsc.g_FrustumCull = 0;
			// The camera is inside this node
			node->bContainsCamera = true;
			bRootContainsCamera = false;
		}
		else
			bRootContainsCamera = true;
	}
	else
	{
		if (node->boundary.ContainsPoint3DOmitY(pCamera->Position()))
		{
			tsc.g_FrustumCull = 0;
			// The camera is inside this node
			node->bContainsCamera = true;
		}
		else
		{
			if (bRootContainsCamera && !bRenderFullMap)
			{
				if (node->parent != NULL || !node->bContainsCamera)
				{
					const ZShadeSandboxTerrain::QuadTree* qtree = m_quadtreeMesh->GetQuadTree();
					XMFLOAT3 eye(pCamera->Position().x, 0.0f, pCamera->Position().z);
					float dist = node->boundary.DistanceToPoint3D(eye);
					float ex = 1.0f;

					if (bRenderExtraRenderDist)
					{
						ex = 2.0f;
					}

					if (dist > ((qtree->MapSize() / bRenderSize) * m_quadtreeMesh->TerrainZScale()) * ex)
					{
						// The camera is not in this node, don't render its children
						node->bContainsCamera = false;
						tsc.g_FrustumCull = 1;
						return;
					}
				}
			}
		}
	}

	// Perform rendering of this node
	if (node->type == ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		RenderMeshBuffers(node);

		if (m_quadtreeMesh->Tessellate())
		{
			switch (m_quadtreeMesh->GetRenderPrimitive())
			{
			case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
			{
				/*m_TerrainTessellationQuadSSAOShader->SetWireframe(bWireframe);
				m_TerrainTessellationQuadSSAOShader->UseCustomWorld(true);
				m_TerrainTessellationQuadSSAOShader->SetCustomWorld(ZShadeSandboxMath::ZMath::GMathFM(mWorld));*/
				m_TerrainTessellationQuadSSAOShader->Render(node->m_index_count, pCamera, camera, tsc, m_quadtreeMesh->GetHeightMapSRV());
			}
			break;
			case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
			{
				//m_TerrainTessellationTriShadowShader->SetWireframe(bWireframe);
				//m_TerrainTessellationTriShadowShader->UseCustomWorld(true);
				//m_TerrainTessellationTriShadowShader->SetCustomWorld(GMathFM(mWorld));
				//m_TerrainTessellationTriShadowShader->Render(node->m_index_count, camera, tsc, m_quadtreeMesh->GetHeightMapSRV());
			}
			break;
			}
		}
		else
		{
			// No Tessellation so just use regular material shader (no shadow yet)
			//m_TextureShader->UseCustomWorld(true);
			//m_TextureShader->SetCustomWorld(GMathFM(mWorld));
			//m_TextureShader->Render11(node->m_index_count, XMMath4(0, 0, 0, 0), pCamera, m_quadtreeMesh->GetMaterial());
		}
	}
	else
	{
		//Render the children
		RenderShadowMap(node->children[0], tsc, pCamera, camera);
		RenderShadowMap(node->children[1], tsc, pCamera, camera);
		RenderShadowMap(node->children[2], tsc, pCamera, camera);
		RenderShadowMap(node->children[3], tsc, pCamera, camera);
	}
}
//================================================================================================================
void QuadTreeRenderer::RenderShadowMap(Camera* pCamera, LightCamera* camera, ZShadeSandboxTerrain::TerrainShadingConst tsc)
{
	// Currently only running DirectX 11
	if (!m_EngineOptions->m_d3dVersion == DIRECTX11)
		return;
	
	mShadowMapTarget->SetRenderTarget();
	{
		mShadowMapTarget->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);

		camera->Update();

		RenderShadowMap(m_quadtreeMesh->GetRootNode(), tsc, pCamera, camera);
	}
}
//================================================================================================================
void QuadTreeRenderer::RenderShadowMap(ZShadeSandboxTerrain::QMeshNode* node, ZShadeSandboxTerrain::TerrainShadingConst tsc, Camera* pCamera, LightCamera* camera)
{
	if (node == 0) return;
	
	// Check to see if the node can be viewed, height doesn't matter in a quad tree.
	bool result = pCamera->ViewingFrustum()->ContainsCube(node->center, (node->fWidth / 2.0f));
	
	// Check to see if the node can be viewed, height doesn't matter in a quad tree.
	if (!result)
		result = pCamera->ViewingFrustum()->ContainsAABB(node->boundary.vMin, node->boundary.vMax);
	
	// If it can't be seen then none of its children can either so don't continue down the tree, this is where the speed is gained.
	if(!result)
	{
		if (node->parent != NULL)
		{
			// If the camera is in its parent node then this node can be rendered if it is a leaf
			if (node->parent->bContainsCamera)
			{
				tsc.g_FrustumCull = 0;
				// The camera is inside this node
				node->bContainsCamera = true;
			}
			else
			{
				// The camera is not in this node
				node->bContainsCamera = false;
				tsc.g_FrustumCull = 1;
				return;
			}
		}
		else
		{
			// Camera is off the map, this is the root node
			tsc.g_FrustumCull = 0;
		}
	}
	else
	{
		tsc.g_FrustumCull = 0;
		
		// We want the first node that is not root to contain the camera so that we only render the node the camera is in
		if (node->parent != NULL)
		{
			// The camera is inside this node
			node->bContainsCamera = true;
		}
	}

	// Still want to see the terrain even though we are no longer on it
	if (node->parent == NULL)
	{
		if (!node->boundary.ContainsPoint3DOmitY(pCamera->Position()))
		{
			tsc.g_FrustumCull = 0;
			// The camera is inside this node
			node->bContainsCamera = true;
			bRootContainsCamera = false;
		}
		else
			bRootContainsCamera = true;
	}
	else
	{
		if (node->boundary.ContainsPoint3DOmitY(pCamera->Position()))
		{
			tsc.g_FrustumCull = 0;
			// The camera is inside this node
			node->bContainsCamera = true;
		}
		else
		{
			if (bRootContainsCamera && !bRenderFullMap)
			{
				if (node->parent != NULL || !node->bContainsCamera)
				{
					const ZShadeSandboxTerrain::QuadTree* qtree = m_quadtreeMesh->GetQuadTree();
					XMFLOAT3 eye(pCamera->Position().x, 0.0f, pCamera->Position().z);
					float dist = node->boundary.DistanceToPoint3D(eye);
					float ex = 1.0f;
					
					if (bRenderExtraRenderDist)
					{
						ex = 2.0f;
					}
					
					if (dist > ((qtree->MapSize() / bRenderSize) * m_quadtreeMesh->TerrainZScale()) * ex)
					{
						// The camera is not in this node, don't render its children
						node->bContainsCamera = false;
						tsc.g_FrustumCull = 1;
						return;
					}
				}
			}
		}
	}

	// Perform rendering of this node
	if (node->type == ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		RenderMeshBuffers(node);

		if (m_quadtreeMesh->Tessellate())
		{
			switch (m_quadtreeMesh->GetRenderPrimitive())
			{
				case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
				{
					/*m_TerrainTessellationQuadShadowShader->SetWireframe(bWireframe);
					m_TerrainTessellationQuadShadowShader->UseCustomWorld(true);
					m_TerrainTessellationQuadShadowShader->SetCustomWorld(ZShadeSandboxMath::ZMath::GMathFM(mWorld));*/
					m_TerrainTessellationQuadShadowShader->Render(node->m_index_count, camera, tsc, m_quadtreeMesh->GetHeightMapSRV());
				}
				break;
				case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
				{
					/*m_TerrainTessellationTriShadowShader->SetWireframe(bWireframe);
					m_TerrainTessellationTriShadowShader->UseCustomWorld(true);
					m_TerrainTessellationTriShadowShader->SetCustomWorld(ZShadeSandboxMath::ZMath::GMathFM(mWorld));*/
					m_TerrainTessellationTriShadowShader->Render(node->m_index_count, camera, tsc, m_quadtreeMesh->GetHeightMapSRV());
				}
				break;
			}
		}
		else
		{
			// No Tessellation so just use regular material shader (no shadow yet)
			//m_TextureShader->UseCustomWorld(true);
			//m_TextureShader->SetCustomWorld(GMathFM(mWorld));
			//m_TextureShader->Render11(node->m_index_count, XMMath4(0, 0, 0, 0), pCamera, m_quadtreeMesh->GetMaterial());
		}
	}
	else
	{
		//Render the children
		RenderShadowMap(node->children[0], tsc, pCamera, camera);
		RenderShadowMap(node->children[1], tsc, pCamera, camera);
		RenderShadowMap(node->children[2], tsc, pCamera, camera);
		RenderShadowMap(node->children[3], tsc, pCamera, camera);
	}
}
//================================================================================================================
void QuadTreeRenderer::Render(Camera* pCamera, ZShadeSandboxLighting::Light* light, ZShadeSandboxTerrain::TerrainShadingConst tsc)
{
	// Currently only running DirectX 11
	if (!m_EngineOptions->m_d3dVersion == DIRECTX11)
		return;
	
	// Need to fix the size of the bounding box with the terrain
	if (bShowBoundingBox)
	{
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.light = light;
		mrp.camera = pCamera;
		//if (bReflect)
		//{
		//	mrp.bReflection = true;
		//	mrp.clipplane = tsc.g_ClipPlane;
		//}
		//m_quadtreeMesh->GetRootBoxMesh()->SetWireframe(true);
		m_quadtreeMesh->GetRootBoxMesh()->Render(mrp);
	}
	
	if (!bTextureUpdated)
	{
		m_quadtreeMesh->GetMaterial()->SetMaterialBlendMapTexture(mBlendMapSRV);
		bTextureUpdated = true;
	}

	/*ZShadeSandboxLighting::ShaderMaterial* mat = m_quadtreeMesh->GetMaterial();

	tsc.g_MaterialDiffuseColor = mat->vDiffuseColor;
	tsc.g_MaterialAmbientColor = mat->vAmbientColor;
	tsc.g_MaterialSpecularPower = mat->fSpecularPower;
	tsc.g_MaterialSpecularIntensity = mat->fSpecularIntensity;

	for (int i = 0; i < mat->TextureCount(); i++)
	{
		switch (mat->GetMaterialTextureType(i))
		{
			case ZShadeSandboxLighting::EMaterialTextureType::eDiffuseArray:
				diffuseArray = mat->GetTexture(i);
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eNormal:
				normalmap = mat->GetTexture(i);
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eBlend:
				blendmap = mat->GetTexture(i);
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eDetail:
				detailmap = mat->GetTexture(i);
			break;
		}
	}*/

	Render(m_quadtreeMesh->GetRootNode(), pCamera, light, tsc);
}
//================================================================================================================
void QuadTreeRenderer::Render(ZShadeSandboxTerrain::QMeshNode* node, Camera* pCamera, ZShadeSandboxLighting::Light* light, ZShadeSandboxTerrain::TerrainShadingConst tsc)
{
	if (node == 0) return;
	
	bool result = pCamera->ViewingFrustum()->ContainsAABB(node->boundary.Center(), node->boundary.Extents());
	
	// If it can't be seen then none of its children can either so don't continue down the tree, this is where the speed is gained.
	if(!result)
	{
		if (node->parent != NULL)
		{
			// If the camera is in its parent node then this node can be rendered if it is a leaf
			if (node->parent->bContainsCamera)
			{
				tsc.g_FrustumCull = 0;
				// The camera is inside this node
				node->bContainsCamera = true;
			}
			else
			{
				// The camera is not in this node
				node->bContainsCamera = false;
				tsc.g_FrustumCull = 1;
				return;
			}
		}
		else
		{
			// Camera is off the map, this is the root node
			tsc.g_FrustumCull = 0;
		}
	}
	else
	{
		tsc.g_FrustumCull = 0;
		
		// We want the first node that is not root to contain the camera so that we only render the node the camera is in
		if (node->parent != NULL)
		{
			// The camera is inside this node
			node->bContainsCamera = true;
		}
	}

	// Still want to see the terrain even though we are no longer on it
	if (node->parent == NULL)
	{
		if (!node->boundary.ContainsPoint3DOmitY(pCamera->Position()))
		{
			tsc.g_FrustumCull = 0;
			// The camera is inside this node
			node->bContainsCamera = true;
			bRootContainsCamera = false;
		}
		else
			bRootContainsCamera = true;
	}
	else
	{
		if (node->boundary.ContainsPoint3DOmitY(pCamera->Position()))
		{
			tsc.g_FrustumCull = 0;
			// The camera is inside this node
			node->bContainsCamera = true;
		}
		else
		{
			if (bRootContainsCamera && !bRenderFullMap)
			{
				if (node->parent != NULL || !node->bContainsCamera)
				{
					const ZShadeSandboxTerrain::QuadTree* qtree = m_quadtreeMesh->GetQuadTree();
					XMFLOAT3 eye(pCamera->Position().x, 0.0f, pCamera->Position().z);
					float dist = node->boundary.DistanceToPoint3D(eye);
					float ex = 1.0f;
					
					if (bRenderExtraRenderDist)
					{
						ex = 2.0f;
					}
					
					if (dist > ((qtree->MapSize() / bRenderSize) * m_quadtreeMesh->TerrainZScale()) * ex)
					{
						// The camera is not in this node, don't render its children
						node->bContainsCamera = false;
						tsc.g_FrustumCull = 1;
						return;
					}
				}
			}
		}
	}

	if (tsc.g_FrustumCull == 0)
	{
		if (bShowBoundingBox && !bShowRootBoundingBox)
		{
			ZShadeSandboxMesh::MeshRenderParameters mrp;
			mrp.light = light;
			mrp.camera = pCamera;
			// Reflections cause a performance issue here
			//if (bReflect)
			//{
			//	mrp.bReflection = true;
			//	mrp.clipplane = tsc.g_ClipPlane;
			//}
			//node->box_mesh->SetWireframe(true);
			node->box_mesh->Render(mrp);
		}
	}
	
	// Perform rendering of this node
	if (node->type == ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		RenderMeshBuffers(node);

		if (m_quadtreeMesh->Tessellate())
		{
			switch (m_quadtreeMesh->GetRenderPrimitive())
			{
				case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
				{
					m_TerrainTessellationQuadShader->Wireframe() = bWireframe;
					m_TerrainTessellationQuadShader->Render(
						node->m_index_count,
						pCamera,
						light,
						tsc,
						m_quadtreeMesh->GetHeightMapSRV(),
						m_quadtreeMesh->GetMaterial()
					);
				}
				break;
				case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
				{
					/*m_TerrainTessellationTriShader->UseCustomWorld(true);
					m_TerrainTessellationTriShader->SetCustomWorld(ZShadeSandboxMath::ZMath::GMathFM(mWorld));
					m_TerrainTessellationTriShader->Render(node->m_index_count, pCamera, lightcamera, bReflect, tsc,
						diffuseArray->getTexture11(), blendmap->getTexture11(),
						m_quadtreeMesh->GetHeightMapSRV(), normalmap->getTexture11(),
						mShadowMapTarget->SRView);*/
				}
				break;
			}
		}
		else
		{
			// No Tessellation so just use regular material shader (no shadow yet)
			//m_TextureShader->UseCustomWorld(true);
			//m_TextureShader->SetCustomWorld(ZShadeSandboxMath::ZMath::GMathFM(mWorld));
			//m_TextureShader->Render11(node->m_index_count, ZShadeSandboxMath::XMMath4(0, 0, 0, 0), pCamera, lightcamera, m_quadtreeMesh->GetMaterial());
		}
	}
	else
	{
		//Render the children
		Render(node->children[0], pCamera, light, tsc);
		Render(node->children[1], pCamera, light, tsc);
		Render(node->children[2], pCamera, light, tsc);
		Render(node->children[3], pCamera, light, tsc);
	}
}
//================================================================================================================
void QuadTreeRenderer::RenderMeshBuffers(ZShadeSandboxTerrain::QMeshNode* node)
{
	unsigned int stride;
	unsigned int offset;

	//Set the vertex buffer stride and offset
	stride = sizeof(ZShadeSandboxMesh::VertexTex);
	offset = 0;

	m_d3d->GetDeviceContext()->IASetVertexBuffers(0, 1, &node->m_vertex_buffer11, &stride, &offset);

	//Set the index buffer to active in the input assembler so it can be rendered
	m_d3d->GetDeviceContext()->IASetIndexBuffer(node->m_index_buffer11, DXGI_FORMAT_R32_UINT, 0);
	
	switch (m_quadtreeMesh->GetRenderPrimitive())
	{
		case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
			m_d3d->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		break;
		case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
			m_d3d->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
		break;
		case ZShadeSandboxMesh::ERenderType::eTriangleStrip:
			m_d3d->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		break;
	}
}
//================================================================================================================
void QuadTreeRenderer::BuildBlendMapSRV()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = m_quadtreeMesh->GetQuadTree()->MapSize();
	texDesc.Height = m_quadtreeMesh->GetQuadTree()->MapSize();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &mBlendMapValues[0];
	data.SysMemPitch = m_quadtreeMesh->GetQuadTree()->MapSize() * sizeof(XMFLOAT4);
	data.SysMemSlicePitch = 0;

	ID3D11Texture2D* hmapTex = 0;
	m_d3d->GetDevice11()->CreateTexture2D(&texDesc, &data, &hmapTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	m_d3d->GetDevice11()->CreateShaderResourceView(hmapTex, &srvDesc, &mBlendMapSRV);

	// SRV saves reference.
	SAFE_RELEASE(hmapTex);
}
//================================================================================================================