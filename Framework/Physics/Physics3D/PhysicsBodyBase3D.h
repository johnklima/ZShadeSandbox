//==================================================================================================================================
// PhysicsBodyBase3D.h
//
//==================================================================================================================================
// History
//
// -Created on 12/17/2014 by DW
// -Modified on 1/28/2015 by DW Changed name to PhysicsBodyBase
// -Modified on 4/13/2015 by DW Added AABB collision data
//==================================================================================================================================
#ifndef __PHYSICSBODYBASE3D_H
#define __PHYSICSBODYBASE3D_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "ZMath.h"
#include "XMMath.h"
#include "PhysicsType.h"
#include "SurfaceType.h"
#include "Vertex.h"
#include "BodySurface.h"
#include "AABB.h"
#include "BoundingBox.h"
#include "Camera.h"

namespace ZShadeSandboxMesh {
	class QuadMesh;
}

//==================================================================================================================================
//==================================================================================================================================
class PhysicsBodyBase3D
{
protected:

	D3D*           m_D3DSystem;
	EngineOptions* m_EngineOptions;
	
	// Is this a static or dynamic physics body
	PhysicsType eType;
	
	// Axis aligned bounding box of the body
	ZShadeSandboxMath::AABB* boundary;
	
	bool bColorHull;
	bool bTextureHull;
	
	XMFLOAT3 mPosition;
	
	ZShadeSandboxMath::XMMath4 vColor;
	
	// World matrix of the physics body (Not sure if need this here)
	XMFLOAT4X4 mWorld;
	
	// Convex hull algorithm
	//QuickHull mConvexHull;
	
	// The vertices from the mesh
	vector<ZShadeSandboxMesh::VertexTex> mMeshPoints;
	
	// The vertices that make up the physics body
	vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > mPoints;
	
	// The Convex hull of the physics body ready to render
	vector<ZShadeSandboxMesh::VertexTex> mRenderHull;
	
	// The Convex hull of the physics body
	vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > mHull;
	
	// The center of the physics body
	XMFLOAT3 mCenter;
	
	// Is the convex hull created
	bool m_hullCreated;
	
	// Is the convex hull needed
	bool m_hullNeeded;
	
public:
	
	PhysicsBodyBase3D();
	PhysicsBodyBase3D(D3D* d3d);
	
	void SetD3D(D3D* d3d);

	// Returns whether this is a static or dynamic physics body
	PhysicsType& EPhysicsType() { return eType; }
	PhysicsType EPhysicsType() const { return eType; }
	
	// Check whether or not a point will enter the AABB of this body
	bool IntersectsAABB(XMFLOAT3 point);
	// Gets the distance from the AABB of this body to a point
	float DistanceAABBToPoint(XMFLOAT3 point);
	
	// Creates an Axis-Aligned bounding box by displacing a point by a value
	void InitAABB(XMFLOAT3 center, XMFLOAT3 fScale);
	
	ZShadeSandboxMath::AABB* GetAABB() { return boundary; }

	void ColorTheHull(ZShadeSandboxMath::XMMath4 color);
	void TextureTheHull();
	
	// Returns the center of the physics body
	XMFLOAT3 Center() { return mCenter; }
	
	// Returns the position in 3D space of the body
	XMFLOAT3 Position() { return mPosition; }
	
	// Creates the convex hull of the physics body
	void CreateConvexHull();
	
	// Determines if the convex hull was created
	bool IsHullCreated() { return m_hullCreated; }
	
	// Determine if the convex hull is needed
	bool IsHullNeeded() { return m_hullNeeded; }
	void SetHullNeeded(bool need) { m_hullNeeded = need; }
	
	// Adds the points to the physics body from a mesh that has a renderable vertex
	void AddPoints(vector<ZShadeSandboxMesh::VertexTex> points);
	
	// Adds the points to the physics body from any mesh
	void AddPoints(vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > points);
	
	// Obtain a point into the point list, useful for something like a 2D Circle
	void AddPoint(ZShadeSandboxMath::Vec3<XMFLOAT3> point);
	
	// Gets the convex hull of the physics body for a vertex shader
	vector< ZShadeSandboxMesh::VertexTex > GetRenderHull() { return mRenderHull; }
	
	// Gets the points that make up the physics body
	vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > GetPoints() { return mPoints; }
	
	// Gets the convex hull of the physics body
	vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > GetHull() { return mHull; }

	void Clone(PhysicsBodyBase3D* c) {}
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PHYSICSBODYBASE3D_H