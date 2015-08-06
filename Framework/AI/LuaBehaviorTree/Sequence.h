/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class Sequence created on 2/15/2012
\******************************************/
#ifndef __SEQUENCE_H
#define __SEQUENCE_H
#include <list>
using std::list;
#ifndef __BEHAVIOREXCEPTION_H
#include "BehaviorException.h"
#endif
#ifndef __STATUS_H
#include "Status.h"
#endif
#ifndef __SELECTOR_H
#include "Selector.h"
#endif
#ifndef __NODE_H
#include "Node.h"
#endif
#ifndef __COMPONENT_H
#include "Component.h"
#endif
#ifndef __BEHAVIOR_H
#include "Behavior.h"
#endif
namespace LuaBehaviorTree {
//------------------------------------------
class Sequence : public Component
{
protected:
	Component* mSelection;
	list<Node*> mChildren;
public:
	Sequence() : mSelection(NULL), Component()
	{}
	Sequence(list<Node*> pChildren)
		: mChildren(pChildren), mSelection(NULL),
		  Component()
	{}
	~Sequence()
	{}
	
	void AddChildTask(Node* pChildTask)
	{
		mChildren.push_back( pChildTask );
	}
    
	void OnInitialize()
	{
		Component::OnInitialize();
		
		list<Node*>::iterator iter = mChildren.begin();
		for (; iter != mChildren.end(); iter++)
		{
			Node* childNode = (*iter);
			childNode->OnInitialize();
		}
	}
	
	virtual Status OnUpdate()
	{
		//Update all the children nodes for this sequence
		list<Node*>::iterator iter = mChildren.begin();
		for (; iter != mChildren.end(); iter++)
		{
			Node* childNode = (*iter);

			childNode->OnStart();

			while (childNode->OnTick() == Status::RUNNING)
			{
				mSelection = childNode;
				return Status::RUNNING;
			}

			mSelection = NULL;
			childNode->OnStop();

			if (childNode->GetStatus() == Status::FAILURE)
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
	
	//Behavior<T>* SearchSubBehavior(string pName)
	//{
	//	//throw BehaviorException("Sequence", "SearchSubBehavior", "Not implemented yet!!");
	//	return NULL;
	//}
	//
	//Task<T>* SearchTask(string pName)
	//{
	//	//throw BehaviorException("Sequence", "SearchTask", "Not implemented yet!!");
	//	return NULL;
	//}
};
//------------------------------------------
}
#endif /* __SEQUENCE_H */