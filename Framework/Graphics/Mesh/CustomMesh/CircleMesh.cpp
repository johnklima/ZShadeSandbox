#include "CircleMesh.h"
#include <algorithm>
using ZShadeSandboxMesh::CircleMesh;
//===============================================================================================================================
CircleMesh::CircleMesh(D3D* d3d, float radius, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
,   fRadius(radius)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::CIRCLE;

	Initialize();
	CreateBuffers();
}
//===============================================================================================================================
CircleMesh::~CircleMesh()
{
}
//===============================================================================================================================
void CircleMesh::Initialize()
{
	//
	// Load Vertices
	//
	
	float iHeight = 1;
	float iStackCount = 1;
	float iSliceCount = 32;

	float stackHeight = iHeight / iStackCount;
	
	uint32 ringCount = iStackCount + 1;
	
	mAttributes->mVertexCount = ringCount;
	
	mAttributes->mTriangleCount = mAttributes->mVertexCount / 3;
	
	uint32 baseIndex = (uint32)0;

	float y = 0.5f * iHeight;
	float dTheta = 2.0f * PI / iSliceCount;
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
#pragma region "Normal Tex"
			mAttributes->mVerticesNormalTex.resize(mAttributes->mVertexCount);
			
			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for(uint32 i = 0; i <= iSliceCount; ++i)
			{
				float x = fRadius * cosf(i * dTheta);
				float z = fRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / iHeight + 0.5f;
				float v = z / iHeight + 0.5f;

				mAttributes->mVerticesNormalTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(x, y, z, 0.0f, 1.0f, 0.0f, u, v));
			}

			// Cap center vertex.
			mAttributes->mVerticesNormalTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f));

			mAttributes->mVertexCount = mAttributes->mVerticesNormalTex.size();
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
#pragma region "Normal Tex Tan"
			mAttributes->mVerticesNormalTexTan.resize(mAttributes->mVertexCount);
			
			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for(uint32 i = 0; i <= iSliceCount; ++i)
			{
				float x = fRadius * cosf(i * dTheta);
				float z = fRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / iHeight + 0.5f;
				float v = z / iHeight + 0.5f;

				mAttributes->mVerticesNormalTexTan.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(x, y, z, 0.0f, 1.0f, 0.0f, u, v, 0.0f, 0.0f, 0.0f));
			}

			// Cap center vertex.
			mAttributes->mVerticesNormalTexTan.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f));

			mAttributes->mVertexCount = mAttributes->mVerticesNormalTexTan.size();
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
#pragma region "Normal Pos"
			mAttributes->mVerticesPos.resize(mAttributes->mVertexCount);
			
			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for(uint32 i = 0; i <= iSliceCount; ++i)
			{
				float x = fRadius * cosf(i * dTheta);
				float z = fRadius * sinf(i * dTheta);

				mAttributes->mVerticesPos.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexPos(x, y, z));
			}

			// Cap center vertex.
			mAttributes->mVerticesPos.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexPos(0.0f, y, 0.0f));

			mAttributes->mVertexCount = mAttributes->mVerticesPos.size();
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
#pragma region "Normal Tex"
			mAttributes->mVerticesTex.resize(mAttributes->mVertexCount);
			
			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for(uint32 i = 0; i <= iSliceCount; ++i)
			{
				float x = fRadius * cosf(i * dTheta);
				float z = fRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / iHeight + 0.5f;
				float v = z / iHeight + 0.5f;

				mAttributes->mVerticesTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x, y, z, u, v));
			}

			// Cap center vertex.
			mAttributes->mVerticesTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0.0f, y, 0.0f, 0.5f, 0.5f));

			mAttributes->mVertexCount = mAttributes->mVerticesTex.size();
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
#pragma region "Normal Color"
			XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

			mAttributes->mVerticesColor.resize(mAttributes->mVertexCount);
			
			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for(uint32 i = 0; i <= iSliceCount; ++i)
			{
				float x = fRadius * cosf(i * dTheta);
				float z = fRadius * sinf(i * dTheta);

				mAttributes->mVerticesColor.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexColor(x, y, z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w));
			}

			// Cap center vertex.
			mAttributes->mVerticesColor.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexColor(0.0f, y, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w));

			mAttributes->mVertexCount = mAttributes->mVerticesColor.size();
#pragma endregion
		}
		break;
	}
	
	//
	// Load Indices
	//
	
	//mIndexCount = (iStackCount * iSliceCount) * 3;
	
	//mIndices.resize( mIndexCount );
	
	// Index of center vertex.
	uint32 centerIndex = (uint32)mAttributes->mVerticesTex.size() - 1;

	uint32 index = 0;
	for(uint32 i = 0; i < iSliceCount; ++i)
	{
		mAttributes->mIndices.push_back(centerIndex);
		mAttributes->mIndices.push_back(baseIndex + i + 1);
		mAttributes->mIndices.push_back(baseIndex + i);
	}

	mAttributes->mIndexCount = mAttributes->mIndices.size();
}
//===============================================================================================================================