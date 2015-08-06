//==================================================================================================================================
// GJK (Gilbert–Johnson–Keerthi distance algorithm)
// 
// http://www.codezealot.org/archives/tag/gjk
//==================================================================================================================================
// History
//
// -Created on 12/10/2014 by Dustin Watson
//==================================================================================================================================
#ifndef __GJK_H
#define __GJK_H
//==============================================================================================================================
//==============================================================================================================================
#include "ZMath.h"
#include <math.h>
#include "EngineOptions.h"
#include "PhysicsBody3D.h"
class PhysicsBody;
//==============================================================================================================================
//==============================================================================================================================
//==============================================================================================================================
#define TOLERANCE	0.00001
//==============================================================================================================================
//==============================================================================================================================
/*
GJK relies on two functions:

Support(shape, \vec{d}), which returns the point on shape which has the highest dot product with \vec{d}.
NearestSimplex(s), which takes a simplex s and returns the simplex on s closest to the origin, and a direction
  toward the origin normal to the new simplex. If s itself contains the origin, NearestSimplex accepts s and the
  two shapes are determined to intersect.
The simplicities handled by NearestSimplex may each be any simplex sub-space of Rn. For example in 3D, they may
be a point, a line segment, a triangle, or a tetrahedron; each defined by 1, 2, 3, or 4 points respectively.

Will use two PhysicsBody objects for the collision since they represent a polygon in my engine
Two types of collision and corresponding CSO face are: face-vertex and edge-edge
*/
//==============================================================================================================================
struct GJK_MTV
{
	ZShadeSandboxMath::Vec3<XMFLOAT3> normal;
	float penetration;
};
//==============================================================================================================================
struct Edge
{
	ZShadeSandboxMath::Vec3<XMFLOAT3> normal;
	float distance;
	int index;
};
//==============================================================================================================================
enum Simplex
{
	LINE,
	TRIANGLE,
	TETRAHEDRON
};
//==============================================================================================================================
class SimplexContainer
{
	ZShadeSandboxMath::Vec3<XMFLOAT3> m_simplex[4];
	
public:
	
	int num;
	
	ZShadeSandboxMath::Vec3<XMFLOAT3> A;
	ZShadeSandboxMath::Vec3<XMFLOAT3> B;
	ZShadeSandboxMath::Vec3<XMFLOAT3> C;
	ZShadeSandboxMath::Vec3<XMFLOAT3> D;
	
	SimplexContainer() : A(m_simplex[0]), B(m_simplex[0]), C(m_simplex[0]), D(m_simplex[0])
	{
		clear();
	}
	
	void clear() { num = 0; }
	
	void Add(ZShadeSandboxMath::Vec3<XMFLOAT3> point)
	{
		num = (std::fmin)(num + 1, 4);
		
		for (int i = num - 1; i > 0; i--)
		{
			m_simplex[i] = m_simplex[i - 1];
			m_simplex[0] = point;
		}
	}
	
	void Set(ZShadeSandboxMath::Vec3<XMFLOAT3> a, ZShadeSandboxMath::Vec3<XMFLOAT3> b, ZShadeSandboxMath::Vec3<XMFLOAT3> c, ZShadeSandboxMath::Vec3<XMFLOAT3> d)
	{
		num = 4;
		
		this->A = a;
		this->B = b;
		this->C = c;
		this->D = d;
	}
	
	void Set(ZShadeSandboxMath::Vec3<XMFLOAT3> a, ZShadeSandboxMath::Vec3<XMFLOAT3> b, ZShadeSandboxMath::Vec3<XMFLOAT3> c)
	{
		num = 3;

		this->A = a;
		this->B = b;
		this->C = c;
	}

	void Set(ZShadeSandboxMath::Vec3<XMFLOAT3> a, ZShadeSandboxMath::Vec3<XMFLOAT3> b)
	{
		num = 2;

		this->A = a;
		this->B = b;
	}

	void Set(ZShadeSandboxMath::Vec3<XMFLOAT3> a)
	{
		num = 1;

		this->A = a;
	}
};
//==============================================================================================================================
struct TriangleSimplex
{
	std::vector< ZShadeSandboxMath::TrianglePrimitive > tri_simplex;
	
	void AddTriangle(ZShadeSandboxMath::TrianglePrimitive tri)
	{
		tri_simplex.push_back( tri );
	}
	
