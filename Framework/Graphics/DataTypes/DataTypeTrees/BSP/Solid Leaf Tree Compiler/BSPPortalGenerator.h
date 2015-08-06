//==================================================================================================================================
// BSPPortalGenerator
//
// http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
//==================================================================================================================================
// History
//
// -Created on 6/27/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __BSPPORTALGENERATOR_H
#define __BSPPORTALGENERATOR_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "Vertex.h"
#include "XMMath.h"
#include "BSPPortal.h"
#include "BSPParameters.h"

//==================================================================================================================================
//==================================================================================================================================
namespace ZShadeSandboxBSP {
class BSPPortalGenerator
{
	struct NodeStack
	{
		long node;
		BYTE jumpBackPoint;
	};

public:
	
	BSPPortalGenerator();
	
	void Build();
	
	static ZShadeSandboxBSP::EPointPosition::Type ClassifyPortal(ZShadeSandboxBSP::BSPPlane* plane, ZShadeSandboxBSP::BSPPortal* portal);
	
	static void SplitPortal(ZShadeSandboxBSP::BSPPortal* portal, ZShadeSandboxBSP::BSPPlane* plane, ZShadeSandboxBSP::BSPPortal* frontSplit, ZShadeSandboxBSP::BSPPortal* backSplit);
	
private:
	
	ZShadeSandboxBSP::BSPPortal* ClipPortal(long node, ZShadeSandboxBSP::BSPPortal* portal);
	ZShadeSandboxBSP::BSPPortal* CalculateInitialPortal(long node);
	void SplitPortalPolygon(ZShadeSandboxBSP::BSPPortal* portal, ZShadeSandboxBSP::BSPPlane* plane, ZShadeSandboxBSP::BSPPortal* frontSplit, ZShadeSandboxBSP::BSPPortal* backSplit);
	void DeletePortal(ZShadeSandboxBSP::BSPPortal* portal);
	void RemovePortalFromList(ZShadeSandboxBSP::BSPPortal* portal);
	bool CheckDuplicatePortal(ZShadeSandboxBSP::BSPPortal* portal, int& index);
	void GetPolygonBounds(ZShadeSandboxBSP::BSPPortal* portal, ZShadeSandboxMath::XMMath3& bmin, ZShadeSandboxMath::XMMath3& bmax);
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__BSPPORTALGENERATOR_H