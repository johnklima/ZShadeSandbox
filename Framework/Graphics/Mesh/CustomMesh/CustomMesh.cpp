#include "ZMath.h"
#include "CustomMesh.h"
#include "TextureManager.h"
#include "ZShadeMessageCenter.h"
#include "MeshUtil.h"
#include "Shaders.h"
#include "MaterialShader.h"
#include "ShadowMapBuildShader.h"
#include "MaterialGBufferShader.h"
#include "MaterialLightShader.h"
#include "MaterialTessellationShader.h"
#include "MaterialLightTessellationShader.h"
using ZShadeSandboxMesh::CustomMesh;
//===============================================================================================================================
CustomMesh::CustomMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, char* filename)
:   mD3DSystem(d3d)
,   m_Wireframe(false)
,   m_DisableIndexBuffer(false)
,   m_TranslateBillboard(false)
,   m_RenderShader(true)
,   mMeshParameters(mp)
{
	Init();
	
	mEngineOptions = d3d->GetEngineOptions();
	
	LoadFromFile(filename);
	
	CreateBuffers();
	
	mMaterial = mp.material;
}
//===============================================================================================================================
CustomMesh::CustomMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:	mD3DSystem(d3d)
,	m_Wireframe(false)
,	m_DisableIndexBuffer(false)
,	m_TranslateBillboard(false)
,   m_RenderShader(true)
,   mMeshParameters(mp)
{
	Init();

	mEngineOptions = d3d->GetEngineOptions();
	
	mMaterial = mp.material;
}
//===============================================================================================================================
CustomMesh::~CustomMesh()
{
	//SAFE_RELEASE( mTexture );
}
//===============================================================================================================================
void CustomMesh::LoadFromFile(char* filename)
{
	MeshData* md = MeshUtil::LoadMesh(filename, mMeshParameters.vertexType);

	mAttributes->mVertexCount = md->vertexCount;
	mAttributes->mIndexCount = md->indexCount;
	mAttributes->mIndices.resize(mAttributes->mIndexCount);

	mAttributes->mTriangleCount = mAttributes->mVertexCount / 3;

	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			mAttributes->mVerticesNormalTex.resize(mAttributes->mVertexCount);
			
			// Load the vertex array and index array with data.
			for (int i = 0; i < mAttributes->mVertexCount; i++)
			{
				mAttributes->mVerticesNormalTex[i].position = md->dataVNT[i].position;
				mAttributes->mVerticesNormalTex[i].normal = md->dataVNT[i].normal;
				mAttributes->mVerticesNormalTex[i].texture = md->dataVNT[i].texture;
				mAttributes->mIndices[i] = i;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
			mAttributes->mVerticesNormalTexTan.resize(mAttributes->mVertexCount);
			
			// Load the vertex array and index array with data.
			for (int i = 0; i < mAttributes->mVertexCount; i++)
			{
				mAttributes->mVerticesNormalTexTan[i].position = md->dataVNTT[i].position;
				mAttributes->mVerticesNormalTexTan[i].normal = md->dataVNTT[i].normal;
				mAttributes->mVerticesNormalTexTan[i].texture = md->dataVNTT[i].texture;
				mAttributes->mVerticesNormalTexTan[i].tangentU = md->dataVNTT[i].tangentU;
				mAttributes->mIndices[i] = i;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
			mAttributes->mVerticesPos.resize(mAttributes->mVertexCount);

			// Load the vertex array and index array with data.
			for (int i = 0; i < mAttributes->mVertexCount; i++)
			{
				mAttributes->mVerticesPos[i].position = md->dataPos[i].position;
				mAttributes->mIndices[i] = i;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			mAttributes->mVerticesTex.resize(mAttributes->mVertexCount);
			
			// Load the vertex array and index array with data.
			for (int i = 0; i < mAttributes->mVertexCount; i++)
			{
				mAttributes->mVerticesTex[i].position = md->dataTex[i].position;
				mAttributes->mVerticesTex[i].texture = md->dataTex[i].texture;
				mAttributes->mIndices[i] = i;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			mAttributes->mVerticesColor.resize(mAttributes->mVertexCount);
			
			// Load the vertex array and index array with data.
			for (int i = 0; i < mAttributes->mVertexCount; i++)
			{
				mAttributes->mVerticesColor[i].position = md->dataColor[i].position;
				mAttributes->mVerticesColor[i].color = md->dataColor[i].color;
				mAttributes->mIndices[i] = i;
			}
		}
		break;
	}
}
//===============================================================================================================================
void CustomMesh::InitializeMesh()
{
	if (mMeshParameters.bUseTessellation)
		InitTessellation();
	else
		Initialize();
}
//===============================================================================================================================
void CustomMesh::Init()
{
	mAttributes = new MeshAttributes();
	mAttributes->mD3D = mD3DSystem;

	mAttributes->mPosition = mMeshParameters.pos;
	mAttributes->mScale = mMeshParameters.scale;
	mAttributes->mRotation = mMeshParameters.rot;

	CreatePhysicsBody();
}
//===============================================================================================================================
void CustomMesh::RemoveTexture(int i)
{
	mMaterial->RemoveTexture(i);
}
//===============================================================================================================================
void CustomMesh::SetMaterialDiffuseTexture(ID3D11ShaderResourceView* texSRV)
{
	mMaterial->SetMaterialDiffuseTexture(texSRV);
}
//===============================================================================================================================
string CustomMesh::TextureName() const
{
	if (mMaterial->GetTexture(0, ZShadeSandboxLighting::EMaterialTextureType::eDiffuse))
		return mMaterial->GetTexture(0)->GetTextureName();
	return "";
}
//===============================================================================================================================
void CustomMesh::Update(float dt)
{
	if (mMeshParameters.rotationAxisX)
		mAttributes->mRotation.x += dt;
	
	if (mMeshParameters.rotationAxisY)
		mAttributes->mRotation.y += dt;
	
	if (mMeshParameters.rotationAxisZ)
		mAttributes->mRotation.z += dt;
}
//===============================================================================================================================
void CustomMesh::Render(ZShadeSandboxMesh::MeshRenderParameters rp)
{
	// Cannot do anything if there is no camera
	if (rp.camera == 0) return;
	
	//
	// Render the mesh buffers
	//
	
	if (rp.useInstancing)
		mAttributes->SetBuffersInstanced(rp.renderType);
	else
		mAttributes->SetBuffers(rp.renderType);
	
	//
	// Render the shader
	//
	
	if (rp.reflection)
		rp.view = rp.camera->ReflectionView4x4();
	else
		rp.view = rp.camera->View4x4();
	
	if (!rp.specifyWorld)
	{
		// Center the camera in this mesh
		if (rp.centerCam)
		{
			XMMATRIX scale, rx, ry, rz, translate;

			scale = XMMatrixScaling(mAttributes->mScale.x, mAttributes->mScale.y, mAttributes->mScale.z);
			rx = XMMatrixRotationX(mAttributes->mRotation.x);
			ry = XMMatrixRotationY(mAttributes->mRotation.y);
			rz = XMMatrixRotationZ(mAttributes->mRotation.z);

			//Get the position of the camera
			XMFLOAT3 eyeVertex = rp.camera->Position();

			if (rp.reflection)
			{
				//Invert the Y coordinate of the camera around the water plane height for the reflected camera position
				eyeVertex.y = -eyeVertex.y + (rp.seaLevel * 2.0f);
			}
			
			translate = XMMatrixTranslation(eyeVertex.x, eyeVertex.y, eyeVertex.z);

			XMMATRIX world = XMMatrixIdentity();

			world = XMMatrixMultiply(world, scale);
			world = XMMatrixMultiply(world, rx);
			world = XMMatrixMultiply(world, ry);
			world = XMMatrixMultiply(world, rz);
			world = XMMatrixMultiply(world, translate);
			
			rp.world = world;
		}
		else
		{
			if (rp.translateBillboard)
				rp.world = mAttributes->BillboardWorldXM(rp.camera->Position());
			else
				rp.world = mAttributes->WorldXM();
		}
	}
	
	Shade( rp );
}
//===============================================================================================================================
void CustomMesh::Shade(ZShadeSandboxMesh::MeshRenderParameters mrp)
{
	// Need to shade the convex hull also (Not that important)
	ZShadeSandboxMath::XMMath4 cp = ZShadeSandboxMath::XMMath4(mrp.clipplane.x, mrp.clipplane.y, mrp.clipplane.z, mrp.clipplane.w);
	
	if (mrp.renderDeferred)
	{
		ZShadeSandboxShader::Shaders::pMaterialGBufferShader->Wireframe() = m_Wireframe;
		ZShadeSandboxShader::Shaders::pMaterialGBufferShader->Render11(
			mAttributes->mIndexCount,
			mAttributes->mInstanceCount,
			mrp,
			mMaterial
		);
		
		return;
	}
	
	// Create a shadow map of the mesh
	if (mrp.shadowMap)
	{
		ZShadeSandboxShader::Shaders::pShadowMapBuildShader->Render(mAttributes->mIndexCount, mrp.world, mrp.light->Perspective());
		return;
	}
	
	if (mMeshParameters.useCustomShader)
	{
		//
		// Adds custom functionality for shading the mesh
		//
		
		if (mMeshParameters.shader != 0)
		{
			mMeshParameters.shader->RenderFunc(mAttributes->mIndexCount, mrp, mMaterial);
		}
		else
		{
			//The shader was not defined
			ZShadeMessageCenter::MsgBoxError(NULL, "CustomMesh: Shader was not defined !!!");
		}

		return;
	}
	
	if (mrp.tessellate)
	{
		// Assign the tessellation attributes to the material
		mMaterial->bEnableDistTess = mrp.enableDistTess;
		mMaterial->fMinTessDist = mrp.minTessDist;
		mMaterial->fMaxTessDist = mrp.maxTessDist;
		mMaterial->fMinTessFactor = mrp.minTess;
		mMaterial->fMaxTessFactor = mrp.maxTess;
		mMaterial->fNoDistTessFactor = mrp.noDistTessFactor;

		switch (mrp.renderType)
		{
			case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
			{
				if (mrp.renderLight)
				{
					ZShadeSandboxShader::Shaders::pTriMaterialLightTessellationShader->Wireframe() = m_Wireframe;
					ZShadeSandboxShader::Shaders::pTriMaterialLightTessellationShader->Render11(
						mAttributes->mIndexCount,
						mAttributes->mInstanceCount,
						mrp,
						mMaterial
					);
				}
				else
				{
					ZShadeSandboxShader::Shaders::pTriMaterialTessellationShader->Wireframe() = m_Wireframe;
					ZShadeSandboxShader::Shaders::pTriMaterialTessellationShader->Render11(
						mAttributes->mIndexCount,
						mAttributes->mInstanceCount,
						mrp,
						mMaterial
					);
				}
			}
			break;
			case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
			{
				if (mrp.renderLight)
				{
					ZShadeSandboxShader::Shaders::pQuadMaterialLightTessellationShader->Wireframe() = m_Wireframe;
					ZShadeSandboxShader::Shaders::pQuadMaterialLightTessellationShader->Render11(
						mAttributes->mIndexCount,
						mAttributes->mInstanceCount,
						mrp,
						mMaterial
					);
				}
				else
				{
					ZShadeSandboxShader::Shaders::pQuadMaterialTessellationShader->Wireframe() = m_Wireframe;
					ZShadeSandboxShader::Shaders::pQuadMaterialTessellationShader->Render11(
						mAttributes->mIndexCount,
						mAttributes->mInstanceCount,
						mrp,
						mMaterial
					);
				}
			}
			break;
			default:
			{
				//The shader was not defined
				ZShadeMessageCenter::MsgBoxError(NULL, "CustomMesh: Tessellation Shader was not defined !!!");
			}
			break;
		}
	}
	else
	{
		if (mrp.renderLight)
		{
			ZShadeSandboxShader::Shaders::pMaterialLightShader->Wireframe() = m_Wireframe;
			ZShadeSandboxShader::Shaders::pMaterialLightShader->Render11(
				mAttributes->mIndexCount,
				mAttributes->mInstanceCount,
				mrp,
				mMaterial
			);
		}
		else
		{
			ZShadeSandboxShader::Shaders::pMaterialShader->Wireframe() = m_Wireframe;
			ZShadeSandboxShader::Shaders::pMaterialShader->Render11(
				mAttributes->mIndexCount,
				mAttributes->mInstanceCount,
				mrp,
				mMaterial
			);
		}
	}
}
//===============================================================================================================================
void CustomMesh::AddInstancePositions(vector<XMFLOAT3> v)
{
	mAttributes->AddInstancePositions(v);
}
//===============================================================================================================================
void CustomMesh::CreateBuffers()
{
	//
	// Create Vertex Buffer
	//
	
	mAttributes->BuildVertexBuffer(mMeshParameters.vertexType);
	
	//
	// Create Index Buffer
	//

	if (!m_DisableIndexBuffer)
	{
		mAttributes->BuildIndexBuffer();
	}
}
//===============================================================================================================================
void CustomMesh::CreateInstanceBuffer()
{
	mAttributes->BuildInstanceBuffer();
}
//===============================================================================================================================
void CustomMesh::RegenVertexBuffer()
{
	mAttributes->ReBuildVertexBuffer(mMeshParameters.vertexType);
}
//===============================================================================================================================
void CustomMesh::RegenInstanceBuffer()
{
	mAttributes->ReBuildInstanceBuffer();
}
//===============================================================================================================================
void CustomMesh::UpdateAABB(XMFLOAT3 center, XMFLOAT3 scale)
{
	mBody->InitAABB(center, scale);
}
//===============================================================================================================================
ZShadeSandboxMath::AABB* CustomMesh::GetAABB() { return mBody->GetAABB(); }
//===============================================================================================================================
PhysicsBody3D* CustomMesh::GetPhysicsBody() { return mBody; }
//===============================================================================================================================
bool CustomMesh::Collides(CustomMesh* mesh)
{
	if (!mBody) return false;
	if (!mesh) return false;
	if (!mesh->GetPhysicsBody()) return false;
	return mBody->CollidesGJK(mesh->GetPhysicsBody());
}
//===============================================================================================================================
bool CustomMesh::IntersectsAABB(XMFLOAT3 point)
{
	return mBody->IntersectsAABB(point);
}
//===============================================================================================================================
float CustomMesh::DistanceAABBToPoint(XMFLOAT3 point)
{
	return mBody->DistanceAABBToPoint(point);
}
//===============================================================================================================================
void CustomMesh::CreatePhysicsBody()
{
	mBody = new PhysicsBody3D(mD3DSystem);
	mBody->InitAABB(mMeshParameters.pos, mMeshParameters.scale);
	
	//switch (mMeshParameters.vertexType)
	//{
	//	//case ZShadeSandboxMesh::EVertexType::Type::VT_Pos: mBody->AddPoints(mAttributes->mVerticesPos); break;
	//	//case ZShadeSandboxMesh::EVertexType::Type::VT_Color: mBody->AddPoints(mAttributes->mVerticesColor); break;
	//	case ZShadeSandboxMesh::EVertexType::Type::VT_Tex: mBody->AddPoints(mAttributes->mVerticesTex); break;
	//	//case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTex: mBody->AddPoints(mAttributes->mVerticesNormalTex); break;
	//	//case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTexTan: mBody->AddPoints(mAttributes->mVerticesNormalTexTan); break;
	//	//case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTexTanBi: mBody->AddPoints(mAttributes->mVerticesNormalTexTanBi); break;
	//	default:
	//	{
	//		//The shader was not defined
	//		ZShadeMessageCenter::MsgBoxError(NULL, "Mesh Vertex Type was not defined !!!");
	//	}
	//	break;
	//}

	//mBody->ColorTheHull(ZShadeSandboxMath::XMMath4(1, 0, 0, 1));
	//mBody->CreateConvexHull();
}
//===============================================================================================================================
void CustomMesh::UpdatePhysicsBody()
{
	if (!mBody) return;
	if (mBody->EPhysicsType() == PhysicsType::DYNAMIC)
	{
		// Update the position of the mesh
	}
}
//===============================================================================================================================