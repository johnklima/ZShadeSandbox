/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class Enumerator created on 2/23/2012
\******************************************/
#ifndef __ENUMERATOR_H
#define __ENUMERATOR_H
#include "Status.h"
#include <list>
using namespace std;
namespace LuaBehaviorTree {
//------------------------------------------
class Enumerator
{
	list<Status> _list;
	list<Status>::iterator it;
public:
	Enumerator()
	{}
	~Enumerator()
	{}

	//Move to the next value
	bool MoveNext()
	{
		if (!NextIsEnd())
		{
			if (!IsEnd())
			{
				it++;
				return true;
			}
			else
				return false;
		}
		else
		{
			//This means we are not at end so keep last value
			return true;
		}
	}

	//Return the current peice of data
	Status Yields()
	{
		return((*it));
	}

	//This function will initialize this iterator
	void Init()
	{
		it = _list.begin();
	}

	//This will assign data to this enumerator
	void Assign(Status data)
	{
		_list.push_back( data );
	}

	//See if we are at the end of the iterator
	bool IsEnd()
	{
		return((it == _list.end())?true:false);
	}

	//See if the list is empty
	bool IsEmpty()
	{
		return((_list.size() == 0)?true:false);
	}

	//See if next value is end of iterator
	bool NextIsEnd()
	{
		list<Status>::iterator iter = it;

		if (iter != _list.end())
			iter++;
		if (iter != _list.end())
			return false;
		else
			return true;
	}

	//Empty the contents of this enumerator
	void Empty()
	{
		if (!IsEmpty())
			_list.clear();
	}
};
//------------------------------------------
}
#endif /* __ENUMERATOR_H */