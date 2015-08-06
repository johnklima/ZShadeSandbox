#include "SkydomeMesh.h"
#include "TextureManager.h"
using ZShadeSandboxMesh::SkydomeMesh;
//===============================================================================================================================
SkydomeMesh::SkydomeMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh(d3d, mp)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::CUSTOM;
	Initialize();
	CreateBuffers();
}
//===============================================================================================================================
SkydomeMesh::~SkydomeMesh()
{
}
//===============================================================================================================================
void SkydomeMesh::Initialize()
{
	MeshData* md = MeshUtil::LoadMesh("Models\\skydome.txt", mMeshParameters.vertexType);
	
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
