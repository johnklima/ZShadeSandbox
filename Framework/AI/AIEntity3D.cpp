#include "AIEntity3D.h"
#include "Scripting.h"
//==================================================================================================================================
//==================================================================================================================================
AIEntity3D::AIEntity3D(D3D* d3d)
:	Sprite(d3d)
,	m_run_behavior(false)
,	m_run_behavior_default(false)
,	m_default_behavior("")
,	m_behavior_name("")
,	tree(0)
,	m_TargetSprite(0)
{
	//mSteeringForce = new AISteeringForce(this);
}
//==================================================================================================================================
AIEntity3D::~AIEntity3D()
{
}
//==================================================================================================================================
void AIEntity3D::SetRunBehavior(bool set)
{
	m_run_behavior = set;
}
//==================================================================================================================================
void AIEntity3D::SetRunBehaviorDefault(bool set)
{
	m_run_behavior_default = set;
}
//==================================================================================================================================
void AIEntity3D::SetBehaviorName(string name)
{
	m_behavior_name = name;
}
//==================================================================================================================================
void AIEntity3D::CalculateSteeringForce()
{
	// Only AI will need to have this since a human can do all of these things.
	if (!m_player)
	{
		//ZShadeSandboxMath::XMMath3 force = mSteeringForce->CalculateForce();
		
		// Update the velocity of the sprite with the new force
		//UpdateVelocity(force.x, force.y);
	}
}
//==================================================================================================================================
void AIEntity3D::updateBehavior()
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
void AIEntity3D::AddDefaultBehavior(string p_default_behavior_name)
{
	m_default_behavior = p_default_behavior_name;
}
//==================================================================================================================================
void AIEntity3D::AddBehaviorSequence(string p_behavior_name, string p_sequence_name, string p_action_func_name, string p_condition_func_name)
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
void AIEntity3D::RegisterBehavior(string p_behavior_name)
{
	BEHAVIOR_INFO* bi = new BEHAVIOR_INFO();

	bi->name = p_behavior_name;

	m_behaviors.push_back( bi );
}
//==================================================================================================================================
void AIEntity3D::CreateTree()
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
void AIEntity3D::Main()
{
	m_script->Main(1, this);
}
//==================================================================================================================================
void AIEntity3D::MoveScript()
{
	// I can move
	if (m_moving)
	{
		m_script->Move(1, this);
	}
}
//==================================================================================================================================
void AIEntity3D::Think()
{
	// I can think about what to do
	if (m_thinking)
	{
		m_script->Think(1, this);
	}
}
//==================================================================================================================================
void AIEntity3D::Talk()
{
	// I decided to talk
	if (m_talking)
	{
		m_script->Talk(1, this);
	}
}
//==================================================================================================================================
void AIEntity3D::Die()
{
	// I was hurt badly and am dying
	if (m_dying)
	{
		m_script->Die(1, this);
	}
}
//==================================================================================================================================
void AIEntity3D::Hit()
{
	// I was hit
	if (m_hit)
	{
		m_script->Hit(1, this);
	}
}
//==================================================================================================================================
void AIEntity3D::Touch()
{
	// I was touched
	if (m_touched)
	{
		m_script->Touch(1, this);
	}
}
//==================================================================================================================================
void AIEntity3D::Attack()
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
void AIEntity3D::Target()
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