/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class PrioritySelector created on 2/15/2012
\******************************************/
#ifndef __PRIORITYSELECTOR_H
#define __PRIORITYSELECTOR_H
#ifndef __SELECTOR_H
#include "Selector.h"
#endif
namespace LuaBehaviorTree {
//------------------------------------------
class PrioritySelector : public Selector
{
	Component* mSelection;
public:
	PrioritySelector() : Selector(), mSelection(NULL)
	{}
	PrioritySelector(list<Sequence*> pChildren)
		: mSelection(NULL), Selector(pChildren)
	{}
	~PrioritySelector()
	{}

	Status OnUpdate()
	{
		list<Sequence*>::iterator iter = mSequences.begin();
		for (; iter != mSequences.end(); iter++)
		{
			Sequence* sequence = (*iter);

			sequence->OnStart();

			while (sequence->OnTick() == Status::RUNNING)
			{
				mSelection = sequence;
				return Status::RUNNING;
			}

			mSelection = NULL;
			sequence->OnStop();

			if (sequence->GetStatus() == Status::SUCCESS)
				return Status::SUCCESS;
		}
		//Could not find a sequence to run (ran out of sequences)
		return Status::FAILURE;
	}
};
//------------------------------------------
}
#endif /* __PRIORITYSELECTOR_H */