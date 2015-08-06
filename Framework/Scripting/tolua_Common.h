//==================================================================================================================================
// tolua_Common.h
//
//==================================================================================================================================
// History
//
// -Created on 2/6/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __TOLUA_COMMON_H
#define __TOLUA_COMMON_H
//==================================================================================================================================
#include <string>
#include <vector>
#include "tolua.h"
#include "lua.hpp"
#include "lualib.h"
#include "lauxlib.h"
#include "AISprite.h"
using std::vector;
//==================================================================================================================================
//==================================================================================================================================
static void tolua_push_sprite(lua_State* L, AISprite* spr)
{
	tolua_pushusertype(L, (void*)spr, "Sprite");
}
//==================================================================================================================================
static bool tolua_ArgSafe_string(lua_State* L, int idx, string errFuncName)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;

	if (!tolua_isstring(L, idx, 0, &tolua_err))
	{
		BetterString str = "#ferror in function arg '";
		str += errFuncName;
		str += "'.";

		tolua_error(L, str.toString().c_str(), &tolua_err);

		return false;
	}
#endif

	return true;
}
//==================================================================================================================================
static bool tolua_ArgSafe_number(lua_State* L, int idx, string errFuncName)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;

	if (!tolua_isnumber(L, idx, 0, &tolua_err))
	{
		BetterString str = "#ferror in function arg '";
		str += errFuncName;
		str += "'.";

		tolua_error(L, str.toString().c_str(), &tolua_err);

		return false;
	}
#endif

	return true;
}
//==================================================================================================================================
static bool tolua_ArgSafe_bool(lua_State* L, int idx, string errFuncName)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;

	if (!tolua_isboolean(L, idx, 0, &tolua_err))
	{
		BetterString str = "#ferror in function arg '";
		str += errFuncName;
		str += "'.";

		tolua_error(L, str.toString().c_str(), &tolua_err);

		return false;
	}
#endif

	return true;
}
//==================================================================================================================================
static AISprite* tolua_Sprite_Safe(lua_State* L, string errFuncName)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;

	if (!tolua_isusertype(L, 1, "Sprite", 0, &tolua_err) || !tolua_isnoobj(L, 2, &tolua_err))
	{
		BetterString str = "#ferror in function '";
		str += errFuncName;
		str += "'.";

		tolua_error(L, str.toString().c_str(), &tolua_err);
		return 0;
	}
	else
#endif
	{
		AISprite* self = (AISprite*)tolua_tousertype(L, 1, 0);

#ifndef TOLUA_RELEASE
		if (!self)
		{
			BetterString str = "#invalid 'self' in function '";
			str += errFuncName;
			str += "'.";

			tolua_error(L, str.toString().c_str(), NULL);
			return 0;
		}
#endif
		{
			return self;
		}
	}

	return 0;
}
//==================================================================================================================================
static void tolua_push_float(lua_State* L, float number)
{
	tolua_pushnumber(L, (float)number);
}
//==================================================================================================================================
static float tolua_get_float(lua_State* L, int idx)
{
	return (float)tolua_tonumber(L, idx, 0);
}
//==================================================================================================================================
static void tolua_push_int(lua_State* L, int number)
{
	tolua_pushnumber(L, (int)number);
}
//==================================================================================================================================
static int tolua_get_int(lua_State* L, int idx)
{
	return (int)tolua_tonumber(L, idx, 0);
}
//==================================================================================================================================
static void tolua_push_string(lua_State* L, const char* str)
{
	tolua_pushstring(L, (const char*)str);
}
//==================================================================================================================================
static string tolua_get_string(lua_State* L, int idx)
{
	return (string)tolua_tostring(L, idx, 0);
}
//==================================================================================================================================
static void tolua_get_string_array(lua_State* L, int start_idx, int N, vector<string>& values)
{
	int idx = start_idx;

	for (int i = 0; i < N; i++)
	{
		values.push_back((string)tolua_tostring(L, idx++, 0));
	}
}
//==================================================================================================================================
static void tolua_push_bool(lua_State* L, bool val)
{
	tolua_pushboolean(L, val);
}
//==================================================================================================================================
static bool tolua_get_bool(lua_State* L, int idx)
{
	return (tolua_toboolean(L, idx, 0) != 0);
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__TOLUA_COMMON_H