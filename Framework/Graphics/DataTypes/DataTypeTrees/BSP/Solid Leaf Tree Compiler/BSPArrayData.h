//==================================================================================================================================
// BSPArrayData
//
// http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
//==================================================================================================================================
// History
//
// -Created on 6/27/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __BSPARRAYDATA_H
#define __BSPARRAYDATA_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "Vertex.h"
#include "XMMath.h"
#include "BSPPolygon.h"
#include "BSPNode.h"
#include "BSPLeaf.h"
#include "BSPPlane.h"
#include "BSPPortal.h"

//==================================================================================================================================
//==================================================================================================================================
namespace ZShadeSandboxBSP {
class BSPArrayData
{
	static BSPArrayData* instance;
	
public:
	
	long			PolygonCapasityCount;
	long			NodeCapasityCount;
	long			LeafCapasityCount;
	long			PlaneCapasityCount;
	long			PortalCapasityCount;
	
	long			lNumberOfPolygons;
	long			lNumberOfNodes;
	long			lNumberOfLeaves;
	long			lNumberOfPlanes;
	long			lNumberOfPortals;
	
	long			bytesPerSet;
	long			PVSCompressedSize;
	
	BSPPolygon		*polygonList;
	BSPPolygon		*polygonArray;
	BSPNode			*nodeArray;
	BSPLeaf			*leafArray;
	BSPPlane		*planeArray;
	BYTE			*pvsData;
	BSPPortal		**portalArray;
	
	BSPArrayData();
	
	static BSPArrayData* Instance();
	
	void AllocateInitalSize();
	
	void IncreasePolygonCount();
	void IncreaseNodeCount();
	void IncreaseLeafCount();
	void IncreasePlaneCount();
	void IncreasePortalCount();
	
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__BSPARRAYDATA_H