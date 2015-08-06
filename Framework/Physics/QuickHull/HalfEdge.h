//===============================================================================================================================
// HalfEdge
//
//===============================================================================================================================
// History
//
// -Created on 4/2/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __HALFEDGE_H
#define __HALFEDGE_H
//===============================================================================================================================
//===============================================================================================================================

#include "ZMath.h"
#include "LNode.h"

namespace ZShadeSandboxMath {
	class HullVertex;
	class HullFace;
}

//===============================================================================================================================
namespace ZShadeSandboxMath {
struct HalfEdge
{
	// Vertex associated with this half-edge
	ZShadeSandboxMath::HullVertex* vertex;

	// Triangular face associated with this half-edge
	ZShadeSandboxMath::HullFace* face;

	// Half-edge associated with the opposite triangle
	HalfEdge* opposite;
	
	HalfEdge* next;
	HalfEdge* prev;
	
	HalfEdge();
	HalfEdge(HullVertex* v, HullFace* f);

	ZShadeSandboxMath::HullVertex* head();
	ZShadeSandboxMath::HullVertex* tail();
	ZShadeSandboxMath::HullFace* oppositeFace();

	// Produces a string identifying this half-edge by the point index value of its tail and head
	BetterString getVertexString();

	// Returns the length of this half-edge
	float length();
	float lengthSquared();
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__HALFEDGE_H