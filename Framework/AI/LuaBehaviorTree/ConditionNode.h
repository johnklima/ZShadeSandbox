/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class ConditionNode created on 2/22/2012
\******************************************/
#ifndef __CONDITIONNODE_H
#define __CONDITIONNODE_H
#ifndef __COMPONENT_H
#include "Component.h"
#endif
#ifndef __DELEGATE_H
#include "LuaDelegate.h"
#endif
#ifndef __NODE_H
#include "Node.h"
#endif
#ifndef __STATUS_H
#include "Status.h"
#endif
namespace LuaBehaviorTree {
//------------------------------------------
class ConditionNode : public Node
{
	LuaDelegate* mCondition;
public:
	ConditionNode()
		: mCondition(NULL),
		  //Component(),
		  Node()
	{
		mIsAssersionNode = true;
		mIsLeaf = true;
	}
	ConditionNode(LuaDelegate* pCondition)
		: mCondition(pCondition),
		  //Component(),
		  Node()
	{
		mIsAssersionNode = true;
		mIsLeaf = true;
	}
	ConditionNode(int pSequenceLevel)
		: mCondition(NULL),
		  Node(pSequenceLevel,true,true)
	{}
	ConditionNode(int pSequenceLevel, LuaDelegate* pCondition)
		: mCondition(pCondition),
		  Node(pSequenceLevel,true,true)
	{}
	ConditionNode(Component* pParent, int pSequenceLevel, LuaDelegate* pCondition)
		: mCondition(pCondition),
		  Node(pParent,pSequenceLevel,true,true)
	{}
	/*ConditionNode(int pSequenceLevel, bool pIsLeaf, bool pIsAssersionNode, int pPriority,
		          Delegate<T, bool>* pCondition)
		: Node<T>(pSequenceLevel,pIsLeaf,pIsAssersionNode,pPriority),
		  Component<T>()
		  mCondition(pCondition)
	{
		mIsAssersionNode = true;
		mIsLeaf = true;
	}
	ConditionNode(Component<T>* pParent, int pSequenceLevel, bool pIsLeaf, bool pIsAssersionNode, int pPriority,
		          Delegate<T, bool>* pCondition)
		: Node<T>(pSequenceLevel,pIsLeaf,pIsAssersionNode,pPriority),
		  Component<T>(pParent)
		  mCondition(pCondition)
	{
		mIsAssersionNode = true;
		mIsLeaf = true;
	}*/
	ConditionNode(Component* pParent, LuaDelegate* pCondition)
		: Node(0,true, true),
		  //Component(pParent)
		  mCondition(pCondition)
	{}
	~ConditionNode()
	{}

	void AddCondition(LuaDelegate* pCondition)
	{
		mCondition = pCondition;
	}

	Status OnUpdate()
	{
		if (mCondition != NULL)
		{
			bool t = mCondition->OnTick();
			if (!mCondition->OnTick())
				return Status::FAILURE;
		}

		return Status::SUCCESS;
	}

	Status OnTick()
	{
		return Component::OnTick();
	}

	void OnStart()
	{
		Component::OnStart();
	}

	void OnStop()
	{
		Component::OnStop();
	}
};
//------------------------------------------
}
#endif /* __CONDITIONNODE_H */