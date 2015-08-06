//==================================================================================================================================
// BSPPolygon.h
//
//==================================================================================================================================
// History
//
// -Created on 1/7/2014 by Dustin Watson
//==================================================================================================================================
#ifndef __BSPPOLYGON_H
#define __BSPPOLYGON_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include <vector>
#include "D3D.h"
#include "ZMath.h"
#include "XMMath.h"
#include "Polygon.h"
#include "ShaderMaterial.h"

//==================================================================================================================================
//==================================================================================================================================
namespace ZShadeSandboxBSP {
class BSPPolygon : public ZShadeSandboxMath::Polygon
{
public:
	
	BSPPolygon();
	
	void CreateMesh(D3D* d3d, ShaderMaterial* material);
	
	void Create(vector<ZShadeSandboxMesh::VertexTex> vertices);
	
	// Create a polygon from the BSP splitter
	void CreateSplitPolygon(vector<ZShadeSandboxMesh::VertexTex> vertices, int counter);
	
	ZShadeSandboxMesh::VertexTex GetVertex(int index);
	
	ZShadeSandboxMath::XMMath3 GetVertexPosition(int index);
	ZShadeSandboxMath::XMMath2 GetVertexTexture(int index);
	
	void BuildIndexBuffer();
	void BuildVertexBuffer();
	void ReBuildVertexBuffer();
	
	void ClearVertices();
	void ClearIndices();
	
	void SetNextPolygon(BSPPolygon* poly)	{ mNext = poly; }
	
	BSPPolygon* NextPolygon()				{ return mNext; }
	
	ZShadeSandboxMesh::PolygonMesh*& Mesh()	{ return mMesh; }
	
	bool& UsedAsSplitter()					{ return bUsedAsSplitter; }
	bool UsedAsSplitter() const				{ return bUsedAsSplitter; }
	
	string& MaterialName()					{ return sMaterialName; }
	string MaterialName() const				{ return sMaterialName; }
	
private:
	
	bool bUsedAsSplitter;
	string sMaterialName;
	
	BSPPolygon*		mNext;
	ZShadeSandboxMesh::PolygonMesh*	mMesh;
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__BSPPOLYGON_H