/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Struct BehaviorException created on 2/22/2012
\******************************************/
#ifndef __BEHAVIOREXCEPTION_H
#define __BEHAVIOREXCEPTION_H
#include <iostream>
#include <string>
using namespace std;
namespace LuaBehaviorTree {
//------------------------------------------
struct BehaviorException
{
	string comp;
	string whr;
	string err;

	BehaviorException(string component, string whr, string err)
	{
		comp = component;
		this->whr = whr;
		this->err = err;
	}

	void Print()
	{
		cout << "Error in: " << comp << "::Happened in: " << whr << "::<" << err << ">" << endl;
	}
};
//------------------------------------------
}
#endif /* __BEHAVIOREXCEPTION_H */