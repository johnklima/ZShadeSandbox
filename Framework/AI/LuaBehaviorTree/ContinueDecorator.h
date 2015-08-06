/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class ContinueDecorator created on 2/13/2012
\******************************************/
#ifndef __CONTINUEDECORATOR_H
#define __CONTINUEDECORATOR_H
#include "Decorator.h"
#include "Status.h"
namespace LuaBehaviorTree {
//------------------------------------------
class ContinueDecorator : public Decorator
{
public:
	ContinueDecorator() : Decorator()
	{}
	ContinueDecorator(Component* pDecoratedComponent, LuaDelegate* pCondition, bool pCanRun)
		: Decorator(pDecoratedComponent, pCondition, pCanRun)
	{}
	ContinueDecorator(Component* pParent, Component* pDecoratedComponent, LuaDelegate* pCondition, bool pCanRun)
		: Decorator(pParent, pDecoratedComponent, pCondition, pCanRun)
	{}
	~ContinueDecorator()
	{}

	Status OnUpdate()
	{
		if (CanRun()) return Status::SUCCESS;

		mDecoratedComponent->OnStart();

		while (mDecoratedComponent->OnTick() == Status::RUNNING)
			return Status::RUNNING;

		mDecoratedComponent->OnStop();

		//The Selector will select how to continue
		if (mDecoratedComponent->GetStatus() == Status::FAILURE)
		{
			if (typeid(mParent).name() == "Selector")
				return Status::FAILURE;
		}

		//Everything else will succeed
		return Status::SUCCESS;
	}
};
//------------------------------------------
}
#endif /* __CONTINUEDECORATOR_H */