/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class BehaviorTree created on 2/15/2012
\******************************************/
#ifndef __BEHAVIORTREE_H
#define __BEHAVIORTREE_H
#include <string>
#include <list>
using std::list;
using std::string;
#include "BehaviorException.h"
#include "Status.h"
#include "Behavior.h"
#include "Node.h"
//#include "Task.h"
namespace LuaBehaviorTree {
//------------------------------------------
class BehaviorTree
{
	Behavior* mActiveBehavior;
	list<Behavior*> mBehaviors;
	//For an event driven behavior this observer will bail out of
	//the tree and re-enter the tree at the requested node.
	
	//figure out what to do with mObserver
	//a particular node needs to be attatched to an observer when a node is entered. 
	Node* mObserver;
public:
	BehaviorTree() : mObserver(NULL), mActiveBehavior(NULL)
	{}
	BehaviorTree(list<Behavior*> pBehaviors)
		: mBehaviors(pBehaviors), mObserver(NULL), mActiveBehavior(NULL)
	{}
	~BehaviorTree()
	{}

	void OnInitialize()
	{
		list<Behavior*>::iterator iter = mBehaviors.begin();
		for (; iter != mBehaviors.end(); iter++)
		{
			Behavior* behave = (*iter);
			behave->OnInitialize();
		}
	}
	
	void RunBehavior(string pName)
	{
		list<Behavior*>::iterator iter = mBehaviors.begin();
		for (; iter != mBehaviors.end(); iter++)
		{
			Behavior* behave = (*iter);
			if (behave->GetName() == pName)
			{
				mActiveBehavior = behave;
				behave->OnStart();
				//behave->OnTick();
			    break;
			}
		}
	}

	void TickActiveBehavior()
	{
	    if (mActiveBehavior != NULL)
			mActiveBehavior->OnTick();
	}
	
	void InsertBehavior(Behavior* pBehavior)
	{
		mBehaviors.push_back( pBehavior );
	}
	
	//This function will use a Breadth-First Search
	Behavior* SearchBehavior(string pName)
	{
		throw BehaviorException("BehaviorTree", "SearchBehavior", "Not implemented yet!!");

		list<Behavior*>::iterator iter = mBehaviors.begin();
		for (; iter != mBehaviors.end(); iter++)
		{
			Behavior* behave = (*iter);
			//Search with Breadth-First
		}
		return NULL;
	}
	
	//This function will use a Depth-First Search
	Task* SearchTask(string pName)
	{
		throw BehaviorException("BehaviorTree", "SearchTask", "Not implemented yet!!");

		list<Behavior*>::iterator iter = mBehaviors.begin();
		for (; iter != mBehaviors.end(); iter++)
		{
			Behavior* behave = (*iter);
			//Search with Depth-First
		}
		return NULL;
	}
};
//------------------------------------------
}
#endif /* __BEHAVIORTREE_H */