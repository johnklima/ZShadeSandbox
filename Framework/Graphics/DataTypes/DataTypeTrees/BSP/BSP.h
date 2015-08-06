//==================================================================================================================================
// BSP
//
// http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
//==================================================================================================================================
// History
//
// -Created on 6/25/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __BSP_H
#define __BSP_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include <string>
using std::string;
#include "BSPNode.h"
#include "BSPLeaf.h"
#include "BSPPlane.h"
#include "BSPPolygon.h"
#include "BSPPortal.h"
#include "BSPParameters.h"
#include "AABB.h"
#include "BSPPortalGenerator.h"
#include "BSPPVSGenerator.h"

//==================================================================================================================================
//==================================================================================================================================
namespace ZShadeSandboxBSP {
class BSP
{
public:
	
	BSP();
	
	void Load(string levelFilename);

	void InitPolygons();
	
	void Build(long node, ZShadeSandboxBSP::BSPPolygon* polygonList);
	
	static ZShadeSandboxBSP::EPointPosition::Type ClassifyPoint(ZShadeSandboxMath::XMMath3 pos, ZShadeSandboxBSP::BSPPlane* plane);
	
private:
	
	void SplitPolygon(ZShadeSandboxBSP::BSPPolygon* polygon, ZShadeSandboxBSP::BSPPlane* plane, ZShadeSandboxBSP::BSPPolygon* frontSplit, ZShadeSandboxBSP::BSPPolygon* backSplit);
	ZShadeSandboxBSP::EPointPosition::Type ClassifyPolygon(ZShadeSandboxBSP::BSPPlane* plane, ZShadeSandboxBSP::BSPPolygon* polygon);
	void DeletePolygon(ZShadeSandboxBSP::BSPPolygon* polygon);
	void CalculateAABB(ZShadeSandboxMath::AABB* box, ZShadeSandboxBSP::BSPPolygon* polygonList);
	long LocateSplitter(ZShadeSandboxBSP::BSPPolygon* polygonList);
	ZShadeSandboxBSP::BSPPolygon* AddPolygon(ZShadeSandboxBSP::BSPPolygon* parent, ZShadeSandboxMesh::VertexTex* vertices, int vertexCount);
	
	/**
	 *	File Format:
	 *
	 *	WORD : # polygons in file
	 *  For Every Polygon N
	 *  WORD : # vertices in polygon N
	 *  For Every Vertex M in the polygon N
	 *	VertexTex : Vertex Number M
	 *	string : Name of material for Polygon N
	 *
	 *	Loads a linked list of polygons from a file (.bsp format)
	 */
	BSPPolygon* LoadBSP(string levelFilename);
	
	BSPPortalGenerator* mBSPPortalGenerator;
	BSPPVSGenerator* mBSPPVSGenerator;
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__BSPLEAF_H