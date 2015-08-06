//==================================================================================================================================
// PhysicsWorld2D.h
//
//==================================================================================================================================
// History
//
// -Created on 12/17/2014 by Dustin Watson
//==================================================================================================================================
#ifndef __PHYSICSWORLD2D_H
#define __PHYSICSWORLD2D_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "ZMath.h"
#include "GJK.h"
#include "Input.h"
#include "ContactStats.h"
#include "Text.h"

//==================================================================================================================================
class TopdownMap;
class PlatformerMap;
class PhysicsBody2D;
//==================================================================================================================================
class PhysicsWorld2D
{
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	
	// If we have a topdown map we want to collide against a hard tile but without gravity
	TopdownMap* m_TopdownMap;
	PlatformerMap* m_PlatformerMap;
	
	// Need to be able to toggle gravity in the tool window
	bool bGravityOn;
	
	vector<PhysicsBody2D*> m_Bodies;
	vector<int> m_BoundBodies;
	
	ContactStats m_ContactStatsPlayerToTile[3];
	
	ZShadeSandboxGraphics::Text text[3][7];
	
public:
	
	// Creates a physics world depending on the environment
	PhysicsWorld2D(D3D* d3d, TopdownMap* map);
	PhysicsWorld2D(D3D* d3d, PlatformerMap* map);
	
	void BuildTileBodies();
	
	// Update the physics bodies in the world due to gravitation and other forces
	void Update(float dt, Keyboard* keyboard);
	
	void RenderContactStats();
	
	// Accessor methods
public:
	
	bool& IsGravityOn() { return bGravityOn; }
	bool IsGravityOn() const { return bGravityOn; }
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PHYSICSWORLD2D_H