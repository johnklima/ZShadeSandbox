//==================================================================================================================================
// AISteeringForce.h
//
// This will go inside the AISprite class
//==================================================================================================================================
// History
//
// -Created on 8/3/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __AISTEERINGFORCE_H
#define __AISTEERINGFORCE_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "ZMath.h"
#include "WaypointSystem.h"
class AISprite;

namespace EDeceleration
{
	enum Type
	{
		eSlow = 3,
		eNormal = 2,
		eFast = 1
	};
}

//==================================================================================================================================
//==================================================================================================================================
class AISteeringForce
{
public:
	
	AISteeringForce(AISprite* sprite);
	ZShadeSandboxMath::XMMath3 CalculateForce();
	
	/*
		Can save time by only calling this to a group behaviour only
		
		if (bFlockSeparation || bFlockAlignment || bFlockCohesion)
			TagNeighbours(mSprite, WorldSpritesContainer, ViewDistance);
	*/
	template <class T, class conT>
	static void TagNeighbours(T* entity, conT& ContainerOfEntities, float radius);
	
public:
	
	float& WanderingRadius() { return fWanderingRadius; }
	float WanderingRadius() const { return fWanderingRadius; }
	float& WanderingDistance() { return fWanderingDistance; }
	float WanderingDistance() const { return fWanderingDistance; }
	float& WanderingJitter() { return fWanderingJitter; }
	float WanderingJitter() const { return fWanderingJitter; }
	float& MinDetectionBoxLength() { return fMinDetectionBoxLength; }
	float MinDetectionBoxLength() const { return fMinDetectionBoxLength; }
	float& WaypointSeekDist() { return fWaypointSeekDist; }
	float WaypointSeekDist() const { return fWaypointSeekDist; }
	float& MaxFleeDistance() { return fMaxFleeDistance; }
	float MaxFleeDistance() const { return fMaxFleeDistance; }
	float& WeightSeek() { return fWeightSeek; }
	float WeightSeek() const { return fWeightSeek; }
	float& WeightFlee() { return fWeightFlee; }
	float WeightFlee() const { return fWeightFlee; }
	float& WeightArrive() { return fWeightArrive; }
	float WeightArrive() const { return fWeightArrive; }
	float& WeightObstacleAvoidance() { return fWeightObstacleAvoidance; }
	float WeightObstacleAvoidance() const { return fWeightObstacleAvoidance; }
	float& WeightSeparation() { return fWeightSeparation; }
	float WeightSeparation() const { return fWeightSeparation; }
	float& WeightAlignment() { return fWeightAlignment; }
	float WeightAlignment() const { return fWeightAlignment; }
	float& WeightCohesion() { return fWeightCohesion; }
	float WeightCohesion() const { return fWeightCohesion; }
	float& WeightWander() { return fWeightWander; }
	float WeightWander() const { return fWeightWander; }
	float& WeightFollowPath() { return fWeightFollowPath; }
	float WeightFollowPath() const { return fWeightFollowPath; }
	float& WeightPursueTarget() { return fWeightPursueTarget; }
	float WeightPursueTarget() const { return fWeightPursueTarget; }
	float& WeightEvadeTarget() { return fWeightEvadeTarget; }
	float WeightEvadeTarget() const { return fWeightEvadeTarget; }
	float& WeightInterpose() { return fWeightInterpose; }
	float WeightInterpose() const { return fWeightInterpose; }
	float& WeightHide() { return fWeightHide; }
	float WeightHide() const { return fWeightHide; }
	float& WeightOffsetPursuit() { return fWeightOffsetPursuit; }
	float WeightOffsetPursuit() const { return fWeightOffsetPursuit; }
	bool& CanSeek() { return bSeek; }
	bool CanSeek() const { return bSeek; }
	bool& CanFlee() { return bFlee; }
	bool CanFlee() const { return bFlee; }
	bool& CanArrive() { return bArrive; }
	bool CanArrive() const { return bArrive; }
	bool& CanAvoidSprites() { return bAvoidSprites; }
	bool CanAvoidSprites() const { return bAvoidSprites; }
	bool& CanFlockSeparation() { return bFlockSeparation; }
	bool CanFlockSeparation() const { return bFlockSeparation; }
	bool& CanFlockAlignment() { return bFlockAlignment; }
	bool CanFlockAlignment() const { return bFlockAlignment; }
	bool& CanFlockCohesion() { return bFlockCohesion; }
	bool CanFlockCohesion() const { return bFlockCohesion; }
	bool& CanWander() { return bWander; }
	bool CanWander() const { return bWander; }
	bool& CanFollowPath() { return bFollowPath; }
	bool CanFollowPath() const { return bFollowPath; }
	bool& CanPursueTarget() { return bPursueTarget; }
	bool CanPursueTarget() const { return bPursueTarget; }
	bool& CanEvadeTarget() { return bEvadeTarget; }
	bool CanEvadeTarget() const { return bEvadeTarget; }
	bool& CanInterpose() { return bInterpose; }
	bool CanInterpose() const { return bInterpose; }
	bool& CanHide() { return bHide; }
	bool CanHide() const { return bHide; }
	bool& CanOffsetPursuit() { return bOffsetPursuit; }
	bool CanOffsetPursuit() const { return bOffsetPursuit; }
	ZShadeSandboxAI::WaypointSystem*& WaypointSystem() { return m_waypointPath; }
	ZShadeSandboxAI::WaypointSystem* WaypointSystem() const { return m_waypointPath; }
	AISprite*& Evader() { return mEvader; }
	AISprite* Evader() const { return mEvader; }
	AISprite*& Pursuer() { return mPursuer; }
	AISprite* Pursuer() const { return mPursuer; }
	AISprite*& Target() { return mTarget; }
	AISprite* Target() const { return mTarget; }
	AISprite*& SpriteA() { return mSpriteA; }
	AISprite* SpriteA() const { return mSpriteA; }
	AISprite*& SpriteB() { return mSpriteB; }
	AISprite* SpriteB() const { return mSpriteB; }
	AISprite*& Leader() { return mLeader; }
	AISprite* Leader() const { return mLeader; }
	ZShadeSandboxMath::XMMath3& LeaderOffset() { return mLeaderOffset; }
	ZShadeSandboxMath::XMMath3 LeaderOffset() const { return mLeaderOffset; }
	ZShadeSandboxMath::XMMath3& TargetPoint() { return mTargetPoint; }
	ZShadeSandboxMath::XMMath3 TargetPoint() const { return mTargetPoint; }
	EDeceleration::Type& TargetDecelerationType() { return mTargetDecelerationType; }
	EDeceleration::Type TargetDecelerationType() const { return mTargetDecelerationType; }
	
private:
	
