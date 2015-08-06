/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class Node created on 2/13/2012
\******************************************/
#ifndef __NODE_H
#define __NODE_H
#ifndef __BEHAVIOREXCEPTION_H
#include "BehaviorException.h"
#endif
#ifndef __DECORATOR_H
#include "Decorator.h"
#endif
#ifndef __STATUS_H
#include "Status.h"
#endif
#ifndef __SEQUENCE_H
#include "Sequence.h"
#endif
#ifndef __BEHAVIOR_H
#include "Behavior.h"
#endif
namespace LuaBehaviorTree {
class Task;
class Sequence;
class Behavior;
//------------------------------------------
class Node : public Component
{
protected:
	//Set all of these variables to be defaulted when node is created
	//If node is a condition node then mIsAssersionNode->true also mIsLeaf->true
	//If there is no subbehavior then mIsLeaf->true


	//This will be what level the node is on.
	//Each level will have the same sequence.
	int mSequenceLevel;
	bool mIsLeaf;
	//Node will check before hand, before the behavior continues, so if the item is
	//not present for example the tree will bail out and prune the tree making sure
	//that we do not have to go through anymore searching.
	//It is a quick check.
	bool mIsAssersionNode;
public:
	Node()
		: mSequenceLevel(1), mIsLeaf(true), mIsAssersionNode(false),
		  Component()
	{}
	Node(int pSequenceLevel, bool pIsLeaf, bool pIsAssersionNode)
		: mSequenceLevel(pSequenceLevel), mIsLeaf(pIsLeaf),
		  mIsAssersionNode(pIsAssersionNode),
		  Component()
	{}
	Node(Component* pParent, int pSequenceLevel, bool pIsLeaf, bool pIsAssersionNode)//, int pPriority)
		: mSequenceLevel(pSequenceLevel), mIsLeaf(pIsLeaf),
		  mIsAssersionNode(pIsAssersionNode),// mPriority(pPriority),
		  Component(pParent)
	{}
	Node(Component* pParent) : mSequenceLevel(1), mIsLeaf(true), mIsAssersionNode(false), Component(pParent)
	{}
	~Node()
	{}
	
	virtual Status OnUpdate()
	{
		//Need to do nothing
		cout << "Node success" << endl;
		return Status::SUCCESS;
	}

	virtual void OnStart()
	{
		Component::OnStart();
	}

	virtual void OnStop()
	{
		Component::OnStop();
	}

	//These functions are for TaskNode
	virtual void AddSubBehavior(Behavior* pSubBehavior)
	{throw BehaviorException("Node","AddSubBehavior","AddSubBehavior function is not accessible from Node");}
	virtual void AddDecorator(Decorator* pTask)
	{throw BehaviorException("Node","AddDecorator","AddDecorator function is not accessible from Node");}
	//This is for ConditionNode
	virtual void AddCondition(LuaDelegate* pCondition)
	{throw BehaviorException("Node","AddCondition","AddCondition function is not accessible from Node");}

	void SetSequenceLevel(int pSequenceLevel){mSequenceLevel = pSequenceLevel;}
	void SetIsLeaf(bool pIsLeaf) { mIsLeaf = pIsLeaf; }

	bool IsLeaf(){return mIsLeaf;}
	bool IsAssersionNode(){return mIsAssersionNode;}
};
//------------------------------------------
}
#endif /* __NODE_H */