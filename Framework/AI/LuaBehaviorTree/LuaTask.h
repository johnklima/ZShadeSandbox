/******************************************\
 Programmer: Dustin Watson
 ------------------------------------------
 CBehaviorTree Created On: 2/13/2012
 ------------------------------------------
 Class LuaTask created on 2/16/2014
\******************************************/
#ifndef __LUATASK_H
#define __LUATASK_H
#include <string>
using std::string;
//#include "Task.h"
#include "Component.h"
#include "Status.h"
#include "tolua.h"
class AISprite;
namespace LuaBehaviorTree {
//------------------------------------------
class LuaTask : public Component
{
	lua_State* l;
	AISprite* m_sprite;
	string mLuaFile;
	string mLuaFuncName;
	string mName;
public:
	LuaTask()
		: Component(), mName("NoName"), l(0), m_sprite(0), mLuaFile(""), mLuaFuncName("")
	{}
	LuaTask(string pName, string pLuaFuncName)
		: Component(), mName(pName), mLuaFuncName(pLuaFuncName)
	{}
	LuaTask(string pName)
		: Component(), mName(pName)
	{}
	LuaTask(Component* pParent, string pName, string pLuaFuncName)
		 : Component(pParent), mName(pName), mLuaFuncName(pLuaFuncName)
	{}
	
	~LuaTask()
	{}

	void AddLuaAction(AISprite* spr, string pLuaFile, string pLuaFuncName)
	{
		m_sprite = spr;
		mLuaFile = pLuaFile;
		mLuaFuncName = pLuaFuncName;
		luaL_dofile(l, mLuaFile.c_str());
	}

	void AddLuaAction(AISprite* spr, string pName, string pLuaFile, string pLuaFuncName)
	{
		m_sprite = spr;
		mLuaFile = pLuaFile;
		mName = pName;
		mLuaFuncName = pLuaFuncName;
		luaL_dofile(l, mLuaFile.c_str());
	}

	void OnInitialize()
	{
		Component::OnInitialize();
	}
	
	//If this method is not overriden it will
	// return failure
	Status OnTick()
	{
		return Status::FAILURE;
	}

	void SetLuaState(lua_State* l)
	{
		this->l = l;
	}

	Status OnUpdate();
};
//------------------------------------------
}
#endif /* __LUATASK_H */