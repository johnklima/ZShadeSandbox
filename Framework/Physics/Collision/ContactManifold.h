//==================================================================================================================================
// ContactManifold.h
//
//==================================================================================================================================
// History
//
// -Created on 12/18/2014 by Dustin Watson
//==================================================================================================================================
#ifndef __CONTACTMANIFOLD_H
#define __CONTACTMANIFOLD_H
//==================================================================================================================================
//==================================================================================================================================
#include "ZMath.h"
//#include "PhysicsBody.h"
#include <vector>
class PhysicsBody;
//==================================================================================================================================
// http://en.wikipedia.org/wiki/Contact_geometry
// http://en.wikipedia.org/wiki/Manifold
// Google: how is a manifold defined in c++
// http://www.randygaul.net/2013/03/28/custom-physics-engine-part-2-manifold-generation/
//==================================================================================================================================
struct ManifoldPoint
{
	// Vector position of contact
	ZShadeSandboxMath::Vec3<XMFLOAT3> position;
	
	// Vector of penetration
	ZShadeSandboxMath::Vec3<XMFLOAT3> normal;
	
	// Depth of penetration
	float          penetration;
	
	// Separation power
	float          impulse;
};
//==================================================================================================================================
struct ContactManifold
{
	std::vector< ManifoldPoint > contactPoints;
	
	bool bColliding;
	
	PhysicsBody* collidingBodyA;
	PhysicsBody* collidingBodyB;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__CONTACTMANIFOLD_H