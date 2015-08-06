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
#include "HullVertex.h"
#include "HullFace.h"
#include "LNode.h"
//===============================================================================================================================
struct HalfEdge : public LNode<HalfEdge>
{
	// Vertex associated with this half-edge
	HullVertex* vertex;

	// Triangular face associated with this half-edge
	HullFace* face;

	// Half-edge associated with the opposite triangle
	HalfEdge* opposite;
	
	//HalfEdge* prev;
	//HalfEdge* next;
	
	HalfEdge()
	{
		data = this;
	}

	HalfEdge(HullVertex* v, HullFace* f)
	{
		data = this;
		vertex = v;
		face = f;
	}

	HullVertex* head()
	{
		return vertex;
	}

	HullVertex* tail()
	{
		return prev != nullptr ? prev->data->vertex : nullptr;
	}

	HullFace* oppositeFace()
	{
		return opposite != nullptr ? opposite->face : nullptr;
	}

	// Produces a string identifying this half-edge by the point index value of its tail and head
	BetterString getVertexString()
	{
		if (tail() != nullptr)
		{
			return "" + ZShadeSandboxGlobal::Convert::ConvertIntToString(tail()->index) + "-" + ZShadeSandboxGlobal::Convert::ConvertIntToString(head()->index);
		}
		else
		{
			return "?-" + head()->index;
		}
	}

	// Returns the length of this half-edge
	float length()
	{
		if (tail() != nullptr)
		{
			Vec3<XMFLOAT3> p1 = head()->point;
			Vec3<XMFLOAT3> p2 = tail()->point;
			return p1.Distance(p2);
		}

		return -1;
	}

	float lengthSquared()
	{
		if (tail() != nullptr)
		{
			Vec3<XMFLOAT3> p1 = head()->point;
			Vec3<XMFLOAT3> p2 = tail()->point;
			return p1.DistanceSquared(p2);
		}

		return -1;
	}
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__HALFEDGE_H