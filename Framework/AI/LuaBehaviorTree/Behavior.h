/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 LuaBehaviorTree Created On: 2/16/2014
 ------------------------------------------
 Class Behavior created on 2/16/2014
\******************************************/
#ifndef __BEHAVIOR_H
#define __BEHAVIOR_H
#include <string>
using std::string;
#include "BehaviorException.h"
#include "Status.h"
#include "Sequence.h"
#include "Selector.h"
namespace LuaBehaviorTree {
//------------------------------------------
//class Task;
class Behavior
{
	//Moving the selector into behavior because the
	//sequence and the selector are complements of each other
	//The sequence bails on success
	//The selector bails on failure (We ran out of children, and none succeeded.)
	//UPDATE UML
	//The selector runs and that runs the sequence

	//The selector will run through the sequence
	// and the sequence will run through the nodes
	
	//the task and condition are added to the decorator
	//the decorator is added to the node
	//the node(s) is(are) added to the sequence
	//the sequence(s) is(are) added to the selector
	
	string mBehaviorName;
	Selector* mSelector;
public:
	Behavior() : mBehaviorName("NoName"), mSelector(NULL)
	{}
	Behavior(string pBehaviorName, Selector* pSelector)
		: mBehaviorName(pBehaviorName), mSelector(pSelector)
	{}
	Behavior(string pBehaviorName)
		: mBehaviorName(pBehaviorName), mSelector(NULL)
	{}
	~Behavior()
	{}
	
	void OnInitialize()
	{
		mSelector->OnInitialize();
	}
	
	void AddSelector(Selector* pSelector)
	{
		mSelector = pSelector;
	}
	
	string GetName() {return mBehaviorName;}
	
	Status OnTick()
	{
		return mSelector->OnTick();
	}
	
	Status OnUpdate()
	{
		return mSelector->OnUpdate();
	}
	
	void OnStart()
	{
		mSelector->OnStart();
	}
	
	void OnStop()
	{
		mSelector->OnStop();
	}
	
	Selector* GetSelector() {return mSelector;}
	
	//This will not return the sequence in this behavior
	//When the Breadth-First Search gets here it will be looking
	//for a sub-behavior from inside the sequences children of nodes.
	
	Sequence* SearchSubBehavior(string pName)
	{
		throw BehaviorException("Behavior", "SearchBehavior", "Not implemented yet!!");
	}
	
	Task* SearchTask(string pName)
	{
		throw BehaviorException("Behavior", "SearchTask", "Not implemented yet!!");
	}
};
//------------------------------------------
}
#endif /* __BEHAVIOR_H */