#include "OceanSurface.h"
#include "OceanParameters.h"
#include "TextureManager.h"
#include "CGlobal.h"
#include "Shader.h"
#include "ShaderCompiler.h"

//==============================================================================================================================
//==============================================================================================================================
OceanSurface::OceanSurface(D3D* d3d, OceanParameters ocean_params)
:   mD3DSystem(d3d)
,   fTime(0)
{
	mOceanParams = new OceanParameters();
	mOceanParams->g_WaveHeight = ocean_params.g_WaveHeight;
	mOceanParams->g_SpecularShininess = ocean_params.g_SpecularShininess;
	mOceanParams->g_LightDirection = ocean_params.g_LightDirection;
	mOceanParams->g_RefractionTint = ocean_params.g_RefractionTint;
	mOceanParams->g_DullColor = ocean_params.g_DullColor;
	mOceanParams->g_OceanSize = ocean_params.g_OceanSize;
	mOceanParams->g_PatchSize = ocean_params.g_PatchSize;
	mOceanParams->g_SeaLevel = ocean_params.g_SeaLevel;
	mOceanParams->g_MinDist = ocean_params.g_MinDist;
	mOceanParams->g_MaxDist = ocean_params.g_MaxDist;
	mOceanParams->g_MinTess = ocean_params.g_MinTess;
	mOceanParams->g_MaxTess = ocean_params.g_MaxTess;
	mOceanParams->g_WindDir = ocean_params.g_WindDir;
	mOceanParams->g_WaveAmplitude = ocean_params.g_WaveAmplitude;
	mOceanParams->g_WindSpeed = ocean_params.g_WindSpeed;
	mOceanParams->g_WindDependency = ocean_params.g_WindDependency;
	mOceanParams->g_TimeScale = ocean_params.g_TimeScale;
	mOceanParams->g_ChoppyScale = ocean_params.g_ChoppyScale;
	mOceanParams->g_PerlinSize = ocean_params.g_PerlinSize;
	mOceanParams->g_PerlinSpeed = ocean_params.g_PerlinSpeed;
	mOceanParams->g_PerlinAmplitude = ocean_params.g_PerlinAmplitude;
	mOceanParams->g_PerlinGradient = ocean_params.g_PerlinGradient;
	mOceanParams->g_PerlinOctave = ocean_params.g_PerlinOctave;
	mOceanParams->g_MapDimension = ocean_params.g_MapDimension;
	mOceanParams->g_PatchLength = ocean_params.g_PatchLength;

	Initialize();
}
//==============================================================================================================================
void OceanSurface::Initialize()
{
	
	mPatchTree = new OceanPatchTree(mD3DSystem, mOceanParams->g_OceanSize, mOceanParams->g_PatchSize, mOceanParams->g_SeaLevel);
	
	mSurfaceShader = new OceanSurfaceShader(mD3DSystem);
	mSurfaceShader->SetWaveMap0(TextureManager::Instance()->GetTexture("Textures\\wave0.dds"));
	mSurfaceShader->SetWaveMap1(TextureManager::Instance()->GetTexture("Textures\\wave1.dds"));
}
//==============================================================================================================================
void OceanSurface::SetReflectionMap(ID3D11ShaderResourceView* srv)
{
	mSurfaceShader->SetReflectionMap(srv);
}
//==============================================================================================================================
void OceanSurface::SetRefractionMap(ID3D11ShaderResourceView* srv)
{
	mSurfaceShader->SetRefractionMap(srv);
}
//==============================================================================================================================
void OceanSurface::SetWireframe(bool wire)
{
	mSurfaceShader->Wireframe() = wire;
}
//==============================================================================================================================
void OceanSurface::Update(float seaLevel, float dt)
{
	fTime += 0.005f * dt;
	mOceanParams->g_Time = fTime;
	mOceanParams->g_SeaLevel = seaLevel;
}
//==============================================================================================================================
void OceanSurface::Render(Camera* camera, float seaLevel, float dt)
{
	Render(camera, seaLevel, dt, mPatchTree->GetPatches());
}
//==============================================================================================================================
void OceanSurface::Render(Camera* camera, float seaLevel, float dt, OceanPatch* patch)
{
	if (patch == 0) return;

	// Check to see if the node can be viewed, height doesn't matter in a quad tree.
	bool result = camera->ViewingFrustum()->ContainsCube(patch->center, (patch->width / 2.0f));

	// Check to see if the node can be viewed, height doesn't matter in a quad tree.
	if (!result)
		result = camera->ViewingFrustum()->ContainsAABB(patch->boundary.vMin, patch->boundary.vMax);

	// If it can't be seen then none of its children can either so don't continue down the tree, this is where the speed is gained.
	if (!result)
	{
		if (patch->parent != NULL)
		{
			// If the camera is in its parent node then this node can be rendered if it is a leaf
			if (patch->parent->containsCamera)
			{
				iFrustumCull = 0;
				// The camera is inside this node
				patch->containsCamera = true;
			}
			else
			{
				// The camera is not in this node
				patch->containsCamera = false;
				iFrustumCull = 1;
				return;
			}
		}
		else
		{
			// Camera is off the map, this is the root node
			iFrustumCull = 0;
		}
	}
	else
	{
		iFrustumCull = 0;

		// We want the first node that is not root to contain the camera so that we only render the node the camera is in
		if (patch->parent != NULL)
		{
			// The camera is inside this node
			patch->containsCamera = true;
		}
	}

	// Still want to see the terrain even though we are no longer on it
	if (patch->parent == NULL)
	{
		if (!patch->boundary.ContainsPoint3DOmitY(camera->Position()))
		{
			iFrustumCull = 0;
			// The camera is inside this node
			patch->containsCamera = true;
			bRootContainsCamera = false;
		}
		else
			bRootContainsCamera = true;
	}
	else
	{
		if (patch->boundary.ContainsPoint3DOmitY(camera->Position()))
		{
			iFrustumCull = 0;
			// The camera is inside this node
			patch->containsCamera = true;
		}
		else
		{
			if (bRootContainsCamera)// && !bRenderFullMap)
			{
				if (patch->parent != NULL || !patch->containsCamera)
				{
					XMFLOAT3 eye(camera->Position().x, 0.0f, camera->Position().z);
					float dist = patch->boundary.DistanceToPoint3D(eye);
					float ex = 1.0f;

					//if (bRenderExtraRenderDist)
					{
						ex = 2.0f;
					}

					if (dist > ((mOceanParams->g_OceanSize / 1) * 1.0f) * ex)
					{
						// The camera is not in this node, don't render its children
						patch->containsCamera = false;
						iFrustumCull = 1;
						return;
					}
				}
			}
		}
	}

	if (patch->isLeaf)
	{
		Update(seaLevel, dt);
		
		unsigned int stride;
		unsigned int offset;
		
		//Set the vertex buffer stride and offset
		stride = sizeof(ZShadeSandboxMesh::VertexTex);
		offset = 0;

		mD3DSystem->GetDeviceContext()->IASetVertexBuffers(0, 1, &patch->mesh->vb, &stride, &offset);
		
		//Set the index buffer to active in the input assembler so it can be rendered
		mD3DSystem->GetDeviceContext()->IASetIndexBuffer(patch->mesh->ib, DXGI_FORMAT_R32_UINT, 0);
		
		mD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		
		//<JPK> ??? which one is "bottom left" and why does it not seem to make a difference ??? 
		XMFLOAT2 bottom_left = XMFLOAT2(patch->mesh->vertices.at(1).position.x, patch->mesh->vertices.at(1).position.z);
		
		// This will make it follow the camera
		//XMMATRIX world, scale, translate;
		//float GridSpacing = 1;
		//// scale to meet the grid spacing and translate only in exact steps of the grid spacing
		//scale = XMMatrixScaling(GridSpacing, GridSpacing, GridSpacing);
		//translate = XMMatrixTranslation((camera->Position().x / GridSpacing) * GridSpacing, mOceanParams->g_SeaLevel, (camera->Position().z / GridSpacing) * GridSpacing);
		//world = XMMatrixMultiply(XMMatrixIdentity(), scale);
		//world = XMMatrixMultiply(world, translate);

		mSurfaceShader->Render11(patch->mesh->indexCount, camera, iFrustumCull, XMMatrixIdentity(), *mOceanParams, bottom_left);
	}
	else
	{
		Render(camera, seaLevel, dt, patch->children[0]);
		Render(camera, seaLevel, dt, patch->children[1]);
		Render(camera, seaLevel, dt, patch->children[2]);
		Render(camera, seaLevel, dt, patch->children[3]);
	}
}
//==============================================================================================================================