/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class Component created on 2/13/2012
\******************************************/
#ifndef __COMPONENT_H
#define __COMPONENT_H
#include <iostream>
#include <list>
#include <string>
using namespace std;
#ifndef __ENUMERATOR_H
#include "Enumerator.h"
#endif
#ifndef __BEHAVIOREXCEPTION
#include "BehaviorException.h"
#endif
#ifndef __STATUS_H
#include "Status.h"
#endif
namespace LuaBehaviorTree {
//------------------------------------------
class Component
{
protected:
	Enumerator mStatusList;
	Component* mParent;
	Status mLastStatus;
public:
	Component() : mLastStatus(Status::NOTHING)
	{}
	Component(Component* pParent)
		: mParent(pParent), mLastStatus(Status::NOTHING)
	{}
	~Component()
	{}

	void OnInitialize()
	{}
	
	virtual Status OnUpdate() = 0;
	
	virtual Status OnTick()
	{
		if (mLastStatus != Status::NOTHING &&
			mLastStatus != Status::RUNNING)
			return mLastStatus;

		if (mStatusList.IsEmpty())
			throw BehaviorException("Component", "OnTick", "Cannot run OnTick before running start");

		if (mStatusList.MoveNext())
			mLastStatus = mStatusList.Yields();
		else
			throw BehaviorException("Component", "OnTick", "There is nothing to run");

		if (mLastStatus != Status::RUNNING)
			OnStop();

		return mLastStatus;
	}
	
	virtual void OnStart()
	{
		//Initially begin the last status with nothing.
		mLastStatus = Status::NOTHING;
		//Add the status return value of this
		// component to the enumerated list.
		mStatusList.Assign( OnUpdate() );
		//Set the enumerated list to the beginning
		// because we are starting this component.
		mStatusList.Init();
	}
	
	virtual void OnStop()
	{
		//Empty the status list after stopping
		// the component
		if (!mStatusList.IsEmpty())
			mStatusList.Empty();

		/*if (mLastStatus == Status::NOTHING)
			mLastStatus = Status::FAILURE;

		//If the last status is running then we will
		//suspend the tree for the observer so we
		//can return back to this point.
		if (mLastStatus != Status::FAILURE)
		{
			//Is there a need to check for Status::SUSPENDED?

			//If we did not succeed and we stopped we will be running
			// so we can suspend the tree and notify the observer
			if (mLastStatus != Status::SUCCESS)
				mLastStatus = Status::SUSPENDED;
			//at this point we either succeeded or failed
			else if (mLastStatus == Status::SUCCESS)
				mLastStatus = Status::SUCCESS;
			else
				mLastStatus = Status::FAILURE;
		}*/

		if (mLastStatus != Status::RUNNING)
			mLastStatus = Status::FAILURE;
	}
	
	Status GetStatus(){return mLastStatus;}
	
	void SetParent(Component* pParent){mParent = pParent;}
};
//------------------------------------------
}
#endif /* __COMPONENT_H */