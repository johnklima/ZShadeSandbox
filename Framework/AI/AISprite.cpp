#include "AISprite.h"
#include "Scripting.h"
//==================================================================================================================================
//==================================================================================================================================
AISprite::AISprite(D3D* d3d)
:	Sprite(d3d)
,	m_run_behavior(false)
,	m_run_behavior_default(false)
,	m_default_behavior("")
,	m_behavior_name("")
,	tree(0)
,	m_TargetSprite(0)
{
	mSteeringForce = new AISteeringForce(this);
}
//==================================================================================================================================
AISprite::AISprite(const AISprite& o) : Sprite(o)
{
}
//==================================================================================================================================
AISprite::~AISprite()
{
}
//==================================================================================================================================
void AISprite::SetRunBehavior(bool set)
{
	m_run_behavior = set;
}
//==================================================================================================================================
void AISprite::SetRunBehaviorDefault(bool set)
{
	m_run_behavior_default = set;
}
//==================================================================================================================================
void AISprite::SetBehaviorName(string name)
{
	m_behavior_name = name;
}
//==================================================================================================================================
void AISprite::CalculateSteeringForce()
{
	// Only AI will need to have this since a human can do all of these things.
	if (!m_player)
	{
		ZShadeSandboxMath::XMMath3 force = mSteeringForce->CalculateForce();
		
		// Update the velocity of the sprite with the new force
		UpdateVelocity(force.x, force.y);
	}
}
//==================================================================================================================================
void AISprite::SetWanderingRadius(float value)
{
	mSteeringForce->WanderingRadius() = value;
}
//==================================================================================================================================
void AISprite::SetWanderingDistance(float value)
{
	mSteeringForce->WanderingDistance() = value;
}
//==================================================================================================================================
void AISprite::SetWanderingJitter(float value)
{
	mSteeringForce->WanderingJitter() = value;
}
//==================================================================================================================================
void AISprite::SetMinDetectionBoxLength(float value)
{
	mSteeringForce->MinDetectionBoxLength() = value;
}
//==================================================================================================================================
void AISprite::SetWaypointSeekDist(float value)
{
	mSteeringForce->WaypointSeekDist() = value;
}
//==================================================================================================================================
void AISprite::SetMaxFleeDistance(float value)
{
	mSteeringForce->MaxFleeDistance() = value;
}
//==================================================================================================================================
void AISprite::SetWeightSeek(float value)
{
	mSteeringForce->WeightSeek() = value;
}
//==================================================================================================================================
void AISprite::SetWeightFlee(float value)
{
	mSteeringForce->WeightFlee() = value;
}
//==================================================================================================================================
void AISprite::SetWeightArrive(float value)
{
	mSteeringForce->WeightArrive() = value;
}
//==================================================================================================================================
void AISprite::SetWeightObstacleAvoidance(float value)
{
	mSteeringForce->WeightObstacleAvoidance() = value;
}
//==================================================================================================================================
void AISprite::SetWeightSeparation(float value)
{
	mSteeringForce->WeightSeparation() = value;
}
//==================================================================================================================================
void AISprite::SetWeightAlignment(float value)
{
	mSteeringForce->WeightAlignment() = value;
}
//==================================================================================================================================
void AISprite::SetWeightCohesion(float value)
{
	mSteeringForce->WeightCohesion() = value;
}
//==================================================================================================================================
void AISprite::SetWeightWander(float value)
{
	mSteeringForce->WeightWander() = value;
}
//==================================================================================================================================
void AISprite::SetWeightFollowPath(float value)
{
	mSteeringForce->WeightFollowPath() = value;
}
//==================================================================================================================================
void AISprite::SetWeightPursueTarget(float value)
{
	mSteeringForce->WeightPursueTarget() = value;
}
//==================================================================================================================================
void AISprite::SetWeightEvadeTarget(float value)
{
	mSteeringForce->WeightEvadeTarget() = value;
}
//==================================================================================================================================
void AISprite::SetWeightInterpose(float value)
{
	mSteeringForce->WeightInterpose() = value;
}
//==================================================================================================================================
void AISprite::SetWeightHide(float value)
{
	mSteeringForce->WeightHide() = value;
}
//==================================================================================================================================
void AISprite::SetWeightOffsetPursuit(float value)
{
	mSteeringForce->WeightOffsetPursuit() = value;
}
//==================================================================================================================================
void AISprite::SetCanSeek(bool value)
{
	mSteeringForce->CanSeek() = value;
}
//==================================================================================================================================
void AISprite::SetCanFlee(bool value)
{
	mSteeringForce->CanFlee() = value;
}
//==================================================================================================================================
void AISprite::SetCanArrive(bool value)
{
	mSteeringForce->CanArrive() = value;
}
//==================================================================================================================================
void AISprite::SetCanAvoidSprites(bool value)
{
	mSteeringForce->CanAvoidSprites() = value;
}
//==================================================================================================================================
void AISprite::SetCanFlockSeparation(bool value)
{
	mSteeringForce->CanFlockSeparation() = value;
}
//==================================================================================================================================
void AISprite::SetCanFlockAlignment(bool value)
{
	mSteeringForce->CanFlockAlignment() = value;
}
//==================================================================================================================================
void AISprite::SetCanFlockCohesion(bool value)
{
	mSteeringForce->CanFlockCohesion() = value;
}
//==================================================================================================================================
void AISprite::SetCanWander(bool value)
{
	mSteeringForce->CanWander() = value;
}
//==================================================================================================================================
void AISprite::SetCanFollowPath(bool value)
{
	mSteeringForce->CanFollowPath() = value;
}
//==================================================================================================================================
void AISprite::SetCanPursueTarget(bool value)
{
	mSteeringForce->CanPursueTarget() = value;
}
//==================================================================================================================================
void AISprite::SetCanEvadeTarget(bool value)
{
	mSteeringForce->CanEvadeTarget() = value;
}
//==================================================================================================================================
void AISprite::SetCanInterpose(bool value)
{
	mSteeringForce->CanInterpose() = value;
}
//==================================================================================================================================
void AISprite::SetCanHide(bool value)
{
	mSteeringForce->CanHide() = value;
}
//==================================================================================================================================
void AISprite::SetCanOffsetPursuit(bool value)
{
	mSteeringForce->CanOffsetPursuit() = value;
}
//==================================================================================================================================
void AISprite::SetWaypointSystem(ZShadeSandboxAI::WaypointSystem* waypoints)
{
	mSteeringForce->WaypointSystem() = waypoints;
}
//==================================================================================================================================
void AISprite::SetEvader(AISprite* sprite)
{
	mSteeringForce->Evader() = sprite;
}
//==================================================================================================================================
void AISprite::SetPursuer(AISprite* sprite)
{
	mSteeringForce->Pursuer() = sprite;
}
//==================================================================================================================================
void AISprite::SetTarget(AISprite* sprite)
{
	mSteeringForce->Target() = sprite;
}
//==================================================================================================================================
void AISprite::SetSpriteA(AISprite* sprite)
{
	mSteeringForce->SpriteA() = sprite;
}
//==================================================================================================================================
void AISprite::SetSpriteB(AISprite* sprite)
{
	mSteeringForce->SpriteB() = sprite;
}
//==================================================================================================================================
void AISprite::SetLeader(AISprite* sprite)
{
	mSteeringForce->Leader() = sprite;
}
//==================================================================================================================================
void AISprite::SetLeaderOffset(ZShadeSandboxMath::XMMath3 point)
{
	mSteeringForce->LeaderOffset() = point;
}
//==================================================================================================================================
void AISprite::SetTargetPoint(ZShadeSandboxMath::XMMath3 point)
{
	mSteeringForce->TargetPoint() = point;
}
//==================================================================================================================================
void AISprite::SetTargetDecelerationType(EDeceleration::Type type)
{
	mSteeringForce->TargetDecelerationType() = type;
}
//==================================================================================================================================
void AISprite::CreateWaypointSystem()
{
	mSteeringForce->WaypointSystem() = new ZShadeSandboxAI::WaypointSystem();
}
//==================================================================================================================================
int AISprite::WaypointSystemSize()
{
	if (mSteeringForce->WaypointSystem())
		return mSteeringForce->WaypointSystem()->Size();
	else
		return 0;
}
//==================================================================================================================================
bool AISprite::WaypointSystemEmpty()
{
	if (mSteeringForce->WaypointSystem())
		return mSteeringForce->WaypointSystem()->Empty();
	else
		return true;
}
//==================================================================================================================================
void AISprite::ClearWaypoints()
{
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->Clear();
}
//==================================================================================================================================
void AISprite::RemoveWaypoint(ZShadeSandboxMath::XMMath3 waypoint)
{
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->RemoveWaypoint(waypoint);
}
//==================================================================================================================================
void AISprite::AddWaypoint(ZShadeSandboxMath::XMMath3 waypoint)
{
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->AddWaypoint(waypoint);
}
//==================================================================================================================================
bool AISprite::CurrentWaypoint(ZShadeSandboxMath::XMMath3& waypoint)
{
	if (mSteeringForce->WaypointSystem())
		return mSteeringForce->WaypointSystem()->CurrentWaypoint(waypoint);
	else
		return false;
}
//==================================================================================================================================
void AISprite::SetNextWaypoint()
{
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->SetNextWaypoint();
}
//==================================================================================================================================
bool AISprite::WaypointSystemFinished()
{
	if (mSteeringForce->WaypointSystem())
		return mSteeringForce->WaypointSystem()->Finished();
	else
		return false;
}
//==================================================================================================================================
void AISprite::SetWaypointSystemCanLoop(bool value)
{
	if (mSteeringForce->WaypointSystem())
		mSteeringForce->WaypointSystem()->CanLoop() = value;
}
//==================================================================================================================================
bool AISprite::WaypointSystemCanLoop() const
{
	if (mSteeringForce->WaypointSystem())
		return mSteeringForce->WaypointSystem()->CanLoop();
	else
		return false;
}
//==================================================================================================================================
int AISprite::CurrentWaypointIndex() const
{
	if (mSteeringForce->WaypointSystem())
		return mSteeringForce->WaypointSystem()->CurrentWaypointIndex();
	else
		return -99999999;
}
//==================================================================================================================================
void AISprite::updateBehavior()
{
	if (tree == 0) return;
	
	//Think about what we are doing
	Think();
	
	if (m_run_behavior)
	{
		tree->RunBehavior(m_behavior_name);
		tree->TickActiveBehavior();
	}
	else
	{
		if (m_run_behavior_default)
		{
			//Run default behavior
			tree->RunBehavior(m_default_behavior);
			tree->TickActiveBehavior();
		}
	}
}
//==================================================================================================================================
void AISprite::AddDefaultBehavior(string p_default_behavior_name)
{
	m_default_behavior = p_default_behavior_name;
}
//==================================================================================================================================
void AISprite::AddBehaviorSequence(string p_behavior_name, string p_sequence_name, string p_action_func_name, string p_condition_func_name)
{
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		if (m_behaviors[i]->name == p_behavior_name)
		{
			SEQUENCE_INFO* si = new SEQUENCE_INFO();
			si->name = p_sequence_name;
			si->action_func_name = p_action_func_name;
			si->condition_func_name = p_condition_func_name;
			m_behaviors[i]->sequence_info.push_back( si );
		}
	}
}
//==================================================================================================================================
void AISprite::RegisterBehavior(string p_behavior_name)
{
	BEHAVIOR_INFO* bi = new BEHAVIOR_INFO();

	bi->name = p_behavior_name;

	m_behaviors.push_back( bi );
}
//==================================================================================================================================
void AISprite::CreateTree()
{
	tree = new BehaviorTree();

	for (int i = 0; i < m_behaviors.size(); i++)
	{
		Behavior* aiBehavior = new Behavior(m_behaviors[i]->name);
		{
			PrioritySelector* selector = new PrioritySelector();
			{
				for (int j = 0; j < m_behaviors[i]->sequence_info.size(); j++)
				{
					Sequence* sequence = new Sequence();
					{
						Node* sequence_node = new TaskNode();
						{
							sequence_node->SetSequenceLevel(1);
							Decorator* sequence_decorator = new Decorator();
							{
								LuaTask* action = new LuaTask();
								LuaDelegate* condition = new LuaDelegate(this);
								action->SetLuaState(Scripting::LuaState());
								condition->SetLuaState(Scripting::LuaState());
								string path = Scripting::GetGameDirectory()->m_scripts_path;
								path.append("\\");
								path.append(m_script_name);
								action->AddLuaAction(this, m_behaviors[i]->sequence_info[j]->name, path, m_behaviors[i]->sequence_info[j]->action_func_name);
								condition->AddFunction(path, m_behaviors[i]->sequence_info[j]->condition_func_name);
								sequence_decorator->SetUseLua(true);
								sequence_decorator->AddDecoratedComponent(action);
								sequence_decorator->AddCondition(condition);
							}
							sequence_node->AddDecorator( sequence_decorator );
						}
						sequence->AddChildTask( sequence_node );
					}
					selector->AddSequence( sequence );
				}
			}
			aiBehavior->AddSelector( selector );
		}
		tree->InsertBehavior( aiBehavior );
	}
}
//==================================================================================================================================
void AISprite::Main()
{
	m_script->Main(1, this);
}
//==================================================================================================================================
void AISprite::MoveScript()
{
	// I can move
	if (m_moving)
	{
		m_script->Move(1, this);
	}
}
//==================================================================================================================================
void AISprite::Think()
{
	// I can think about what to do
	if (m_thinking)
	{
		m_script->Think(1, this);
	}
}
//==================================================================================================================================
void AISprite::Talk()
{
	// I decided to talk
	if (m_talking)
	{
		m_script->Talk(1, this);
	}
}
//==================================================================================================================================
void AISprite::Die()
{
	// I was hurt badly and am dying
	if (m_dying)
	{
		m_script->Die(1, this);
	}
}
//==================================================================================================================================
void AISprite::Hit()
{
	// I was hit
	if (m_hit)
	{
		m_script->Hit(1, this);
	}
}
//==================================================================================================================================
void AISprite::Touch()
{
	// I was touched
	if (m_touched)
	{
		m_script->Touch(1, this);
	}
}
//==================================================================================================================================
void AISprite::Attack()
{
	// I will be attacking a sprite
	if (m_attacked)
	{
		if (m_TargetSprite != NULL)
		{
			m_script->Attack(2, this, m_TargetSprite);
		}
	}
}
//==================================================================================================================================
void AISprite::Target()
{
	// I will be targeting a sprite
	if (m_targeted)
	{
		if (m_TargetSprite != NULL)
		{
			m_script->Target(2, this, m_TargetSprite);
		}
	}
}
//==================================================================================================================================