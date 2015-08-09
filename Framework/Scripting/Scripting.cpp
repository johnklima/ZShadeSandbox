#include "Scripting.h"
#include "tolua_Sprite.h"
#include "tolua_Funcs.h"
#include "CGlobal.h"
//==================================================================================================================================
//==================================================================================================================================
Scripting* Scripting::instance = 0;
D3D* Scripting::m_D3DSystem = 0;
EngineOptions* Scripting::m_EngineOptions = 0;
lua_State* Scripting::L = 0;
GameDirectory2D* Scripting::m_gd = 0;
map<string, Script*> Scripting::m_scripts;
string Scripting::m_ActiveWorldName = "";
string Scripting::m_ActiveMapName = "";
//==================================================================================================================================
//==================================================================================================================================
Scripting::Scripting(D3D* d3d, GameDirectory2D* gd)
{
	Scripting::SetD3D(d3d);
	Scripting::SetGameDirectory(gd);
	Scripting::SetEngineOptions(d3d->GetEngineOptions());
	
	Initialize();
}
//==================================================================================================================================
Scripting::~Scripting()
{
	Destroy();
}
//==================================================================================================================================
void Scripting::NewInstance(D3D* d3d, GameDirectory2D* gd)
{
	instance = new Scripting(d3d, gd);
}
//==================================================================================================================================
Scripting* Scripting::Instance()
{
	return instance;
}
//==================================================================================================================================
void Scripting::RunMain()
{
	// Call the main entry point
	m_scripts["main"]->Main();
}
//==================================================================================================================================
void Scripting::Initialize()
{
	try
	{
		L = luaL_newstate();

		// Make sure the default scripts exist
		if (!ScriptExists("main.lua"))
			throw exception("Script main does not exist!!");
		if (!ScriptExists("start.lua"))
			throw exception("Script start does not exist!!");
		if (!ScriptExists("continue.lua"))
			throw exception("Script continue does not exist!!");
		if (!ScriptExists("options.lua"))
			throw exception("Script options does not exist!!");
		if (!ScriptExists("exit.lua"))
			throw exception("Script exit does not exist!!");

		// Loads all the scripts in the script directory
		if (!LocateScripts())
			throw exception("Scripts do not exist!!");
		
		// initalize all lua standard library functions
		luaL_openlibs(L);
		
		// initalize tolua
		tolua_open(L);
		
		// Make AISprite class accessible from LUA
		tolua_Sprite_open();
		
		// Make Various other functions accessible from LUA
		tolua_Funcs_open();
	}
	catch (std::exception const &e)
	{
		MessageBox(NULL, e.what(), "Lua Error", MB_OK);
	}
}
//==================================================================================================================================
void Scripting::Destroy()
{
	if (L)
	{
		lua_close(L);
	}
}
//==================================================================================================================================
Script* Scripting::GetScript(string name)
{
	return m_scripts[name];
}
//==================================================================================================================================
bool Scripting::LocateScripts()
{
	WIN32_FIND_DATA FindFileData;
	string script = m_gd->m_scripts_path;
	script.append("\\*.lua");
	HANDLE hFind = FindFirstFile(script.c_str(), &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	} 
	else 
	{
		do
		{
			string str = FindFileData.cFileName;
			LoadScript(str);
			//scripts.push_back(str);
		} while (FindNextFile(hFind, &FindFileData));
	}

	FindClose(hFind);

	return true;
}
//==================================================================================================================================
bool Scripting::ScriptExists(string name)
{
	//if (m_scripts[name] != NULL)
	string script = m_gd->m_scripts_path;
	script.append("\\");
	script.append(name);
	if (CGlobal::DoesFileExist(script))
		return true;
	else
		return false;
}
//==================================================================================================================================
void Scripting::LoadScript(string name)
{
	Script* s = new Script(Scripting::GetD3D(), Scripting::GetGameDirectory(), Scripting::LuaState(), name);
	BetterString str(name);
	int index = str.get_index('.');
	BetterString str_name = str.substring(0, index);
	m_scripts[str_name] = s;
}
//==================================================================================================================================