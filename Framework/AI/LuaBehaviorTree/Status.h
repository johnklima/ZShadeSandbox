/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class Status created on 2/13/2012
\******************************************/
#ifndef __STATUS_H
#define __STATUS_H
namespace LuaBehaviorTree {
//------------------------------------------
struct Status
{
	Status()
		: mStatus(Status::NOTHING)
	{}
	Status(const int& t)
		: mStatus(t)
	{}

	int mStatus;

	friend bool operator == (Status& t, Status& other)
	{
		return(t.mStatus == other.mStatus)?true:false;
	}
	friend bool operator == (Status& t, const int other)
	{
		return(t.mStatus == other)?true:false;
	}

	friend bool operator != (Status& t, Status& other)
	{
		return(t.mStatus != other.mStatus)?true:false;
	}
	friend bool operator != (Status& t, const int other)
	{
		return(t.mStatus != other)?true:false;
	}

	Status& operator = (Status& other)
	{
		mStatus = other.mStatus;
		return(*this);
	}
	Status& operator = (const int& other)
	{
		mStatus = other;
		return(*this);
	}

	static const int NOTHING = 0;
	static const int RUNNING = 1;
	static const int SUCCESS = 2;
	static const int FAILURE = 3;
	/*
	When the behavior tree bails out of a node it will SUSPEND the tree
	at that node and tell the tree that we will use that node first when
	we return.
	*/
	static const int SUSPENDED = 4;
};
//------------------------------------------
}
#endif /* __STATUS_H */