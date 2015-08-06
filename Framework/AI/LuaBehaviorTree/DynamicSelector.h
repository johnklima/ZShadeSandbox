/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class DynamicSelector created on 2/15/2012
\******************************************/
#ifndef __DYNAMICSELECTOR_H
#define __DYNAMICSELECTOR_H
#ifndef __BEHAVIOREXCEPTION_H
#include "BehaviorException.h"
#endif
#include "Selector.h"
namespace LuaBehaviorTree {
//------------------------------------------
class DynamicSelector : public Selector
{
public:
	DynamicSelector() : Selector()
	{}
	DynamicSelector(list<Sequence*> pChildren)
		: Selector(pChildren)
	{}
	~DynamicSelector()
	{}

	Status OnUpdate()
	{
		throw BehaviorException("DynamicSelector", "OnUpdate", "Not implemented yet!!");
		return Status::SUCCESS;
	}
};
//------------------------------------------
}
#endif /* __DYNAMICSELECTOR_H */