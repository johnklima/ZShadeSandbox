//==================================================================================================================================
// ScriptBase.h
//
//==================================================================================================================================
// History
//
// -Created on 2/3/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __SCRIPTBASE_H
#define __SCRIPTBASE_H
//==================================================================================================================================
//==================================================================================================================================
#include "tolua.h"
#include "lua.hpp"
#include "lualib.h"
#include "lauxlib.h"
#include "D3D.h"
#include "AISprite.h"
//==================================================================================================================================
class ScriptBase
{
protected:
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	GameDirectory2D* m_gd;
	
	// Local accessor for the lua state
	lua_State* L;
	
	string m_script_name;

public:
	
	ScriptBase(D3D* d3d, GameDirectory2D* gd, lua_State* L, string name);
	
	// Runs a generic script
	void RunScript(string script);
	void RunScript(string script, string func_name, int args = 0);
	
	// Runs a function in this script
	void RunFunc(string func_name, int args = 0, AISprite* spr = NULL, AISprite* target_spr = NULL);
	
	// Push a Sprite to the script
	void PushSprite(AISprite* spr);
	
	// Accessor methods
public:
	
	string& ScriptName() { return m_script_name; }
	string ScriptName() const { return m_script_name; }
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__SCRIPTBASE_H