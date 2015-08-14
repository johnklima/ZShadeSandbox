//==================================================================================================================================
// PolygonMesh
//
//==================================================================================================================================
// History
//
// -Created on 6/26/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __POLYGONMESH_H
#define __POLYGONMESH_H
//==================================================================================================================================
//==================================================================================================================================

#include <vector>
#include "D3D.h"
#include "ZMath.h"
#include "Polygon.h"
#include "Vertex.h"
#include "MeshAttributes.h"
#include "Camera.h"
#include "ShaderMaterial.h"
#include "MeshRenderParameters.h"

//==================================================================================================================================
//==================================================================================================================================
namespace ZShadeSandboxMesh {
class PolygonMesh
{
public:
	
	PolygonMesh(ZShadeSandboxMath::Polygon* poly, ZShadeSandboxLighting::ShaderMaterial* material);
	
	bool& Wireframe() { return m_Wireframe; }
	
	XMFLOAT3& Position();
	XMFLOAT3& Scale();
	XMFLOAT3& Rotate();
	
	ZShadeSandboxMath::Polygon*& Poly() { return mPolygon; }
	ZShadeSandboxMath::Polygon* Poly() const { return mPolygon; }
	
	void BuildIndexBuffer();
	void BuildVertexBuffer(ZShadeSandboxMesh::EVertexType::Type type);
	void ReBuildVertexBuffer(ZShadeSandboxMesh::EVertexType::Type type);
	
	void Render(ZShadeSandboxMesh::MeshRenderParameters mrp);
	
protected:
	
	bool m_Wireframe;
	
	ZShadeSandboxLighting::ShaderMaterial* mMaterial;
	
	ZShadeSandboxMath::Polygon* mPolygon;
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__POLYGONMESH_H