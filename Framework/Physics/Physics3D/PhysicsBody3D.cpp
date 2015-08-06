#include "PhysicsBody3D.h"
#include "Forces.h"
#include "XMMath.h"
#include "ShapeContact.h"
#include "AABB.h"
#include "GJK.h"
#include "SurfaceDefinition.h"
//#include "Map.h"
//#include "PlatformerMap.h"
//==================================================================================================================================
//==================================================================================================================================
PhysicsBody3D::PhysicsBody3D()
:	PhysicsBodyBase3D()
{
}
//==================================================================================================================================
PhysicsBody3D::PhysicsBody3D(D3D* d3d)
:   PhysicsBodyBase3D(d3d)
{
}
//==================================================================================================================================
void PhysicsBody3D::SetD3D(D3D* d3d)
{
	m_D3DSystem = d3d;
	m_EngineOptions = d3d->GetEngineOptions();
	PhysicsBodyBase3D::SetD3D(d3d);
}
//===============================================================================================================================
void PhysicsBody3D::InitializeBaseParameters()
{
	// Initialize the body to not move
	eType = STATIC;
	
	jumping = false;
	jumpKeyDown = false;
	bJumpKey = false;
	
	bMovingLeft = false;
	bMovingRight = false;
	bMovingUp = false;
	bMovingDown = false;
	
	speedX = 0.0f;
	speedY = 0.0f;
	speedZ = 0.0f;
	
	iDamageToOther = 0;
	
	bVulnerableFromLeft = false;
	bVulnerableFromRight = false;
	bVulnerableFromAbove = false;
	bVulnerableFromBelow = false;
	
	bPlayer = false;
	
	bMoving = false;
}
//===============================================================================================================================
void PhysicsBody3D::Initialize(XMFLOAT3 pos, PhysicsType pt)
{
	InitializeBaseParameters();
	
	mPosition = pos;
	
	eType = pt;
}
//===============================================================================================================================
void PhysicsBody3D::Move(float dt)
{
	// A static body cannot move
	if (eType == STATIC) return;
	
	// Add the contact values so that the physics body can move with other objects
	/*m_TopLeftPosition.x += speedX * dt + fContactXValue;
	m_TopLeftPosition.y += speedY * dt + fContactYValue;
	
	m_HardboxPosition.x += speedX * dt + fContactXValue;
	m_HardboxPosition.y += speedY * dt + fContactYValue;
	
	// If we are on a platform that can alter the physics bodies movement
	// like ice or glue then alter the players physics
	if (mCurrentPlatform)
	{
		accXf = mCurrentPlatform->BodySurface()->fSurfaceAccXf;
		accXb = mCurrentPlatform->BodySurface()->fSurfaceAccXb;
		decX = mCurrentPlatform->BodySurface()->fSurfaceDecX;
		maxSpeedX = mCurrentPlatform->BodySurface()->fSurfaceMaxSpeedX;
		maxSpeedY = mCurrentPlatform->BodySurface()->fSurfaceMaxSpeedY;
		accY = mCurrentPlatform->BodySurface()->fSurfaceAccY;
		decY = mCurrentPlatform->BodySurface()->fSurfaceDecY;
	}
	else
	{
		// We are in flight. Use standard gravity.
		// Including the Y-deceleration is also important here because transitioning from a platform
		// such as a ladder with no Y-deceleration to flight will cause gravity to malfunction
		// if the Y-deceleration is not reset.
		
		accY = SSurfaceDefinition::NormalSurface->fSurfaceAccY;
		decY = SSurfaceDefinition::NormalSurface->fSurfaceDecY;
	}
	
	// If we are jumping from a platform with no collision detection (eg. ladder),
	// use normal physics rather than the physics of the platform. If we don't do this
	// for ladders, there will be no gravity until we are no longer touching the ladder.
	if (jumping)
	{
		if (mJumpPlatform->AllowInside())
		{
			accXf = SSurfaceDefinition::NormalSurface->fSurfaceAccXf;
			accXb = SSurfaceDefinition::NormalSurface->fSurfaceAccXb;
			decX = SSurfaceDefinition::NormalSurface->fSurfaceDecX;
			maxSpeedX = SSurfaceDefinition::NormalSurface->fSurfaceMaxSpeedX;
			maxSpeedY = SSurfaceDefinition::NormalSurface->fSurfaceMaxSpeedY;
			accY = SSurfaceDefinition::NormalSurface->fSurfaceAccY;
			decY = SSurfaceDefinition::NormalSurface->fSurfaceDecY;
		}
	}

	bool moveRequestX = false;
	bool moveRequestY = false;

	// Accelerate Leftwards
	if (bMovingLeft)
	{
		//speedX -= accX;
		
		// How much acceleration to apply depends on whether we are trying to move
		// in the same direction as now, or the opposite way
		// if we try moving in the current direction of travel, accXf is applied
		if (speedX <= 0)
			speedX -= accXf;
		else
			speedX -= accXb;
		
		moveRequestX = true;
	}

	// Accelerate Rightwards
	if (bMovingRight)
	{
		//speedX += accX;
		
		if (speedX >= 0)
			speedX += accXf;
		else
			speedX += accXb;
		
		moveRequestX = true;
	}
	
	// Walk up and down platforms which allow it (eg. ladders)
	if (bMovingUpLadder)
	{
		// The standing* flags are important here. We don't want to allow upwards movement on a
		// platform which we are standing on the upper edge of (eg. ladder). Therefore the contact
		// the bottom of the player is making with the platform is ignored, but some other part of
		// the player must be touching the platform. When standing at the top of the platform,
		// this will not be the case and upwards movement will be prevented.
		if (mCurrentPlatform)
		{
			if (mCurrentPlatform->BodySurface()->fSurfaceSpeedUp != 0.0f && (bPlatformLeft || bPlatformRight || bPlatformTop))
			{
				speedY = -mCurrentPlatform->BodySurface()->fSurfaceSpeedUp;
			}

			moveRequestY = true;
		}
	}
	
	if (bMovingDownLadder)
	{
		// Moving downards does not have the same complications as moving upwards, since when the
		// player moves beyond the bottom edge they will simply start to fall.
		if (mCurrentPlatform)
		{
			if (mCurrentPlatform->BodySurface()->fSurfaceSpeedDown != 0.0f)
			{
				speedY = mCurrentPlatform->BodySurface()->fSurfaceSpeedDown;
			}

			moveRequestY = true;
		}
	}
	
	// If gravity is disabled this will accelerate the body at the request
	// of the user
	
	if (bMovingUp)
	{
		speedY -= accY;
	}
	
	if (bMovingDown)
	{
		speedY += accY;
	}
	
	// Jump if not already jumping and the jump key was released earlier
	if (JumpEnabled())
	{
		if (GetAsyncKeyState(' ') && !jumping && !jumpKeyDown)
		{
			// We are allowed to jump under three conditions:
			// 1. We are not already jumping (prevents re-jumping in mid-air)
			// and either:
			// 2a. We are standing on the top of a platform's surface
			// or:
			// 2b. We are standing inside a platform with collision detection disabled (eg. ladder)
			// and attempting to jump left or right. Jumping straight upwards while in a vertical
			// platform is prevented. Note this condition can only be true for platforms without
			// collision detection, since if we are standing in both the top and bottom of a normal
			// platform, we have been crushed and are dead
			if ((bPlatformBottom && !bPlatformTop) || (bPlatformBottom && bPlatformTop && speedX != 0))
			{
				jumping = true;
				jumpKeyDown = true;
				speedY = -jumpStartSpeedY;
				mJumpPlatform = mCurrentPlatform;
			}
		}

		// Jump key released
		if (!GetAsyncKeyState(' '))
			jumpKeyDown = false;
	}
	
	// Toggle display of geometry bounding boxes
	// ...

	//
	// Apply forces of gravity and friction
	//
	
	if (FrictionEnabled())
	{
		// Limit the sideways acceleration of the body
		
		if (speedX > maxSpeedX)
			speedX = maxSpeedX;

		if (speedX < -maxSpeedX)
			speedX = -maxSpeedX;
	}
	
	// Apply the force of gravity if it is turned on
	if (bGravityOn)
	{
		// Limit the force of gravity (terminal velocity)
		
		if (speedY > maxSpeedY)
			speedY = maxSpeedY;

		if (speedY < -maxSpeedY)
			speedY = -maxSpeedY;
		
		speedY += accY;
	}
	
	if (FrictionEnabled())
	{
		// Decelerate the body's sideways movement if left or right was not pressed
		if (!moveRequestX)
		{
			if (speedX < 0)
				speedX += decX;

			if (speedX > 0)
				speedX -= decX;

			// Deceleration may produce a speed that is greater than zero but
			// smaller than the smallest unit of deceleration. These lines ensure
			// that the body does not keep travelling at slow speed forever after
			// decelerating.

			//if (speedX > 0 && speedX < decX || speedX < 0 && speedX > -decX)
			//	speedX = 0;
			if (speedX > 0 && speedX < speedX + decX)
				speedX = 0;
			if (speedX < 0 && speedX > speedX - decX)
				speedX = 0;
		}
	}
	
	// Decelerate the player's vertical movement if up or down wasn't pressed
	// AND we are not jumping
	// Applying vertical deceleration while jumping distorts the effect of gravity
	if (!moveRequestY && !jumping)
	{
		if (speedY < 0)
			speedY += decY;

		if (speedY > 0)
			speedY -= decY;

		// Deceleration may produce a speed that is greater than zero but
		// smaller than the smallest unit of deceleration. These lines ensure
		// that the body does not keep travelling at slow speed forever after
		// decelerating.

		//if (speedY > 0 && speedY < decY || speedY < 0 && speedY > -decY)
		//	speedY = 0;
		if (speedY > 0 && speedY < speedY + decY)
			speedY = 0;
		if (speedY < 0 && speedY > speedY - decY)
			speedY = 0;
	}*/
}
//===============================================================================================================================
bool PhysicsBody3D::CollidesBody(PhysicsBody3D* body, float dt)
{
	// A static body cannot collide since it is not moving
	if (eType == STATIC) return false;
	
	if (body == NULL) return false;
	
	// What direction did the collision come from
	bool bTop, bBottom, bRight, bLeft, bFront, bBack;
	
	// ...
	
	// If we have any contact
	if (bTop || bBottom || bLeft || bRight || bFront || bBack)
	{
		// Update the body as to what part it has been hit: top, bottom, left, right, front , back
		OnCollision(bTop, bBottom, bLeft, bRight, bFront, bBack, body->DamageToOther());
		
		// Also update the other body to notify where it is being hit
		// The other body is hit on the opposite direction the current body is being hit
		body->OnCollision(bBottom, bTop, bRight, bLeft, bBack, bFront, iDamageToOther);
		
		// Found a contact
		return true;
	}
	
	// Did not find any contact
	return false;
}
//===============================================================================================================================
bool PhysicsBody3D::CollidesGJK(PhysicsBody3D* body)
{
	GJK gjk(m_EngineOptions);
	if (gjk.Intersect(this, body))
	{
		// Solve this collision
		for (;;)
		{
			// Move the physics body in the direction of the collision normal until it is free
			break;// Not Implemented Yet
		}

		return true;
	}

	return false;
}
//===============================================================================================================================