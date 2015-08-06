#include "Vertex.h"
#include "CubeMesh.h"
#include <algorithm>
using ZShadeSandboxMesh::CubeMesh;
//===============================================================================================================================
CubeMesh::CubeMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, char* filename)
:   ZShadeSandboxMesh::CustomMesh(d3d, mp, filename)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::CUBE;
}
//===============================================================================================================================
CubeMesh::CubeMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh(d3d, mp)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::CUBE;

	Initialize();
	
	CreateBuffers();
}
//===============================================================================================================================
CubeMesh::CubeMesh(D3D* d3d, ZShadeSandboxMath::BoxPrimitive box, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh(d3d, mp)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::CUBE;
	
	//
	// Load Vertices
	//
	
	XMFLOAT4 v[8];
	box.GetVertices( v );
	
	// 0,1,2,3 are on bottom
	// 4,5,6,7 are on top
	
	//Front Face
	ZShadeSandboxMesh::VertexTex ff1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[0].x, v[0].y, v[0].z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex ff2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[1].x, v[1].y, v[1].z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex ff3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[4].x, v[4].y, v[4].z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex ff4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[5].x, v[5].y, v[5].z, 1.0f, 1.0f);
	
	//Back Face
	ZShadeSandboxMesh::VertexTex bf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[2].x, v[2].y, v[2].z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex bf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[3].x, v[3].y, v[3].z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex bf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[6].x, v[6].y, v[6].z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex bf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[7].x, v[7].y, v[7].z, 1.0f, 1.0f);
	
	//Left Face
	ZShadeSandboxMesh::VertexTex lf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[4].x, v[4].y, v[4].z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex lf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[7].x, v[7].y, v[7].z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex lf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[0].x, v[0].y, v[0].z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex lf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[3].x, v[3].y, v[3].z, 1.0f, 1.0f);
	
	//Right Face
	ZShadeSandboxMesh::VertexTex rf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[5].x, v[5].y, v[5].z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex rf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[6].x, v[6].y, v[6].z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex rf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[1].x, v[1].y, v[1].z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex rf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[2].x, v[2].y, v[2].z, 1.0f, 1.0f);
	
	//Top Face
	ZShadeSandboxMesh::VertexTex tf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[4].x, v[4].y, v[4].z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex tf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[5].x, v[5].y, v[5].z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex tf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[6].x, v[6].y, v[6].z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex tf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[7].x, v[7].y, v[7].z, 1.0f, 1.0f);
	
	//Bottom Face
	ZShadeSandboxMesh::VertexTex bof1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[0].x, v[0].y, v[0].z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex bof2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[1].x, v[1].y, v[1].z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex bof3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[2].x, v[2].y, v[2].z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex bof4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v[3].x, v[3].y, v[3].z, 1.0f, 1.0f);
	
	ZShadeSandboxMesh::VertexTex verts[NUM_CUBE_VERTS] =
	{
		//(0,0)-------(0,1)
		//
		//
		//
		//(0,1)-------(1,1)

		//Front Face, 
		ff1, ff2, ff3, ff4,

		//Back Face
		bf1, bf2, bf3, bf4,

		//Left Face
		lf1, lf2, lf3, lf4,

		//Right Face
		rf1, rf2, rf3, rf4,

		//Top Face
		tf1, tf2, tf3, tf4,
		
		//Bottom Face
		bof1, bof2, bof3, bof4
	};

	mAttributes->mVerticesTex.resize(NUM_CUBE_VERTS);

	for (int i = 0; i < NUM_CUBE_VERTS; i++)
	{
		mAttributes->mVerticesTex[i] = verts[i];
	}

	mAttributes->mVertexCount = NUM_CUBE_VERTS;
	
	//
	// Load Indices
	//

	UINT indices[NUM_CUBE_INDICES] =
	{
		//Front Face
		0, 1, 2,
		2, 1, 3,

		//Back Face
		4, 5, 6,
		6, 5, 7,

		//Left Face
		8, 9, 10,
		10, 9, 11,

		//Right Face
		12, 13, 14,
		14, 13, 15,

		//Top Face
		16, 17, 18,
		18, 17, 19,

		//Bottom Face
		20, 21, 22,
		22, 21, 23
	};

	mAttributes->mIndices.resize(NUM_CUBE_INDICES);

	for (int i = 0; i < NUM_CUBE_INDICES; i++)
	{
		mAttributes->mIndices[i] = indices[i];
	}
	
	CreateBuffers();
}
//===============================================================================================================================
CubeMesh::CubeMesh(D3D* d3d, XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3, XMFLOAT3 p4, float cap, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh(d3d, mp)
{
	// Creates a CubeMesh that takes 4 points and generates a cube assuming these 4 points are the base of the cube
	XMFLOAT3 boxBase1 = p1;
	XMFLOAT3 boxBase2 = p2;
	XMFLOAT3 boxBase3 = p3;
	XMFLOAT3 boxBase4 = p4;
	
	// Adjust the height of the box around this node along the y axis
	XMFLOAT3 boxTop1(boxBase1.x, boxBase1.y + cap, boxBase1.z);
	XMFLOAT3 boxTop2(boxBase2.x, boxBase2.y + cap, boxBase2.z);
	XMFLOAT3 boxTop3(boxBase3.x, boxBase3.y + cap, boxBase3.z);
	XMFLOAT3 boxTop4(boxBase4.x, boxBase4.y + cap, boxBase4.z);
	
	//Front Face
	ZShadeSandboxMesh::VertexTex ff1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxBase1.x, boxBase1.y, boxBase1.z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex ff2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxBase2.x, boxBase2.y, boxBase2.z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex ff3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxTop1.x, boxTop1.y, boxTop1.z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex ff4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxTop2.x, boxTop2.y, boxTop2.z, 1.0f, 1.0f);
	
	//Back Face
	ZShadeSandboxMesh::VertexTex bf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxBase3.x, boxBase3.y, boxBase3.z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex bf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxBase4.x, boxBase4.y, boxBase4.z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex bf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxTop3.x, boxTop3.y, boxTop3.z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex bf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxTop4.x, boxTop4.y, boxTop4.z, 1.0f, 1.0f);
	
	//Left Face
	ZShadeSandboxMesh::VertexTex lf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxBase1.x, boxBase1.y, boxBase1.z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex lf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxBase3.x, boxBase3.y, boxBase3.z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex lf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxTop1.x, boxTop1.y, boxTop1.z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex lf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxTop3.x, boxTop3.y, boxTop3.z, 1.0f, 1.0f);
	
	//Right Face
	ZShadeSandboxMesh::VertexTex rf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxBase2.x, boxBase2.y, boxBase2.z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex rf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxBase4.x, boxBase4.y, boxBase4.z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex rf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxTop2.x, boxTop2.y, boxTop2.z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex rf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxTop4.x, boxTop4.y, boxTop4.z, 1.0f, 1.0f);
	
	//Top Face
	ZShadeSandboxMesh::VertexTex tf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxTop1.x, boxTop1.y, boxTop1.z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex tf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxTop2.x, boxTop2.y, boxTop2.z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex tf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxTop3.x, boxTop3.y, boxTop3.z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex tf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxTop4.x, boxTop4.y, boxTop4.z, 1.0f, 1.0f);
	
	//Bottom Face
	ZShadeSandboxMesh::VertexTex bof1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxBase1.x, boxBase1.y, boxBase1.z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex bof2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxBase2.x, boxBase2.y, boxBase2.z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex bof3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxBase3.x, boxBase3.y, boxBase3.z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex bof4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(boxBase4.x, boxBase4.y, boxBase4.z, 1.0f, 1.0f);
	
	ZShadeSandboxMesh::VertexTex verts[NUM_CUBE_VERTS] =
	{
		//(0,0)-------(0,1)
		//
		//
		//
		//(0,1)-------(1,1)

		//Front Face
		ff1, ff2, ff3, ff4,

		//Back Face
		bf1, bf2, bf3, bf4,

		//Left Face
		lf1, lf2, lf3, lf4,

		//Right Face
		rf1, rf2, rf3, rf4,

		//Top Face
		tf1, tf2, tf3, tf4,

		//Bottom Face
		bof1, bof2, bof3, bof4
	};
	
	mAttributes->mVerticesTex.resize(NUM_CUBE_VERTS);

	for (int i = 0; i < NUM_CUBE_VERTS; i++)
	{
		mAttributes->mVerticesTex[i] = verts[i];
	}

	mAttributes->mVertexCount = NUM_CUBE_VERTS;
	
	//
	// Load Indices
	//

	UINT indices[NUM_CUBE_INDICES] =
	{
		//Front Face
		0, 1, 2,
		2, 1, 3,

		//Back Face
		4, 5, 6,
		6, 5, 7,

		//Left Face
		8, 9, 10,
		10, 9, 11,

		//Right Face
		12, 13, 14,
		14, 13, 15,

		//Top Face
		16, 17, 18,
		18, 17, 19,

		//Bottom Face
		20, 21, 22,
		22, 21, 23
	};

	mAttributes->mIndices.resize(NUM_CUBE_INDICES);

	for (int i = 0; i < NUM_CUBE_INDICES; i++)
	{
		mAttributes->mIndices[i] = indices[i];
	}
	
	CreateBuffers();
}
//===============================================================================================================================
CubeMesh::~CubeMesh()
{
}
//===============================================================================================================================
void CubeMesh::Initialize()
{
	//
	// Load Vertices
	//
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
#pragma region "Vertex Normal Tex"
			//Front Face
			ZShadeSandboxMesh::VertexNormalTex ff1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex ff2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex ff3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexNormalTex ff4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
			
			//Back Face
			ZShadeSandboxMesh::VertexNormalTex bf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex bf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex bf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexNormalTex bf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
			
			//Left Face
			ZShadeSandboxMesh::VertexNormalTex lf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex lf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex lf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexNormalTex lf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
			
			//Right Face
			ZShadeSandboxMesh::VertexNormalTex rf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, 1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex rf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex rf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexNormalTex rf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
			
			//Top Face
			ZShadeSandboxMesh::VertexNormalTex tf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex tf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex tf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexNormalTex tf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			
			//Bottom Face
			ZShadeSandboxMesh::VertexNormalTex bof1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex bof2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex bof3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexNormalTex bof4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			
			ZShadeSandboxMesh::VertexNormalTex verts[NUM_CUBE_VERTS] =
			{
				//(0,0)-------(0,1)
				//
				//
				//
				//(0,1)-------(1,1)

				//Front Face
				ff1, ff2, ff3, ff4,

				//Back Face
				bf1, bf2, bf3, bf4,

				//Left Face
				lf1, lf2, lf3, lf4,

				//Right Face
				rf1, rf2, rf3, rf4,

				//Top Face
				tf1, tf2, tf3, tf4,

				//Bottom Face
				bof1, bof2, bof3, bof4
			};

			mAttributes->mVerticesNormalTex.resize(NUM_CUBE_VERTS);

			for (int i = 0; i < NUM_CUBE_VERTS; i++)
			{
				mAttributes->mVerticesNormalTex[i] = verts[i];
			}

			mAttributes->mVertexCount = NUM_CUBE_VERTS;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
#pragma region "Vertex Normal Tex Tan"
			//Front Face
			ZShadeSandboxMesh::VertexNormalTexTan ff1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan ff2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan( 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan ff3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan ff4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			
			//Back Face
			ZShadeSandboxMesh::VertexNormalTexTan bf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan( 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan bf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan bf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan bf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			
			//Left Face
			ZShadeSandboxMesh::VertexNormalTexTan lf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan lf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan lf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan lf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			
			//Right Face
			ZShadeSandboxMesh::VertexNormalTexTan rf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan rf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan rf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan rf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			
			//Top Face
			ZShadeSandboxMesh::VertexNormalTexTan tf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan tf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan( 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan tf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan tf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			
			//Bottom Face
			ZShadeSandboxMesh::VertexNormalTexTan bof1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan( 1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan bof2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan bof3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan bof4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			
			ZShadeSandboxMesh::VertexNormalTexTan verts[NUM_CUBE_VERTS] =
			{
				//(0,0)-------(0,1)
				//
				//
				//
				//(0,1)-------(1,1)

				//Front Face
				ff1, ff2, ff3, ff4,

				//Back Face
				bf1, bf2, bf3, bf4,

				//Left Face
				lf1, lf2, lf3, lf4,

				//Right Face
				rf1, rf2, rf3, rf4,

				//Top Face
				tf1, tf2, tf3, tf4,

				//Bottom Face
				bof1, bof2, bof3, bof4
			};

			mAttributes->mVerticesNormalTexTan.resize(NUM_CUBE_VERTS);

			for (int i = 0; i < NUM_CUBE_VERTS; i++)
			{
				mAttributes->mVerticesNormalTexTan[i] = verts[i];
			}

			mAttributes->mVertexCount = NUM_CUBE_VERTS;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
#pragma region "Vertex Pos"
			//Front Face
			ZShadeSandboxMesh::VertexPos ff1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f,  1.0f, -1.0f);
			ZShadeSandboxMesh::VertexPos ff2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos( 1.0f,  1.0f, -1.0f);
			ZShadeSandboxMesh::VertexPos ff3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f, -1.0f, -1.0f);
			ZShadeSandboxMesh::VertexPos ff4 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos( 1.0f, -1.0f, -1.0f);
			
			//Back Face
			ZShadeSandboxMesh::VertexPos bf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos( 1.0f,  1.0f, 1.0f);
			ZShadeSandboxMesh::VertexPos bf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f,  1.0f, 1.0f);
			ZShadeSandboxMesh::VertexPos bf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos( 1.0f, -1.0f, 1.0f);
			ZShadeSandboxMesh::VertexPos bf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f, -1.0f, 1.0f);
			
			//Left Face
			ZShadeSandboxMesh::VertexPos lf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f,  1.0f,  1.0f);
			ZShadeSandboxMesh::VertexPos lf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f,  1.0f, -1.0f);
			ZShadeSandboxMesh::VertexPos lf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f, -1.0f,  1.0f);
			ZShadeSandboxMesh::VertexPos lf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f, -1.0f, -1.0f);
			
			//Right Face
			ZShadeSandboxMesh::VertexPos rf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(1.0f,  1.0f, -1.0f);
			ZShadeSandboxMesh::VertexPos rf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(1.0f,  1.0f,  1.0f);
			ZShadeSandboxMesh::VertexPos rf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(1.0f, -1.0f, -1.0f);
			ZShadeSandboxMesh::VertexPos rf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(1.0f, -1.0f,  1.0f);
			
			//Top Face
			ZShadeSandboxMesh::VertexPos tf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f, 1.0f,  1.0f);
			ZShadeSandboxMesh::VertexPos tf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos( 1.0f, 1.0f,  1.0f);
			ZShadeSandboxMesh::VertexPos tf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f, 1.0f, -1.0f);
			ZShadeSandboxMesh::VertexPos tf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos( 1.0f, 1.0f, -1.0f);
			
			//Bottom Face
			ZShadeSandboxMesh::VertexPos bof1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos( 1.0f, -1.0f,  1.0f);
			ZShadeSandboxMesh::VertexPos bof2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f, -1.0f,  1.0f);
			ZShadeSandboxMesh::VertexPos bof3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos( 1.0f, -1.0f, -1.0f);
			ZShadeSandboxMesh::VertexPos bof4 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f, -1.0f, -1.0f);
			
			ZShadeSandboxMesh::VertexPos verts[NUM_CUBE_VERTS] =
			{
				//(0,0)-------(0,1)
				//
				//
				//
				//(0,1)-------(1,1)

				//Front Face
				ff1, ff2, ff3, ff4,

				//Back Face
				bf1, bf2, bf3, bf4,

				//Left Face
				lf1, lf2, lf3, lf4,

				//Right Face
				rf1, rf2, rf3, rf4,

				//Top Face
				tf1, tf2, tf3, tf4,

				//Bottom Face
				bof1, bof2, bof3, bof4
			};

			mAttributes->mVerticesPos.resize(NUM_CUBE_VERTS);

			for (int i = 0; i < NUM_CUBE_VERTS; i++)
			{
				mAttributes->mVerticesPos[i] = verts[i];
			}

			mAttributes->mVertexCount = NUM_CUBE_VERTS;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
