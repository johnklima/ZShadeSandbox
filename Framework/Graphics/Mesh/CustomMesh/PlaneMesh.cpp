#include "PlaneMesh.h"
using ZShadeSandboxMesh::PlaneMesh;
//===============================================================================================================================
PlaneMesh::PlaneMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, char* filename)
:   ZShadeSandboxMesh::CustomMesh(d3d, mp, filename)
,   iPlaneSize(0)
,   fPlaneHeight(0)
,   bCenterPlane(false)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::PLANE;
}
//===============================================================================================================================
PlaneMesh::PlaneMesh(int planeSize, float planeHeight, D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, bool centerPlane)
:   ZShadeSandboxMesh::CustomMesh(d3d, mp)
,   iPlaneSize(planeSize)
,   fPlaneHeight(planeHeight)
,   bCenterPlane(centerPlane)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::PLANE;
	Initialize();
	CreateBuffers();
}
//===============================================================================================================================
PlaneMesh::~PlaneMesh()
{
}
//===============================================================================================================================
void PlaneMesh::Initialize()
{
	int numVertices = 6;
	
	mAttributes->mTriangleCount = mAttributes->mVertexCount / 3;
	
	float halfWidth = 0;
	
	if (bCenterPlane)
	{
		halfWidth = ((float)iPlaneSize - 1.0f) / 2.0f;
		iPlaneSize = iPlaneSize - halfWidth;
	}
	
	//
	// Load Vertices and Indices
	//

	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
#pragma region "Vertex Normal Tex"
			mAttributes->mVertexCount = numVertices;
			mAttributes->mVerticesNormalTex.resize(mAttributes->mVertexCount);

			// Load the vertex array with data.
			mAttributes->mVerticesNormalTex[0].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, iPlaneSize);  // Top left.
			mAttributes->mVerticesNormalTex[0].normal = XMFLOAT3(0, 1, 0);
			mAttributes->mVerticesNormalTex[0].texture = XMFLOAT2(0.0f, 0.0f);//0, 0

			mAttributes->mVerticesNormalTex[1].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mAttributes->mVerticesNormalTex[1].normal = XMFLOAT3(0, 1, 0);
			mAttributes->mVerticesNormalTex[1].texture = XMFLOAT2(1.0f, 0.0f);//1, 0

			mAttributes->mVerticesNormalTex[2].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mAttributes->mVerticesNormalTex[2].normal = XMFLOAT3(0, 1, 0);
			mAttributes->mVerticesNormalTex[2].texture = XMFLOAT2(0.0f, 1.0f);//0, 1

			mAttributes->mVerticesNormalTex[3].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mAttributes->mVerticesNormalTex[3].normal = XMFLOAT3(0, 1, 0);
			mAttributes->mVerticesNormalTex[3].texture = XMFLOAT2(0.0f, 1.0f);//0, 1

			mAttributes->mVerticesNormalTex[4].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mAttributes->mVerticesNormalTex[4].normal = XMFLOAT3(0, 1, 0);
			mAttributes->mVerticesNormalTex[4].texture = XMFLOAT2(1.0f, 0.0f);//1, 0

			mAttributes->mVerticesNormalTex[5].position = XMFLOAT3(iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom right.
			mAttributes->mVerticesNormalTex[5].normal = XMFLOAT3(0, 1, 0);
			mAttributes->mVerticesNormalTex[5].texture = XMFLOAT2(1.0f, 1.0f);//1, 1
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
#pragma region "Vertex Tex"
			mAttributes->mVertexCount = numVertices;
			mAttributes->mVerticesTex.resize(mAttributes->mVertexCount);

			// Load the vertex array with data.
			mAttributes->mVerticesTex[0].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, iPlaneSize);  // Top left.
			mAttributes->mVerticesTex[0].texture = XMFLOAT2(0.0f, 0.0f);

			mAttributes->mVerticesTex[1].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mAttributes->mVerticesTex[1].texture = XMFLOAT2(1.0f, 0.0f);

			mAttributes->mVerticesTex[2].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mAttributes->mVerticesTex[2].texture = XMFLOAT2(0.0f, 1.0f);

			mAttributes->mVerticesTex[3].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mAttributes->mVerticesTex[3].texture = XMFLOAT2(0.0f, 1.0f);

			mAttributes->mVerticesTex[4].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mAttributes->mVerticesTex[4].texture = XMFLOAT2(1.0f, 0.0f);

			mAttributes->mVerticesTex[5].position = XMFLOAT3(iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom right.
			mAttributes->mVerticesTex[5].texture = XMFLOAT2(1.0f, 1.0f);
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
#pragma region "Vertex Color"
			mAttributes->mVertexCount = numVertices;
			mAttributes->mVerticesColor.resize(mAttributes->mVertexCount);

			XMFLOAT4 diffuseColor = mMeshParameters.material->vDiffuseColor;

			// Load the vertex array with data.
			mAttributes->mVerticesColor[0].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, iPlaneSize);  // Top left.
			mAttributes->mVerticesColor[0].color = diffuseColor;

			mAttributes->mVerticesColor[1].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mAttributes->mVerticesColor[1].color = diffuseColor;

			mAttributes->mVerticesColor[2].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mAttributes->mVerticesColor[2].color = diffuseColor;

			mAttributes->mVerticesColor[3].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mAttributes->mVerticesColor[3].color = diffuseColor;

			mAttributes->mVerticesColor[4].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mAttributes->mVerticesColor[4].color = diffuseColor;

			mAttributes->mVerticesColor[5].position = XMFLOAT3(iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom right.
			mAttributes->mVerticesColor[5].color = diffuseColor;
#pragma endregion
		}
		break;
	}

	mAttributes->mIndexCount = 6;
	mAttributes->mIndices.resize(mAttributes->mIndexCount);

	// Load the index array with data.
	mAttributes->mIndices[0] = 0;
	mAttributes->mIndices[1] = 1;
	mAttributes->mIndices[2] = 2;
	mAttributes->mIndices[3] = 3;
	mAttributes->mIndices[4] = 4;
	mAttributes->mIndices[5] = 5;
}
//===============================================================================================================================
void PlaneMesh::UpdateBuffers(float planeHeight)
{
	fPlaneHeight = planeHeight;
	
	float halfWidth = 0;
	
	if (bCenterPlane)
	{
		halfWidth = ((float)iPlaneSize - 1.0f) / 2.0f;
		iPlaneSize = iPlaneSize - halfWidth;
	}

	// Update the AABB
	UpdateAABB(mAttributes->mPosition, XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize));

	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			mAttributes->mVerticesNormalTex[0].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, iPlaneSize);  // Top left.
			mAttributes->mVerticesNormalTex[0].normal = XMFLOAT3(0, 1, 0);
			mAttributes->mVerticesNormalTex[0].texture = XMFLOAT2(0.0f, 0.0f);

			mAttributes->mVerticesNormalTex[1].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mAttributes->mVerticesNormalTex[1].normal = XMFLOAT3(0, 1, 0);
			mAttributes->mVerticesNormalTex[1].texture = XMFLOAT2(1.0f, 0.0f);

			mAttributes->mVerticesNormalTex[2].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mAttributes->mVerticesNormalTex[2].normal = XMFLOAT3(0, 1, 0);
			mAttributes->mVerticesNormalTex[2].texture = XMFLOAT2(0.0f, 1.0f);

			mAttributes->mVerticesNormalTex[3].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mAttributes->mVerticesNormalTex[3].normal = XMFLOAT3(0, 1, 0);
			mAttributes->mVerticesNormalTex[3].texture = XMFLOAT2(0.0f, 1.0f);

			mAttributes->mVerticesNormalTex[4].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mAttributes->mVerticesNormalTex[4].normal = XMFLOAT3(0, 1, 0);
			mAttributes->mVerticesNormalTex[4].texture = XMFLOAT2(1.0f, 0.0f);

			mAttributes->mVerticesNormalTex[5].position = XMFLOAT3(iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom right.
			mAttributes->mVerticesNormalTex[5].normal = XMFLOAT3(0, 1, 0);
			mAttributes->mVerticesNormalTex[5].texture = XMFLOAT2(1.0f, 1.0f);
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			// Load the vertex array with data.
			mAttributes->mVerticesTex[0].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, iPlaneSize);  // Top left.
			mAttributes->mVerticesTex[0].texture = XMFLOAT2(0.0f, 0.0f);

			mAttributes->mVerticesTex[1].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mAttributes->mVerticesTex[1].texture = XMFLOAT2(1.0f, 0.0f);

			mAttributes->mVerticesTex[2].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mAttributes->mVerticesTex[2].texture = XMFLOAT2(0.0f, 1.0f);

			mAttributes->mVerticesTex[3].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mAttributes->mVerticesTex[3].texture = XMFLOAT2(0.0f, 1.0f);

			mAttributes->mVerticesTex[4].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mAttributes->mVerticesTex[4].texture = XMFLOAT2(1.0f, 0.0f);

			mAttributes->mVerticesTex[5].position = XMFLOAT3(iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom right.
			mAttributes->mVerticesTex[5].texture = XMFLOAT2(1.0f, 1.0f);
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			XMFLOAT4 diffuseColor = mMeshParameters.material->vDiffuseColor;

			// Load the vertex array with data.
			mAttributes->mVerticesColor[0].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, iPlaneSize);  // Top left.
			mAttributes->mVerticesColor[0].color = diffuseColor;

			mAttributes->mVerticesColor[1].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mAttributes->mVerticesColor[1].color = diffuseColor;

			mAttributes->mVerticesColor[2].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mAttributes->mVerticesColor[2].color = diffuseColor;

			mAttributes->mVerticesColor[3].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mAttributes->mVerticesColor[3].color = diffuseColor;

			mAttributes->mVerticesColor[4].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mAttributes->mVerticesColor[4].color = diffuseColor;

			mAttributes->mVerticesColor[5].position = XMFLOAT3(iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom right.
			mAttributes->mVerticesColor[5].color = diffuseColor;
		}
		break;
	}

	// Rebuild the vertex buffer
	RegenVertexBuffer();
}
//===============================================================================================================================