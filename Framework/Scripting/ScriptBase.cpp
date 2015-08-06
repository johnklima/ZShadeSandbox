#include "ScriptBase.h"
//==================================================================================================================================
//==================================================================================================================================
ScriptBase::ScriptBase(D3D* d3d, GameDirectory2D* gd, lua_State* L, string name)
:   m_D3DSystem(d3d)
,   m_gd(gd)
,   m_script_name(name)
{
	this->L = L;

	//string main_path = m_gd->m_scripts_path;
	//main_path.append("\\");
	//main_path.append(m_script_name);
	//luaL_dofile(L, main_path.c_str());
}
//==================================================================================================================================
void ScriptBase::RunScript(string script)
{
	try
	{
		string main_path = m_gd->m_scripts_path;
		main_path.append("\\");
		main_path.append(script);
		
		luaL_loadfile(L, main_path.c_str());
		lua_pcall(L, 0, 0, 0);
		lua_getglobal(L, "main");
		lua_pcall(L, 0, 0, 0);
	}
	catch (std::exception const &e)
	{
		MessageBox(NULL, e.what(), "Lua Error", MB_OK);
	}
}
//==================================================================================================================================
void ScriptBase::RunScript(string script, string func_name, int args)
{
	try
	{
		string main_path = m_gd->m_scripts_path;
		main_path.append("\\");
		main_path.append(script);
		
		luaL_loadfile(L, main_path.c_str());
		lua_pcall(L, 0, 0, 0);
		lua_getglobal(L, func_name.c_str());
		lua_pcall(L, args, 0, 0);
	}
	catch (std::exception const &e)
	{
		MessageBox(NULL, e.what(), "Lua Error", MB_OK);
	}
}
//==================================================================================================================================
void ScriptBase::RunFunc(string func_name, int args, AISprite* spr, AISprite* target_spr)
{
	try
	{
		string main_path = m_gd->m_scripts_path;
		main_path.append("\\");
		main_path.append(m_script_name);

		//luaL_loadfile(L, main_path.c_str());
		luaL_dofile(L, main_path.c_str());
		lua_pcall(L, 0, 0, 0);
		lua_getglobal(L, func_name.c_str());
		if (spr != NULL) tolua_pushusertype(L, (void*)spr, "Sprite");
		if (target_spr != NULL) tolua_pushusertype(L, (void*)target_spr, "Sprite");
		lua_pcall(L, args, 0, 0);
	}
	catch (std::exception const &e)
	{
		MessageBox(NULL, e.what(), "Lua Error", MB_OK);
	}
	// any other exception as lua_error with no description
	catch (...){
		luaL_error(L, "Unknown error");
	}
}
//==================================================================================================================================
void ScriptBase::PushSprite(AISprite* spr)
{
	try
	{
		tolua_pushusertype(L, (void*)spr, "Sprite");
	}
	catch (std::exception const &e)
	{
		MessageBox(NULL, e.what(), "Lua Error", MB_OK);
	}
}
//==================================================================================================================================