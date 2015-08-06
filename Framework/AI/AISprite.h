//==================================================================================================================================
// AISprite.h
//
//==================================================================================================================================
// History
//
// -Created in 2013 by Dustin Watson
// -Updated on 8/4/2015 by Dustin Watson (Added Steering behaviour)
//==================================================================================================================================
#ifndef __AISPRITE_H
#define __AISPRITE_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include <string>
#include <vector>
using namespace std;
#include "BehaviorTree.h"
#include "PrioritySelector.h"
#include "Sequence.h"
#include "LuaTask.h"
#include "TaskNode.h"
#include "LuaDelegate.h"
#include "Sprite.h"
#include "AISteeringForce.h"
using namespace LuaBehaviorTree;
class Script;

//==================================================================================================================================
//==================================================================================================================================
struct SEQUENCE_INFO
{
	string name;
	string condition_func_name;
	string action_func_name;
};
//==================================================================================================================================
struct BEHAVIOR_INFO
{
	string name;
	vector<SEQUENCE_INFO*> sequence_info;
};
//==================================================================================================================================
//==================================================================================================================================
class AISprite : public Sprite
{
public:
	
	AISprite(D3D* d3d);
	AISprite(const AISprite& o);
	~AISprite();

	void SetRunBehavior(bool set);
	void SetRunBehaviorDefault(bool set);
	void SetBehaviorName(string name);

	void updateBehavior();

	void AddDefaultBehavior(string p_default_behavior_name);
	void AddBehaviorSequence(string p_behavior_name, string p_sequence_name, string p_action_func_name, string p_condition_func_name);
	void RegisterBehavior(string p_behavior_name);

	void CreateTree();
	
	void Main();
	void Think();
	void MoveScript();
	void Talk();
	void Die();
	void Hit();
	void Touch();
	void Attack();
	void Target();
	
	// The physics world will calculate this force to apply to the sprite
	void CalculateSteeringForce();
	
public:
	
	void SetTargetSprite(AISprite* ts) { m_TargetSprite = ts; }
	AISprite* GetTargetSprite() { return m_TargetSprite; }
	
