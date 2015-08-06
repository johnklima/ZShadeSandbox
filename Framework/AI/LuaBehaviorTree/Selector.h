/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class Selector created on 2/15/2012
\******************************************/
#ifndef __SELECTOR_H
#define __SELECTOR_H
#include "Status.h"
#ifndef __BEHAVIOREXCEPTION_H
#include "BehaviorException.h"
#endif
#ifndef __COMPONENT_H
#include "Component.h"
#endif
#ifndef __SEQUENCE_H
#include "Sequence.h"
#endif
namespace LuaBehaviorTree {
class Sequence;
//------------------------------------------
class Selector : public Component
{
protected:
	list<Sequence*> mSequences;
public:
	Selector()
	{}
	Selector(list<Sequence*> pSequences)
		: mSequences(pSequences)
	{}
	~Selector()
	{}

	void OnInitialize()
	{
		Component::OnInitialize();
	}
	
	virtual Status OnUpdate()
	{
		throw BehaviorException("Selector", "OnUpdate", "Using base Selector");
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
	
	void AddSequence(Sequence* pSequence) { mSequences.push_back(pSequence); }
	
	Sequence* SearchSequence()
	{
		throw BehaviorException("Selector", "SearchSequence", "Not implemented yet!!");
		return NULL;
	}
};
//------------------------------------------
}
#endif /* __SELECTOR_H */