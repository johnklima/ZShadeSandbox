/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class LoopDecorator created on 2/13/2012
\******************************************/
#ifndef __LOOPDECORATOR_H
#define __LOOPDECORATOR_H
#ifndef __BEHAVIOREXCEPTION_H
#include "BehaviorException.h"
#endif
#include "Decorator.h"
#include "Status.h"
namespace LuaBehaviorTree {
//------------------------------------------
class LoopDecorator : public Decorator
{
	int mCounter;
public:
	LoopDecorator() : mCounter(0), Decorator()
	{}
	LoopDecorator(int pCounter)
		: mCounter(pCounter), Decorator()
	{}
	LoopDecorator(int pCounter, Component* pDecoratedComponent, LuaDelegate* pCondition, bool pCanRun)
		: mCounter(pCounter), Decorator(pDecoratedComponent, pCondition, pCanRun)
	{}
	LoopDecorator(Component* pParent, Component* pDecoratedComponent, LuaDelegate* pCondition, bool pCanRun)
		: Decorator(pParent, pDecoratedComponent, pCondition, pCanRun)
	{}
	LoopDecorator(int pCounter, Component* pParent, Component* pDecoratedComponent, LuaDelegate* pCondition, bool pCanRun)
		: mCounter(pCounter), Decorator(pParent, pDecoratedComponent, pCondition, pCanRun)
	{}
	~LoopDecorator()
	{}

	void SetCounter(int pCounter) { mCounter = pCounter; }

	Status OnUpdate()
	{
		throw BehaviorException("LoopDecorator", "OnUpdate", "Not implemented yet!!");
		return Status::SUCCESS;
	}
};
//------------------------------------------
}
#endif /* __LOOPDECORATOR_H */