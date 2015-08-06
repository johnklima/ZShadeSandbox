/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class WaitDecorator created on 2/13/2012
\******************************************/
#ifndef __WAITDECORATOR_H
#define __WAITDECORATOR_H
#include <time.h>
#include "Decorator.h"
#include "Status.h"
namespace LuaBehaviorTree {
//------------------------------------------
class WaitDecorator : public Decorator
{
	time_t _end;
	int mTimeInSeconds;
public:
	WaitDecorator()
		: mTimeInSeconds(0), _end(0),
		  Decorator()
	{}
	WaitDecorator(int pTimeInSeconds)
		: mTimeInSeconds(pTimeInSeconds), _end(pTimeInSeconds),
		  Decorator()
	{}
	WaitDecorator(int pTimeInSeconds, Component* pDecoratedComponent, LuaDelegate* pCondition, bool pCanRun)
		: mTimeInSeconds(pTimeInSeconds), _end(pTimeInSeconds),
		  Decorator(pDecoratedComponent, pCondition, pCanRun)
	{}
	WaitDecorator(Component* pParent, Component* pDecoratedComponent, LuaDelegate* pCondition, bool pCanRun)
		: Decorator(pParent, pDecoratedComponent, pCondition, pCanRun)
	{}
	WaitDecorator(int pTimeInSeconds, Component* pParent, Component* pDecoratedComponent, LuaDelegate* pCondition, bool pCanRun)
		: mTimeInSeconds(pTimeInSeconds), _end(pTimeInSeconds),
		  Decorator(pParent, pDecoratedComponent, pCondition, pCanRun)
	{}
	~WaitDecorator()
	{}

	void SetTimeInSeconds(int pTimeInSeconds)
	{
		mTimeInSeconds = pTimeInSeconds;
		_end = mTimeInSeconds;
	}

	void OnStart()
	{
		_end = time( NULL ) + mTimeInSeconds;
		Decorator::OnStart();
	}
	
	void OnStop()
	{
		_end = 0;
		Decorator::OnStop();
	}
	
	Status OnUpdate()
	{
		while (time(0) < _end)
		{
			if (mDecoratedComponent != NULL)
			{
				if (mDecoratedComponent->OnTick() == Status::RUNNING)
					break;
			}
			else
			{
				if (CanRun()) break;
			}
			return Status::RUNNING;
		}

		if (time(0) < _end)
			return Status::FAILURE;

		mDecoratedComponent->OnStart();

		while (mDecoratedComponent->OnTick() == Status::RUNNING)
			return Status::RUNNING;

		mDecoratedComponent->OnStop();

		if (mDecoratedComponent->GetStatus() == Status::FAILURE)
			return Status::FAILURE;

		return Status::SUCCESS;
	}
};
//------------------------------------------
}
#endif /* __WAITDECORATOR_H */