#pragma region "Vertex Texture"
			//Front Face
			ZShadeSandboxMesh::VertexTex ff1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex ff2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex ff3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexTex ff4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f);
			
			//Back Face
			ZShadeSandboxMesh::VertexTex bf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex bf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex bf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexTex bf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
			
			//Left Face
			ZShadeSandboxMesh::VertexTex lf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex lf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex lf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexTex lf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f);
			
			//Right Face
			ZShadeSandboxMesh::VertexTex rf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex rf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex rf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexTex rf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f);
			
			//Top Face
			ZShadeSandboxMesh::VertexTex tf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex tf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1.0f, 1.0f,  1.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex tf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexTex tf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1.0f, 1.0f, -1.0f, 1.0f, 1.0f);
			
			//Bottom Face
			ZShadeSandboxMesh::VertexTex bof1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1.0f, -1.0f,  1.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex bof2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex bof3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexTex bof4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f);
			
			ZShadeSandboxMesh::VertexTex verts[NUM_CUBE_VERTS] =
			{
				//(0,0)-------(0,1)
				//
				//
				//
				//(0,1)-------(1,1)

				//Front Face
				ff1, ff2, ff3, ff4,

				//Back Face
				bf1, bf2, bf3, bf4,

				//Left Face
				lf1, lf2, lf3, lf4,

				//Right Face
				rf1, rf2, rf3, rf4,

				//Top Face
				tf1, tf2, tf3, tf4,

				//Bottom Face
				bof1, bof2, bof3, bof4
			};

			mAttributes->mVerticesTex.resize(NUM_CUBE_VERTS);

			for (int i = 0; i < NUM_CUBE_VERTS; i++)
			{
				mAttributes->mVerticesTex[i] = verts[i];
			}

			mAttributes->mVertexCount = NUM_CUBE_VERTS;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
