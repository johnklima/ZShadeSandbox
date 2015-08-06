#include "BSPPolygon.h"
#include "XMMath.h"
using ZShadeSandboxBSP::BSPPolygon;
//==================================================================================================================================
//==================================================================================================================================
BSPPolygon::BSPPolygon()
:   ZShadeSandboxMath::Polygon()
,	mNext(0)
{
}
//==================================================================================================================================
void BSPPolygon::CreateMesh(D3D* d3d, ZShadeSandboxLighting::ShaderMaterial* material)
{
	mMesh = new ZShadeSandboxMesh::PolygonMesh(this, material);
	Attributes()->mD3D = d3d;
}
//==================================================================================================================================
void BSPPolygon::Create(vector<ZShadeSandboxMesh::VertexTex> vertices)
{
	ZShadeSandboxMath::Polygon::Create(vertices);
	
	bUsedAsSplitter = false;
	sMaterialName = "";
}
//==================================================================================================================================
void BSPPolygon::CreateSplitPolygon(vector<ZShadeSandboxMesh::VertexTex> vertices, int counter)
{
	attributes->mVertexCount = counter;
	attributes->mIndexCount = (attributes->mVertexCount - 2) * 3;
	attributes->mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexTex);
	attributes->mTriangleCount = attributes->mVertexCount / 3;

	InitVertexList(vertices);
	InitIndexList();
	
	//
	// Add the vertices
	//

	for (int vertexID = 0; vertexID < attributes->mVertexCount; vertexID++)
	{
		attributes->mVerticesTex[vertexID] = vertices[vertexID];
	}

	//
	// Add the indices
	//

	UINT v0, v1, v2;

	for (int indexID = 0; indexID < attributes->mIndexCount / 3; indexID++)
	{
		if (indexID == 0)
		{
			v0 = 0;
			v1 = 1;
			v2 = 2;
		}
		else
		{
			v1 = v2;
			v2++;
		}

		attributes->mIndices[(indexID * 3) + 0] = v0;
		attributes->mIndices[(indexID * 3) + 1] = v1;
		attributes->mIndices[(indexID * 3) + 2] = v2;
	}

	//
	// Generate the polygon normal
	//

	/*ZShadeSandboxMath::XMMath3 v0Pos(attributes->mVerticesTex[attributes->mIndices[0]].position);
	ZShadeSandboxMath::XMMath3 v1Pos(attributes->mVerticesTex[attributes->mIndices[1]].position);
	ZShadeSandboxMath::XMMath3 vLastPos(attributes->mVerticesTex[attributes->mIndices[attributes->mIndexCount - 1]].position);
	ZShadeSandboxMath::XMMath3 edge1 = v1Pos - v0Pos;
	ZShadeSandboxMath::XMMath3 edge2 = vLastPos - v0Pos;
	ZShadeSandboxMath::XMMath3 edgeNormal = edge1.Cross(edge2);
	edgeNormal.Normalize();
	normal = edgeNormal;*/
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 BSPPolygon::GetVertexPosition(int index)
{
	return Polygon::GetVertexPosition(index, ZShadeSandboxMesh::EVertexType::Type::VT_Tex);
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath2 BSPPolygon::GetVertexTexture(int index)
{
	return Polygon::GetVertexTexture(index, ZShadeSandboxMesh::EVertexType::Type::VT_Tex);
}
//==================================================================================================================================
ZShadeSandboxMesh::VertexTex BSPPolygon::GetVertex(int index)
{
	return GetVertexTex(index);
}
//==================================================================================================================================
void BSPPolygon::BuildIndexBuffer()
{
	mMesh->BuildIndexBuffer();
}
//==================================================================================================================================
void BSPPolygon::BuildVertexBuffer()
{
	mMesh->BuildVertexBuffer(ZShadeSandboxMesh::EVertexType::Type::VT_Tex);
}
//==================================================================================================================================
void BSPPolygon::ReBuildVertexBuffer()
{
	mMesh->ReBuildVertexBuffer(ZShadeSandboxMesh::EVertexType::Type::VT_Tex);
}
//==================================================================================================================================
void BSPPolygon::ClearVertices()
{
	attributes->mVerticesTex.clear();
}
//==================================================================================================================================
void BSPPolygon::ClearIndices()
{
	attributes->mIndices.clear();
}
//==================================================================================================================================