//==================================================================================================================================
// BSPPortal
//
// http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
//==================================================================================================================================
// History
//
// -Created on 6/27/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __BSPPORTAL_H
#define __BSPPORTAL_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "Vertex.h"
#include "XMMath.h"
#include "BSPPolygon.h"

//==================================================================================================================================
//==================================================================================================================================
namespace ZShadeSandboxBSP {
class BSPPortal : public ZShadeSandboxBSP::BSPPolygon
{
public:
	
	BSPPortal();
	
	//void SetNextPortal(BSPPortal* n) { next = n; }
	//void SetPrevPortal(BSPPortal* p) { prev = p; }
	BSPPortal*& NextPortal() { return next; }
	BSPPortal*& PrevPortal() { return prev; }
	BYTE& LeafCount() { return leafCount; }
	long& LeafOwnerArray(int i) { return leafOwnerArray[i]; }
	
	BSPPortal* NextPortal() const { return next; }
	BSPPortal* PrevPortal() const { return prev; }
	BYTE LeafCount() const { return leafCount; }
	long LeafOwnerArray(int i) const { return leafOwnerArray[i]; }
	
	long* LeafOwnerArray() { return leafOwnerArray; }

	void IncLeafCount() { leafCount++; }

private:
	
	BSPPortal* next;
	BSPPortal* prev;
	BYTE leafCount;
	long leafOwnerArray[2];
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__BSPPORTAL_H