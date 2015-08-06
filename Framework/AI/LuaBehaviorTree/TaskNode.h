/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class ConditionNode created on 2/22/2012
\******************************************/
#ifndef __TASKNODE_H
#define __TASKNODE_H
#ifndef __BEHAVIOREXCEPTION_H
#include "BehaviorException.h"
#endif
#ifndef __BEHAVIOR_H
#include "Behavior.h"
#endif
#ifndef __DECORATOR_H
#include "Decorator.h"
#endif
#ifndef __NODE_H
#include "Node.h"
#endif
#ifndef __STATUS_H
#include "Status.h"
#endif
namespace LuaBehaviorTree {
//------------------------------------------
class TaskNode : public Node
{
	Decorator* mTask;
	Behavior* mSubBehavior;
public:
	TaskNode()
		: mTask(NULL), mSubBehavior(NULL),
		  Node()
	{
		mIsLeaf = true;
	}
	TaskNode(Decorator* pTask, Behavior* pSubBehavior = NULL)
		: mTask(pTask), mSubBehavior(pSubBehavior),
		  Node()
	{
		if (pSubBehavior == NULL)
			mIsLeaf = true;
		else
			mIsLeaf = false;
	}
	TaskNode(Component* pParent, Decorator* pTask, Behavior* pSubBehavior = NULL)
		: Node(0,false,false),
		  mTask(pTask), mSubBehavior(pSubBehavior)
	{
		if (pSubBehavior == NULL)
			mIsLeaf = true;
		else
			mIsLeaf = false;
	}
	TaskNode(int pSequenceLevel, Decorator* pTask, Behavior* pSubBehavior = NULL)
		: mTask(pTask), mSubBehavior(pSubBehavior),
		  Node(pSequenceLevel,false, true)
	{
		if (pSubBehavior == NULL)
			mIsLeaf = true;
		else
			mIsLeaf = false;
	}
	TaskNode(Component* pParent, int pSequenceLevel, Decorator* pTask, Behavior* pSubBehavior = NULL)
		: mTask(pTask), mSubBehavior(pSubBehavior),
		  Node(pSequenceLevel,false, true)
	{
		if (pSubBehavior == NULL)
			mIsLeaf = true;
		else
			mIsLeaf = false;
	}
	/*TaskNode(int pSequenceLevel, bool pIsLeaf, bool pIsAssersionNode, int pPriority,
		     Decorator<T>* pTask, Behavior<T>* pSubBehavior = NULL)
		: Node<T>(pSequenceLevel,pIsLeaf,pIsAssersionNode,pPriority),
		  mTask(pTask), mSubBehavior(pSubBehavior)
	{}
	TaskNode(Component<T>* pParent, int pSequenceLevel, bool pIsLeaf, bool pIsAssersionNode, int pPriority,
		     Decorator<T>* pTask, Behavior<T>* pSubBehavior = NULL)
		: Node<T>(pParent, pSequenceLevel,pIsLeaf,pIsAssersionNode,pPriority),
		  mTask(pTask), mSubBehavior(pSubBehavior)
	{}*/
	~TaskNode()
	{}

	Status OnUpdate()
	{
		//if (mTask != NULL) return mTask->OnUpdate();
		//if (mSubBehavior != NULL) return mSubBehavior->OnUpdate();
		return Status::SUCCESS;
	}

	void OnStart()
	{
		Node::OnStart();
		if (mTask != NULL) mTask->OnStart();
		if (mSubBehavior != NULL)
			mSubBehavior->OnStart();
	}

	void OnStop()
	{
		Node::OnStop();
		if (mTask != NULL) mTask->OnStop();
		if (mSubBehavior != NULL) mSubBehavior->OnStop();
	}

	void AddSubBehavior(Behavior* pSubBehavior)
	{
		mSubBehavior = pSubBehavior;
		mIsLeaf = false;
	}

	void AddDecorator(Decorator* pTask)
	{
		mTask = pTask;
	}

	Behavior* SearchSubBehavior(string pName)
	{
		throw BehaviorException("TaskNode", "SearchSubBehavior", "Not implemented yet!!");
	}

	Task* SearchTask(string pName)
	{
		throw BehaviorException("TaskNode", "SearchTask", "Not implemented yet!!");
	}
};
//------------------------------------------
}
#endif /* __TASKNODE_H */