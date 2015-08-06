//==================================================================================================================================
// ContactSolver.h
//
//==================================================================================================================================
// History
//
// -Created on 12/18/2014 by Dustin Watson
//==================================================================================================================================
#ifndef __CONTACTSOLVER_H
#define __CONTACTSOLVER_H
//==================================================================================================================================
//==================================================================================================================================
#include "GJK.h"
#include "ContactManifold.h"
//==================================================================================================================================
// Solves a contact between two colliding physics bodies
class ContactSolver
{
	// Find the farthest vertex in a mesh along the normal of the translation vector
	int FurthestIndex(vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > points, ZShadeSandboxMath::Vec3<XMFLOAT3> normal);
	
public:
	
	ContactSolver();
	
	void Solve(PhysicsBody3D* body1, PhysicsBody3D* body2, ContactManifold* manifold);
	
	// Finding contact points (http://www.codezealot.org/archives/394)
	// 1) Finding the Features of the shapes
	//    - We can find the collision feature of a shape by finding the farthest vertex in the shape.
	// 2) Look at the adjacent two vertices to determine which edge is the "closest".
	//    - We can find the closest as the edge who is most perpendicular to the separation normal.
	
	// Calculates the contact points needed for a contact manifold from a minimum translation vector
	void CalculateContactPoints(PhysicsBody3D* body1, PhysicsBody3D* body2, GJK_MTV* mtv, ContactManifold* manifold);
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__CONTACTSOLVER_H