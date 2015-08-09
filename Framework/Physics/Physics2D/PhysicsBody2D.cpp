#include "PhysicsBody2D.h"
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
PhysicsBody2D::PhysicsBody2D()
:	PhysicsBodyBase2D()
{
}
//==================================================================================================================================
PhysicsBody2D::PhysicsBody2D(D3D* d3d)
:   PhysicsBodyBase2D(d3d)
{
}
//==================================================================================================================================
void PhysicsBody2D::SetD3D(D3D* d3d)
{
	m_D3DSystem = d3d;
	m_EngineOptions = d3d->GetEngineOptions();
	PhysicsBodyBase2D::SetD3D(d3d);
}
//===============================================================================================================================
void PhysicsBody2D::InitializeParametersTopdown()
{
	// Set acceleration and speed
	float mScale = 60.f;
	
	// Set jump and gravity forces
	jumpStartSpeedY = 0;
	accY = 0;
	
	// acceleration of x and y are same since there is no gravity
	// and we are moving up, down, left or right
	accXf = 0.2f * mScale;
	accXb = 0.2f * mScale;
	accY = 0.2f * mScale;
	decX = 0.3f * mScale;
	maxSpeed = 5.0f * mScale;
	maxSpeedY = 10.0f * mScale;
	
	bGravityOn = false;
	
	bJumpEnabled = false;
	bFrictionEnabled = false;
}
//===============================================================================================================================
void PhysicsBody2D::InitializeParametersPlatformer()
{
	// Set acceleration and speed
	float mScale = 60.f;
	
	accXf = 0.2f * mScale;
	accXb = 0.2f * mScale;
	decX = 0.3f * mScale;
	maxSpeed = 5.0f * mScale;
	maxSpeedY = 10.0f * mScale;
	
	// Set jump and gravity forces
	jumpStartSpeedY = 8.f * mScale;
	accY = 0.5f * mScale;
	
	bGravityOn = true;
	
	bJumpEnabled = true;
	bFrictionEnabled = true;
}
//===============================================================================================================================
void PhysicsBody2D::InitializeBaseParameters()
{
	// Initialize the body to not move
	eType = STATIC;
	
	mCurrentPlatform = 0;

	jumping = false;
	jumpKeyDown = false;
	bJumpKey = false;
	
	bMovingLeft = false;
	bMovingRight = false;
	bMovingUp = false;
	bMovingDown = false;
	
	maxForce = 100;
	
	speed = 0.f;
	speedY = 0.f;
	
	iDamageToOther = 0;
	
	bVulnerableFromLeft = false;
	bVulnerableFromRight = false;
	bVulnerableFromAbove = false;
	bVulnerableFromBelow = false;
	
	bPlayer = false;
	
	vHeading = XMFLOAT3(0, 0, 0);
	
	bTileInvisible = false;
	bPlatform = false;
	bAllowInside = false;
	bAllowFallThrough = false;
	bMoving = false;
	bAllowFromLeft = false;
	bAllowFromRight = false;
	bAllowFromAbove = false;
	bAllowFromBelow = false;

	fContactXValue = 0;
	fContactYValue = 0;
}
//===============================================================================================================================
void PhysicsBody2D::Initialize(XMFLOAT3 pos, PhysicsType pt, ESurfaceType::Type surfaceType, int body_width, int body_height)
{
	InitializeBaseParameters();
	
	m_TopLeftPosition = pos;
	m_HardboxPosition = pos;

	fInitialX = pos.x;
	fInitialY = pos.y;
	
	fPlatformDx = 0;
	fPlatformDy = 0;

	mSurfaceType = surfaceType;
	UpdateSurfaceType();

	eType = pt;
	
	iBodyWidth = body_width;
	iBodyHeight = body_height;
	
	InitCollisionPoints(iBodyWidth, iBodyHeight);
	
	// Generate a new body for static geometry
	GenerateBody();
	
	InitHardboxMesh();
}
//===============================================================================================================================
void PhysicsBody2D::UpdateSurfaceType()
{
	switch (mSurfaceType)
	{
		case ESurfaceType::Type::eNormal:
		{
			mSurface = SSurfaceDefinition::NormalSurface;
		}
		break;
		case ESurfaceType::Type::eFrictionless:
		{
			mSurface = SSurfaceDefinition::FrictionlessSurface;
		}
		break;
		case ESurfaceType::Type::eSticky:
		{
			mSurface = SSurfaceDefinition::StickySurface;
		}
		break;
		case ESurfaceType::Type::eLadder:
		{
			mSurface = SSurfaceDefinition::LadderSurface;
		}
		break;
		case ESurfaceType::Type::eJumpThrough:
		{
			mSurface = SSurfaceDefinition::JumpThroughSurface;
		}
		break;
	}
}
//===============================================================================================================================
void PhysicsBody2D::UpdateVelocity(float x, float y)
{
	accXf += x;
	accXb += x;
	accY += y;
}
//===============================================================================================================================
XMFLOAT3 PhysicsBody2D::Velocity()
{
	XMFLOAT3 velocity;
	
	if (speed <= 0)
	{
		velocity.x = accXf;
	}
	else
	{
		velocity.x = accXb;
	}
	
	velocity.y = accY;
	velocity.z = 0;
	
	return velocity;
}
//===============================================================================================================================
XMFLOAT3 PhysicsBody2D::Heading()
{
	return vHeading;
}
//===============================================================================================================================
void PhysicsBody2D::MovePlatformer(float dt)
{
	// A static body cannot move
	if (eType == STATIC) return;
	
	// Add the contact values so that the physics body can move with other objects
	m_TopLeftPosition.x += speed * dt + fContactXValue;
	m_TopLeftPosition.y += speedY * dt + fContactYValue;
	
	m_HardboxPosition.x += speed * dt + fContactXValue;
	m_HardboxPosition.y += speedY * dt + fContactYValue;
	
	// If we are on a platform that can alter the physics bodies movement
	// like ice or glue then alter the physics bodies physics
	if (mCurrentPlatform)
	{
		accXf = mCurrentPlatform->BodySurface()->fSurfaceAccXf;
		accXb = mCurrentPlatform->BodySurface()->fSurfaceAccXb;
		decX = mCurrentPlatform->BodySurface()->fSurfaceDecX;
		maxSpeed = mCurrentPlatform->BodySurface()->fSurfaceMaxSpeedX;
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
			maxSpeed = SSurfaceDefinition::NormalSurface->fSurfaceMaxSpeedX;
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
		// How much acceleration to apply depends on whether we are trying to move
		// in the same direction as now, or the opposite way
		// if we try moving in the current direction of travel, accXf is applied
		if (speed <= 0)
			speed -= accXf;
		else
			speed -= accXb;
		
		moveRequestX = true;
	}

	// Accelerate Rightwards
	if (bMovingRight)
	{
		if (speed >= 0)
			speed += accXf;
		else
			speed += accXb;
		
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
	
	// Do not remember why this is here
	//if (bMovingUp)
	//{
	//	speedY -= accY;
	//}
	//if (bMovingDown)
	//{
	//	speedY += accY;
	//}
	
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
			if ((bPlatformBottom && !bPlatformTop) || (bPlatformBottom && bPlatformTop && speed != 0))
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
		
		if (speed > maxSpeed)
			speed = maxSpeed;

		if (speed < -maxSpeed)
			speed = -maxSpeed;
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
			if (speed < 0)
				speed += decX;

			if (speed > 0)
				speed -= decX;

			// Deceleration may produce a speed that is greater than zero but
			// smaller than the smallest unit of deceleration. These lines ensure
			// that the body does not keep travelling at slow speed forever after
			// decelerating.

			//if (speedX > 0 && speedX < decX || speedX < 0 && speedX > -decX)
			//	speedX = 0;
			if (speed > 0 && speed < speed + decX)
				speed = 0;
			if (speed < 0 && speed > speed - decX)
				speed = 0;
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
		}
	}
	
	if (ZShadeSandboxMath::XMMath3(Velocity().x, Velocity().y, 0).LengthSquared() > 0.00000001)
	{
		ZShadeSandboxMath::XMMath3 heading(Velocity().x, Velocity().y, 0);
		heading.Normalize();
		vHeading = heading;
	}
	
	/*
	// If we just started moving in X, start the X animation at the first frame
	if (speedX != 0 && prevSpeedX == 0)
		playerXanim.Reset();

	// If we just stopped moving in X, clamp the X animation to the first frame
	if (speedX == 0)
		playerXanim.Reset(true);

	// Which way the player is facing (used when still)
	if (speedX != 0)
		playerXfacing = (speedX > 0? 1 : -1);

	// Player ladder animation

	// If we just arrived on a ladder, reset the ladder animation to the first frame
	if (standingPlatform)
		if (standingPlatform->surface == Ladder)
			if (!previousPlatform)
				playerLadderAnim.Reset(true);
			else if (previousPlatform->surface != Ladder)
				playerLadderAnim.Reset(true);

	// If we're on a ladder but just stopped moving in Y, pause the animation
	if (standingPlatform)
		if (standingPlatform->surface == Ladder)
			if (speedY == 0 && prevSpeedY != 0)
				playerLadderAnim.Pause();
	
	// If we're on a ladder and just start moving in Y, unpause the animation
	if (standingPlatform)
		if (standingPlatform->surface == Ladder)
			if (speedY != 0 && prevSpeedY == 0)
				playerLadderAnim.Pause(false);

	// If the player is dead, re-spawn
	if (dead)
		spawnPlayer();
	*/

	// Generate a new body based on movement
	GenerateBody();
}
//===============================================================================================================================
void PhysicsBody2D::MoveTopdown(float dt)
{
	// A static body cannot move
	if (eType == STATIC) return;
	
	// Add the contact values so that the physics body can move with other objects
	m_TopLeftPosition.x += speed * dt + fContactXValue;
	m_TopLeftPosition.y += speed * dt + fContactYValue;
	
	m_HardboxPosition.x += speed * dt + fContactXValue;
	m_HardboxPosition.y += speed * dt + fContactYValue;
	
	bool moveRequestX = false;
	bool moveRequestY = false;
	
	// Accelerate Leftwards
	if (bMovingLeft)
	{
		// How much acceleration to apply depends on whether we are trying to move
		// in the same direction as now, or the opposite way
		// if we try moving in the current direction of travel, accXf is applied
		if (speed <= 0)
			speed -= accXf;
		else
			speed -= accXb;
		
		moveRequestX = true;
	}

	// Accelerate Rightwards
	if (bMovingRight)
	{
		if (speed >= 0)
			speed += accXf;
		else
			speed += accXb;
		
		moveRequestX = true;
	}
	
	if (bMovingUp)
	{
		speed -= accY;
		
		moveRequestY = true;
	}
	
	if (bMovingDown)
	{
		speed += accY;
		
		moveRequestY = true;
	}
	
	if (FrictionEnabled())
	{
		// Limit the acceleration of the body
		
		if (speed > maxSpeed)
			speed = maxSpeed;

		if (speed < -maxSpeed)
			speed = -maxSpeed;
	}
	
	// This will allow for having the body not slide on ice
	if (FrictionEnabled())
	{
		// Decelerate the body's movement if left or right was not pressed
		if (!moveRequestX)
		{
			if (speed < 0)
				speed += decX;

			if (speed > 0)
				speed -= decX;

			// Deceleration may produce a speed that is greater than zero but
			// smaller than the smallest unit of deceleration. These lines ensure
			// that the body does not keep travelling at slow speed forever after
			// decelerating.

			if (speed > 0 && speed < speed + decX)
				speed = 0;
			if (speed < 0 && speed > speed - decX)
				speed = 0;
		}
		
		// Decelerate the body's movement if up or down was not pressed
		if (!moveRequestY)
		{
			if (speed < 0)
				speed += decY;

			if (speed > 0)
				speed -= decY;

			// Deceleration may produce a speed that is greater than zero but
			// smaller than the smallest unit of deceleration. These lines ensure
			// that the body does not keep travelling at slow speed forever after
			// decelerating.

			if (speed > 0 && speed < speed + decY)
				speed = 0;
			if (speed < 0 && speed > speed - decY)
				speed = 0;
		}
	}
	
	if (ZShadeSandboxMath::XMMath3(Velocity().x, Velocity().y, 0).LengthSquared() > 0.00000001)
	{
		ZShadeSandboxMath::XMMath3 heading(Velocity().x, Velocity().y, 0);
		heading.Normalize();
		vHeading = heading;
	}
	
	// Generate a new body based on movement
	GenerateBody();
}
//===============================================================================================================================
void PhysicsBody2D::CollideBodiesPlatformer(vector<PhysicsBody2D*> bodies, vector<int> &boundBodies, ContactStats* cs, float dt)
{
	// A static body cannot collide since it is not moving
	if (eType == STATIC) return;
	
	// What kind of collision has occured
	bool contactXLeft = true, contactXRight = true, contactYBottom = true, contactYTop = true;
	
	static const int iterations = 3;
	
	// If the player is standing on a moving platform, specify an offset to the desired
	// movement vector to also move him/her relative to the platform's speed
	float nextMoveXExtra = 0;
	float nextMoveYExtra = 0;
	
	if (mCurrentPlatform)
	{
		if (mCurrentPlatform->Moving())
		{
			// Compute how many pixels the platform has moved by since the last frame
			nextMoveXExtra = mCurrentPlatform->Body().x - mCurrentPlatform->PreviousBodyBounds().x;
			nextMoveYExtra = mCurrentPlatform->Body().y - mCurrentPlatform->PreviousBodyBounds().y;
		}
	}
	
	PhysicsBody2D* prevPlatform = mCurrentPlatform;
	PhysicsBody2D* platform = nullptr;
	mCurrentPlatform = nullptr;
	
	// Also store which part of the platform is touching the player
	// Used to alter the player's allowed movements for eg. ladders
	bPlatformLeft = bPlatformRight = bPlatformTop = bPlatformBottom = false;
	
	// Movement vector candidates from the collision detection code (for information only)
	static vector<std::pair<float, float> > nextMoveCandidate[iterations];

	int iteration = 0;
	for (iteration = 0; iteration < iterations; iteration++)
	{
		nextMoveCandidate[iteration].clear();
	}

	// Keep interating the contact solver until the maximum number of iterations is reached
	// or no collisions are detected
	for (iteration = 0; iteration < iterations && (contactXLeft || contactXRight || contactYBottom || contactYTop); iteration++)
	{
		// Clear the list of bounding objects the physics body is in contact with
		if (boundBodies.size() > 0) boundBodies.clear();
		
		// Calculate the amount of X and Y movement expected by the physics body
		float nextMoveX = m_HardboxPosition.x + speed * dt + nextMoveXExtra;
		float nextMoveY = m_HardboxPosition.y + speedY * dt + nextMoveYExtra;

		// Store the candidate move (for information only - not used in calculations)
		nextMoveCandidate[iteration].push_back(std::pair<float, float>(nextMoveX, nextMoveY));

		// Create to bounding body for this physics body's next move
		ZShadeSandboxMath::BoundingBox body_box = BodyBoundsPredict(XMFLOAT3(nextMoveX, nextMoveY, 0));

		// Check each tile to see if it intersects with the body bounds
		for (int o = 0; o < bodies.size(); o++)
		{
			// We don't need to check all four corners on each object and body by hand
			// Using a neat quirk of mathematics, we can take the combined bounding box
			// of the body and the object. If it's width or height exceeds the total
			// width or height of the object + the body, then the bounding boxes do
			// not intersect, otherwise they do.

			PhysicsBody2D* pb = bodies[o];

			if (pb == NULL) continue;
			
			// The tile is not collidable
			if (pb->TileInvisible()) continue;
			
			if (body_box.Intersect(pb->Body()))
			{
				boundBodies.push_back(o);
				
				if (bPlayer) cs[iteration].boundingBoxes++;
			}
		}

		// No collisions found yet
		contactXLeft = contactXRight = contactYBottom = contactYTop = false;

		float projectedMoveX, projectedMoveY;
		float originalMoveX, originalMoveY;

		float vectorLength;

		int segments;

		// Store the original final expected movement of the player so we can
		// see if it has been modified due to a collision or potential collision later
		originalMoveX = nextMoveX;
		originalMoveY = nextMoveY;
		
		// Reset the player collision point hit checks for crushing
		// Unlike the contact* variables, these are not reset after each object is tested
		// as we need to know whether the player is colliding with multiple objects from
		// different directions at the same time to detect being crushed or squashed
		bool cPtop = false, cPbottom = false, cPleft = false, cPright = false;

		// Iterate over each object whose bounding box intersects with the player's bounding box
		// until a collision is found
		for (vector<int>::iterator it = boundBodies.begin(); it != boundBodies.end() && !contactXLeft && !contactXRight && !contactYBottom && !contactYTop; it++)
		{
			// ================================================================================
			// Speculative contacts section
			//
			// We will traverse along the movement vector of the body from its current
			// position until the final position for the frame to check if any geometry lies
			// in the way. If so, the vector is adjusted to end at the geometry's intersection
			// with the physics bodies movement vector. This solves the so-called 'bullet through
			// paper' problem.
			// ================================================================================

			// If collision resolution is disabled for the object, skip speculative contacts altogether
			// since we don't need to know if a collision is coming up
			if (!bodies[(*it)]->AllowInside())
			{
				if (bodies[(*it)] == NULL) continue;

				// The tile is not collidable
				if (bodies[(*it)]->TileInvisible()) continue;

				// We will test the four possible directions of the physics body movement individually
				// dir: 0 = top, 1 = bottom, 2 = left, 3 = right
				for (int dir = 0; dir < 4; dir++)
				{
					// Skip the test if the expected direction of movement makes the test irrelevant
					// For example, we only want to test the top of the body's head when movement is
					// upwards, not downwards. This is really important! If we don't do this, we can
					// get stuck in a ceiling when falling for example.

					if (dir == 0 && nextMoveY > 0) continue;
					if (dir == 1 && nextMoveY < 0) continue;
					if (dir == 2 && nextMoveX > 0) continue;
					if (dir == 3 && nextMoveX < 0) continue;

					// Skip the test if we are allowed to collide with the object from the current
					// direction of movement

					if (dir == 0 && bodies[(*it)]->AllowFromBelow()) continue;
					if (dir == 1 && bodies[(*it)]->AllowFromAbove()) continue;
					if (dir == 2 && bodies[(*it)]->AllowFromRight()) continue;
					if (dir == 3 && bodies[(*it)]->AllowFromLeft())  continue;

					// Our current position along the anticipated movement vector of the body this frame
					projectedMoveX = projectedMoveY = 0;

					// Calculate the length of the movement vector using Pythagoras Therom
					vectorLength = sqrt(SQR(nextMoveX) + SQR(nextMoveY));

					segments = 0;

					// Advance along the vector until it intersects with one of the tiles or we are done
					while (!bodies[(*it)]->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2).y + m_HardboxPosition.y + projectedMoveY, 0))
						&& !bodies[(*it)]->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2 + 1).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2 + 1).y + m_HardboxPosition.y + projectedMoveY, 0))
						&& segments < vectorLength)
					{
						projectedMoveX += nextMoveX / vectorLength;
						projectedMoveY += nextMoveY / vectorLength;

						segments++;

						if (bPlayer) cs[iteration].speculativeContacts++;
					}

					// If we have a intersection
					if (segments < vectorLength)
					{
						// Apply correction for over-movement
						if (segments > 0)
						{
							projectedMoveX -= nextMoveX / vectorLength;
							projectedMoveY -= nextMoveY / vectorLength;
						}

						// Adjust the X or Y component of the vector depending on which
						// direction we are testing

						if (dir >= 2 && dir <= 3)
							nextMoveX = projectedMoveX;

						if (dir >= 0 && dir <= 1)
							nextMoveY = projectedMoveY;
					}
				}
			}

			// ================================================================================
			// Penetration resolution
			//
			// Here we look for existing collisions and nudge the physics body in the opposite
			// direction until the collision is solved. The purpose of iteration is because
			// the correction may cause collisions with other pieces of geometry. Iteration
			// also solves the so-called 'jitter' problem where a collision between the
			// body and the geometry is constantly solved and then re-introduced as the
			// body's position is nudged backwards and forwards between the same two points
			// repeatedly, creating jitter in the body's movement.
			// ================================================================================

			// If collision resolution is disabled for the object, we aren't going to change the movement vector
			// so we store a copy so we can restore it after the hit testing is completed
			float preresMoveX = nextMoveX, preresMoveY = nextMoveY;

			// We will test the four possible directions of movement for the physics body
			// dir: 0 = top, 1 = bottom, 2 = left, 3 = right
			for (int dir = 0; dir < 4; dir++)
			{
				// Skip the test if the expected direction of movement makes the test irrelevant
				// For example, we only want to test the top of the body's head when movement is
				// upwards, not downwards. This is really important! If we don't do this, we can
				// make corrections in the wrong direction, causing the body to magically jump
				// up to platforms or stick to ceilings.

				//if (dir == 0 && nextMoveY > 0) continue;
				//if (dir == 1 && nextMoveY < 0) continue;
				//if (dir == 2 && nextMoveX > 0) continue;
				//if (dir == 3 && nextMoveX < 0) continue;

				//projectedMoveX = (dir >= 2 ? nextMoveX : 0);
				//projectedMoveY = (dir <  2 ? nextMoveY : 0);

				if (bodies[(*it)]->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2).x + m_HardboxPosition.x + preresMoveX, CollisionPoint(dir * 2).y + m_HardboxPosition.y + preresMoveY, 0))
				&&	bodies[(*it)]->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2 + 1).x + m_HardboxPosition.x + preresMoveX, CollisionPoint(dir * 2 + 1).y + m_HardboxPosition.y + preresMoveY, 0)))
				{
					// For each case, the movement vector is corrected to avoid a penetration collision

					// Falling into an object with collision resolution disabled is a special case (case 1).
					// If we have just landed on the object from flight (falling), we can either be
					// allowed to continue falling into the object or to stop at the object's
					// surface. The latter is the desired behaviour for ladders. In other words,
					// downward collision detection is temporarily turned on when falling into
					// such an object from flight. When already touching or inside the object,
					// moving downwards is allowed so no penetration resolution is applied.
					if (dir == 0)
					{
						if (!bodies[(*it)]->AllowInside() && !bodies[(*it)]->AllowFromBelow())
						{
							projectedMoveY++;
							cPtop = true;
						}
						else
						{
							contactYTop = true;
						}
					}

					if (dir == 1)
					{
						if (!bodies[(*it)]->AllowInside() || (!prevPlatform && !bodies[(*it)]->AllowFallThrough()) && !bodies[(*it)]->AllowFromAbove())
						{
							projectedMoveY--;
							cPbottom = true;

							if (bodies[(*it)]->AllowInside())
							{
								contactYBottom = true;
							}
						}
						else
						{
							contactYBottom = true;
						}
					}

					if (dir == 2)
					{
						if (!bodies[(*it)]->AllowInside() && !bodies[(*it)]->AllowFromRight())
						{
							projectedMoveX++;
							cPleft = true;
						}
						else
						{
							contactXLeft = true;
						}
					}

					if (dir == 3)
					{
						if (!bodies[(*it)]->AllowInside() && !bodies[(*it)]->AllowFromLeft())
						{
							projectedMoveX--;
							cPright = true;
						}
						else
						{
							contactXRight = true;
						}
					}
					
					if (bPlayer) cs[iteration].penetrationCorrections++;
				}

				if (dir >= 2 && dir <= 3)
					nextMoveX = projectedMoveX;

				if (dir >= 0 && dir <= 1)
					nextMoveY = projectedMoveY;
			}

			// Check for contacts
			// IMPORTANT! We can't do this inside the directional loop because we have to make all the
			// adjustments first. Note this contact flag assignment also includes the results of the
			// speculative contacts section.

			// NOTE! We check the original move vector from before any objects were processed
			// rather than the one stored before this object's penetration resolution,
			// since we want to know if we have been displaced from the desired final position but
			// don't care about any intermediate corrections calculated during the processing
			// of previous objects.

			// While the contact* flags are reset for each object to be tested, the cP* flags are
			// aggregated over the whole iteration (all objects to be tested). They are used to test
			// if the player has been crushed by objects in the game world (death).

			// Allowed pass-through directions on pass-through platforms are not considered as
			// collisions overall but the contact* flags are retained temporarily so we can use
			// them in pass-through platform handling below. They are cancelled out afterwards.
			// Note that for pass-through platforms, this code segment only sets contact* flags
			// for sides where pass-through is not allowed. For sides where pass-through is allowed,
			// the flags are set in the loop above.
			if (!bodies[(*it)]->AllowInside())
			{
				if (nextMoveY > originalMoveY) { contactYTop = true; }
				if (nextMoveY < originalMoveY) { contactYBottom = true; }
				if (nextMoveX > originalMoveX) { contactXLeft = true; }
				if (nextMoveX < originalMoveX) { contactXRight = true; }
			}
			
			// Pass-through platform handling
			// At this point, we know which edges of the pass-through platform the player is touching,
			// and penetration resolution has been applied to only the sides where pass-through is not
			// allowed. We must now therefore consider which corrections to reverse if we are still
			// passing through the platform.

			// We leverage the fact that standingPlatform is not set (set to in flight) even when moving
			// through a pass-through platform.

			// It's important not to do these tests for objects you are allowed to collide with
			// as they can turn off contacts that should be registered

			// The preresMove* conditions are needed (mostly on contactYbottom) to prevent eg. a jump
			// or other quick move from being interrupted and the player abruptly stopped on the platform
			// just as he/she finishes passing through it.
			if (!bodies[(*it)]->AllowInside())
			{
				// If the top side is solid but we are not yet all the way through the platform,
				// don't apply penetration resolution to force the player up to the surface
				if (contactYBottom && (contactYTop || contactXLeft || contactXRight || preresMoveY < 0) && !bodies[(*it)]->AllowFromAbove() && !mCurrentPlatform)
					contactYBottom = false;

				// If the bottom side is solid but we are not yet all the way through the platform,
				// don't apply penetration resolution to force the player underneath
				if (contactYTop && (contactYBottom || contactXLeft || contactXRight || preresMoveY > 0) && !bodies[(*it)]->AllowFromBelow() && !mCurrentPlatform)
					contactYTop = false;

				// The same for one-way left and right 'doors'
				if (contactXLeft && (contactXRight || contactYTop || contactYBottom || preresMoveX > 0) && !bodies[(*it)]->AllowFromRight() && !mCurrentPlatform)
					contactXLeft = false;

				if (contactXRight && (contactXLeft || contactYTop || contactYBottom || preresMoveX < 0) && !bodies[(*it)]->AllowFromLeft() && !mCurrentPlatform)
					contactXRight = false;
			}

			// Finally, cancel out any detected collisions from directions allowed for pass-through
			// If we don't do this, unwanted corrections may be applied to the movement vector
			// causing the pass-through platforms not to work
			contactYTop &= !bodies[(*it)]->AllowFromBelow();
			contactYBottom &= !bodies[(*it)]->AllowFromAbove();
			contactXLeft &= !bodies[(*it)]->AllowFromRight();
			contactXRight &= !bodies[(*it)]->AllowFromLeft();

			// If we have just hit the surface of a platform from falling and
			// collisions are enabled for the object, reset the jumping flag
			// to allow a new jump.
			// (works also for pass-through platforms due to the manipulation of contactYbottom in the above code)
			if (contactYBottom && !prevPlatform && !bodies[(*it)]->AllowInside() && !bodies[(*it)]->AllowFromAbove())
				jumping = false;

			// If we have just hit *any* surface of a platform from anywhere
			// else and collisions are disabled for the object,
			// reset the jumping flag to allow a new jump. Once we touch such
			// an object we are considered to be 'inside' it, causing the jump
			// to end (eg. ladders)

			// The check to make sure the platform we have just hit is not the
			// last one we are standing on is important. If we jumped away from
			// a platform with disabled collision detection, we don't want to
			// be 'caught' in it before the jump has brought us far enough away
			// from it to no longer be touching the player
			if ((contactYTop || contactYBottom || contactXLeft || contactXRight) && prevPlatform != bodies[(*it)] && bodies[(*it)]->AllowInside())
				jumping = false;

			// The player can't continue jumping if we hit the side of something, must fall instead
			// Without this, a player hiting a wall during a jump will continue trying to travel
			// upwards. You may or may not want this behaviour.
			if ((contactXLeft || contactXRight) && contactYTop && speedY < 0 && !bodies[(*it)]->AllowInside() && !bodies[(*it)]->AllowFromBelow())
				speedY = nextMoveY = 0;
			
			// If we are in contact with a platform's top edge, note which one
			// Note for pass-through platforms, contactYbottom will be false until
			// the platform has been passed all the way through and landed on,
			// so no false positive pass-through platform will be set here
			if (contactYBottom && !contactYTop && !contactXLeft && !contactXRight)
			{
				platform = bodies[(*it)];
				bPlatformBottom = true;
				bPlatformLeft = bPlatformRight = bPlatformTop = false;
			}
			
			if (bodies[(*it)]->AllowInside())
			{
				// If collisions are disabled for the object but we are touching it,
				// we consider ourselves to be standing on it (eg. ladders)
				if (contactXLeft || contactXRight || contactYTop || contactYBottom)
				{
					platform = bodies[(*it)];

					bPlatformTop = contactYTop;
					bPlatformBottom = contactYBottom;
					bPlatformLeft = contactXLeft;
					bPlatformRight = contactXRight;
				}

				// If we have just touched down on the surface of a platform where fall-through
				// is not allowed, allow the player's Y position to be corrected.
				if (prevPlatform != bodies[(*it)] && contactYBottom && !bodies[(*it)]->AllowFallThrough())
				{
				}
				else
				{
					// In all other cases, ie. when standing on/in the same platform as the last frame,
					// or fall-through is allowed, do not apply penetration correction
					contactXLeft = contactXRight = contactYTop = contactYBottom = false;
				}
			}

			if (contactXLeft || contactXRight || contactYTop || contactYBottom)
			{
				if (bPlayer) cs[iteration].contacts++;
			}

			nextMoveCandidate[iteration].push_back(std::pair<float, float>(nextMoveX, nextMoveY));
		}

		// If a contact has been detected, apply the re-calculated movement vector
		// and disable any further movement this frame (in either X or Y as appropriate)

		if (contactYBottom || contactYTop)
		{
			m_TopLeftPosition.y += nextMoveY;
			m_HardboxPosition.y += nextMoveY;
			nextMoveYExtra = 0;
			speedY = 0;

			// The body is not jumping if it is touching the ground
			if (contactYBottom)
				jumping = false;
		}

		if (contactXLeft || contactXRight)
		{
			m_TopLeftPosition.x += nextMoveX;
			m_HardboxPosition.x += nextMoveX;
			nextMoveXExtra = 0;
			speed = 0;
		}
		
		// Check if the physics body is dead from being crushed
		if ((cPleft && cPright) || (cPtop && cPbottom))
		{
			bCrushed = true;
		}
	}
	
	fContactXValue = ((!contactXLeft && !contactXRight) ? nextMoveXExtra : 0);
	fContactYValue = ((!contactYTop && !contactYBottom) ? nextMoveXExtra : 0);
	
	mCurrentPlatform = platform;
}
//===============================================================================================================================
void PhysicsBody2D::CollideBodiesTopdown(vector<PhysicsBody2D*> bodies, vector<int> &boundBodies, ContactStats* cs, float dt)
{
	// A static body cannot collide since it is not moving
	if (eType == STATIC) return;
	
	// What kind of collision has occured
	bool contactXLeft = true, contactXRight = true, contactYBottom = true, contactYTop = true;
	
	// What direction did the collision come from
	bool bTop, bBottom, bRight, bLeft;
	
	static const int iterations = 3;
	
	// Movement vector candidates from the collision detection code (for information only)
	static vector<std::pair<float, float> > nextMoveCandidate[iterations];
	
	int iteration = 0;
	for (iteration = 0; iteration < iterations; iteration++)
	{
		nextMoveCandidate[iteration].clear();
	}

	// Keep interating the contact solver until the maximum number of iterations is reached
	// or no collisions are detected
	for (iteration = 0; iteration < iterations && (contactXLeft || contactXRight || contactYBottom || contactYTop); iteration++)
	{
		// Clear the list of bounding objects the physics body is in contact with
		if (boundBodies.size() > 0) boundBodies.clear();
		
		// Calculate the amount of X and Y movement expected by the physics body
		float nextMoveX = m_HardboxPosition.x + speed * dt;
		float nextMoveY = m_HardboxPosition.y + speed * dt;

		// Store the candidate move (for information only - not used in calculations)
		nextMoveCandidate[iteration].push_back(std::pair<float, float>(nextMoveX, nextMoveY));

		// Create to bounding body for this physics body's next move
		ZShadeSandboxMath::BoundingBox body_box = BodyBoundsPredict(XMFLOAT3(nextMoveX, nextMoveY, 0));

		// Check each tile to see if it intersects with the body bounds
		for (int o = 0; o < bodies.size(); o++)
		{
			// We don't need to check all four corners on each object and body by hand
			// Using a neat quirk of mathematics, we can take the combined bounding box
			// of the body and the object. If it's width or height exceeds the total
			// width or height of the object + the body, then the bounding boxes do
			// not intersect, otherwise they do.

			PhysicsBody2D* pb = bodies[o];

			if (pb == NULL) continue;
			
			if (body_box.Intersect(pb->Body()))
			{
				boundBodies.push_back(o);
				
				if (bPlayer) cs[iteration].boundingBoxes++;
			}
		}

		// No collisions found yet
		contactXLeft = contactXRight = contactYBottom = contactYTop = false;

		float projectedMoveX, projectedMoveY;
		float originalMoveX, originalMoveY;

		float vectorLength;

		int segments;

		// Store the original final expected movement of the player so we can
		// see if it has been modified due to a collision or potential collision later
		originalMoveX = nextMoveX;
		originalMoveY = nextMoveY;
		
		// Reset the player collision point hit checks for crushing
		// Unlike the contact* variables, these are not reset after each object is tested
		// as we need to know whether the player is colliding with multiple objects from
		// different directions at the same time to detect being crushed or squashed
		bool cPtop = false, cPbottom = false, cPleft = false, cPright = false;

		// Iterate over each object whose bounding box intersects with the player's bounding box
		// until a collision is found
		for (vector<int>::iterator it = boundBodies.begin(); it != boundBodies.end() && !contactXLeft && !contactXRight && !contactYBottom && !contactYTop; it++)
		{
			// ================================================================================
			// Speculative contacts section
			//
			// We will traverse along the movement vector of the body from its current
			// position until the final position for the frame to check if any geometry lies
			// in the way. If so, the vector is adjusted to end at the geometry's intersection
			// with the physics bodies movement vector. This solves the so-called 'bullet through
			// paper' problem.
			// ================================================================================

			// If collision resolution is disabled for the object, skip speculative contacts altogether
			// since we don't need to know if a collision is coming up
			if (!bodies[(*it)]->AllowInside())
			{
				if (bodies[(*it)] == NULL) continue;

				// We will test the four possible directions of the physics body movement individually
				// dir: 0 = top, 1 = bottom, 2 = left, 3 = right
				for (int dir = 0; dir < 4; dir++)
				{
					// Skip the test if the expected direction of movement makes the test irrelevant
					// For example, we only want to test the top of the body's head when movement is
					// upwards, not downwards. This is really important! If we don't do this, we can
					// get stuck in a ceiling when falling for example.

					if (dir == 0 && nextMoveY > 0) continue;
					if (dir == 1 && nextMoveY < 0) continue;
					if (dir == 2 && nextMoveX > 0) continue;
					if (dir == 3 && nextMoveX < 0) continue;

					// Our current position along the anticipated movement vector of the body this frame
					projectedMoveX = projectedMoveY = 0;

					// Calculate the length of the movement vector using Pythagoras Therom
					vectorLength = sqrt(SQR(nextMoveX) + SQR(nextMoveY));

					segments = 0;

					// Advance along the vector until it intersects with one of the tiles or we are done
					while (!bodies[(*it)]->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2).y + m_HardboxPosition.y + projectedMoveY, 0))
						&& !bodies[(*it)]->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2 + 1).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2 + 1).y + m_HardboxPosition.y + projectedMoveY, 0))
						&& segments < vectorLength)
					{
						projectedMoveX += nextMoveX / vectorLength;
						projectedMoveY += nextMoveY / vectorLength;

						segments++;
					}

					// If we have a intersection
					if (segments < vectorLength)
					{
						// Apply correction for over-movement
						if (segments > 0)
						{
							projectedMoveX -= nextMoveX / vectorLength;
							projectedMoveY -= nextMoveY / vectorLength;
						}

						// Adjust the X or Y component of the vector depending on which
						// direction we are testing

						if (dir >= 2 && dir <= 3)
							nextMoveX = projectedMoveX;

						if (dir >= 0 && dir <= 1)
							nextMoveY = projectedMoveY;
					}
				}
			}

			// ================================================================================
			// Penetration resolution
			//
			// Here we look for existing collisions and nudge the physics body in the opposite
			// direction until the collision is solved. The purpose of iteration is because
			// the correction may cause collisions with other pieces of geometry. Iteration
			// also solves the so-called 'jitter' problem where a collision between the
			// body and the geometry is constantly solved and then re-introduced as the
			// body's position is nudged backwards and forwards between the same two points
			// repeatedly, creating jitter in the body's movement.
			// ================================================================================

			// We will test the four possible directions of movement for the physics body
			// dir: 0 = top, 1 = bottom, 2 = left, 3 = right
			for (int dir = 0; dir < 4; dir++)
			{
				// Skip the test if the expected direction of movement makes the test irrelevant
				// For example, we only want to test the top of the body's head when movement is
				// upwards, not downwards. This is really important! If we don't do this, we can
				// make corrections in the wrong direction, causing the body to magically jump
				// up to platforms or stick to ceilings.

				if (dir == 0 && nextMoveY > 0) continue;
				if (dir == 1 && nextMoveY < 0) continue;
				if (dir == 2 && nextMoveX > 0) continue;
				if (dir == 3 && nextMoveX < 0) continue;

				projectedMoveX = (dir >= 2 ? nextMoveX : 0);
				projectedMoveY = (dir <  2 ? nextMoveY : 0);

				// Traverse backwards in X or Y (but not both at the same time)
				// until the body is no longer colliding with the geometry

				// Note: This code also enables walking up gently sloping surfaces:
				// as the force of gravity pulls down on the player and causes surface contact,
				// the correction pushes the body away from the inside of the platform up
				// to the surface. Without this, the body would get stuck in any sloping
				// platform.
				// This same code also prevents walking up platforms that are too steeply sloped.
				// The act of falling causes collisions with the left or right side of the
				// body, causing him/her to be nudged left or right and be free to continue
				// falling further. The maximum incline of the slope the body can walk up
				// is directly related to the selected gravitational force.

				// Advance along the vector until it intersects with one of the tiles or we are done
				while (!bodies[(*it)]->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2).y + m_HardboxPosition.y + projectedMoveY, 0))
					&& !bodies[(*it)]->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2 + 1).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2 + 1).y + m_HardboxPosition.y + projectedMoveY, 0)))
				{
					if (dir == 0) projectedMoveY++;
					if (dir == 1) projectedMoveY--;
					if (dir == 2) projectedMoveX++;
					if (dir == 3) projectedMoveX--;
				}

				if (dir >= 2 && dir <= 3)
					nextMoveX = projectedMoveX;

				if (dir >= 0 && dir <= 1)
					nextMoveY = projectedMoveY;
			}
			
			// Detect what type of contact has occured based on a comparison of the original
			// expected movement vector and the new one
			
			if (nextMoveY > originalMoveY && originalMoveY < 0)
				contactYTop = true;

			if (nextMoveY < originalMoveY && originalMoveY > 0)
				contactYBottom = true;

			//if (abs(nextMoveX - originalMoveX) > 0.01f)
			//	contactX = true;

			// The body cannot continue jumping if we hit the side of something, must fall instead
			// Without this, a body hitting a wall during a jump will continue traveling upwards.
			// A topdown map does not have jumping
			//if (contactX && speed < 0)
			//	speed = nextMoveY = 0;
		}
		
		// If a contact has been detected, apply the re-calculated movement vector
		// and disable any further movement this frame (in either X or Y as appropriate)

		if (contactYBottom || contactYTop)
		{
			m_TopLeftPosition.y += nextMoveY;
			m_HardboxPosition.y += nextMoveY;

			speed = 0;
			
			// The body is not jumping if it is touching the ground
			// Body has made contact on bottom of other body
			// A topdown map does not have jumping
			if (contactYBottom)
			{
				jumping = false;
				bBottom = true;
			}
			
			// Body has made contact on top of another body
			if (contactYTop)
				bTop = true;
		}

		if (contactXLeft || contactXRight)
		{
			m_TopLeftPosition.x += nextMoveX;
			m_HardboxPosition.x += nextMoveX;

			// Which direction is the body facing
			if (speed < 0)
				bLeft = true;
			else
				bRight = true;
			
			speed = 0;
		}
	}
}
//===============================================================================================================================
bool PhysicsBody2D::CollidesBodyPlatformer(PhysicsBody2D* body, float dt)
{
	// A static body cannot collide since it is not moving
	if (eType == STATIC) return false;
	
	if (body == NULL) return false;
	
	// What direction did the collision come from
	bool bTop, bBottom, bRight, bLeft;
	
	// What kind of collision has occured
	bool contactX = true, contactYBottom = true, contactYTop = true;
	
	static const int iterations = 3;
	
	// Keep interating the contact solver until the maximum number of iterations is reached
	// or no collisions are detected
	for (int iteration = 0; iteration < iterations && (contactX || contactYBottom || contactYTop); iteration++)
	{
		// Calculate the amount of X and Y movement expected by the physics body
		float nextMoveX = m_HardboxPosition.x + speed * dt;
		float nextMoveY = m_HardboxPosition.y + speedY * dt;

		// Create to bounding body for this physics body's next move
		ZShadeSandboxMath::BoundingBox body_box = BodyBoundsPredict(XMFLOAT3(nextMoveX, nextMoveY, 0));
		
		// Check to see if the two bodies are intersecting
		bool intersecting = false;
		
		// We don't need to check all four corners on each body and current body by hand
		// Using a neat quirk of mathematics, we can take the combined bounding box
		// of the body and the other body. If it's width or height exceeds the total
		// width or height of the other body + the current body, then the bounding boxes do
		// not intersect, otherwise they do.

		/*BodyBox bb;

		bb.left = min(BBodyBox().left, body->BBodyBox().left);
		bb.top = min(BBodyBox().top, body->BBodyBox().top);
		bb.right = min(BBodyBox().right, body->BBodyBox().right);
		bb.bottom = min(BBodyBox().bottom, body->BBodyBox().bottom);

		// If a bounding box intersects with the player's bounding box, add the
		// geometry to the list of geometries to check for collisions
		if (bb.right - bb.left < body->BBodyBox().right - body->BBodyBox().left + BBodyBox().right - BBodyBox().left &&
			bb.bottom - bb.top < body->BBodyBox().bottom - body->BBodyBox().top + BBodyBox().bottom - BBodyBox().top)
		{
			intersecting = true;
		}*/
		
		if (body_box.Intersect(body->Body()))
		{
			intersecting = true;
		}

		
		// No collisions found yet
		contactX = contactYBottom = contactYTop = false;

		float projectedMoveX, projectedMoveY;
		float originalMoveX, originalMoveY;

		float vectorLength;

		int segments;

		// Store the original final expected movement of the player so we can
		// see if it has been modified due to a collision or potential collision later
		originalMoveX = nextMoveX;
		originalMoveY = nextMoveY;
		
		// If the boxes are not intersecting then do nothing
		if (intersecting)
		{
			// Check Speculative contacts and see what side the collision is occuring
			
			// We will test the four possible directions of the physics body movement individually
			// dir: 0 = top, 1 = bottom, 2 = left, 3 = right
			for (int dir = 0; dir < 4; dir++)
			{
				// Skip the test if the expected direction of movement makes the test irrelevant
				// For example, we only want to test the top of the body's head when movement is
				// upwards, not downwards. This is really important! If we don't do this, we can
				// get stuck in a ceiling when falling for example.

				if (dir == 0 && nextMoveY > 0) continue;
				if (dir == 1 && nextMoveY < 0) continue;
				if (dir == 2 && nextMoveX > 0) continue;
				if (dir == 3 && nextMoveX < 0) continue;

				// Our current position along the anticipated movement vector of the body this frame
				projectedMoveX = projectedMoveY = 0;

				// Calculate the length of the movement vector using Pythagoras Therom
				vectorLength = sqrt(SQR(nextMoveX) + SQR(nextMoveY));

				segments = 0;

				// Advance along the vector until it intersects with one of the tiles or we are done
				while (!body->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2).y + m_HardboxPosition.y + projectedMoveY, 0))
					&& !body->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2 + 1).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2 + 1).y + m_HardboxPosition.y + projectedMoveY, 0))
					&& segments < vectorLength)
				{
					projectedMoveX += nextMoveX / vectorLength;
					projectedMoveY += nextMoveY / vectorLength;

					segments++;
				}

				// If we have a intersection
				if (segments < vectorLength)
				{
					// Apply correction for over-movement
					if (segments > 0)
					{
						projectedMoveX -= nextMoveX / vectorLength;
						projectedMoveY -= nextMoveY / vectorLength;
					}

					// Adjust the X or Y component of the vector depending on which
					// direction we are testing

					if (dir >= 2 && dir <= 3)
						nextMoveX = projectedMoveX;

					if (dir >= 0 && dir <= 1)
						nextMoveY = projectedMoveY;
				}
			}
			
			// Penetration resolution
			
			// We will test the four possible directions of movement for the physics body
			// dir: 0 = top, 1 = bottom, 2 = left, 3 = right
			for (int dir = 0; dir < 4; dir++)
			{
				// Skip the test if the expected direction of movement makes the test irrelevant
				// For example, we only want to test the top of the body's head when movement is
				// upwards, not downwards. This is really important! If we don't do this, we can
				// make corrections in the wrong direction, causing the body to magically jump
				// up to platforms or stick to ceilings.

				if (dir == 0 && nextMoveY > 0) continue;
				if (dir == 1 && nextMoveY < 0) continue;
				if (dir == 2 && nextMoveX > 0) continue;
				if (dir == 3 && nextMoveX < 0) continue;

				projectedMoveX = (dir >= 2 ? nextMoveX : 0);
				projectedMoveY = (dir <  2 ? nextMoveY : 0);

				// Traverse backwards in X or Y (but not both at the same time)
				// until the body is no longer colliding with the geometry

				// Note: This code also enables walking up gently sloping surfaces:
				// as the force of gravity pulls down on the player and causes surface contact,
				// the correction pushes the body away from the inside of the platform up
				// to the surface. Without this, the body would get stuck in any sloping
				// platform.
				// This same code also prevents walking up platforms that are too steeply sloped.
				// The act of falling causes collisions with the left or right side of the
				// body, causing him/her to be nudged left or right and be free to continue
				// falling further. The maximum incline of the slope the body can walk up
				// is directly related to the selected gravitational force.

				// Advance along the vector until it intersects with one of the tiles or we are done
				while (!body->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2).y + m_HardboxPosition.y + projectedMoveY, 0))
					&& !body->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2 + 1).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2 + 1).y + m_HardboxPosition.y + projectedMoveY, 0)))
				{
					if (dir == 0) projectedMoveY++;
					if (dir == 1) projectedMoveY--;
					if (dir == 2) projectedMoveX++;
					if (dir == 3) projectedMoveX--;
				}

				if (dir >= 2 && dir <= 3)
					nextMoveX = projectedMoveX;

				if (dir >= 0 && dir <= 1)
					nextMoveY = projectedMoveY;
			}
			
			// Detect what type of contact has occured based on a comparison of the original
			// expected movement vector and the new one
			
			if (nextMoveY > originalMoveY && originalMoveY < 0)
				contactYTop = true;

			if (nextMoveY < originalMoveY && originalMoveY > 0)
				contactYBottom = true;

			if (abs(nextMoveX - originalMoveX) > 0.01f)
				contactX = true;

			// The body cannot continue jumping if we hit the side of something, must fall instead
			// Without this, a body hitting a wall during a jump will continue traveling upwards.
			if (contactX && speedY < 0)
				speedY = nextMoveY = 0;
		}
		else break;
		
		// If a contact has been detected, apply the re-calculated movement vector
		// and disable any further movement this frame (in either X or Y as appropriate)

		if (contactYBottom || contactYTop)
		{
			m_TopLeftPosition.y += nextMoveY;
			m_HardboxPosition.y += nextMoveY;

			speedY = 0;

			// The body is not jumping if it is touching the ground
			// Body has made contact on bottom of other body
			if (contactYBottom)
			{
				jumping = false;
				bBottom = true;
			}
			
			// Body has made contact on top of other body
			if (contactYTop)
				bTop = true;
		}

		if (contactX)
		{
			m_TopLeftPosition.x += nextMoveX;
			m_HardboxPosition.x += nextMoveX;

			// Which direction is the body facing
			if (speed < 0)
				bLeft = true;
			else
				bRight = true;
			
			speed = 0;
		}
	}
	
	// If we have any contact
	if (bTop || bBottom || bLeft || bRight)
	{
		// Update the body as to what part it has been hit: top, bottom, left, right
		OnCollision(bTop, bBottom, bLeft, bRight, body->DamageToOther());
		
		// Also update the other body to notify where it is being hit
		// The other body is hit on the opposite direction the current body is being hit
		body->OnCollision(bBottom, bTop, bRight, bLeft, iDamageToOther);
		
		// Found a contact
		return true;
	}
	
	// Did not find any contact
	return false;
}
//===============================================================================================================================
bool PhysicsBody2D::CollidesBodyTopdown(PhysicsBody2D* body, float dt)
{
	// A static body cannot collide since it is not moving
	if (eType == STATIC) return false;
	
	if (body == NULL) return false;
	
	// What direction did the collision come from
	bool bTop, bBottom, bRight, bLeft;
	
	// What kind of collision has occured
	bool contactX = true, contactYBottom = true, contactYTop = true;
	
	static const int iterations = 3;
	
	// Keep interating the contact solver until the maximum number of iterations is reached
	// or no collisions are detected
	for (int iteration = 0; iteration < iterations && (contactX || contactYBottom || contactYTop); iteration++)
	{
		// Calculate the amount of X and Y movement expected by the physics body
		float nextMoveX = m_HardboxPosition.x + speed * dt;
		float nextMoveY = m_HardboxPosition.y + speed * dt;

		// Create to bounding body for this physics body's next move
		ZShadeSandboxMath::BoundingBox body_box = BodyBoundsPredict(XMFLOAT3(nextMoveX, nextMoveY, 0));
		
		// Check to see if the two bodies are intersecting
		bool intersecting = false;
		
		// We don't need to check all four corners on each body and current body by hand
		// Using a neat quirk of mathematics, we can take the combined bounding box
		// of the body and the other body. If it's width or height exceeds the total
		// width or height of the other body + the current body, then the bounding boxes do
		// not intersect, otherwise they do.
		
		if (body_box.Intersect(body->Body()))
		{
			intersecting = true;
		}

		
		// No collisions found yet
		contactX = contactYBottom = contactYTop = false;

		float projectedMoveX, projectedMoveY;
		float originalMoveX, originalMoveY;

		float vectorLength;

		int segments;

		// Store the original final expected movement of the player so we can
		// see if it has been modified due to a collision or potential collision later
		originalMoveX = nextMoveX;
		originalMoveY = nextMoveY;
		
		// If the boxes are not intersecting then do nothing
		if (intersecting)
		{
			// Check Speculative contacts and see what side the collision is occuring
			
			// We will test the four possible directions of the physics body movement individually
			// dir: 0 = top, 1 = bottom, 2 = left, 3 = right
			for (int dir = 0; dir < 4; dir++)
			{
				// Skip the test if the expected direction of movement makes the test irrelevant
				// For example, we only want to test the top of the body's head when movement is
				// upwards, not downwards. This is really important! If we don't do this, we can
				// get stuck in a ceiling when falling for example.

				if (dir == 0 && nextMoveY > 0) continue;
				if (dir == 1 && nextMoveY < 0) continue;
				if (dir == 2 && nextMoveX > 0) continue;
				if (dir == 3 && nextMoveX < 0) continue;

				// Our current position along the anticipated movement vector of the body this frame
				projectedMoveX = projectedMoveY = 0;

				// Calculate the length of the movement vector using Pythagoras Therom
				vectorLength = sqrt(SQR(nextMoveX) + SQR(nextMoveY));

				segments = 0;

				// Advance along the vector until it intersects with one of the tiles or we are done
				while (!body->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2).y + m_HardboxPosition.y + projectedMoveY, 0))
					&& !body->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2 + 1).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2 + 1).y + m_HardboxPosition.y + projectedMoveY, 0))
					&& segments < vectorLength)
				{
					projectedMoveX += nextMoveX / vectorLength;
					projectedMoveY += nextMoveY / vectorLength;

					segments++;
				}

				// If we have a intersection
				if (segments < vectorLength)
				{
					// Apply correction for over-movement
					if (segments > 0)
					{
						projectedMoveX -= nextMoveX / vectorLength;
						projectedMoveY -= nextMoveY / vectorLength;
					}

					// Adjust the X or Y component of the vector depending on which
					// direction we are testing

					if (dir >= 2 && dir <= 3)
						nextMoveX = projectedMoveX;

					if (dir >= 0 && dir <= 1)
						nextMoveY = projectedMoveY;
				}
			}
			
			// Penetration resolution
			
			// We will test the four possible directions of movement for the physics body
			// dir: 0 = top, 1 = bottom, 2 = left, 3 = right
			for (int dir = 0; dir < 4; dir++)
			{
				// Skip the test if the expected direction of movement makes the test irrelevant
				// For example, we only want to test the top of the body's head when movement is
				// upwards, not downwards. This is really important! If we don't do this, we can
				// make corrections in the wrong direction, causing the body to magically jump
				// up to platforms or stick to ceilings.

				if (dir == 0 && nextMoveY > 0) continue;
				if (dir == 1 && nextMoveY < 0) continue;
				if (dir == 2 && nextMoveX > 0) continue;
				if (dir == 3 && nextMoveX < 0) continue;

				projectedMoveX = (dir >= 2 ? nextMoveX : 0);
				projectedMoveY = (dir <  2 ? nextMoveY : 0);

				// Traverse backwards in X or Y (but not both at the same time)
				// until the body is no longer colliding with the geometry

				// Note: This code also enables walking up gently sloping surfaces:
				// as the force of gravity pulls down on the player and causes surface contact,
				// the correction pushes the body away from the inside of the platform up
				// to the surface. Without this, the body would get stuck in any sloping
				// platform.
				// This same code also prevents walking up platforms that are too steeply sloped.
				// The act of falling causes collisions with the left or right side of the
				// body, causing him/her to be nudged left or right and be free to continue
				// falling further. The maximum incline of the slope the body can walk up
				// is directly related to the selected gravitational force.

				// Advance along the vector until it intersects with one of the tiles or we are done
				while (!body->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2).y + m_HardboxPosition.y + projectedMoveY, 0))
					&& !body->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2 + 1).x + m_HardboxPosition.x + projectedMoveX, CollisionPoint(dir * 2 + 1).y + m_HardboxPosition.y + projectedMoveY, 0)))
				{
					if (dir == 0) projectedMoveY++;
					if (dir == 1) projectedMoveY--;
					if (dir == 2) projectedMoveX++;
					if (dir == 3) projectedMoveX--;
				}

				if (dir >= 2 && dir <= 3)
					nextMoveX = projectedMoveX;

				if (dir >= 0 && dir <= 1)
					nextMoveY = projectedMoveY;
			}
			
			// Detect what type of contact has occured based on a comparison of the original
			// expected movement vector and the new one
			
			if (nextMoveY > originalMoveY && originalMoveY < 0)
				contactYTop = true;

			if (nextMoveY < originalMoveY && originalMoveY > 0)
				contactYBottom = true;

			if (abs(nextMoveX - originalMoveX) > 0.01f)
				contactX = true;

			// The body cannot continue jumping if we hit the side of something, must fall instead
			// Without this, a body hitting a wall during a jump will continue traveling upwards.
			//if (contactX && speedY < 0)
			//	speedY = nextMoveY = 0;
		}
		else break;
		
		// If a contact has been detected, apply the re-calculated movement vector
		// and disable any further movement this frame (in either X or Y as appropriate)

		if (contactYBottom || contactYTop)
		{
			m_TopLeftPosition.y += nextMoveY;
			m_HardboxPosition.y += nextMoveY;

			speed = 0;

			// The body is not jumping if it is touching the ground
			// Body has made contact on bottom of other body
			if (contactYBottom)
			{
				jumping = false; // A topdown map does not have jumping
				bBottom = true;
			}
			
			// Body has made contact on top of other body
			if (contactYTop)
				bTop = true;
		}

		if (contactX)
		{
			m_TopLeftPosition.x += nextMoveX;
			m_HardboxPosition.x += nextMoveX;

			// Which direction is the body facing
			if (speed < 0)
				bLeft = true;
			else
				bRight = true;
			
			speed = 0;
		}
	}
	
	// If we have any contact
	if (bTop || bBottom || bLeft || bRight)
	{
		// Update the body as to what part it has been hit: top, bottom, left, right
		OnCollision(bTop, bBottom, bLeft, bRight, body->DamageToOther());
		
		// Also update the other body to notify where it is being hit
		// The other body is hit on the opposite direction the current body is being hit
		body->OnCollision(bBottom, bTop, bRight, bLeft, iDamageToOther);
		
		// Found a contact
		return true;
	}
	
	// Did not find any contact
	return false;
}
//===============================================================================================================================