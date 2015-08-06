#include "BSPArrayData.h"
using ZShadeSandboxBSP::BSPArrayData;
//==================================================================================================================================
//==================================================================================================================================
BSPArrayData* BSPArrayData::instance = 0;
//==================================================================================================================================
BSPArrayData::BSPArrayData()
{
	PolygonCapasityCount = 100;
	NodeCapasityCount = 100;
	LeafCapasityCount = 100;
	PlaneCapasityCount = 100;
	PortalCapasityCount = 100;

	lNumberOfPolygons = 0;
	lNumberOfNodes = 0;
	lNumberOfLeaves = 0;
	lNumberOfPlanes = 0;
	lNumberOfPortals = 0;
	
	AllocateInitalSize();
}
//==================================================================================================================================
BSPArrayData* BSPArrayData::Instance()
{
	if (instance == 0)
		instance = new BSPArrayData();
	return instance;
}
//==================================================================================================================================
void BSPArrayData::AllocateInitalSize()
{
	polygonArray = (ZShadeSandboxBSP::BSPPolygon*)malloc(PolygonCapasityCount * sizeof(ZShadeSandboxBSP::BSPPolygon));
	ZeroMemory(polygonArray, sizeof(ZShadeSandboxBSP::BSPPolygon) * PolygonCapasityCount);
	
	nodeArray = (ZShadeSandboxBSP::BSPNode*)malloc(NodeCapasityCount * sizeof(ZShadeSandboxBSP::BSPNode));
	ZeroMemory(nodeArray, sizeof(ZShadeSandboxBSP::BSPNode) * NodeCapasityCount);
	
	leafArray = (ZShadeSandboxBSP::BSPLeaf*)malloc(LeafCapasityCount * sizeof(ZShadeSandboxBSP::BSPLeaf));
	ZeroMemory(leafArray, sizeof(ZShadeSandboxBSP::BSPLeaf) * LeafCapasityCount);
	
	planeArray = (ZShadeSandboxBSP::BSPPlane*)malloc(PlaneCapasityCount * sizeof(ZShadeSandboxBSP::BSPPlane));
	ZeroMemory(planeArray, sizeof(ZShadeSandboxBSP::BSPPlane) * PlaneCapasityCount);
	
	portalArray = (ZShadeSandboxBSP::BSPPortal**)malloc(PortalCapasityCount * sizeof(ZShadeSandboxBSP::BSPPortal*));
	ZeroMemory(portalArray, sizeof(ZShadeSandboxBSP::BSPPortal*) * PortalCapasityCount);
}
//==================================================================================================================================
void BSPArrayData::IncreasePolygonCount()
{
	lNumberOfPolygons++;
	
	if (lNumberOfPolygons == PolygonCapasityCount)
	{
		PolygonCapasityCount += 100;
		
		polygonArray = (ZShadeSandboxBSP::BSPPolygon*)realloc(polygonArray, PolygonCapasityCount * sizeof(ZShadeSandboxBSP::BSPPolygon));
	}
}
//==================================================================================================================================
void BSPArrayData::IncreaseNodeCount()
{
	lNumberOfNodes++;
	
	if (lNumberOfNodes == NodeCapasityCount)
	{
		NodeCapasityCount += 100;
		
		nodeArray = (BSPNode*)realloc(nodeArray, NodeCapasityCount * sizeof(ZShadeSandboxBSP::BSPNode));
	}
}
//==================================================================================================================================
void BSPArrayData::IncreaseLeafCount()
{
	lNumberOfLeaves++;
	
	if (lNumberOfLeaves == LeafCapasityCount)
	{
		LeafCapasityCount += 100;
		
		leafArray = (ZShadeSandboxBSP::BSPLeaf*)realloc(leafArray, LeafCapasityCount * sizeof(ZShadeSandboxBSP::BSPLeaf));
	}
}
//==================================================================================================================================
void BSPArrayData::IncreasePlaneCount()
{
	lNumberOfPlanes++;
	
	if (lNumberOfPlanes == PlaneCapasityCount)
	{
		PlaneCapasityCount += 100;
		
		planeArray = (ZShadeSandboxBSP::BSPPlane*)realloc(planeArray, PlaneCapasityCount * sizeof(ZShadeSandboxBSP::BSPPlane));
	}
}
//==================================================================================================================================
void BSPArrayData::IncreasePortalCount()
{
	lNumberOfPortals++;

	if (lNumberOfPortals == PortalCapasityCount)
	{
		PortalCapasityCount += 100;

		portalArray = (ZShadeSandboxBSP::BSPPortal**)realloc(portalArray, PortalCapasityCount * sizeof(ZShadeSandboxBSP::BSPPortal));
	}
}
//==================================================================================================================================
