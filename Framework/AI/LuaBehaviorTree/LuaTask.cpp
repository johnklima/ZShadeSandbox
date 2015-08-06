#include "LuaTask.h"
#include "AISprite.h"
//==================================================================================================================================
//==================================================================================================================================
Status LuaTask::OnUpdate()
{
	if (BetterString(mLuaFuncName) == "talk")
		m_sprite->Talk();
	else if (BetterString(mLuaFuncName) == "move")
		m_sprite->MoveScript();
	else if (BetterString(mLuaFuncName) == "die")
		m_sprite->Die();
	else if (BetterString(mLuaFuncName) == "hit")
		m_sprite->Hit();
	else if (BetterString(mLuaFuncName) == "touch")
		m_sprite->Touch();
	else if (BetterString(mLuaFuncName) == "attack")
		m_sprite->Attack();
	else if (BetterString(mLuaFuncName) == "target")
		m_sprite->Target();
	else// Call regular function
	{
		//luaL_dofile(l, mLuaFile.c_str());
		lua_pcall(l, 0, 0, 0);
		lua_getglobal(l, mLuaFuncName.c_str());
		//luaW_push(l, m_sprite);
		tolua_pushusertype(l, (void*)m_sprite, "Sprite");
		lua_pcall(l, 1, 0, 0);
	}
	return Status::RUNNING;
}
//==================================================================================================================================