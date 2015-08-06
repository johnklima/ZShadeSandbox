/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class Decorator created on 2/13/2012
 ------------------------------------------
 Modified for lua on 2/16/2014
\******************************************/
#ifndef __DECORATOR_H
#define __DECORATOR_H
#include "LuaDelegate.h"
#include "Component.h"
#include "Status.h"
namespace LuaBehaviorTree {
//------------------------------------------
class Decorator : public Component
{
protected:
	bool CanRun(){return mCanRun;}
	
	Component* mDecoratedComponent;
	LuaDelegate* mCondition;
	LuaDelegate* mLuaCondition;

	bool useLua;
	//This variable will look at the condition and decide if this
	//decorator can run.
	bool mCanRun;
public:
	Decorator() : mCanRun(true), mDecoratedComponent(NULL), mCondition(NULL), useLua(false)
	{}
	Decorator(Component* pDecoratedComponent, LuaDelegate* pCondition, bool pCanRun)
		: mCanRun(pCanRun), mDecoratedComponent(pDecoratedComponent), mCondition(pCondition), useLua(false)
	{}
	Decorator(Component* pParent, Component* pDecoratedComponent, LuaDelegate* pCondition, bool pCanRun)
		: Component(pParent), mCanRun(pCanRun), mDecoratedComponent(pDecoratedComponent), mCondition(pCondition),
		  useLua(false)
	{}
	~Decorator()
	{}

	void OnInitialize()
	{
		Component::OnInitialize();
		mDecoratedComponent->OnInitialize();
	}
	
	void AddDecoratedComponent(Component* pDecoratedComponent)
	{
		mDecoratedComponent = pDecoratedComponent;
	}
	
	void SetUseLua(bool ul)
	{
		useLua = ul;
	}

	void AddCondition(LuaDelegate* pCondition)
	{
		mLuaCondition = pCondition;
	}
	
	void SetCanRun(bool pRun)
	{
		mCanRun = pRun;
	}
	
	virtual Status OnUpdate()
	{
		if (useLua)
		{
			if (mLuaCondition != NULL)
			{
				if (!mLuaCondition->OnTick())
					return Status::FAILURE;
			}
			else if (!CanRun())
				return Status::FAILURE;
		}
		else
		{
			if (mCondition != NULL)
			{
				if (!mCondition->OnTick())
					return Status::FAILURE;
			}
			else if (!CanRun())
				return Status::FAILURE;
		}

		mDecoratedComponent->OnStart();

		while (mDecoratedComponent->OnTick() == Status::RUNNING)
			return Status::RUNNING;

		mDecoratedComponent->OnStop();

		if (mDecoratedComponent->GetStatus() == Status::FAILURE)
			return Status::FAILURE;

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
};
//------------------------------------------
}
#endif /* __DECORATOR_H */