#include "tolua_Sprite.h"
#include "Scripting.h"
#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"
#include "LuaKeyMapper.h"
#include "GameState.h"
#include "Scripting.h"
#include "AudioSystem.h"
#include "tolua_Common.h"
#include "Environment2DMapHelper.h"
#include "EnvironmentMenuHelper.h"
#include "ZShadeMessageCenter.h"
#include "Environment.h"
#include "Environment2D.h"
//==================================================================================================================================
//==================================================================================================================================
/* Exported function */
TOLUA_API int tolua_Funcs_open();
//==================================================================================================================================
//==================================================================================================================================
static int tolua_load_music(lua_State* L)
{
	string str = tolua_get_string(L, 1);

	// Load the music
	if (AudioSystem::Instance() != 0)
		AudioSystem::Instance()->LoadMusic(str);

	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_play_music(lua_State* L)
{
	string str = tolua_get_string(L, 1);

	// Play the music
	if (AudioSystem::Instance() != 0)
		AudioSystem::Instance()->PlayMusicSegment(str);

	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_stop_music(lua_State* L)
{
	string str = tolua_get_string(L, 1);

	// Stop the music
	if (AudioSystem::Instance() != 0)
		AudioSystem::Instance()->StopMusicSegment(str);

	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_is_music_playing(lua_State* L)
{
	string str = tolua_get_string(L, 1);

	// Stop the music
	if (AudioSystem::Instance() != 0)
		tolua_push_bool(L, AudioSystem::Instance()->IsMusicSegmentPlaying(str));

	//return the number of return values; which is 1
	return 1;
}
//==================================================================================================================================
static int tolua_load_sound(lua_State* L)
{
	string str = tolua_get_string(L, 1);
	
	// Load the sound
	if (AudioSystem::Instance() != 0)
		AudioSystem::Instance()->LoadSound(str);

	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_play_sound(lua_State* L)
{
	string str = tolua_get_string(L, 1);
	
	// Play the sound
	if (AudioSystem::Instance() != 0)
		AudioSystem::Instance()->Play_Sound(str);

	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_win_print(lua_State* L)
{
	string str = tolua_get_string(L, 1);
	
	//MessageBox(NULL, str.c_str(), "Win Print", MB_OK);
	ZShadeMessageCenter::MsgBoxOK(NULL, str.c_str(), "Win Print");
	
	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_exit_game(lua_State* l)
{
	ExitProcess(3);
	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_set_state(lua_State* L)
{
	string str = tolua_get_string(L, 1);
	
	if (strcmp(str.c_str(), "MAINMENU") == 0)
	{
		GameState::GetInstance()->SetState(GS_MAINMENU);
	}

	if (strcmp(str.c_str(), "INGAME") == 0)
	{
		GameState::GetInstance()->SetState(GS_INGAME);
	}

	if (strcmp(str.c_str(), "INGAMEMENU") == 0)
	{
		GameState::GetInstance()->SetState(GS_INGAMEMENU);
	}

	if (strcmp(str.c_str(), "QUIT") == 0)
	{
		GameState::GetInstance()->SetState(GS_QUIT);
	}
	
	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_load_main_menu(lua_State* L)
{
	string str = tolua_get_string(L, 1);
	
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return 0;

	string path = env2D->GetGD2D()->m_xml_main_menu_path;

	EnvironmentMenuHelper::LoadAMenu(path, str);
	
	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_load_in_game_menu(lua_State* L)
{
	string str = tolua_get_string(L, 1);
	
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return 0;

	string path = env2D->GetGD2D()->m_xml_ingame_menu_path;

	EnvironmentMenuHelper::LoadAMenu(path, str);
	
	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_set_active_render_menu(lua_State* L)
{
	string str = tolua_get_string(L, 1);
	
	EnvironmentMenuHelper::SetActiveRenderedMenu(str);
	
	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_load_world(lua_State* L)
{
	string str = tolua_get_string(L, 1);
	string filename = tolua_get_string(L, 2);

	Environment2DMapHelper::LoadWorld(str, filename, Scripting::GetEngineOptions()->m_inEditor);
	
	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_load_map(lua_State* L)
{
	string world_name = tolua_get_string(L, 1);
	string map_name = tolua_get_string(L, 2);

	Environment2DMapHelper::LoadMapInWorld(world_name, map_name, Scripting::GetEngineOptions()->m_inEditor);
	
	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_set_render_map(lua_State* L)
{
	string world_name = tolua_get_string(L, 1);
	string map_name = tolua_get_string(L, 2);

	Environment2DMapHelper::SetActiveRenderedMap(world_name, map_name);

	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_key_pressed(lua_State* L)
{
	string text = tolua_get_string(L, 1);

	if (Keyboard::GetKeyboard()->IsKeyDown(LuaKeyMapper::GetInstance()->get_key(text)))
	{
		tolua_push_bool(L, true);
	}
	else
	{
		tolua_push_bool(L, false);
	}

	//return the number of return values; which is 1
	return 1;
}
//==================================================================================================================================
static int tolua_num_sprites_in_map(lua_State* L)
{
	tolua_push_int(L, Environment2DMapHelper::GetNumSpritesInMap());
	//return the number of return values; which is 1
	return 1;
}
//==================================================================================================================================
static int tolua_get_sprite(lua_State* L)
{
	if (tolua_ArgSafe_number(L, 1, "get_sprite_in_map"))
	{
		int SpriteID = tolua_get_int(L, 1);
		tolua_push_sprite(L, Environment2DMapHelper::GetSpriteInMap(SpriteID));
	}

	//return the number of return values; which is 0
	return 1;
}
//==================================================================================================================================
static int tolua_add_sprite(lua_State* L)
{
	AISprite* spr;
	if (spr = tolua_Sprite_Safe(L, "add_sprite_in_map"))
	{
		Environment2DMapHelper::AddSpriteInMap(spr);
	}

	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_remove_sprite(lua_State* L)
{
	AISprite* spr;
	if (spr = tolua_Sprite_Safe(L, "remove_sprite_in_map"))
	{
		Environment2DMapHelper::RemoveSpriteInMap(spr);
	}

	//return the number of return values; which is 0
	return 0;
}
//==================================================================================================================================
static int tolua_in_editor(lua_State* L)
{
	tolua_push_bool(L, Scripting::GetEngineOptions()->m_inEditor);
	//return the number of return values; which is 1
	return 1;
}
//==================================================================================================================================
TOLUA_API int tolua_Funcs_open()
{
	lua_State* L = Scripting::LuaState();
	
	if (L != NULL)
	{
		tolua_open(L);
		tolua_module(L, NULL, 1);
		tolua_beginmodule(L, NULL);
		tolua_function(L, "load_music", tolua_load_music);
		tolua_function(L, "play_music", tolua_play_music);
		tolua_function(L, "stop_music", tolua_stop_music);
		tolua_function(L, "is_music_playing", tolua_is_music_playing);
		tolua_function(L, "load_sound", tolua_load_sound);
		tolua_function(L, "play_sound", tolua_play_sound);
		tolua_function(L, "win_print", tolua_win_print);
		tolua_function(L, "exit_game", tolua_exit_game);
		tolua_function(L, "set_state", tolua_set_state);
		tolua_function(L, "load_main_menu", tolua_load_main_menu);
		tolua_function(L, "load_in_game_menu", tolua_load_in_game_menu);
		tolua_function(L, "set_active_render_menu", tolua_set_active_render_menu);
		tolua_function(L, "load_world", tolua_load_world);
		tolua_function(L, "load_map", tolua_load_map);
		tolua_function(L, "set_render_map", tolua_set_render_map);
		tolua_function(L, "key_pressed", tolua_key_pressed);
		tolua_function(L, "num_sprites_in_map", tolua_num_sprites_in_map);
		tolua_function(L, "get_sprite_in_map", tolua_get_sprite);
		tolua_function(L, "add_sprite_in_map", tolua_add_sprite);
		tolua_function(L, "remove_sprite_in_map", tolua_remove_sprite);
		tolua_function(L, "in_editor", tolua_in_editor);
		tolua_endmodule(L);
	}

	return 1;
}
//==================================================================================================================================
#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
TOLUA_API int toluaopen_Funcs()
{
	return tolua_Funcs_open();
}
#endif
//==================================================================================================================================