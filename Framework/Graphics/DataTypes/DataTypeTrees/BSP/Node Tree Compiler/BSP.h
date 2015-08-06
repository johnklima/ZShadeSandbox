//==================================================================================================================================
// BSP.h
//
// This class will generate the binary space partition from the geometry input
// http://www.cs.utah.edu/~jsnider/SeniorProj/BSP1/default.html Part I
// http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm Part II
//==================================================================================================================================
// History
//
// -Created on 1/7/2014 by Dustin Watson
//==================================================================================================================================
#ifndef __BSP_H
#define __BSP_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "ZMath.h"
#include "Vertex.h"
#include "BSPPolygon.h"
#include "BSPNode.h"
#include "BSPParameters.h"

namespace ZShadeSandboxBSP {
/*
	One entry is 1 unit squared of 3D world space
	2 digit represents a NE facing wall
	3 digit represents a NW facing wall
	If a number other than 0 is to any side of the 1 then a wall is not build for that side
	 so each 1 digit represents a 1x1 cube in world space
	This full grid is 20x40
*/
static BYTE BSPMap[] =
{
	0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,2,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1,
	0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
	0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
	0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,3,1,
	0,2,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,
	1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,
	0,1,0,0,0,0,1,2,0,0,0,1,0,0,0,1,0,0,0,1,
	0,1,0,0,0,1,2,0,0,0,0,1,1,0,0,0,0,0,0,1,
	0,1,0,0,0,1,0,0,0,0,0,3,1,0,0,0,0,0,0,1,
	0,1,0,1,1,2,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
	1,2,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,1, 
	1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,
	1,0,0,1,2,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,
	1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

//==================================================================================================================================
//==================================================================================================================================
class BSP
{
public:
	
	BSP();
	
	// This function quickly initializes polygons from the BSPMap for testing
	void InitPolygons();
	
	bool LineOfSight(ZShadeSandboxMath::XMMath3 start, ZShadeSandboxMath::XMMath3 end);
	
	ZShadeSandboxBSP::BSPPolygon* GetPolygons()	{ return mPolygonList; }
	ZShadeSandboxBSP::BSPNode*& GetRootNode()		{ return mBSPRoot; }
	
	// This is used in compiler and render function
	static ZShadeSandboxBSP::EPointPosition::Type ClassifyPoint(ZShadeSandboxMath::XMMath3 pos, ZShadeSandboxBSP::BSPPolygon* plane);
	
private:
	
	ZShadeSandboxBSP::BSPPolygon* AddPolygon(ZShadeSandboxBSP::BSPPolygon* parent, vector<ZShadeSandboxMesh::VertexTex> vertices);
	//BSPPolygon* AddPolygon(BSPPolygon* parent, ZShadeSandboxMesh::VertexTex* vertices, int vertexCount = 4);
	
	void Build(ZShadeSandboxBSP::BSPNode* node, ZShadeSandboxBSP::BSPPolygon* polygonList);
	
	ZShadeSandboxBSP::EPointPosition::Type ClassifyPolygon(ZShadeSandboxBSP::BSPPolygon* plane, ZShadeSandboxBSP::BSPPolygon* polygon);
	
	bool GetIntersect
	(	ZShadeSandboxMath::XMMath3& linestart
	,	ZShadeSandboxMath::XMMath3& lineend
	,	ZShadeSandboxMath::XMMath3& vertex
	,	ZShadeSandboxMath::XMMath3& normal
	,	ZShadeSandboxMath::XMMath3& intersection
	,	float& percentage
	);
	
	void SplitPolygon(ZShadeSandboxBSP::BSPPolygon* polygon, ZShadeSandboxBSP::BSPPolygon* plane, ZShadeSandboxBSP::BSPPolygon* frontSplit, ZShadeSandboxBSP::BSPPolygon* backSplit);
	
	ZShadeSandboxBSP::BSPPolygon* LocateSplitter(ZShadeSandboxBSP::BSPPolygon* polygonList);
	
	bool LineOfSight(ZShadeSandboxMath::XMMath3 start, ZShadeSandboxMath::XMMath3 end, BSPNode* node);
	
private:
	
	// LinkedList of Polygons in the BSP
	ZShadeSandboxBSP::BSPPolygon* mPolygonList;
	
	// List of nodes in the BSP
	ZShadeSandboxBSP::BSPNode* mBSPRoot;
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__BSP_H