//==================================================================================================================================
// AIEntity3D.h
//
//==================================================================================================================================
// History
//
// -Created in 2013 by Dustin Watson
// -Updated on 8/4/2015 by Dustin Watson (Added Steering behaviour)
//==================================================================================================================================
#ifndef __AIENTITY3D_H
#define __AIENTITY3D_H
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
#include "Entity3D.h"
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
class AIEntity3D : public Entity3D
{
public:
	
	AIEntity3D(D3D* d3d);
	~AIEntity3D();
	
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
	
	
	
private:
	
	// Steering force functions that can be used in lua scripting to make the sprite do something
	// Basically the sprite can do something it just needs to know how, for example
	//   If this sprite should pursue another sprite it should locate a sprite within the extent of the search radius
	//   and pursue the nearest sprite
	
	//AISteeringForce* mSteeringForce;
	string m_default_behavior;
	vector<BEHAVIOR_INFO*> m_behaviors;
	bool m_run_behavior;
	bool m_run_behavior_default;
	string m_behavior_name;//Current behavior to update
	BehaviorTree* tree;
	AIEntity3D* m_TargetEntity;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__AIENTITY3D_H