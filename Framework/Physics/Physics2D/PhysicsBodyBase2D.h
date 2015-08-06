//==================================================================================================================================
// PhysicsBodyBase2D.h
//
//http://www.wildbunny.co.uk/blog/2011/12/14/how-to-make-a-2d-platform-game-part-2-collision-detection/
//http://www.hobbygamedev.com/int/platformer-game-source-in-processing/
//https://katyscode.wordpress.com/2013/01/18/2d-platform-games-collision-detection-for-dummies/
//==================================================================================================================================
// History
//
// -Created on 12/17/2014 by DW
// -Modified on 1/28/2015 by DW Changed name to PhysicsBodyBase
// -Modified on 4/13/2015 by DW Added AABB collision data
//==================================================================================================================================
#ifndef __PHYSICSBODYBASE2D_H
#define __PHYSICSBODYBASE2D_H
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
class PhysicsBodyBase2D
{
protected:

	D3D*           m_D3DSystem;
	EngineOptions* m_EngineOptions;
	
	// Is this a static or dynamic physics body
	PhysicsType eType;
	
	// Axis aligned bounding box of the body
	ZShadeSandboxMath::AABB* boundary;
	
	int iBodyWidth;
	int iBodyHeight;

	XMFLOAT3 m_CenterOffset;
	XMFLOAT3 m_HalfExtents;

	// Top-left coordinate of the Physics body and the position to adjust the image
	XMFLOAT3 m_TopLeftPosition;
	
	// position to adjust the collision mesh
	XMFLOAT3 m_HardboxPosition;
	
	float fInitialX;
	float fInitialY;
	float fPlatformDx;
	float fPlatformDy;
	
	ZShadeSandboxMesh::QuadMesh* m_HardboxMesh;
	
	ZShadeSandboxMath::BoundingBox m_body;
	
	// For ladders and other objects you should be able to walk through,
	// this flag disables collision resolution and allows the player to move
	// around inside the object
	bool bAllowInside;

	// For platforms with allowInside set, this flag specifies whether
	// you can enter the object by falling through the top surface
	// (subject to its surface's physics rules)
	// or whether the player should come to rest when landing on it
	// (and may then move downards through it)
	bool bAllowFallThrough;

	// These flags disable collision detection when the player is moving
	// in a specific direction; note this is not the same as disabling
	// collision detection for a specific side of the object.
	// This can be used to create platforms you can jump through but are
	// solid to walk on, or one-way walls or barriers that you can walk
	// or fall through but not go back the other way again.
	// NOTE: Enabling one or more of these means the player can be inside
	// the object, and must still be allowed to go in the opposite direction
	// until he/she has passed through it completely.
	bool bAllowFromLeft;
	bool bAllowFromRight;
	bool bAllowFromAbove;
	bool bAllowFromBelow;

	bool bMoving;

	// The bounding box of the platform on the previous frame
	// (used to calculate movement distance to adjust the player's position when standing on the platform)
	ZShadeSandboxMath::BoundingBox m_previousBody;

	//
	// Check for eight points of collision
	// (2 head, 2 feet, 2 left, 2 right)
	//
	
	vector<XMFLOAT3> m_CollisionPoints;
	
	// The Physics Body has capability to be a surface since
	// a physics body like a sprite can stand on a tile and the tile has a physics body
	ESurfaceType::Type mSurfaceType;
	SBodySurface* mSurface;
	
public:
	
	PhysicsBodyBase2D();
	PhysicsBodyBase2D(D3D* d3d);
	
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
	
	void InitCollisionPoints(int body_width, int body_height);
	
	void InitHardboxMesh();
	void RenderHardboxMesh(Camera* camera, XMFLOAT2 offset);
	void MoveHardboxMesh(float newX, float newY);
	void ScaleHardboxMesh(int newWidth, int newHeight);
	
	void GenerateBody();
	
	void UpdateMovingPlatform();

	ESurfaceType::Type& SurfaceType()		{ return mSurfaceType; }
	ESurfaceType::Type SurfaceType() const	{ return mSurfaceType; }
	
	SBodySurface*& BodySurface()			{ return mSurface; }
	SBodySurface* BodySurface() const		{ return mSurface; }
	
	ZShadeSandboxMath::BoundingBox& PreviousBodyBounds() { return m_previousBody; }
	ZShadeSandboxMath::BoundingBox PreviousBodyBounds() const { return m_previousBody; }

	float& InitialX() { return fInitialX; }
	float InitialX() const { return fInitialX; }
	float& InitialY() { return fInitialY; }
	float InitialY() const { return fInitialY; }

	float& PlatformDx() { return fPlatformDx; }
	float PlatformDx() const { return fPlatformDx; }
	float& PlatformDy() { return fPlatformDy; }
	float PlatformDy() const { return fPlatformDy; }

	bool& Moving() { return bMoving; }
	bool Moving() const { return bMoving; }

	bool& AllowInside() { return bAllowInside; }
	bool AllowInside() const { return bAllowInside; }

	bool& AllowFallThrough() { return bAllowFallThrough; }
	bool AllowFallThrough() const { return bAllowFallThrough; }

	bool& AllowFromLeft() { return bAllowFromLeft; }
	bool AllowFromLeft() const { return bAllowFromLeft; }
	bool& AllowFromRight() { return bAllowFromRight; }
	bool AllowFromRight() const { return bAllowFromRight; }
	bool& AllowFromAbove() { return bAllowFromAbove; }
	bool AllowFromAbove() const { return bAllowFromAbove; }
	bool& AllowFromBelow() { return bAllowFromBelow; }
	bool AllowFromBelow() const { return bAllowFromBelow; }

	// Create a bounds that extends to a predicted point
	ZShadeSandboxMath::BoundingBox BodyBoundsPredict(XMFLOAT3 point);
	ZShadeSandboxMath::BoundingBox BodyBoundsPredict(float x, float y);
	
	ZShadeSandboxMath::BoundingBox& Body() { return m_body; }
	ZShadeSandboxMath::BoundingBox Body() const { return m_body; }
	
	// Returns the m_TopLeftPosition of the physics body (Top-left coordinate)
	XMFLOAT3& TopLeftPosition() { return m_TopLeftPosition; }
	XMFLOAT3 TopLeftPosition() const { return m_TopLeftPosition; }
	
	XMFLOAT3& HardboxPosition() { return m_HardboxPosition; }
	XMFLOAT3 HardboxPosition() const { return m_HardboxPosition; }

	int& BodyWidth() { return iBodyWidth; }
	int BodyWidth() const { return iBodyWidth; }

	int& BodyHeight() { return iBodyHeight; }
	int BodyHeight() const { return iBodyHeight; }
	
	XMFLOAT3 CollisionPoint(int i) { return m_CollisionPoints[i]; }
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PHYSICSBODYBASE2D_H