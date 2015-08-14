//==================================================================================================================================
// PhysicsBody2D.h
//
//==================================================================================================================================
// History
//
// -Created on 1/21/2015 by DW
// -Modified on 1/28/2015 by DW Changed name to PhysicsBody
//==================================================================================================================================
#ifndef __PHYSICSBODY2D_H
#define __PHYSICSBODY2D_H
//==================================================================================================================================
//==================================================================================================================================
#include "PhysicsBodyBase2D.h"
#include "ContactStats.h"
//#include "ContactManifold.h"
//class ContactManifold;
//class PhysicsBodyBase;
//==================================================================================================================================
//class Map;
//class PlatformerMap;
//==================================================================================================================================
class PhysicsBody2D : public PhysicsBodyBase2D
{
protected:
	
	// Need to integrate this, it will represent the radius of extension for this physics body
	// and will be the extent in which it will reach to. In other words a circle can be created
	// from it.
	float boundingRadius;
	
	// This represents the maximum amount of allowed force on the physics body to power itself
	// (rockets and thrust)
	float maxForce;
	
	// Represents a force calculated for accelerating this body
	XMFLOAT2 force;
	
	// Represents the total mass of this body
	float mass;
	
	XMFLOAT2 velocity;
	XMFLOAT2 heading;
	XMFLOAT2 acceleration;
	
	// Is this an AI doing its own movement
	bool steering;
	
	bool platformer;
	
	// The amount of X acceleration to apply when the player moves left or right
	// The amount of X deceleration to apply when the player does not move left or right
	float accXf, accXb;
	float decX;
	
	// The amount of X and Y movement to occur on the next frame, if a topdown map is used then speedY is not needed
	float speedX, speedY;
	
	// The maximum movement speed allowed for the player
	float maxSpeedX, maxSpeedY;
	
	// The amount of upward force to apply when the player first presses jump
	float jumpStartSpeedY;

	// The amount of Y acceleration to apply on each frame due to the force of gravity
	float accY;
	
	// The amount of Y deceleration to apply on each frame when the body does not try to move up or down
	// This is updated depending on the type of the last surface stood on, and reset when in mid-air
	float decY;
	
	// True if currently jumping (prevents the player jumping while in mid-air)
	bool jumping;

	// True if the jump key is currently held down (key must be released to allow a new jump)
	bool jumpKeyDown;
	
	// This is for platformer map, it represents the tile as empty space just like no tile was present
	bool bTileInvisible;
	
	// Linear movement variables
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
	
	bool bPlatform;
	
	// Is this body the player
	bool bPlayer;
	
	// The platform the sprite is on
	PhysicsBody2D* mCurrentPlatform;

	// The platform we jumped from (if currently jumping). This is only needed for platforms
	// with no collision detection (eg. ladders) to allow its physics model to be ignored
	// while we are jumping while still inside them, otherwise you get strange effects.
	PhysicsBody2D* mJumpPlatform;
	
	bool bPlatformLeft;
	bool bPlatformRight;
	bool bPlatformTop;
	bool bPlatformBottom;

	float fContactXValue;
	float fContactYValue;
	
public:
	
	PhysicsBody2D();
	PhysicsBody2D(D3D* d3d);
	
	void SetD3D(D3D* d3d);

	void Initialize(XMFLOAT3 pos, PhysicsType pt, ESurfaceType::Type surfaceType, int body_width, int body_height);
	
	void UpdateSurfaceType();

	void InitializeParametersTopdown();
	void InitializeParametersPlatformer();
	void InitializeBaseParameters();
	
	// First parameter is a collection of physics bodies in the map
	// Second parameter is a collection of the bound bodies the physics body is in contact with
	void CollideBodiesPlatformer(vector<PhysicsBody2D*> bodies, vector<int> &boundBodies, ContactStats* cs, float dt);
	void CollideBodiesTopdown(vector<PhysicsBody2D*> bodies, vector<int> &boundBodies, ContactStats* cs, float dt);
	
	// Checks to see if any point of this physics body touches the other physics body
	bool CollidesBodyPlatformer(PhysicsBody2D* body, float dt);
	bool CollidesBodyTopdown(PhysicsBody2D* body, float dt);
	
	// Need a function to collide with another dynamic body
	// Need Ladder and Platform collision
	
	// Movement must be done after the collision contact solver
	void MovePlatformer(float dt);
	void MoveTopdown(float dt);
	
	// Override this method in the main class for collision that happens to it. (If it takes damage)
	virtual void OnCollision(bool bTop, bool bBottom, bool bLeft, bool bRight, int damageAmount) {}
	
	// Accessor methods
public:
	
	//void UpdateVelocity(float x, float y);
	
	XMFLOAT3 Speed() { return XMFLOAT3(speedX, speedY, 0); }
	XMFLOAT3 MaxSpeed() { return XMFLOAT3(maxSpeedX, maxSpeedY, 0); }

	bool& Platformer() { return platformer; }
	bool Platformer() const { return platformer; }
	
	bool& Steering() { return steering; }
	bool Steering() const { return steering; }
	
	XMFLOAT2& Force() { return force; }
	XMFLOAT2 Force() const { return force; }
	
	float& Mass() { return mass; }
	float Mass() const { return mass; }
	
	XMFLOAT2& Velocity() { return velocity; }
	XMFLOAT2 Velocity() const { return velocity; }
	
	XMFLOAT2& Heading() { return heading; }
	XMFLOAT2 Heading() const { return heading; }
	
	float& BoundingRadius() { return boundingRadius; }
	float BoundingRadius() const { return boundingRadius; }
	
	float& MaxForce() { return maxForce; }
	float MaxForce() const { return maxForce; }
	
	bool IsPlatform() { return bPlatform; }
	bool IsCrushed() { return bCrushed; }
	
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

	float& AccXf() { return accXf; }
	float AccXf() const { return accXf; }

	float& AccXb() { return accXb; }
	float AccXb() const { return accXb; }
	
	float& AccY() { return accY; }
	float AccY() const { return accY; }
	
	float& DecY() { return decY; }
	float DecY() const { return decY; }

	float& SpeedX() { return speedX; }
	float SpeedX() const { return speedX; }
	
	float& SpeedY() { return speedY; }
	float SpeedY() const { return speedY; }
	
	float& MaxSpeedX() { return maxSpeedX; }
	float MaxSpeedX() const { return maxSpeedX; }
	
	float& MaxSpeedY() { return maxSpeedY; }
	float MaxSpeedY() const { return maxSpeedY; }
	
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
	
	bool& TileInvisible() { return bTileInvisible; }
	bool TileInvisible() const { return bTileInvisible; }
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PHYSICSBODY2D_H