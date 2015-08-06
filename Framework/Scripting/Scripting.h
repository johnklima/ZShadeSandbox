//==================================================================================================================================
// Scripting.h
//
// Main center and entry point for scripts
//==================================================================================================================================
// History
//
// -Created on 2/4/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __SCRIPTING_H
#define __SCRIPTING_H
//==================================================================================================================================
//==================================================================================================================================
#include <Windows.h>
#include <vector>
#include <sstream>
#include <exception>
#include "tolua.h"
#include "lua.hpp"
#include "lualib.h"
#include "lauxlib.h"
#include "Script.h"
#include "tolua_Sprite.h"
#include "GameDirectory.h"
using namespace std;
//==================================================================================================================================
class Scripting
{
	// Easy accessor for lua
	static D3D* m_D3DSystem;
	static EngineOptions* m_EngineOptions;
	static lua_State* L;
	static GameDirectory2D* m_gd;
	
	static map<string, Script*> m_scripts;
	
	static string m_ActiveWorldName;
	static string m_ActiveMapName;

	static Scripting* instance;

public:
	
	Scripting(D3D* d3d, GameDirectory2D* gd);
	~Scripting();
	
	static void Initialize();
	static void Destroy();
	static bool LocateScripts();
	static bool ScriptExists(string name);
	
	// Creates a brand new script
	static void LoadScript(string name);
	
	static Script* GetScript(string name);
	
	// static accessors
public:
	
	static void NewInstance(D3D* d3d, GameDirectory2D* gd);
	static Scripting* Instance();
	
	static void SetActiveWorldName(string world_name) { m_ActiveWorldName = world_name; }
	static string GetActiveWorldName() { return m_ActiveWorldName; }

	static void SetActiveMapName(string map_name) { m_ActiveMapName = map_name; }
	static string GetActiveMapName() { return m_ActiveMapName; }

	static lua_State* LuaState() { return L; }
	static GameDirectory2D* GetGameDirectory() { return m_gd; }
	static D3D* GetD3D() { return m_D3DSystem; }
	static EngineOptions* GetEngineOptions() { return m_EngineOptions; }

	static void SetGameDirectory(GameDirectory2D* gd) { m_gd = gd; }
	static void SetD3D(D3D* d3d) { m_D3DSystem = d3d; }
	static void SetEngineOptions(EngineOptions* eo) { m_EngineOptions = eo; }
};
//==================================================================================================================================
// name is a string
// func is the name of a lua function that follows 'int func_name(lua_State* L)'
#define LUA_REGISTER_FUNCTION(name, func) \
{ \
	if (Scripting::LuaState() != NULL) \
	{ \
		lua_register(Scripting::LuaState(), name, func); \
	} \
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__SCRIPTING_H