	void SetWanderingRadius(float value);
	void SetWanderingDistance(float value);
	void SetWanderingJitter(float value);
	void SetMinDetectionBoxLength(float value);
	void SetWaypointSeekDist(float value);
	void SetMaxFleeDistance(float value);
	void SetWeightSeek(float value);
	void SetWeightFlee(float value);
	void SetWeightArrive(float value);
	void SetWeightObstacleAvoidance(float value);
	void SetWeightSeparation(float value);
	void SetWeightAlignment(float value);
	void SetWeightCohesion(float value);
	void SetWeightWander(float value);
	void SetWeightFollowPath(float value);
	void SetWeightPursueTarget(float value);
	void SetWeightEvadeTarget(float value);
	void SetWeightInterpose(float value);
	void SetWeightHide(float value);
	void SetWeightOffsetPursuit(float value);
	void SetCanSeek(bool value);
	void SetCanFlee(bool value);
	void SetCanArrive(bool value);
	void SetCanAvoidSprites(bool value);
	void SetCanFlockSeparation(bool value);
	void SetCanFlockAlignment(bool value);
	void SetCanFlockCohesion(bool value);
	void SetCanWander(bool value);
	void SetCanFollowPath(bool value);
	void SetCanPursueTarget(bool value);
	void SetCanEvadeTarget(bool value);
	void SetCanInterpose(bool value);
	void SetCanHide(bool value);
	void SetCanOffsetPursuit(bool value);
	void SetWaypointSystem(ZShadeSandboxAI::WaypointSystem* waypoints);
	void SetEvader(AISprite* sprite);
	void SetPursuer(AISprite* sprite);
	void SetTarget(AISprite* sprite);
	void SetSpriteA(AISprite* sprite);
	void SetSpriteB(AISprite* sprite);
	void SetLeader(AISprite* sprite);
	void SetLeaderOffset(ZShadeSandboxMath::XMMath3 point);
	void SetTargetPoint(ZShadeSandboxMath::XMMath3 point);
	void SetTargetDecelerationType(EDeceleration::Type type);
	
	
	float WanderingRadius() const { return mSteeringForce->WanderingRadius(); }
	float WanderingDistance() const { return mSteeringForce->WanderingDistance(); }
	float WanderingJitter() const { return mSteeringForce->WanderingJitter(); }
	float MinDetectionBoxLength() const { return mSteeringForce->MinDetectionBoxLength(); }
	float WaypointSeekDist() const { return mSteeringForce->WaypointSeekDist(); }
	float MaxFleeDistance() const { return mSteeringForce->MaxFleeDistance(); }
	float WeightSeek() const { return mSteeringForce->WeightSeek(); }
	float WeightFlee() const { return mSteeringForce->WeightFlee(); }
	float WeightArrive() const { return mSteeringForce->WeightArrive(); }
	float WeightObstacleAvoidance() const { return mSteeringForce->WeightObstacleAvoidance(); }
	float WeightSeparation() const { return mSteeringForce->WeightSeparation(); }
	float WeightAlignment() const { return mSteeringForce->WeightAlignment(); }
	float WeightCohesion() const { return mSteeringForce->WeightCohesion(); }
	float WeightWander() const { return mSteeringForce->WeightWander(); }
	float WeightFollowPath() const { return mSteeringForce->WeightFollowPath(); }
	float WeightPursueTarget() const { return mSteeringForce->WeightPursueTarget(); }
	float WeightEvadeTarget() const { return mSteeringForce->WeightEvadeTarget(); }
	float WeightInterpose() const { return mSteeringForce->WeightInterpose(); }
	float WeightHide() const { return mSteeringForce->WeightHide(); }
	float WeightOffsetPursuit() const { return mSteeringForce->WeightOffsetPursuit(); }
	bool CanSeek() const { return mSteeringForce->CanSeek(); }
	bool CanFlee() const { return mSteeringForce->CanFlee(); }
	bool CanArrive() const { return mSteeringForce->CanArrive(); }
	bool CanAvoidSprites() const { return mSteeringForce->CanAvoidSprites(); }
	bool CanFlockSeparation() const { return mSteeringForce->CanFlockSeparation(); }
	bool CanFlockAlignment() const { return mSteeringForce->CanFlockAlignment(); }
	bool CanFlockCohesion() const { return mSteeringForce->CanFlockCohesion(); }
	bool CanWander() const { return mSteeringForce->CanWander(); }
	bool CanFollowPath() const { return mSteeringForce->CanFollowPath(); }
	bool CanPursueTarget() const { return mSteeringForce->CanPursueTarget(); }
	bool CanEvadeTarget() const { return mSteeringForce->CanEvadeTarget(); }
	bool CanInterpose() const { return mSteeringForce->CanInterpose(); }
	bool CanHide() const { return mSteeringForce->CanHide(); }
	bool CanOffsetPursuit() const { return mSteeringForce->CanOffsetPursuit(); }
	ZShadeSandboxAI::WaypointSystem* WaypointSystem() const { return mSteeringForce->WaypointSystem(); }
	AISprite* Evader() const { return mSteeringForce->Evader(); }
	AISprite* Pursuer() const { return mSteeringForce->Pursuer(); }
	AISprite* Target() const { return mSteeringForce->Target(); }
	AISprite* SpriteA() const { return mSteeringForce->SpriteA(); }
	AISprite* SpriteB() const { return mSteeringForce->SpriteB(); }
	AISprite* Leader() const { return mSteeringForce->Leader(); }
	ZShadeSandboxMath::XMMath3 LeaderOffset() const { return mSteeringForce->LeaderOffset(); }
	ZShadeSandboxMath::XMMath3 TargetPoint() const { return mSteeringForce->TargetPoint(); }
	EDeceleration::Type TargetDecelerationType() const { return mSteeringForce->TargetDecelerationType(); }
	
	void CreateWaypointSystem();
	int WaypointSystemSize();
	bool WaypointSystemEmpty();
	void ClearWaypoints();
	void RemoveWaypoint(ZShadeSandboxMath::XMMath3 waypoint);
	void AddWaypoint(ZShadeSandboxMath::XMMath3 waypoint);
	bool CurrentWaypoint(ZShadeSandboxMath::XMMath3& waypoint);
	void SetNextWaypoint();
	bool WaypointSystemFinished();
	void SetWaypointSystemCanLoop(bool value);
	bool WaypointSystemCanLoop() const;
	int CurrentWaypointIndex() const;
	
	// Need some perception
	// Make the sprite run a sweep for any other sprite that is nearby
	// Make the sprite always target the nearest sprite
	
	// If a sprite is in range of my bounding radius then I will pursue it
	// If I am pursued then then I need to evade
	void TargetClosestSprite();
	
	
private:
	
	// Steering force functions that can be used in lua scripting to make the sprite do something
	// Basically the sprite can do something it just needs to know how, for example
	//   If this sprite should pursue another sprite it should locate a sprite within the extent of the search radius
	//   and pursue the nearest sprite
	
	AISteeringForce* mSteeringForce;
	string m_default_behavior;
	vector<BEHAVIOR_INFO*> m_behaviors;
	bool m_run_behavior;
	bool m_run_behavior_default;
	string m_behavior_name;//Current behaviour to update
	BehaviorTree* tree;
	AISprite* m_TargetSprite;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__AISPRITE_H