	ZShadeSandboxMath::XMMath3 Seek(ZShadeSandboxMath::XMMath3 targetPoint);
	ZShadeSandboxMath::XMMath3 Flee(ZShadeSandboxMath::XMMath3 targetPoint);
	ZShadeSandboxMath::XMMath3 Arrive(ZShadeSandboxMath::XMMath3 targetPoint, EDeceleration::Type deceleration);
	ZShadeSandboxMath::XMMath3 Pursuit(AISprite* evader);
	ZShadeSandboxMath::XMMath3 Evade(AISprite* pursuer);
	ZShadeSandboxMath::XMMath3 Wander();
	ZShadeSandboxMath::XMMath3 AvoidSprite(std::vector<AISprite*> &spriteObstacles);
	ZShadeSandboxMath::XMMath3 Interpose(AISprite* agentA, AISprite* agentB);
	ZShadeSandboxMath::XMMath3 Hide(AISprite* target, std::vector<AISprite*> &spriteObstacles);
	ZShadeSandboxMath::XMMath3 FollowPath();
	ZShadeSandboxMath::XMMath3 OffsetPursuit(AISprite* leader, ZShadeSandboxMath::XMMath3 offset);
	ZShadeSandboxMath::XMMath3 Separation(std::vector<AISprite*>& neighbours);
	ZShadeSandboxMath::XMMath3 Alignment(std::vector<AISprite*>& neighbours);
	ZShadeSandboxMath::XMMath3 Cohesion(std::vector<AISprite*>& neighbours);
	bool AccumulateForce(ZShadeSandboxMath::XMMath3 &runningTotal, ZShadeSandboxMath::XMMath3 forceToAdd);
	ZShadeSandboxMath::XMMath3 GetHidingPosition(ZShadeSandboxMath::XMMath3 obstaclePos, float obstacleRadius, ZShadeSandboxMath::XMMath3 targetPoint);
	
private:
	
	// The sprite that this belongs to
	AISprite* mSprite;
	
	float fMaxFleeDistance;

	float fWanderingRadius;
	float fWanderingDistance;
	
	// Maximum amount of random displacement
	float fWanderingJitter;
	
	// A point constrained to the parameter of a circle of radius fWanderingRadius in the center of the sprite.
	// The initial position of m_wanderTarget needs to be set in the constructor
	ZShadeSandboxMath::XMMath3 m_wanderTarget;
	
	float fDbBoxLength;
	float fMinDetectionBoxLength;
	
	float fWaypointSeekDist;
	
	ZShadeSandboxAI::WaypointSystem* m_waypointPath;
	
	// The sprite that this sprite is pursuing
	AISprite* mEvader;
	
	// The sprite that this sprite is evading
	AISprite* mPursuer;
	
	// The sprite that this sprite is hiding from
	AISprite* mTarget;
	
	// The two sprites that this sprite will interpose
	AISprite* mSpriteA;
	AISprite* mSpriteB;
	
	// The sprite that this sprite is following
	AISprite* mLeader;
	ZShadeSandboxMath::XMMath3 mLeaderOffset;
	
	ZShadeSandboxMath::XMMath3 mTargetPoint;
	EDeceleration::Type mTargetDecelerationType;
	
	float fWeightSeek;
	float fWeightFlee;
	float fWeightArrive;
	float fWeightObstacleAvoidance;
	float fWeightSeparation;
	float fWeightAlignment;
	float fWeightCohesion;
	float fWeightWander;
	float fWeightFollowPath;
	float fWeightPursueTarget;
	float fWeightEvadeTarget;
	float fWeightInterpose;
	float fWeightHide;
	float fWeightOffsetPursuit;
	
	bool bSeek;
	bool bFlee;
	bool bArrive;
	bool bAvoidSprites;
	bool bFlockSeparation;
	bool bFlockAlignment;
	bool bFlockCohesion;
	bool bWander;
	bool bFollowPath;
	bool bPursueTarget;
	bool bEvadeTarget;
	bool bInterpose;
	bool bHide;
	bool bOffsetPursuit;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__AISTEERINGFORCE_H