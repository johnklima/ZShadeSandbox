#include "AISteeringForce.h"
#include "Environment2DMapHelper.h"
#include "WaypointSystem.h"
#include "AISprite.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
//==================================================================================================================================
//==================================================================================================================================
AISteeringForce::AISteeringForce(AISprite* sprite)
:	mSprite(sprite)
{
	m_wanderTarget.x = mSprite->TopLeftPosition().x;
	m_wanderTarget.y = mSprite->TopLeftPosition().y;
	m_wanderTarget.z = 0;
	fWanderingRadius = 0;
	fWanderingDistance = 0;
	fWanderingJitter = 0;
	fDbBoxLength = 0;
	fMinDetectionBoxLength = 0;
	fWaypointSeekDist = 0;
	m_waypointPath = 0;
	mEvader = 0;
	mPursuer = 0;
	mTarget = 0;
	mSpriteA = 0;
	mSpriteB = 0;
	mLeader = 0;
	mLeaderOffset = ZShadeSandboxMath::XMMath3(0, 0, 0);
	mTargetPoint = ZShadeSandboxMath::XMMath3(0, 0, 0);
	mTargetDecelerationType = EDeceleration::Type::eFast;
	fWeightSpriteAvoidance = 1;
	fWeightSeparation = 1;
	fWeightAlignment = 1;
	fWeightWander = 1;
	fWeightFollowPath = 1;
	fWeightPursueTarget = 1;
	fWeightEvadeTarget = 1;
	fWeightInterpose = 1;
	fWeightHide = 1;
	fWeightOffsetPursuit = 1;
	bSeek = false;
	bFlee = false;
	bArrive = false;
	bAvoidSprites = false;
	bFlockSeparation = false;
	bFlockAlignment = false;
	bFlockCohesion = false;
	bWander = false;
	bFollowPath = false;
	bPursueTarget = false;
	bEvadeTarget = false;
	bInterpose = false;
	bHide = false;
	bOffsetPursuit = false;
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::Seek(ZShadeSandboxMath::XMMath3 targetPoint)
{
	ZShadeSandboxMath::XMMath3 sprPosition(mSprite->TopLeftPosition().x, mSprite->TopLeftPosition().y, mSprite->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 velocity(mSprite->Velocity().x, mSprite->Velocity().y, 0);
	
	ZShadeSandboxMath::XMMath3 desiredVelocity = targetPoint - sprPosition;
	desiredVelocity.Normalize();
	desiredVelocity *= mSprite->MaxSpeed();
	
	return desiredVelocity - velocity;
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::Flee(ZShadeSandboxMath::XMMath3 targetPoint)
{
	ZShadeSandboxMath::XMMath3 sprPosition(mSprite->TopLeftPosition().x, mSprite->TopLeftPosition().y, mSprite->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 velocity(mSprite->Velocity().x, mSprite->Velocity().y, 0);
	
	float distSqr = fMaxFleeDistance * fMaxFleeDistance;
	if (sprPosition.DistanceSquared(targetPoint) > distSqr)
	{
		return ZShadeSandboxMath::XMMath3(0, 0, 0);
	}
	
	ZShadeSandboxMath::XMMath3 desiredVelocity = sprPosition - targetPoint;
	desiredVelocity.Normalize();
	desiredVelocity *= mSprite->MaxSpeed();
	
	return desiredVelocity - velocity;
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::Arrive(ZShadeSandboxMath::XMMath3 targetPoint, EDeceleration::Type deceleration)
{
	ZShadeSandboxMath::XMMath3 sprPosition(mSprite->TopLeftPosition().x, mSprite->TopLeftPosition().y, mSprite->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 velocity(mSprite->Velocity().x, mSprite->Velocity().y, 0);
	ZShadeSandboxMath::XMMath3 toTarget = targetPoint - sprPosition;
	
	float dist = toTarget.Length();
	
	if (dist > 0)
	{
		// Deceleration is enumerated as an integer, so this value is required to provide fine tweaking
		const float decelerationTweaker = 0.3f;
		
		float speedX = dist / ((float)deceleration * decelerationTweaker);
		float speedY = dist / ((float)deceleration * decelerationTweaker);
		
		// Clamp the speed so that it does not exceed maximum speed
		speedX = min(speedX, mSprite->MaxSpeed().x);
		speedY = min(speedY, mSprite->MaxSpeed().y);

		// Do not need to normalize the toTarget vector because we have already gone to the trouble
		// of calculating its length
		ZShadeSandboxMath::XMMath3 desiredVelocity = (toTarget * ZShadeSandboxMath::XMMath3(speedX, speedY, 0)) / dist;
		
		return desiredVelocity - velocity;
	}
	
	return ZShadeSandboxMath::XMMath3(0, 0, 0);
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::Pursuit(AISprite* evader)
{
	ZShadeSandboxMath::XMMath3 sprPosition(mSprite->TopLeftPosition().x, mSprite->TopLeftPosition().y, mSprite->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 velocity(mSprite->Velocity().x, mSprite->Velocity().y, 0);
	ZShadeSandboxMath::XMMath3 sprHeading(mSprite->Heading().x, mSprite->Heading().y, 0);
	ZShadeSandboxMath::XMMath3 evaderPosition(evader->TopLeftPosition().x, evader->TopLeftPosition().y, evader->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 evaderVelocity(evader->Velocity().x, evader->Velocity().y, 0);
	ZShadeSandboxMath::XMMath3 evaderHeading(evader->Heading().x, evader->Heading().y, 0);
	
	ZShadeSandboxMath::XMMath3 toEvader = evaderPosition - sprPosition;
	
	float realativeHeading = sprHeading.Dot(evaderHeading);
	
	if (toEvader.Dot(sprHeading) > 0 && realativeHeading < -0.95)
	{
		return Seek(evaderPosition);
	}
	
	// Predict where the evader will be
	float lookAheadTimeX = toEvader.Length() / (mSprite->MaxSpeed().x + evader->MaxSpeed().x);
	float lookAheadTimeY = toEvader.Length() / (mSprite->MaxSpeed().y + evader->MaxSpeed().y);
	
	// Seek to the predicted future position
	return Seek(evaderPosition + evaderVelocity * ZShadeSandboxMath::XMMath3(lookAheadTimeX, lookAheadTimeY, 0));
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::Evade(AISprite* pursuer)
{
	ZShadeSandboxMath::XMMath3 sprPosition(mSprite->TopLeftPosition().x, mSprite->TopLeftPosition().y, mSprite->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 velocity(mSprite->Velocity().x, mSprite->Velocity().y, 0);
	ZShadeSandboxMath::XMMath3 pursuerPosition(pursuer->TopLeftPosition().x, pursuer->TopLeftPosition().y, pursuer->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 pursuerVelocity(pursuer->Velocity().x, pursuer->Velocity().y, 0);
	
	ZShadeSandboxMath::XMMath3 toPursuer = pursuerPosition - sprPosition;
	
	// The look-ahead time is proportional to the distance between the pursuer and the evader
	// and is inversely proportional to the sum of the agents' velocities
	float lookAheadTimeX = toPursuer.Length() / (mSprite->MaxSpeed().x + pursuer->MaxSpeed().x);
	float lookAheadTimeY = toPursuer.Length() / (mSprite->MaxSpeed().y + pursuer->MaxSpeed().y);

	// Now flee away from the predicted future position
	return Flee(pursuerPosition + pursuerVelocity * ZShadeSandboxMath::XMMath3(lookAheadTimeX, lookAheadTimeY, 0));
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::Wander()
{
	// Create the impression of a random walk through the environment
	
	ZShadeSandboxMath::ZMath::RandomSeed();

	float rval1 = ZShadeSandboxMath::ZMath::RandF<float>(-1, 1);
	float rval2 = ZShadeSandboxMath::ZMath::RandF<float>(-1, 1);

	m_wanderTarget.x += (rval1 * fWanderingJitter);
	m_wanderTarget.y += (rval2 * fWanderingJitter);
	
	// Re-project this new vector back into a unit circle
	m_wanderTarget.Normalize();
	
	// Increase the length of the vector to the same as the radius of the wander circle
	m_wanderTarget *= fWanderingRadius;
	
	// Move the target into a position WanderDist in front of the agent
	ZShadeSandboxMath::XMMath3 targetLocal = m_wanderTarget + ZShadeSandboxMath::XMMath3(fWanderingDistance, 0, 0);
	
	ZShadeSandboxMath::XMMath3 sprPosition(mSprite->TopLeftPosition().x, mSprite->TopLeftPosition().y, mSprite->TopLeftPosition().z);
	
	return targetLocal - sprPosition;
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::AvoidSprite(std::vector<AISprite*> &spriteObstacles)
{
	// The detection box length is proportional to the agent's velocity
	fDbBoxLength = fMinDetectionBoxLength + (mSprite->Speed().x / mSprite->MaxSpeed().y) * fMinDetectionBoxLength;
	
	// Tag all obstacles within range of the box for processing
	// mSprite->World()->TagObstaclesWithinViewRange(mSprite, fDbBoxLength);
	mSprite->TagNearestSpritesInMap();
	
	AISprite* closestIntersectingSprite = 0;
	
	float distToClosestIP = FLT_MAX;
	
	ZShadeSandboxMath::XMMath3 localPosOfClosestObstacle;
	
	auto curOb = spriteObstacles.begin();
	
	while (curOb != spriteObstacles.end())
	{
		// If the obstacle has been tagged within range proceed
		if ((*curOb)->IsTagged())
		{
			ZShadeSandboxMath::XMMath3 obstaclePosition((*curOb)->TopLeftPosition().x, (*curOb)->TopLeftPosition().y, (*curOb)->TopLeftPosition().z);
			
			// If the position has a negative X value then it must lie behind the agent
			if (obstaclePosition.x >= 0)
			{
				// If the distance from the x axis to the object's position is less than
				// its radius + half the width of the detection box then there is
				// a potential intersection
				float expandedRadius = (*curOb)->BoundingRadius() + mSprite->BoundingRadius();
				
				if (abs(obstaclePosition.y) < expandedRadius)
				{
					// Do a line/circle intersection test. The center of the circle is represented by (cX, cY).
					// The intersection points are given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0.
					// We only need to look at the smallest positive value of X because that will
					// be the closest point of intersection.
					float cX = obstaclePosition.x;
					float cY = obstaclePosition.y;
					
					float sqrtPart = sqrt(expandedRadius * expandedRadius - cY * cY);
					
					float ip = 0;// A - sqrtPart;
					
					//if (ip <= 0)
					//{
					//	ip = A + sqrtPart;
					//}
					
					// Test to see if this is the closest so far. If it is, keep a record of the obstacle and its
					// local coordinates
					
					if (ip < distToClosestIP)
					{
						distToClosestIP = ip;
						
						closestIntersectingSprite = *curOb;
						
						localPosOfClosestObstacle = obstaclePosition;
					}
				}
			}
		}
		
		++curOb;
	}
	
	// If we found an intersecting obstacle, create a steering force away from it
	if (closestIntersectingSprite)
	{
		ZShadeSandboxMath::XMMath3 steeringForce;
		
		// The closer the agent is to an object, the stronger the steering force will be
		float multiplier = 1.0f + (fDbBoxLength - localPosOfClosestObstacle.x) / fDbBoxLength;
		
		// Find the lateral force
		steeringForce.y = (closestIntersectingSprite->BoundingRadius() - localPosOfClosestObstacle.y) * multiplier;
		
		// Apply a braking force proportional to the obstacle's distance from the vehicle
		const float BrakingWeight = 0.2f;
		
		steeringForce.x = (closestIntersectingSprite->BoundingRadius() - localPosOfClosestObstacle.x) * BrakingWeight;
		steeringForce.z = 0;
		
		return steeringForce;
	}
	
	return ZShadeSandboxMath::XMMath3(0, 0, 0);
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::Interpose(AISprite* agentA, AISprite* agentB)
{
	// This will move the sprite in-between agentA and agentB
	
	ZShadeSandboxMath::XMMath3 sprPosition(mSprite->TopLeftPosition().x, mSprite->TopLeftPosition().y, mSprite->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 velocity(mSprite->Velocity().x, mSprite->Velocity().y, 0);
	ZShadeSandboxMath::XMMath3 agentAPosition(agentA->TopLeftPosition().x, agentA->TopLeftPosition().y, agentA->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 agentAVelocity(agentA->Velocity().x, agentA->Velocity().y, 0);
	ZShadeSandboxMath::XMMath3 agentBPosition(agentB->TopLeftPosition().x, agentB->TopLeftPosition().y, agentB->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 agentBVelocity(agentB->Velocity().x, agentB->Velocity().y, 0);
	
	// 1st we need to figure out where the two sprites are going to be at time T in the future.
	// This is approximated by determining the time taken to reach the midway point at the
	// current time at max speed.
	ZShadeSandboxMath::XMMath3 midPoint = (agentAPosition + agentBPosition) / 2;
	
	float timeToReachMidPointX = sprPosition.Distance(midPoint) / mSprite->MaxSpeed().x;
	float timeToReachMidPointY = sprPosition.Distance(midPoint) / mSprite->MaxSpeed().y;

	// Now we have T, assume that both sprites will continue on a straight line
	ZShadeSandboxMath::XMMath3 aPos = agentAPosition + agentAVelocity * timeToReachMidPointX;
	ZShadeSandboxMath::XMMath3 bPos = agentBPosition + agentBVelocity * timeToReachMidPointY;
	
	// Calculate the midpoint of these predicted positions
	midPoint = (agentAPosition + agentBPosition) / 2;
	
	// Allow the sprite to reach the target position as fast as possible
	return Arrive(midPoint, EDeceleration::Type::eFast);
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::Hide(AISprite* target, std::vector<AISprite*> &spriteObstacles)
{
	float distToClosest = FLT_MAX;
	
	ZShadeSandboxMath::XMMath3 BestHidingSpot;
	
	ZShadeSandboxMath::XMMath3 targetPosition(target->TopLeftPosition().x, target->TopLeftPosition().y, target->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 sprPosition(mSprite->TopLeftPosition().x, mSprite->TopLeftPosition().y, mSprite->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 velocity(mSprite->Velocity().x, mSprite->Velocity().y, 0);
	
	auto curOb = spriteObstacles.begin();
	
	while (curOb != spriteObstacles.end())
	{
		ZShadeSandboxMath::XMMath3 obstaclePosition((*curOb)->TopLeftPosition().x, (*curOb)->TopLeftPosition().y, (*curOb)->TopLeftPosition().z);
		
		// Calculate the position of the hiding spot for this obstacle
		ZShadeSandboxMath::XMMath3 hidingSpot = GetHidingPosition(obstaclePosition, (*curOb)->BoundingRadius(), targetPosition);
		
		// Work in distance-squared space to find the closest hiding spot to the sprite
		float dist = hidingSpot.DistanceSquared(sprPosition);
		
		if (dist < distToClosest)
		{
			distToClosest = dist;
			
			BestHidingSpot = hidingSpot;
		}
		
		++curOb;
	}
	
	// If no suitable obstacle's are found then just evade the target
	if (distToClosest == FLT_MAX)
	{
		return Evade(target);
	}
	
	// Get to the best hiding spot as quickly as possible
	return Arrive(BestHidingSpot, EDeceleration::Type::eFast);
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::FollowPath()
{
	if (m_waypointPath && !m_waypointPath->Empty())
	{
		ZShadeSandboxMath::XMMath3 sprPosition(mSprite->TopLeftPosition().x, mSprite->TopLeftPosition().y, mSprite->TopLeftPosition().z);
		
		ZShadeSandboxMath::XMMath3 currentWaypoint;
		
		if (m_waypointPath->CurrentWaypoint(currentWaypoint))
		{
			// Move to next waypoint target if close enough to current waypoint target
			if (currentWaypoint.DistanceSquared(sprPosition) < (fWaypointSeekDist * fWaypointSeekDist))
			{
				m_waypointPath->SetNextWaypoint();
			}
			
			if (!m_waypointPath->Finished())
			{
				return Seek(currentWaypoint);
			}
			else
			{
				return Arrive(currentWaypoint, EDeceleration::Type::eNormal);
			}
		}
	}

	return ZShadeSandboxMath::XMMath3(0, 0, 0);
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::OffsetPursuit(AISprite* leader, ZShadeSandboxMath::XMMath3 offset)
{
	ZShadeSandboxMath::XMMath3 sprPosition(mSprite->TopLeftPosition().x, mSprite->TopLeftPosition().y, mSprite->TopLeftPosition().z);
	ZShadeSandboxMath::XMMath3 leaderVelocity(leader->Velocity().x, leader->Velocity().y, 0);
	
	ZShadeSandboxMath::XMMath3 toOffset = offset - sprPosition;
	
	// The look-ahead time is proportional to the distance between the leader and the pursuer;
	// and is inversely proportional to the sum of both sprites' velocities
	float lookAheadTimeX = toOffset.Length() / (mSprite->MaxSpeed().x + leader->MaxSpeed().x);
	float lookAheadTimeY = toOffset.Length() / (mSprite->MaxSpeed().y + leader->MaxSpeed().y);

	// Now arrive at the predicted future position of the offset
	return Arrive(offset + leaderVelocity * ZShadeSandboxMath::XMMath3(lookAheadTimeX, lookAheadTimeY, 0), EDeceleration::Type::eFast);
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::Separation(std::vector<AISprite*>& neighbours)
{
	ZShadeSandboxMath::XMMath3 sprPosition(mSprite->TopLeftPosition().x, mSprite->TopLeftPosition().y, mSprite->TopLeftPosition().z);
	
	ZShadeSandboxMath::XMMath3 steeringForce;
	
	for (int a = 0; a < neighbours.size(); ++a)
	{
		// Make sure this sprite is not included in the calculations and that the sprite being examined is nearby
		if ((neighbours[a] != mSprite) && (neighbours[a]->IsTagged()))
		{
			ZShadeSandboxMath::XMMath3 neighbourPosition(
				neighbours[a]->TopLeftPosition().x, neighbours[a]->TopLeftPosition().y, neighbours[a]->TopLeftPosition().z
			);
			
			ZShadeSandboxMath::XMMath3 toAgent = sprPosition - neighbourPosition;
			
			float length = toAgent.Length();
			
			toAgent.Normalize();
			
			// Scale the force inversely proportional to the sprite's distance from its neighbour
			steeringForce += toAgent / length;
		}
	}
	
	return steeringForce;
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::Alignment(std::vector<AISprite*>& neighbours)
{
	ZShadeSandboxMath::XMMath3 sprHeading(mSprite->Heading().x, mSprite->Heading().y, 0);
	
	ZShadeSandboxMath::XMMath3 averageHeading;
	
	int neighbourCount = 0;
	
	// Iterate through all tagged sprites and sum their heading vectors
	for (int a = 0; a < neighbours.size(); ++a)
	{
		// Make sure this sprite is not included in the calculations and that the sprite being examined is nearby
		if ((neighbours[a] != mSprite) && (neighbours[a]->IsTagged()))
		{
			ZShadeSandboxMath::XMMath3 neighbourHeading(
				neighbours[a]->Heading().x, neighbours[a]->Heading().y, 0
			);
			
			averageHeading += neighbourHeading;
			
			++neighbourCount;
		}
	}
	
	// If the neighbourhood contained one or more sprites, average their heading vectors
	if (neighbourCount > 0)
	{
		averageHeading /= (float)neighbourCount;
		
		averageHeading -= sprHeading;
	}
	
	return averageHeading;
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::Cohesion(std::vector<AISprite*>& neighbours)
{
	ZShadeSandboxMath::XMMath3 centerOfMass, steeringForce;
	
	int neighbourCount = 0;
	
	// Iterate through all tagged sprites and sum their position vectors
	for (int a = 0; a < neighbours.size(); ++a)
	{
		// Make sure this sprite is not included in the calculations and that the sprite being examined is nearby
		if ((neighbours[a] != mSprite) && (neighbours[a]->IsTagged()))
		{
			ZShadeSandboxMath::XMMath3 neighbourPosition(
				neighbours[a]->Heading().x, neighbours[a]->Heading().y, 0
			);
			
			centerOfMass += neighbourPosition;
			
			++neighbourCount;
		}
	}
	
	if (neighbourCount > 0)
	{
		// The center of mass is the average of the sum of positions
		centerOfMass /= (float)neighbourCount;
		
		steeringForce = Seek(centerOfMass);
	}
	
	return steeringForce;
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::CalculateForce()
{
	std::vector<AISprite*> sprites;
	
	TopdownMap* topdownMap = Environment2DMapHelper::ActiveTopdownMap();
	if (topdownMap != 0)
	{
		// Get the list of sprites in the map
		sprites = topdownMap->Sprites();
	}
	
	PlatformerMap* platformerMap = Environment2DMapHelper::ActivePlatformerMap();
	if (platformerMap != 0)
	{
		// Get the list of sprites in the map
		sprites = platformerMap->Sprites();
	}
	
	
	ZShadeSandboxMath::XMMath3 steeringForce, force;
	
	// If the AI does nothing then the steering force will be negligible and it will not move
	steeringForce = ZShadeSandboxMath::XMMath3(0, 0, 0);
	
	// Prioritized Dithering
	const float fSpriteAvoidance	= 0.9f;
	const float fFlockSeparation	= 0.8f;
	const float fFlockAlignment 	= 0.5f;
	const float fFlockCohesion 		= 0.5f;
	const float fWander 			= 0.8f;
	const float fFollowPath			= 0.8f;
	const float fPursueTarget		= 0.6f;
	const float fEvadeTarget		= 0.6f;
	const float fInterpose			= 0.5f;
	const float fHide				= 0.7f;
	const float fOffsetPursuit		= 0.5f;
	const float fSeek				= 0.4f;
	const float fFlee				= 0.4f;
	const float fArrive				= 0.4f;
	
	ZShadeSandboxMath::ZMath::RandomSeed();
	
	if (bAvoidSprites && ZShadeSandboxMath::ZMath::RandF(0, 1) > fSpriteAvoidance)
	{
		if (sprites.size() == 0) return steeringForce;
		
		force = AvoidSprite(sprites) * fWeightSpriteAvoidance / fSpriteAvoidance;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
		//if (steeringForce.x != 0 && steeringForce.y != 0)
		//{
			// steeringForce.Truncate(mSprite->MaxForce());
		//	return steeringForce;
		//}
	}
	
	if (bFlockSeparation && ZShadeSandboxMath::ZMath::RandF(0, 1) > fFlockSeparation)
	{
		if (sprites.size() == 0) return steeringForce;
		
		force = Separation(sprites) * fWeightSeparation / fFlockSeparation;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	
	if (bFlockAlignment && ZShadeSandboxMath::ZMath::RandF(0, 1) > fFlockAlignment)
	{
		if (sprites.size() == 0) return steeringForce;
		
		force = Alignment(sprites) * fWeightAlignment / fFlockAlignment;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	
	if (bFlockCohesion && ZShadeSandboxMath::ZMath::RandF(0, 1) > fFlockCohesion)
	{
		if (sprites.size() == 0) return steeringForce;
		
		force = Cohesion(sprites) * fWeightCohesion / fFlockCohesion;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	
	float rval = ZShadeSandboxMath::ZMath::RandF(0, 1);
	if (bWander)// && rval > fWander)
	{
		steeringForce = Wander() * fWeightWander / fWander;
		
		//if (!AccumulateForce(steeringForce, force))
		//{
		//	return steeringForce;
		//}
	}
	
	if (bFollowPath && ZShadeSandboxMath::ZMath::RandF(0, 1) > fFollowPath)
	{
		force = FollowPath() * fWeightFollowPath / fFollowPath;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	
	if (bPursueTarget && ZShadeSandboxMath::ZMath::RandF(0, 1) > fPursueTarget)
	{
		if (mEvader == 0) return steeringForce;
		
		force = Pursuit(mEvader) * fWeightPursueTarget / fPursueTarget;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	
	if (bEvadeTarget && ZShadeSandboxMath::ZMath::RandF(0, 1) > fEvadeTarget)
	{
		if (mPursuer == 0) return steeringForce;
		
		force = Evade(mPursuer) * fWeightEvadeTarget / fEvadeTarget;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	
	if (bInterpose && ZShadeSandboxMath::ZMath::RandF(0, 1) > fInterpose)
	{
		if ((mSpriteA == 0) && (mSpriteB == 0)) return steeringForce;
		
		force = Interpose(mSpriteA, mSpriteB) * fWeightInterpose / fInterpose;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	
	if (bHide && ZShadeSandboxMath::ZMath::RandF(0, 1) > fHide)
	{
		if (mTarget == 0) return steeringForce;
		if (sprites.size() == 0) return steeringForce;

		force = Hide(mTarget, sprites) * fWeightHide / fHide;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	
	if (bOffsetPursuit && ZShadeSandboxMath::ZMath::RandF(0, 1) > fOffsetPursuit)
	{
		if (mLeader == 0) return steeringForce;
		
		force = OffsetPursuit(mLeader, mLeaderOffset) * fWeightOffsetPursuit / fOffsetPursuit;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	
	if (bSeek && ZShadeSandboxMath::ZMath::RandF(0, 1) > fSeek)
	{
		force = Seek(mTargetPoint) * fWeightSeek / fSeek;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	
	if (bFlee && ZShadeSandboxMath::ZMath::RandF(0, 1) > fFlee)
	{
		force = Flee(mTargetPoint) * fWeightFlee / fFlee;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	
	if (bArrive && ZShadeSandboxMath::ZMath::RandF(0, 1) > fArrive)
	{
		force = Arrive(mTargetPoint, mTargetDecelerationType) * fWeightArrive / fArrive;
		
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	
	return steeringForce;
}
//==================================================================================================================================
bool AISteeringForce::AccumulateForce(ZShadeSandboxMath::XMMath3 &runningTotal, ZShadeSandboxMath::XMMath3 forceToAdd)
{
	// Calculate how much steering force the sprite has used so far
	float magnitudeSoFar = runningTotal.Length();
	
	// Calculate how much steering force remains to be used by this sprite
	float magnitudeRemaining = mSprite->MaxForce() - magnitudeSoFar;
	
	if (magnitudeRemaining <= 0.0f)
	{
		return false;
	}
	
	float magnitudeToAdd = forceToAdd.Length();
	
	// If the magnitude of the sum of forceToAdd and the running total does not exceed the maximum force available
	// to this sprite, just add together. Otherwise add as much of the forceToAdd vector as possible without going
	// over the max.
	if (magnitudeToAdd < magnitudeRemaining)
	{
		runningTotal += forceToAdd;
	}
	else
	{
		forceToAdd.Normalize();
		
		runningTotal += forceToAdd * magnitudeRemaining;
	}
	
	return true;
}
//==================================================================================================================================
template <class T, class conT>
void AISteeringForce::TagNeighbours(T* entity, conT& ContainerOfEntities, float radius)
{
	ZShadeSandboxMath::XMMath3 entityPosition(entity->TopLeftPosition().x, entity->TopLeftPosition().y, entity->TopLeftPosition().z);
	
	// Iterate through all entities checking for range
	for (typename conT::iterator curEntity = ContainerOfEntities.begin();
		 curEntity != ContainerOfEntities.end();
		 ++curEntity)
	{
		// Clear any current tags
		(*curEntity)->UnTag();
		
		ZShadeSandboxMath::XMMath3 curEntityPosition((*curEntity)->TopLeftPosition().x, (*curEntity)->TopLeftPosition().y, (*curEntity)->TopLeftPosition().z);
		
		ZShadeSandboxMath::XMMath3 to = curEntityPosition - entityPosition;
		
		// The bounding radius of the other is taken into account by adding it to the range
		float range = radius + (*curEntity)->BoundingRadius();
		
		// If the entity is within range then tag it
		if (((*curEntity) != entity) && (to.LengthSquared() < range * range))
		{
			(*curEntity)->Tag();
		}
	}
}
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 AISteeringForce::GetHidingPosition(ZShadeSandboxMath::XMMath3 obstaclePos, float obstacleRadius, ZShadeSandboxMath::XMMath3 targetPoint)
{
	// Calculate how far away the sprite is to be from the chosen obstacle's bounding radius
	const float DistanceFromBoundary = 30.0f;
	
	float distAway = obstacleRadius + DistanceFromBoundary;
	
	// Calculate the heading toward the object from the target
	ZShadeSandboxMath::XMMath3 toObstacle = obstaclePos - targetPoint;
	toObstacle.Normalize();
	
	// Scale it to size and add to the obstacle's position to get the hiding spot
	return (toObstacle * distAway) + obstaclePos;
}
//==================================================================================================================================