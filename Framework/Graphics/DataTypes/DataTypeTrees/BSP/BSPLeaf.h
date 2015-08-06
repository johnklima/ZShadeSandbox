//==================================================================================================================================
// BSPLeaf
//
// http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
//==================================================================================================================================
// History
//
// -Created on 6/25/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __BSPLEAF_H
#define __BSPLEAF_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "AABB.h"

//==================================================================================================================================
//==================================================================================================================================
namespace ZShadeSandboxBSP {
struct BSPLeaf
{
	long startPolygon;
	long endPolygon;
	long portalIndexList[50];
	long numberOfPortals;
	long pvsIndex;
	ZShadeSandboxMath::AABB boundingBox;
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__BSPLEAF_H