/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class ParallelSequence created on 2/15/2012
\******************************************/
#ifndef __PARALLELSEQUENCE_H
#define __PARALLELSEQUENCE_H
#include <list>
using std::list;
#include "Status.h"
#include "Sequence.h"
namespace LuaBehaviorTree {
//------------------------------------------
class ParallelSequence : public Sequence
{
public:
	ParallelSequence() : Sequence()
	{}
	ParallelSequence(list<Node*> pChildren)
		: Sequence(pChildren)
	{}
	~ParallelSequence()
	{}

	Status OnUpdate()
	{
		//This will run all child nodes in parallel
		list<Node*>::iterator iter = mChildren.begin();
		for (; iter != mChildren.end(); iter++)
		{
			Node* childNode = (*iter);
			childNode->OnStart();
			if (childNode->GetStatus() == Status::FAILURE)
				return Status::FAILURE;
		}
		return Status::SUCCESS;
	}
};
//------------------------------------------
}
#endif /* __PARALLELSEQUENCE_H */