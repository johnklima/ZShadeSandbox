#include "TriangleMesh.h"
#include <algorithm>
using ZShadeSandboxMesh::TriangleMesh;
//===============================================================================================================================
TriangleMesh::TriangleMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
{
	mMeshType = ZShadeSandboxMesh::EMeshType::TRI;

	Initialize();
	CreateBuffers();
}
//===============================================================================================================================
TriangleMesh::~TriangleMesh()
{
}
//===============================================================================================================================
void TriangleMesh::Initialize()
{
	const static int numVerts = 3;
	const static int numInd = 3;
	
	// Sample Adjacency information for the mesh
	//verts[0].next = verts[1];
	//verts[0].prev = verts[2];
	
	//verts[1].next = verts[0];
	//verts[1].prev = verts[2];
	
	//verts[2].next = verts[1];
	//verts[2].prev = verts[0];
	
	//
	// Load Vertices
	//
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			ZShadeSandboxMesh::VertexNormalTex t1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex t2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex t3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			
			ZShadeSandboxMesh::VertexNormalTex verts[numVerts] =
			{
				t1, t2, t3
			};
			
			mAttributes->mVerticesNormalTex.resize( numVerts );
			
			for (int i = 0; i < numVerts; i++)
			{
				mAttributes->mVerticesNormalTex[i] = verts[i];
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
			ZShadeSandboxMesh::VertexNormalTexTan t1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan t2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan t3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			
			ZShadeSandboxMesh::VertexNormalTexTan verts[numVerts] =
			{
				t1, t2, t3
			};
			
			mAttributes->mVerticesNormalTexTan.resize(numVerts);
			
			for (int i = 0; i < numVerts; i++)
			{
				mAttributes->mVerticesNormalTexTan[i] = verts[i];
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
			ZShadeSandboxMesh::VertexPos t1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f, 1.0f, -1.0f);
			ZShadeSandboxMesh::VertexPos t2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(1.0f, 1.0f, -1.0f);
			ZShadeSandboxMesh::VertexPos t3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f, -1.0f, -1.0f);
			
			ZShadeSandboxMesh::VertexPos verts[numVerts] =
			{
				t1, t2, t3
			};
			
			mAttributes->mVerticesPos.resize(numVerts);
			
			for (int i = 0; i < numVerts; i++)
			{
				mAttributes->mVerticesPos[i] = verts[i];
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			ZShadeSandboxMesh::VertexTex t1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex t2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex t3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f);
			
			ZShadeSandboxMesh::VertexTex verts[numVerts] =
			{
				t1, t2, t3
			};
			
			mAttributes->mVerticesTex.resize(numVerts);
			
			for (int i = 0; i < numVerts; i++)
			{
				mAttributes->mVerticesTex[i] = verts[i];
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

			ZShadeSandboxMesh::VertexColor t1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, 1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor t2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, 1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor t3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, -1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			ZShadeSandboxMesh::VertexColor verts[numVerts] =
			{
				t1, t2, t3
			};
			
			mAttributes->mVerticesColor.resize(numVerts);
			
			for (int i = 0; i < numVerts; i++)
			{
				mAttributes->mVerticesColor[i] = verts[i];
			}
		}
		break;
	}
	
	mAttributes->mVertexCount = numVerts;
	mAttributes->mTriangleCount = mAttributes->mVertexCount / 3;
	
	//
	// Load Indices
	//

	UINT indices[numInd] =
	{
		0, 1, 2,
	};

	mAttributes->mIndices.resize(numInd);

	for (int i = 0; i < numInd; i++)
	{
		mAttributes->mIndices[i] = indices[i];
	}
}
//===============================================================================================================================