	void AddTriangle(ZShadeSandboxMath::Vec3<XMFLOAT3> p1, ZShadeSandboxMath::Vec3<XMFLOAT3> p2, ZShadeSandboxMath::Vec3<XMFLOAT3> p3)
	{
		tri_simplex.push_back(ZShadeSandboxMath::TrianglePrimitive(p1, p2, p3));
	}
	
	ZShadeSandboxMath::TrianglePrimitive& operator [] (int i)
	{
		return tri_simplex[i];
	}

	void RemoveTriangle(ZShadeSandboxMath::TrianglePrimitive tri)
	{
		std::vector< ZShadeSandboxMath::TrianglePrimitive >::iterator i;
		std::vector< ZShadeSandboxMath::TrianglePrimitive >::iterator iter = tri_simplex.begin();
		for (; iter != tri_simplex.end(); iter++)
		{
			ZShadeSandboxMath::TrianglePrimitive t = (*iter);

			if (t == tri)
				i = iter;
		}

		tri_simplex.erase(i);
	}
	
	void RemoveTriangle(ZShadeSandboxMath::Vec3<XMFLOAT3> p1, ZShadeSandboxMath::Vec3<XMFLOAT3> p2, ZShadeSandboxMath::Vec3<XMFLOAT3> p3)
	{
		std::vector< ZShadeSandboxMath::TrianglePrimitive >::iterator i;
		std::vector< ZShadeSandboxMath::TrianglePrimitive >::iterator iter = tri_simplex.begin();
		for (; iter != tri_simplex.end(); iter++)
		{
			ZShadeSandboxMath::TrianglePrimitive t = (*iter);

			if (t == ZShadeSandboxMath::TrianglePrimitive(p1, p2, p3))
				i = iter;
		}

		tri_simplex.erase(i);
	}
	
	void clear()
	{
		tri_simplex.clear();
	}
};
//==============================================================================================================================
class GJK
{
	EngineOptions* m_EngineOptions;

	GJK_MTV* m_mtv;
	
	vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > mesh1Points;
	vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > mesh2Points;

	// There are three simplexes to test: line segment, triangle and tetrahedron
	// A single simplex of a point is skipped
	SimplexContainer m_Simplex;
	
	TriangleSimplex m_EPASimplex3D;
	
	// This function will find a minkowski point furthest from both meshes
	ZShadeSandboxMath::Vec3<XMFLOAT3> Support(ZShadeSandboxMath::Vec3<XMFLOAT3> direction);
	
	// This will check the simplex to determine if a collision if found
	bool CheckSimplex(ZShadeSandboxMath::Vec3<XMFLOAT3>& direction);
	
	// Checks the face of a tetrahedron
	bool CheckFace(ZShadeSandboxMath::Vec3<XMFLOAT3>& direction);
	
	struct PointWeight
	{
		ZShadeSandboxMath::Vec3<XMFLOAT3> point;
		float distance;
	};
	
	// Returns the furthest point from a set of points in a direction
	ZShadeSandboxMath::Vec3<XMFLOAT3> FindMax(std::vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > meshPoints, ZShadeSandboxMath::Vec3<XMFLOAT3> direction);
	
	// Helper function to find the distance from a point to a plane normal
	float find_distance(ZShadeSandboxMath::Vec3<XMFLOAT3> point, ZShadeSandboxMath::Vec3<XMFLOAT3> faceNormal);
	
	void CheckEdgeDistance(ZShadeSandboxMath::Vec3<XMFLOAT3> A, ZShadeSandboxMath::Vec3<XMFLOAT3> B, int j, Edge& closest);
	
	Edge FindClosestEdge2D();
	
	ZShadeSandboxMath::TrianglePrimitive FindClosestTriangle();
	Edge FindClosestEdge(ZShadeSandboxMath::TrianglePrimitive tri);
	
	// Calculate the expanding polytope algorithm to find the collision information
	void EPA(PhysicsBody3D* body1, PhysicsBody3D* body2);
	
public:
	
	GJK(EngineOptions* eo);
	
	GJK_MTV* CollisionInfo() { return m_mtv; }
	
	// Determines if two polygonal mesh physics bodies will intersect
	bool Intersect(PhysicsBody3D* body1, PhysicsBody3D* body2);
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__GJK_H