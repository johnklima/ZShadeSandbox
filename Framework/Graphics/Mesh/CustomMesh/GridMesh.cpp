#include "GridMesh.h"
#include <algorithm>
using ZShadeSandboxMesh::GridMesh;
//===============================================================================================================================
GridMesh::GridMesh(float width, float height, float depth, D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
,   fWidth(width)
,   fHeight(height)
,   fDepth(depth)
{
	fScreenWidth = mEngineOptions->m_screenWidth;
	fScreenHeight = mEngineOptions->m_screenHeight;
	
	mMeshType = ZShadeSandboxMesh::EMeshType::GRID;

	Initialize();
	
	CreateBuffers();
}
//===============================================================================================================================
GridMesh::~GridMesh()
{
}
//===============================================================================================================================
void GridMesh::Initialize()
{
	UINT vertexCount = fWidth * fHeight;
	UINT faceCount = (fWidth - 1) * (fHeight - 1) * 2;
	
	float halfWidth = 0.5f * fWidth;
	float halfDepth = 0.5f * fDepth;
	
	float dx = fWidth / (fWidth - 1);
	float dz = fDepth / (fHeight - 1);
	
	float du = 1.0f / (fWidth - 1);
	float dv = 1.0f / (fHeight - 1);
	
	//
	// Load Vertices
	//
	
	mAttributes->mVertexCount = vertexCount;
	mAttributes->mTriangleCount = mAttributes->mVertexCount / 3;
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			mAttributes->mVerticesNormalTex.resize(mAttributes->mVertexCount);
			
			for (UINT i = 0; i < fHeight; ++i)
			{
				float z = -halfDepth - i * dz;
				
				for (UINT j = 0; j < fWidth; ++j)
				{
					float x = -halfWidth + j * dx;
					
					mAttributes->mVerticesNormalTex[i * fWidth + j].position = XMFLOAT3(x, 0, z);
					mAttributes->mVerticesNormalTex[i * fWidth + j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
					
					// Stretch the texture over the grid
					mAttributes->mVerticesNormalTex[i * fWidth + j].texture.x = j * du;
					mAttributes->mVerticesNormalTex[i * fWidth + j].texture.y = i * du;
				}
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
			mAttributes->mVerticesNormalTexTan.resize(mAttributes->mVertexCount);
			
			for (UINT i = 0; i < fHeight; ++i)
			{
				float z = -halfDepth - i * dz;
				
				for (UINT j = 0; j < fWidth; ++j)
				{
					float x = -halfWidth + j * dx;
					
					mAttributes->mVerticesNormalTexTan[i * fWidth + j].position = XMFLOAT3(x, 0, z);
					mAttributes->mVerticesNormalTexTan[i * fWidth + j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
					mAttributes->mVerticesNormalTexTan[i * fWidth + j].tangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);
					
					// Stretch the texture over the grid
					mAttributes->mVerticesNormalTexTan[i * fWidth + j].texture.x = j * du;
					mAttributes->mVerticesNormalTexTan[i * fWidth + j].texture.y = i * du;
				}
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
			mAttributes->mVerticesPos.resize(mAttributes->mVertexCount);
			
			for (UINT i = 0; i < fHeight; ++i)
			{
				float z = -halfDepth - i * dz;
				
				for (UINT j = 0; j < fWidth; ++j)
				{
					float x = -halfWidth + j * dx;
					
					mAttributes->mVerticesPos[i * fWidth + j].position = XMFLOAT3(x, 0, z);
				}
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			mAttributes->mVerticesTex.resize(mAttributes->mVertexCount);
			
			for (UINT i = 0; i < fHeight; ++i)
			{
				float z = -halfDepth - i * dz;
				
				for (UINT j = 0; j < fWidth; ++j)
				{
					float x = -halfWidth + j * dx;
					
					mAttributes->mVerticesTex[i * fWidth + j].position = XMFLOAT3(x, 0, z);
					
					// Stretch the texture over the grid
					mAttributes->mVerticesTex[i * fWidth + j].texture.x = j * du;
					mAttributes->mVerticesTex[i * fWidth + j].texture.y = i * du;
				}
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

			mAttributes->mVerticesColor.resize(mAttributes->mVertexCount);
			
			for (UINT i = 0; i < fHeight; ++i)
			{
				float z = -halfDepth - i * dz;
				
				for (UINT j = 0; j < fWidth; ++j)
				{
					float x = -halfWidth + j * dx;
					
					mAttributes->mVerticesColor[i * fWidth + j].position = XMFLOAT3(x, 0, z);
					mAttributes->mVerticesColor[i * fWidth + j].color = diffuseColor;
				}
			}
		}
		break;
	}
	
	//
	// Load Indices
	//
	
	mAttributes->mIndexCount = faceCount * 3;
	mAttributes->mIndices.resize(mAttributes->mIndexCount);
	
	// Iterate over each quad and compute indices.
	UINT k = 0;
	
	for (UINT i = 0; i < fHeight - 1; ++i)
	{
		for (UINT j = 0; j < fWidth - 1; ++j)
		{
			mAttributes->mIndices[k] = i * fWidth + j;
			mAttributes->mIndices[k + 1] = i * fWidth + j + 1;
			mAttributes->mIndices[k + 2] = (i + 1) * fWidth + j;
			
			mAttributes->mIndices[k + 3] = (i + 1) * fWidth + j;
			mAttributes->mIndices[k + 4] = i * fWidth + j + 1;
			mAttributes->mIndices[k + 5] = (i + 1) * fWidth + j + 1;

			k += 6; // next quad
		}
	}
}
//===============================================================================================================================