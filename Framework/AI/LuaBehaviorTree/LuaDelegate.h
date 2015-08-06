/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class LuaDelegate created on 2/16/2014
\******************************************/
#ifndef __LUADELEGATE_H
#define __LUADELEGATE_H
#include <string>
using namespace std;
//#include "luawrapper.hpp"
#include "tolua.h"
#include "BetterString.h"
class AISprite;
namespace LuaBehaviorTree {
//------------------------------------------
//template <class type>
class LuaDelegate
{
	lua_State* l;
	string mLuaFile;
	string mLuaFuncName;
	AISprite* mCallingSprite;
public:
	LuaDelegate(AISprite* callingSprite);
	LuaDelegate(string pLuaFile, string pLuaFuncName);
	~LuaDelegate();

	void AddFunction(string pLuaFile, string pLuaFuncName);

	void SetLuaState(lua_State* l);

	bool OnTick();
};
//------------------------------------------
}
#endif /* __LUADELEGATE_H */