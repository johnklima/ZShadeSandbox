//===============================================================================================================================
// PointColliderController
//
// This class will allow a point to collide against the terrain and slide against it
//===============================================================================================================================
// History
//
// -Created on 6/19/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __POINTCOLLIDERCONTROLLER_H
#define __POINTCOLLIDERCONTROLLER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "Vertex.h"
#include "QNodeType.h"
#include "D3D.h"
#include "AABB.h"
#include "QMeshNode.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxPhysics {
class PointColliderController
{
	struct CollisionPacket
	{
		// World Space Ellipsoid Info
		XMVECTOR	ellipsoidSpace;
		XMVECTOR	wPosition;
		XMVECTOR	wVelocity;
		
		// Ellipsoid Space Ellipsoid Info
		XMVECTOR	ePosition;
		XMVECTOR	eVelocity;
		XMVECTOR	eNormalizedVelocity;
		
		// Collision Info
		bool		collisionFound;
		float		nearestDistance;
		XMVECTOR	intersectionPoint;
		int			collisionRecursionDepth;
	};
	
public:
	
	PointColliderController();
	~PointColliderController();
	
	bool Colliding
	(	float unitsPerMeter
	,	float gravity
	,	XMFLOAT3 position
	,	float moveStrafe
	,	XMFLOAT3 right
	,	float moveWalk
	,	XMFLOAT3 forward
	,	vector<XMFLOAT3> vertices
	,	vector<UINT> indices
	,	XMFLOAT3& intersectionPoint
	);

private:
	
	XMVECTOR CollisionSlide(vector<XMFLOAT3> vertices, vector<UINT> indices);
	XMVECTOR CollisionWithWorld(vector<XMFLOAT3> vertices, vector<UINT> indices);
	bool SphereCollidingWithTriangle(XMVECTOR& vertex0, XMVECTOR& vertex1, XMVECTOR& vertex2, XMVECTOR& normal);
	bool CheckPointInTriangle(const XMVECTOR& point, XMVECTOR& vertex0, XMVECTOR& vertex1, XMVECTOR& vertex2);
	
	// Returns the lowest root of the quadratic equation
	bool GetLowestRoot(float a, float b, float c, float maxR, float* root);
	
private:
	
	float fUnitsPerMeter;
	float fGravity;
	
	vector<XMFLOAT3> mCollidablePositions;
	
	CollisionPacket mCollisionPacket;
};
}
//===============================================================================================================================
#endif//__POINTCOLLIDERCONTROLLER_H