#pragma region "Vertex Color"
			XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

			//Front Face
			ZShadeSandboxMesh::VertexColor ff1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, 1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor ff2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, 1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor ff3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, -1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor ff4 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, -1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			//Back Face
			ZShadeSandboxMesh::VertexColor bf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, 1.0f, 1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor bf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, 1.0f, 1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor bf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, -1.0f, 1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor bf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, -1.0f, 1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			//Left Face
			ZShadeSandboxMesh::VertexColor lf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, 1.0f, 1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor lf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, 1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor lf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, -1.0f, 1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor lf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, -1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			//Right Face
			ZShadeSandboxMesh::VertexColor rf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, 1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor rf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, 1.0f, 1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor rf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, -1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor rf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, -1.0f, 1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			//Top Face
			ZShadeSandboxMesh::VertexColor tf1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, 1.0f, 1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor tf2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, 1.0f, 1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor tf3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, 1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor tf4 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, 1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			//Bottom Face
			ZShadeSandboxMesh::VertexColor bof1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, -1.0f, 1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor bof2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, -1.0f, 1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor bof3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, -1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor bof4 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, -1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			ZShadeSandboxMesh::VertexColor verts[NUM_CUBE_VERTS] =
			{
				//(0,0)-------(0,1)
				//
				//
				//
				//(0,1)-------(1,1)

				//Front Face
				ff1, ff2, ff3, ff4,

				//Back Face
				bf1, bf2, bf3, bf4,

				//Left Face
				lf1, lf2, lf3, lf4,

				//Right Face
				rf1, rf2, rf3, rf4,

				//Top Face
				tf1, tf2, tf3, tf4,

				//Bottom Face
				bof1, bof2, bof3, bof4
			};

			mAttributes->mVerticesColor.resize(NUM_CUBE_VERTS);

			for (int i = 0; i < NUM_CUBE_VERTS; i++)
			{
				mAttributes->mVerticesColor[i] = verts[i];
			}

			mAttributes->mVertexCount = NUM_CUBE_VERTS;
#pragma endregion
		}
		break;
	}
	
	mAttributes->mTriangleCount = mAttributes->mVertexCount / 3;
	
	//
	// Load Indices
	//

	UINT indices[NUM_CUBE_INDICES] =
	{
		//Front Face
		0, 1, 2,
		2, 1, 3,

		//Back Face
		4, 5, 6,
		6, 5, 7,

		//Left Face
		8, 9, 10,
		10, 9, 11,

		//Right Face
		12, 13, 14,
		14, 13, 15,

		//Top Face
		16, 17, 18,
		18, 17, 19,

		//Bottom Face
		20, 21, 22,
		22, 21, 23
	};

	mAttributes->mIndices.resize(NUM_CUBE_INDICES);

	for (int i = 0; i < NUM_CUBE_INDICES; i++)
	{
		mAttributes->mIndices[i] = indices[i];
	}
}
//===============================================================================================================================
