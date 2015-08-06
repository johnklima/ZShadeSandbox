//==================================================================================================================================
// PhysicsBody3D.h
//
//==================================================================================================================================
// History
//
// -Created on 1/21/2015 by DW
// -Modified on 1/28/2015 by DW Changed name to PhysicsBody
//==================================================================================================================================
#ifndef __PHYSICSBODY3D_H
#define __PHYSICSBODY3D_H
//==================================================================================================================================
//==================================================================================================================================
#include "PhysicsBodyBase3D.h"
#include "ContactStats.h"
//#include "ContactManifold.h"
//class ContactManifold;
//class PhysicsBodyBase;
//==================================================================================================================================
//class Map;
//class PlatformerMap;
//==================================================================================================================================
class PhysicsBody3D : public PhysicsBodyBase3D
{
protected:
	
	//ContactManifold m_ContactManifold;
	
	// The amount of acceleration to apply when the physics body moves in a direction
	float forwardAccelerationX, backwardAccelerationX;
	float forwardAccelerationZ, backwardAccelerationZ;
	float forwardDecelerationX, backwardDecelerationX;
	float forwardDecelerationZ, backwardDecelerationZ;
	
	// The amount of X and Z movement to occur on the next frame
	float speedX, speedY, speedZ;
	
	// The maximum movement speed allowed for the physics body
	float maxSpeedX, maxSpeedY, maxSpeedZ;
	
	// The amount of upward force to apply when the physics body first presses jump
	float jumpStartSpeedY;
	
	// The amount of Y acceleration to apply on each frame due to the force of gravity
	float accelerationY;
	
	// The amount of Y deceleration to apply on each frame when the body does not try to move up or down
	// This is updated depending on the type of the last surface stood on, and reset when in mid-air
	float decelerationY;
	
	// True if currently jumping (prevents the physics body jumping while in mid-air)
	bool jumping;

	// True if the jump key is currently held down (key must be released to allow a new jump)
	bool jumpKeyDown;
	
	// Linear movement variables
	bool bMoving;
	bool bMovingLeft;
	bool bMovingRight;
	bool bMovingUp;
	bool bMovingDown;
	bool bMovingUpLadder;
	bool bMovingDownLadder;

	// Maps any key to this bool to see if a jump happened
	bool bJumpKey;
	
	bool bGravityOn;
	
	bool bJumpEnabled;
	bool bFrictionEnabled;
	
	// The amount of damage done to another body on collision
	int iDamageToOther;
	
	// Sides from which the body is vulnerable to being attacked (collided) by another body
	bool bVulnerableFromLeft;
	bool bVulnerableFromRight;
	bool bVulnerableFromAbove;
	bool bVulnerableFromBelow;
	
	// This physics body can be crushed
	bool bCrushed;
	
	// Is this body the player
	bool bPlayer;
	
public:
	
	PhysicsBody3D();
	PhysicsBody3D(D3D* d3d);
	
	void SetD3D(D3D* d3d);

	// Does this physics body collide with another physics body using GJK
	bool CollidesGJK(PhysicsBody3D* body);
	
	void Initialize(XMFLOAT3 pos, PhysicsType pt);
	
	void InitializeBaseParameters();
	
	// Checks to see if any point of this physics body touches the other physics body
	bool CollidesBody(PhysicsBody3D* body, float dt);
	
	// Movement must be done after the collision contact solver
	void Move(float dt);
	
	// Override this method in the main class for collision that happens to it. (If it takes damage)
	virtual void OnCollision(bool bTop, bool bBottom, bool bLeft, bool bRight, bool bFront, bool bBack, int damageAmount) {}
	
	// Accessor methods
public:
	
	bool& JumpEnabled() { return bJumpEnabled; }
	bool JumpEnabled() const { return bJumpEnabled; }
	bool& FrictionEnabled() { return bFrictionEnabled; }
	bool FrictionEnabled() const { return bFrictionEnabled; }
	bool& IsGravityOn() { return bGravityOn; }
	bool IsGravityOn() const { return bGravityOn; }
	bool& JumpKey() { return bJumpKey; }
	bool JumpKey() const { return bJumpKey; }
	bool& MovingLeft() { return bMovingLeft; }
	bool MovingLeft() const { return bMovingLeft; }
	bool& MovingRight() { return bMovingRight; }
	bool MovingRight() const { return bMovingRight; }
	bool& MovingUp() { return bMovingUp; }
	bool MovingUp() const { return bMovingUp; }
	bool& MovingDown() { return bMovingDown; }
	bool MovingDown() const { return bMovingDown; }
	bool& MovingUpLadder() { return bMovingUpLadder; }
	bool MovingUpLadder() const { return bMovingUpLadder; }
	bool& MovingDownLadder() { return bMovingDownLadder; }
	bool MovingDownLadder() const { return bMovingDownLadder; }
	float& ForwardAccelerationX() { return forwardAccelerationX; }
	float ForwardAccelerationX() const { return forwardAccelerationX; }
	float& BackwardAccelerationX() { return backwardAccelerationX; }
	float BackwardAccelerationX() const { return backwardAccelerationX; }
	float& ForwardAccelerationZ() { return forwardAccelerationZ; }
	float ForwardAccelerationZ() const { return forwardAccelerationZ; }
	float& BackwardAccelerationZ() { return backwardAccelerationZ; }
	float BackwardAccelerationZ() const { return backwardAccelerationZ; }
	float& AccerationY() { return accelerationY; }
	float AccerationY() const { return accelerationY; }
	float& DecelerationY() { return decelerationY; }
	float DecelerationY() const { return decelerationY; }
	float& SpeedX() { return speedX; }
	float SpeedX() const { return speedX; }
	float& SpeedY() { return speedY; }
	float SpeedY() const { return speedY; }
	float& SpeedZ() { return speedZ; }
	float SpeedZ() const { return speedZ; }
	float& MaxSpeedX() { return maxSpeedX; }
	float MaxSpeedX() const { return maxSpeedX; }
	float& MaxSpeedY() { return maxSpeedY; }
	float MaxSpeedY() const { return maxSpeedY; }
	float& MaxSpeedZ() { return maxSpeedZ; }
	float MaxSpeedZ() const { return maxSpeedZ; }
	float& JumpStartSpeedY() { return jumpStartSpeedY; }
	float JumpStartSpeedY() const { return jumpStartSpeedY; }
	bool& IsJumping() { return jumping; }
	bool IsJumping() const { return jumping; }
	bool& IsJumpKeyDown() { return jumpKeyDown; }
	bool IsJumpKeyDown() const { return jumpKeyDown; }
	int& DamageToOther() { return iDamageToOther; }
	int DamageToOther() const { return iDamageToOther; }
	bool& VulnerableFromLeft() { return bVulnerableFromLeft; }
	bool VulnerableFromLeft() const { return bVulnerableFromLeft; }
	bool& VulnerableFromRight() { return bVulnerableFromRight; }
	bool VulnerableFromRight() const { return bVulnerableFromRight; }
	bool& VulnerableFromAbove() { return bVulnerableFromAbove; }
	bool VulnerableFromAbove() const { return bVulnerableFromAbove; }
	bool& VulnerableFromBelow() { return bVulnerableFromBelow; }
	bool VulnerableFromBelow() const { return bVulnerableFromBelow; }
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PHYSICSBODY3D_H