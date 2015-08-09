#include "tolua_Sprite.h"
#include "Scripting.h"
#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"
#include "AISprite.h"
//#include "Environment2D.h"
#include "tolua_Common.h"
//==================================================================================================================================
//==================================================================================================================================
/* Exported function */
TOLUA_API int tolua_Sprite_open();
//==================================================================================================================================
// function to release collected object via destructor
#ifdef __cplusplus
static int tolua_collect_Sprite(lua_State* L)
{
	AISprite* self = (AISprite*)tolua_tousertype(L, 1, 0);
	Mtolua_delete(self);
	return 0;
}
#endif
//==================================================================================================================================
// function to register type
static void tolua_reg_types(lua_State* L)
{
	tolua_usertype(L, "Sprite");
}
//==================================================================================================================================
// Gets the sprite from lua and checks if the first argument is a number
static AISprite* tolua_Sprite_Safe_number(lua_State* L, string errFuncName)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;

	if (!tolua_isusertype(L, 1, "Sprite", 0, &tolua_err) || !tolua_isnumber(L, 2, 0, &tolua_err) || !tolua_isnoobj(L, 3, &tolua_err))
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
// Gets the sprite from lua and checks if the first argument is a string
static AISprite* tolua_Sprite_Safe_string(lua_State* L, string errFuncName)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;

	if (!tolua_isusertype(L, 1, "Sprite", 0, &tolua_err) || !tolua_isstring(L, 2, 0, &tolua_err) || !tolua_isnoobj(L, 3, &tolua_err))
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
// Gets the sprite from lua and checks if 2..N argument is a string
static AISprite* tolua_Sprite_Safe_string_array(lua_State* L, int N, string errFuncName)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	
	int idx = 2;
	
	for (int i = 0; i < N; i++)
	{
		if (!tolua_isstring(L, idx++, 0, &tolua_err))
		{
			BetterString str = "#ferror in function '";
			str += errFuncName;
			str += "'.";
			
			tolua_error(L, str.toString().c_str(), &tolua_err);
			return 0;
		}
	}
	
	if (!tolua_isusertype(L, 1, "Sprite", 0, &tolua_err))
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
// Gets the sprite from lua and checks if the first argument is a bool
static AISprite* tolua_Sprite_Safe_bool(lua_State* L, string errFuncName)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;

	if (!tolua_isusertype(L, 1, "Sprite", 0, &tolua_err) || !tolua_isboolean(L, 2, 0, &tolua_err) || !tolua_isnoobj(L, 3, &tolua_err))
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
static AISprite* tolua_Sprite_Safe_sprite(lua_State* L, string errFuncName)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;

	if (!tolua_isusertype(L, 1, "Sprite", 0, &tolua_err) || !((AISprite*)tolua_tousertype(L, 2, &tolua_err)))
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
}
//==================================================================================================================================
// method: new of class AISprite
#ifndef TOLUA_DISABLE_tolua_new_Sprite
static int tolua_new_Sprite(lua_State* L)
{
	#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	
	if (!tolua_isusertable(L, 1, "Sprite", 0, &tolua_err) || !tolua_isnoobj(L , 2, &tolua_err))
	{
		tolua_error(L, "#ferror in function 'new'.", &tolua_err);
		return 0;
	}
	else
	#endif
	{
		AISprite* tolua_ret = (AISprite*)Mtolua_new((AISprite)(Scripting::GetD3D()));
		tolua_pushusertype(L, (void*)tolua_ret, "Sprite");
	}
	
	return 1;
}
#endif //#ifndef TOLUA_DISABLE_tolua_new_Sprite
//==================================================================================================================================
// method: new_local of class AISprite
#ifndef TOLUA_DISABLE_tolua_new_local_Sprite
static int tolua_new_local_Sprite(lua_State* L)
{
	#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	
	if (!tolua_isusertable(L, 1, "Sprite", 0, &tolua_err) || !tolua_isnoobj(L, 2, &tolua_err))
	{
		tolua_error(L,"#ferror in function 'new'.", &tolua_err);
		return 0;
	}
	else
	#endif
	{
		AISprite* tolua_ret = (AISprite*)Mtolua_new((AISprite)(Scripting::GetD3D()));
		tolua_pushusertype(L, (void*)tolua_ret, "Sprite");
		//tolua_register_gc(L, lua_gettop(L));
	}
	
	return 1;
}
#endif //#ifndef TOLUA_DISABLE_tolua_new_local_Sprite
//==================================================================================================================================
// method: delete of class AISprite
#ifndef TOLUA_DISABLE_tolua_delete_Sprite
static int tolua_delete_Sprite(lua_State* L)
{
	#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	
	if (!tolua_isusertype(L, 1, "Sprite", 0, &tolua_err) || !tolua_isnoobj(L, 2, &tolua_err))
	{
		tolua_error(L, "#ferror in function 'delete'.", &tolua_err);
		return 0;
	}
	else
	#endif
	{
		AISprite* self = (AISprite*)tolua_tousertype(L, 1, 0);
		
		#ifndef TOLUA_RELEASE
		if (!self)
			tolua_error(L, "invalid 'self' in function 'delete'", NULL);
		#endif
		
		Mtolua_delete(self);
	}
	
	return 0;
}
#endif //#ifndef TOLUA_DISABLE_tolua_delete_Sprite
//==================================================================================================================================
// method: setHealth of class AISprite
#ifndef TOLUA_DISABLE_tolua_setHealth_Sprite
static int tolua_setHealth_Sprite(lua_State* L)
{	
	AISprite* self = tolua_Sprite_Safe_number(L, "setHealth");
	
	if (self != 0)
	{
		self->HP() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //#ifndef TOLUA_DISABLE_tolua_setHealth_Sprite
//==================================================================================================================================
// method: getHealth of class AISprite
#ifndef TOLUA_DISABLE_tolua_getHealth_Sprite
static int tolua_getHealth_Sprite(lua_State* L)
{	
	AISprite* self = tolua_Sprite_Safe(L, "getHealth");
	
	if (self != 0)
	{
		tolua_push_int(L, self->HP());
	}

	return 1;
}
#endif //#ifndef TOLUA_DISABLE_tolua_getHealth_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setX_Sprite
static int tolua_setX_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setX");
	
	if (self != 0)
	{
		self->X() = tolua_get_float(L, 2);
	}
	
	return 0;
}
#endif //#ifndef TOLUA_DISABLE_tolua_setX_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getX_Sprite
static int tolua_getX_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getX");
	
	if (self != 0)
	{
		tolua_push_float(L, self->X());
	}
	
	return 1;
}
#endif //#ifndef TOLUA_DISABLE_tolua_getX_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setY_Sprite
static int tolua_setY_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setY");
	
	if (self != 0)
	{
		self->Y() = tolua_get_float(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setY_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getY_Sprite
static int tolua_getY_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getY");
	
	if (self != 0)
	{
		tolua_push_float(L, self->Y());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getY_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWidth_Sprite
static int tolua_setWidth_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWidth");
	
	if (self != 0)
	{
		self->Width() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWidth_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getWidth_Sprite
static int tolua_getWidth_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getWidth");
	
	if (self != 0)
	{
		tolua_push_int(L, self->Width());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getWidth_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setHeight_Sprite
static int tolua_setHeight_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setHeight");
	
	if (self != 0)
	{
		self->Height() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setHeight_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getHeight_Sprite
static int tolua_getHeight_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getHeight");
	
	if (self != 0)
	{
		tolua_push_int(L, self->Height());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getHeight_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_createTexture_Sprite
static int tolua_createTexture_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "createTexture");
	
	if (self != 0)
	{
		self->CreateTexture(
			Scripting::GetGameDirectory()->m_sprites_path,
			"Render",
			Scripting::GetEngineOptions()->m_screenWidth,
			Scripting::GetEngineOptions()->m_screenHeight);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_createTexture_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_addAnimation_Sprite
static int tolua_addAnimation_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "addAnimation");
	
	if (self != 0)
	{
		self->AddAnimation();
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_addAnimation_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setName_Sprite
static int tolua_setName_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_string(L, "setName");
	
	if (self != 0)
	{
		self->Name() = tolua_get_string(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setName_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getName_Sprite
static int tolua_getName_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getName");
	
	if (self != 0)
	{
		tolua_push_string(L, self->Name().c_str());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getName_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setVision_Sprite
static int tolua_setVision_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setVision");
	
	if (self != 0)
	{
		self->Vision() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setVision_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getVision_Sprite
static int tolua_getVision_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getVision");
	
	if (self != 0)
	{
		tolua_push_int(L, self->Vision());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getVision_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setBaseTextureName_Sprite
static int tolua_setBaseTextureName_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_string(L, "setBaseTextureName");
	
	if (self != 0)
	{
		self->BaseTextureFilename() = tolua_get_string(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setBaseTextureName_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getBaseTextureName_Sprite
static int tolua_getBaseTextureName_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getBaseTextureName");
	
	if (self != 0)
	{
		tolua_push_string(L, self->BaseTextureFilename().c_str());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getBaseTextureName_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setHard_Sprite
static int tolua_setHard_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setHard");
	
	if (self != 0)
	{
		self->IsHard() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setHard_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isHard_Sprite
static int tolua_isHard_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isHard");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->IsHard());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isHard_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setAnimationProfileName_Sprite
static int tolua_setAnimationProfileName_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_string(L, "setAnimationProfileName");
	
	if (self != 0)
	{
		self->AnimationProfileName() = tolua_get_string(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setAnimationProfileName_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getAnimationProfileName_Sprite
static int tolua_getAnimationProfileName_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getAnimationProfileName");
	
	if (self != 0)
	{
		tolua_push_string(L, self->AnimationProfileName().c_str());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getAnimationProfileName_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setHealthMax_Sprite
static int tolua_setHealthMax_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setHealthMax");

	if (self != 0)
	{
		self->HPMax() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setHealthMax_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getHealthMax_Sprite
static int tolua_getHealthMax_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getHealthMax");

	if (self != 0)
	{
		tolua_push_int(L, self->HPMax());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getHealthMax_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setStatic_Sprite
static int tolua_setStatic_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setStatic");
	
	if (self != 0)
	{
		//self->IsStatic() = tolua_get_bool(L, 2);
		self->EPhysicsType() = (tolua_get_bool(L, 2)) ? STATIC : DYNAMIC;
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setStatic_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isStatic_Sprite
static int tolua_isStatic_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isStatic");
	
	if (self != 0)
	{
		tolua_push_bool(L, (self->EPhysicsType() == STATIC) ? true : false);
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isStatic_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setMana_Sprite
static int tolua_setMana_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setMana");
	
	if (self != 0)
	{
		self->Mana() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setMana_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getMana_Sprite
static int tolua_getMana_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getMana");
	
	if (self != 0)
	{
		tolua_push_int(L, self->Mana());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getMana_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setGold_Sprite
static int tolua_setGold_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setGold");
	
	if (self != 0)
	{
		self->Gold() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setGold_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getGold_Sprite
static int tolua_getGold_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getGold");
	
	if (self != 0)
	{
		tolua_push_int(L, self->Gold());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getGold_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setStrength_Sprite
static int tolua_setStrength_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setStrength");
	
	if (self != 0)
	{
		self->Strength() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setStrength_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getStrength_Sprite
static int tolua_getStrength_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getStrength");
	
	if (self != 0)
	{
		tolua_push_int(L, self->Strength());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getStrength_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setSpeed_Sprite
static int tolua_setSpeed_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setSpeed");
	
	if (self != 0)
	{
		self->Speed() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setSpeed_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getSpeed_Sprite
static int tolua_getSpeed_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getSpeed");
	
	if (self != 0)
	{
		tolua_push_int(L, self->Speed());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getSpeed_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setDefense_Sprite
static int tolua_setDefense_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setDefense");
	
	if (self != 0)
	{
		self->Defense() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setDefense_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getDefense_Sprite
static int tolua_getDefense_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getDefense");
	
	if (self != 0)
	{
		tolua_push_int(L, self->Defense());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getDefense_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setExperience_Sprite
static int tolua_setExperience_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setExperience");
	
	if (self != 0)
	{
		self->Experience() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setExperience_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getExperience_Sprite
static int tolua_getExperience_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getExperience");
	
	if (self != 0)
	{
		tolua_push_int(L, self->Experience());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getExperience_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setLevel_Sprite
static int tolua_setLevel_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setLevel");
	
	if (self != 0)
	{
		self->Level() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setLevel_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getLevel_Sprite
static int tolua_getLevel_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getLevel");
	
	if (self != 0)
	{
		tolua_push_int(L, self->Level());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getLevel_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setDamageAmount_Sprite
static int tolua_setDamageAmount_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setDamageAmount");
	
	if (self != 0)
	{
		self->DamageAmount() = tolua_get_float(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setDamageAmount_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getDamageAmount_Sprite
static int tolua_getDamageAmount_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getDamageAmount");
		
	if (self != 0)
	{
		tolua_push_float(L, self->DamageAmount());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getDamageAmount_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setNoHit_Sprite
static int tolua_setNoHit_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setNoHit");
		
	if (self != 0)
	{
		self->IsNoHit() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setNoHit_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getNoHit_Sprite
static int tolua_getNoHit_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getNoHit");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->IsNoHit());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getNoHit_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setTouch_Sprite
static int tolua_setTouch_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setTouch");
		
	if (self != 0)
	{
		self->IsTouch() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setTouch_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getTouch_Sprite
static int tolua_getTouch_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getTouch");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->IsTouch());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getTouch_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setScriptName_Sprite
static int tolua_setScriptName_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_string(L, "setScriptName");
	
	if (self != 0)
	{
		self->ScriptName() = tolua_get_string(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setScriptName_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getScriptName_Sprite
static int tolua_getScriptName_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getScriptName");
	
	if (self != 0)
	{
		tolua_push_string(L, self->ScriptName().c_str());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getScriptName_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanTeleport_Sprite
static int tolua_setCanTeleport_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanTeleport");
		
	if (self != 0)
	{
		self->CanTeleport() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanTeleport_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getCanTeleport_Sprite
static int tolua_getCanTeleport_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getCanTeleport");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->CanTeleport());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getCanTeleport_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setTeleportMapName_Sprite
static int tolua_setTeleportMapName_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_string(L, "setTeleportMapName");
	
	if (self != 0)
	{
		self->TeleportMapName() = tolua_get_string(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setTeleportMapName_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getTeleportMapName_Sprite
static int tolua_getTeleportMapName_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getTeleportMapName");
	
	if (self != 0)
	{
		tolua_push_string(L, self->TeleportMapName().c_str());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getTeleportMapName_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setTeleportX_Sprite
static int tolua_setTeleportX_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setTeleportX");
	
	if (self != 0)
	{
		self->TeleportX() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setTeleportX_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getTeleportX_Sprite
static int tolua_getTeleportX_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getTeleportX");
	
	if (self != 0)
	{
		tolua_push_int(L, self->TeleportX());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getTeleportX_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setTeleportY_Sprite
static int tolua_setTeleportY_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setTeleportY");
	
	if (self != 0)
	{
		self->TeleportY() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setTeleportY_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getTeleportY_Sprite
static int tolua_getTeleportY_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getTeleportY");
	
	if (self != 0)
	{
		tolua_push_int(L, self->TeleportY());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getTeleportY_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setBackgroundSprite_Sprite
static int tolua_setBackgroundSprite_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setBackgroundSprite");
		
	if (self != 0)
	{
		self->IsBackgroundSprite() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setBackgroundSprite_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isBackgroundSprite_Sprite
static int tolua_isBackgroundSprite_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isBackgroundSprite");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->IsBackgroundSprite());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isBackgroundSprite_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setEntitySprite_Sprite
static int tolua_setEntitySprite_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setEntitySprite");
		
	if (self != 0)
	{
		self->IsEntitySprite() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setEntitySprite_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isEntitySprite_Sprite
static int tolua_isEntitySprite_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isEntitySprite");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->IsEntitySprite());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isEntitySprite_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setInvisibleSprite_Sprite
static int tolua_setInvisibleSprite_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setInvisibleSprite");
		
	if (self != 0)
	{
		self->IsInvisibleSprite() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setInvisibleSprite_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isInvisibleSprite_Sprite
static int tolua_isInvisibleSprite_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isInvisibleSprite");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->IsInvisibleSprite());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isInvisibleSprite_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setDefaultSequenceID_Sprite
static int tolua_setDefaultSequenceID_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setDefaultSequenceID");
	
	if (self != 0)
	{
		self->DefaultSequence() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setDefaultSequenceID_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getDefaultSequenceID_Sprite
static int tolua_getDefaultSequenceID_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getDefaultSequenceID");
	
	if (self != 0)
	{
		tolua_push_int(L, self->DefaultSequence());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getDefaultSequenceID_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setAnimationSpeed_Sprite
static int tolua_setAnimationSpeed_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setAnimationSpeed");
	
	if (self != 0)
	{
		self->AnimationSpeed() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setAnimationSpeed_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getAnimationSpeed_Sprite
static int tolua_getAnimationSpeed_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getAnimationSpeed");
	
	if (self != 0)
	{
		tolua_push_int(L, self->AnimationSpeed());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getAnimationSpeed_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setMoving_Sprite
static int tolua_setMoving_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setMoving");
		
	if (self != 0)
	{
		self->Moving() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setMoving_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isMoving_Sprite
static int tolua_isMoving_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isMoving");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->Moving());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isMoving_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setThinking_Sprite
static int tolua_setThinking_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setThinking");
		
	if (self != 0)
	{
		self->Thinking() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setThinking_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isThinking_Sprite
static int tolua_isThinking_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isThinking");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->Thinking());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isThinking_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setTalking_Sprite
static int tolua_setTalking_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setTalking");
		
	if (self != 0)
	{
		self->Talking() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setTalking_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isTalking_Sprite
static int tolua_isTalking_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isTalking");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->Talking());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isTalking_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setDying_Sprite
static int tolua_setDying_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setDying");
		
	if (self != 0)
	{
		self->IsDying() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setDying_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isDying_Sprite
static int tolua_isDying_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isDying");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->IsDying());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isDying_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setHit_Sprite
static int tolua_setHit_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setHit");
		
	if (self != 0)
	{
		self->IsHit() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setHit_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isHit_Sprite
static int tolua_isHit_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isHit");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->IsHit());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isHit_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setTouched_Sprite
static int tolua_setTouched_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setTouched");
		
	if (self != 0)
	{
		self->IsTouched() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setTouched_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isTouched_Sprite
static int tolua_isTouched_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isTouched");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->IsTouched());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isTouched_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setAttacking_Sprite
static int tolua_setAttacking_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setAttacking");
		
	if (self != 0)
	{
		self->Attacking() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setAttacking_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isAttacking_Sprite
static int tolua_isAttacking_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isAttacking");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->Attacking());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isAttacking_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setTargeted_Sprite
static int tolua_setTargeted_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setTargeted");
		
	if (self != 0)
	{
		self->Targeted() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setTargeted_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isTargeted_Sprite
static int tolua_isTargeted_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isTargeted");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->Targeted());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isTargeted_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setAnimationSequence_Sprite
static int tolua_setAnimationSequence_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setAnimationSequence");
	
	if (self != 0)
	{
		self->SetSequence(tolua_get_int(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setAnimationSequence_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setDirectionMoved_Sprite
static int tolua_setDirectionMoved_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setDirectionMoved");
	
	if (self != 0)
	{
		self->DirectionMoved() = tolua_get_int(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setDirectionMoved_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getDirectionMoved_Sprite
static int tolua_getDirectionMoved_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getDirectionMoved");
	
	if (self != 0)
	{
		tolua_push_int(L, self->DirectionMoved());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getDirectionMoved_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_currentSequenceExists_Sprite
static int tolua_currentSequenceExists_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "currentSequenceExists");
	
	if (self != 0)
	{
		if (self->CurrentSequenceExists() == NULL)
			tolua_push_bool(L, false);
		else
			tolua_push_bool(L, true);
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_currentSequenceExists_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isLastFrame_Sprite
static int tolua_isLastFrame_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isLastFrame");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->IsCurrentSequenceOnLastFrame());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isLastFrame_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setNotOnLastFrame_Sprite
static int tolua_setNotOnLastFrame_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "setHealthMax");
	
	if (self != 0)
	{
		self->SetCurrentSequenceNotOnLastFrame();
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setNotOnLastFrame_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isMovingUp_Sprite
static int tolua_isMovingUp_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isMovingUp");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->MoveSpriteUp());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isMovingUp_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_moveUp_Sprite
static int tolua_moveUp_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "moveUp");
		
	if (self != 0)
	{
		self->SetMoveSpriteUp(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_moveUp_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isMovingDown_Sprite
static int tolua_isMovingDown_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isMovingDown");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->MoveSpriteDown());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isMovingDown_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_moveDown_Sprite
static int tolua_moveDown_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "moveDown");
		
	if (self != 0)
	{
		self->SetMoveSpriteDown(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_moveDown_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isMovingLeft_Sprite
static int tolua_isMovingLeft_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isMovingLeft");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->MoveSpriteLeft());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isMovingLeft_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_moveLeft_Sprite
static int tolua_moveLeft_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "moveLeft");
		
	if (self != 0)
	{
		self->SetMoveSpriteLeft(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_moveLeft_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_isMovingRight_Sprite
static int tolua_isMovingRight_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isMovingRight");
		
	if (self != 0)
	{
		tolua_push_bool(L, self->MoveSpriteRight());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_isMovingRight_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_moveRight_Sprite
static int tolua_moveRight_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "moveRight");
		
	if (self != 0)
	{
		self->SetMoveSpriteRight(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_moveRight_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_addDefaultBehavior_Sprite
static int tolua_addDefaultBehavior_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_string(L, "addDefaultBehavior");
	
	if (self != 0)
	{
		self->AddDefaultBehavior(tolua_get_string(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_addDefaultBehavior_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_addBehaviorSequence_Sprite
static int tolua_addBehaviorSequence_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_string_array(L, 4, "addBehaviorSequence");
	
	if (self != 0)
	{
		vector<string> values;
		
		// Look for a string from 2..5
		tolua_get_string_array(L, 2, 4, values);
		
		if (values.size() == 4)
		{
			// 4 inputs, behavior, sequence, actionName, conditionName
			self->AddBehaviorSequence(values[0], values[1], values[2], values[3]);
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_addBehaviorSequence_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_registerBehavior_Sprite
static int tolua_registerBehavior_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_string(L, "registerBehavior");
	
	if (self != 0)
	{
		self->RegisterBehavior(tolua_get_string(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_registerBehavior_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_createBehaviorTree_Sprite
static int tolua_createBehaviorTree_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "createBehaviorTree");
	
	if (self != 0)
	{
		self->CreateTree();
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_createBehaviorTree_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canRunBehavior_Sprite
static int tolua_canRunBehavior_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "canRunBehavior");
	
	if (self != 0)
	{
		self->SetRunBehavior(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_canRunBehavior_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setRunBehaviorName_Sprite
static int tolua_setRunBehaviorName_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_string(L, "setRunBehaviorName");
	
	if (self != 0)
	{
		self->SetBehaviorName(tolua_get_string(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setRunBehaviorName_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canRunDefaultBehavior_Sprite
static int tolua_canRunDefaultBehavior_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "canRunDefaultBehavior");
	
	if (self != 0)
	{
		self->SetRunBehaviorDefault(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_canRunDefaultBehavior_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_say1_Sprite
static int tolua_say1_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_string(L, "say1");
	
	if (self != 0)
	{
		self->say(tolua_get_string(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_say1_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_say2_Sprite
static int tolua_say2_Sprite(lua_State* L)
{
	// text, font_size
	AISprite* self = tolua_Sprite_Safe(L, "say2");
	
	if (tolua_ArgSafe_string(L, 2, "say2") &&
		tolua_ArgSafe_number(L, 3, "say2"))
	{
		if (self != 0)
		{
			self->say(tolua_get_string(L, 2), tolua_get_int(L, 3));
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_say2_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_say3_Sprite
static int tolua_say3_Sprite(lua_State* L)
{
	// text, x, y, font_size
	AISprite* self = tolua_Sprite_Safe(L, "say3");
	
	if (tolua_ArgSafe_string(L, 2, "say3") &&
		tolua_ArgSafe_number(L, 3, "say3") &&
		tolua_ArgSafe_number(L, 4, "say3") &&
		tolua_ArgSafe_number(L, 5, "say3"))
	{
		if (self != 0)
		{
			self->say(tolua_get_string(L, 2), tolua_get_int(L, 3), tolua_get_int(L, 4), tolua_get_int(L, 5));
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_say3_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_say4_Sprite
static int tolua_say4_Sprite(lua_State* L)
{
	// text, r, g, b, font_size
	AISprite* self = tolua_Sprite_Safe(L, "say4");
	
	if (tolua_ArgSafe_string(L, 2, "say4") &&
		tolua_ArgSafe_number(L, 3, "say4") &&
		tolua_ArgSafe_number(L, 4, "say4") &&
		tolua_ArgSafe_number(L, 5, "say4") &&
		tolua_ArgSafe_number(L, 6, "say4"))
	{
		if (self != 0)
		{
			self->say(tolua_get_string(L, 2),
				tolua_get_int(L, 3), tolua_get_int(L, 4), tolua_get_int(L, 5), tolua_get_int(L, 6));
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_say4_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_say5_Sprite
static int tolua_say5_Sprite(lua_State* L)
{
	// text, x, y, r, g, b, font_size
	AISprite* self = tolua_Sprite_Safe(L, "say5");
	
	if (tolua_ArgSafe_string(L, 2, "say5") &&
		tolua_ArgSafe_number(L, 3, "say5") &&
		tolua_ArgSafe_number(L, 4, "say5") &&
		tolua_ArgSafe_number(L, 5, "say5") &&
		tolua_ArgSafe_number(L, 6, "say5") &&
		tolua_ArgSafe_number(L, 7, "say5") &&
		tolua_ArgSafe_number(L, 8, "say5"))
	{
		if (self != 0)
		{
			self->say(tolua_get_string(L, 2),
				tolua_get_int(L, 3), tolua_get_int(L, 4),
				tolua_get_int(L, 5), tolua_get_int(L, 6), tolua_get_int(L, 7),
				tolua_get_int(L, 8));
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_say5_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_sayStop1_Sprite
static int tolua_sayStop1_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_string(L, "sayStop1");
	
	if (self != 0)
	{
		self->say_stop(tolua_get_string(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_sayStop1_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_sayStop2_Sprite
static int tolua_sayStop2_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "sayStop2");
	
	if (tolua_ArgSafe_string(L, 2, "sayStop2") &&
		tolua_ArgSafe_number(L, 3, "sayStop2"))
	{
		if (self != 0)
		{
			self->say_stop(tolua_get_string(L, 2), tolua_get_int(L, 3));
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_sayStop2_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_sayStop3_Sprite
static int tolua_sayStop3_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "sayStop3");
	
	if (tolua_ArgSafe_string(L, 2, "sayStop3") &&
		tolua_ArgSafe_number(L, 3, "sayStop3") &&
		tolua_ArgSafe_number(L, 4, "sayStop3") &&
		tolua_ArgSafe_number(L, 5, "sayStop3"))
	{
		if (self != 0)
		{
			self->say_stop(tolua_get_string(L, 2), tolua_get_int(L, 3), tolua_get_int(L, 4), tolua_get_int(L, 5));
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_sayStop3_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_sayStop4_Sprite
static int tolua_sayStop4_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "sayStop4");
	
	if (tolua_ArgSafe_string(L, 2, "sayStop4") &&
		tolua_ArgSafe_number(L, 3, "sayStop4") &&
		tolua_ArgSafe_number(L, 4, "sayStop4") &&
		tolua_ArgSafe_number(L, 5, "sayStop4") &&
		tolua_ArgSafe_number(L, 6, "sayStop4"))
	{
		if (self != 0)
		{
			self->say_stop(tolua_get_string(L, 2),
				tolua_get_int(L, 3), tolua_get_int(L, 4), tolua_get_int(L, 5), tolua_get_int(L, 6));
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_sayStop4_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_sayStop5_Sprite
static int tolua_sayStop5_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "sayStop5");
	
	if (tolua_ArgSafe_string(L, 2, "sayStop5") &&
		tolua_ArgSafe_number(L, 3, "sayStop5") &&
		tolua_ArgSafe_number(L, 4, "sayStop5") &&
		tolua_ArgSafe_number(L, 5, "sayStop5") &&
		tolua_ArgSafe_number(L, 6, "sayStop5") &&
		tolua_ArgSafe_number(L, 7, "sayStop5") &&
		tolua_ArgSafe_number(L, 8, "sayStop5"))
	{
		if (self != 0)
		{
			self->say_stop(tolua_get_string(L, 2),
				tolua_get_int(L, 3), tolua_get_int(L, 4),
				tolua_get_int(L, 5), tolua_get_int(L, 6), tolua_get_int(L, 7),
				tolua_get_int(L, 8));
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_sayStop5_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_collideMsg_Sprite
static int tolua_collideMsg_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "collideMsg");
	
	if (self != 0)
	{
		tolua_push_string(L, self->CollideMsg().c_str());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_collideMsg_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_collidedSpriteInfo_Sprite
static int tolua_collidedSpriteInfo_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "collidedSpriteInfo");
	
	if (self != 0)
	{
		if (self->GetCollidedSprite() != NULL)
			tolua_push_string(L, self->GetCollidedSprite()->CollideMsg().c_str());
		else
			tolua_push_string(L, "");
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_collidedSpriteInfo_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getMaxSpeed_Sprite
static int tolua_getMaxSpeed_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getMaxSpeed");
	
	if (self != 0)
	{
		tolua_push_float(L, self->MaxSpeed());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getMaxSpeed_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setMaxSpeed_Sprite
static int tolua_setMaxSpeed_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setMaxSpeed");
	
	if (self != 0)
	{
		self->MaxSpeed() = tolua_get_float(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setMaxSpeed_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getInventoryMessage_Sprite
static int tolua_getInventoryMessage_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "getInventoryMessage");
	
	if (self != 0)
	{
		tolua_push_string(L, self->InventoryMessage().c_str());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getInventoryMessage_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setInventoryMessage_Sprite
static int tolua_setInventoryMessage_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setInventoryMessage");
	
	if (self != 0)
	{
		self->InventoryMessage() = tolua_get_string(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setInventoryMessage_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_getInventoryMessage_Sprite
static int tolua_isAlwaysSeenByPlayer_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "isAlwaysSeenByPlayer");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->AlwaysSeenByPlayer());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_getInventoryMessage_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setInventoryMessage_Sprite
static int tolua_setAlwaysSeenByPlayer_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setAlwaysSeenByPlayer");
	
	if (self != 0)
	{
		self->AlwaysSeenByPlayer() = tolua_get_bool(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setInventoryMessage_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_createWaypointSystem_Sprite
static int tolua_createWaypointSystem_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "createWaypointSystem");
	
	if (self != 0)
	{
		self->CreateWaypointSystem();
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_createWaypointSystem_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_waypointSystemSize_Sprite
static int tolua_waypointSystemSize_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "waypointSystemSize");
	
	if (self != 0)
	{
		tolua_push_int(L, self->WaypointSystemSize());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_waypointSystemSize_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_waypointSystemEmpty_Sprite
static int tolua_waypointSystemEmpty_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "waypointSystemEmpty");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->WaypointSystemEmpty());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_waypointSystemEmpty_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_clearWaypoints_Sprite
static int tolua_clearWaypoints_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "clearWaypoints");
	
	if (self != 0)
	{
		self->ClearWaypoints();
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_clearWaypoints_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_removeWaypoint_Sprite
static int tolua_removeWaypoint_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "removeWaypoint");
	
	if (tolua_ArgSafe_number(L, 2, "removeWaypoint")
	&&	tolua_ArgSafe_number(L, 3, "removeWaypoint")
	&&	tolua_ArgSafe_number(L, 4, "removeWaypoint"))
	{
		if (self != 0)
		{
			self->RemoveWaypoint(ZShadeSandboxMath::XMMath3(tolua_get_float(L, 2), tolua_get_float(L, 3), tolua_get_float(L, 4)));
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_removeWaypoint_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_addWaypoint_Sprite
static int tolua_addWaypoint_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "addWaypoint");
	
	if (tolua_ArgSafe_number(L, 2, "addWaypoint")
	&&	tolua_ArgSafe_number(L, 3, "addWaypoint")
	&&	tolua_ArgSafe_number(L, 4, "addWaypoint"))
	{
		if (self != 0)
		{
			self->AddWaypoint(ZShadeSandboxMath::XMMath3(tolua_get_float(L, 2), tolua_get_float(L, 3), tolua_get_float(L, 4)));
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_addWaypoint_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_currentWaypointX_Sprite
static int tolua_currentWaypointX_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "currentWaypointX");
	
	if (self != 0)
	{
		tolua_push_float(L, self->CurrentWaypointX());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_currentWaypointX_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_currentWaypointY_Sprite
static int tolua_currentWaypointY_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "currentWaypointY");
	
	if (self != 0)
	{
		tolua_push_float(L, self->CurrentWaypointY());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_currentWaypointY_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_currentWaypointZ_Sprite
static int tolua_currentWaypointZ_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "currentWaypointZ");
	
	if (self != 0)
	{
		tolua_push_float(L, self->CurrentWaypointZ());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_currentWaypointZ_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setNextWaypoint_Sprite
static int tolua_setNextWaypoint_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "setNextWaypoint");
	
	if (self != 0)
	{
		self->SetNextWaypoint();
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setNextWaypoint_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_waypointSystemFinished_Sprite
static int tolua_waypointSystemFinished_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "waypointSystemFinished");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->WaypointSystemFinished());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_waypointSystemFinished_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWaypointSystemCanLoop_Sprite
static int tolua_setWaypointSystemCanLoop_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setWaypointSystemCanLoop");
	
	if (self != 0)
	{
		self->SetWaypointSystemCanLoop(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWaypointSystemCanLoop_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_waypointSystemCanLoop_Sprite
static int tolua_waypointSystemCanLoop_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "waypointSystemCanLoop");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->WaypointSystemCanLoop());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_waypointSystemCanLoop_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_currentWaypointIndex_Sprite
static int tolua_currentWaypointIndex_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "currentWaypointIndex");
	
	if (self != 0)
	{
		tolua_push_int(L, self->CurrentWaypointIndex());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_currentWaypointIndex_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWanderingRadius_Sprite
static int tolua_setWanderingRadius_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWanderingRadius");
	
	if (self != 0)
	{
		self->SetWanderingRadius(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWanderingRadius_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWanderingDistance_Sprite
static int tolua_setWanderingDistance_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWanderingDistance");
	
	if (self != 0)
	{
		self->SetWanderingDistance(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWanderingDistance_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWanderingJitter_Sprite
static int tolua_setWanderingJitter_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWanderingJitter");
	
	if (self != 0)
	{
		self->SetWanderingJitter(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWanderingJitter_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setMinDetectionBoxLength_Sprite
static int tolua_setMinDetectionBoxLength_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setMinDetectionBoxLength");
	
	if (self != 0)
	{
		self->SetMinDetectionBoxLength(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setMinDetectionBoxLength_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWaypointSeekDist_Sprite
static int tolua_setWaypointSeekDist_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWaypointSeekDist");
	
	if (self != 0)
	{
		self->SetWaypointSeekDist(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWaypointSeekDist_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setMaxFleeDistance_Sprite
static int tolua_setMaxFleeDistance_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setMaxFleeDistance");
	
	if (self != 0)
	{
		self->SetMaxFleeDistance(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setMaxFleeDistance_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightSeek_Sprite
static int tolua_setWeightSeek_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightSeek");
	
	if (self != 0)
	{
		self->SetWeightSeek(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightSeek_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightFlee_Sprite
static int tolua_setWeightFlee_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightFlee");
	
	if (self != 0)
	{
		self->SetWeightFlee(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightFlee_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightArrive_Sprite
static int tolua_setWeightArrive_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightFlee");
	
	if (self != 0)
	{
		self->SetWeightArrive(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightArrive_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightSpriteAvoidance_Sprite
static int tolua_setWeightSpriteAvoidance_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightSpriteAvoidance");
	
	if (self != 0)
	{
		self->SetWeightSpriteAvoidance(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightSpriteAvoidance_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightSeparation_Sprite
static int tolua_setWeightSeparation_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightSeparation");
	
	if (self != 0)
	{
		self->SetWeightSeparation(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightSeparation_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightAlignment_Sprite
static int tolua_setWeightAlignment_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightAlignment");
	
	if (self != 0)
	{
		self->SetWeightAlignment(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightAlignment_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightCohesion_Sprite
static int tolua_setWeightCohesion_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightCohesion");
	
	if (self != 0)
	{
		self->SetWeightCohesion(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightCohesion_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightWander_Sprite
static int tolua_setWeightWander_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightWander");
	
	if (self != 0)
	{
		self->SetWeightWander(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightWander_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightFollowPath_Sprite
static int tolua_setWeightFollowPath_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightFollowPath");
	
	if (self != 0)
	{
		self->SetWeightFollowPath(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightFollowPath_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightPursueTarget_Sprite
static int tolua_setWeightPursueTarget_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightPursueTarget");
	
	if (self != 0)
	{
		self->SetWeightPursueTarget(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightPursueTarget_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightEvadeTarget_Sprite
static int tolua_setWeightEvadeTarget_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightEvadeTarget");
	
	if (self != 0)
	{
		self->SetWeightEvadeTarget(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightEvadeTarget_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightInterpose_Sprite
static int tolua_setWeightInterpose_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightInterpose");
	
	if (self != 0)
	{
		self->SetWeightInterpose(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightInterpose_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightHide_Sprite
static int tolua_setWeightHide_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightHide");
	
	if (self != 0)
	{
		self->SetWeightHide(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightHide_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setWeightOffsetPursuit_Sprite
static int tolua_setWeightOffsetPursuit_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setWeightOffsetPursuit");
	
	if (self != 0)
	{
		self->SetWeightOffsetPursuit(tolua_get_float(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setWeightOffsetPursuit_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanSeek_Sprite
static int tolua_setCanSeek_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanSeek");
	
	if (self != 0)
	{
		self->SetCanSeek(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanSeek_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanFlee_Sprite
static int tolua_setCanFlee_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanFlee");
	
	if (self != 0)
	{
		self->SetCanFlee(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanFlee_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanArrive_Sprite
static int tolua_setCanArrive_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanArrive");
	
	if (self != 0)
	{
		self->SetCanArrive(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanArrive_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanAvoidSprites_Sprite
static int tolua_setCanAvoidSprites_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanAvoidSprites");
	
	if (self != 0)
	{
		self->SetCanAvoidSprites(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanAvoidSprites_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanFlockSeparation_Sprite
static int tolua_setCanFlockSeparation_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanFlockSeparation");
	
	if (self != 0)
	{
		self->SetCanFlockSeparation(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanFlockSeparation_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanFlockAlignment_Sprite
static int tolua_setCanFlockAlignment_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanFlockAlignment");
	
	if (self != 0)
	{
		self->SetCanFlockAlignment(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanFlockAlignment_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanFlockCohesion_Sprite
static int tolua_setCanFlockCohesion_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanFlockCohesion");
	
	if (self != 0)
	{
		self->SetCanFlockCohesion(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanFlockCohesion_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanWander_Sprite
static int tolua_setCanWander_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanWander");
	
	if (self != 0)
	{
		self->SetCanWander(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanWander_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanFollowPath_Sprite
static int tolua_setCanFollowPath_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanFollowPath");
	
	if (self != 0)
	{
		self->SetCanFollowPath(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanFollowPath_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanPursueTarget_Sprite
static int tolua_setCanPursueTarget_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanPursueTarget");
	
	if (self != 0)
	{
		self->SetCanPursueTarget(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanPursueTarget_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanEvadeTarget_Sprite
static int tolua_setCanEvadeTarget_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanEvadeTarget");
	
	if (self != 0)
	{
		self->SetCanEvadeTarget(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanEvadeTarget_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanInterpose_Sprite
static int tolua_setCanInterpose_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanInterpose");
	
	if (self != 0)
	{
		self->SetCanInterpose(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanInterpose_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanHide_Sprite
static int tolua_setCanHide_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanHide");
	
	if (self != 0)
	{
		self->SetCanHide(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanHide_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setCanOffsetPursuit_Sprite
static int tolua_setCanOffsetPursuit_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_bool(L, "setCanOffsetPursuit");
	
	if (self != 0)
	{
		self->SetCanOffsetPursuit(tolua_get_bool(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setCanOffsetPursuit_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setEvader_Sprite
static int tolua_setEvader_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_sprite(L, "setEvader");
	
	if (self != 0)
	{
		self->SetEvader(tolua_get_sprite(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setEvader_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setPursuer_Sprite
static int tolua_setPursuer_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_sprite(L, "setPursuer");
	
	if (self != 0)
	{
		self->SetPursuer(tolua_get_sprite(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setPursuer_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setHideTarget_Sprite
static int tolua_setHideTarget_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_sprite(L, "setHideTarget");
	
	if (self != 0)
	{
		self->SetHideTarget(tolua_get_sprite(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setHideTarget_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setSpriteA_Sprite
static int tolua_setSpriteA_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_sprite(L, "setSpriteA");
	
	if (self != 0)
	{
		self->SetSpriteA(tolua_get_sprite(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setSpriteA_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setSpriteB_Sprite
static int tolua_setSpriteB_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_sprite(L, "setSpriteB");
	
	if (self != 0)
	{
		self->SetSpriteB(tolua_get_sprite(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setSpriteB_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setLeader_Sprite
static int tolua_setLeader_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_sprite(L, "setLeader");
	
	if (self != 0)
	{
		self->SetLeader(tolua_get_sprite(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setLeader_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setLeaderOffset_Sprite
static int tolua_setLeaderOffset_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "setLeaderOffset");
	
	if (tolua_ArgSafe_number(L, 2, "setLeaderOffset")
	&&	tolua_ArgSafe_number(L, 3, "setLeaderOffset")
	&&	tolua_ArgSafe_number(L, 4, "setLeaderOffset"))
	{
		if (self != 0)
		{
			self->SetLeaderOffset(ZShadeSandboxMath::XMMath3(tolua_get_float(L, 2), tolua_get_float(L, 3), tolua_get_float(L, 4)));
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setLeaderOffset_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setTargetPoint_Sprite
static int tolua_setTargetPoint_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "setTargetPoint");
	
	if (tolua_ArgSafe_number(L, 2, "setTargetPoint")
	&&	tolua_ArgSafe_number(L, 3, "setTargetPoint")
	&&	tolua_ArgSafe_number(L, 4, "setTargetPoint"))
	{
		if (self != 0)
		{
			self->SetTargetPoint(ZShadeSandboxMath::XMMath3(tolua_get_float(L, 2), tolua_get_float(L, 3), tolua_get_float(L, 4)));
		}
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setTargetPoint_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setTargetDecelerationType_Sprite
static int tolua_setTargetDecelerationType_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setTargetDecelerationType");
	
	if (self != 0)
	{
		self->SetTargetDecelerationType(tolua_get_int(L, 2));
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setTargetDecelerationType_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_wanderingRadius_Sprite
static int tolua_wanderingRadius_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "wanderingRadius");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WanderingRadius());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_wanderingRadius_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_wanderingDistance_Sprite
static int tolua_wanderingDistance_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "wanderingDistance");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WanderingDistance());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_wanderingDistance_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_wanderingJitter_Sprite
static int tolua_wanderingJitter_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "wanderingJitter");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WanderingJitter());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_wanderingJitter_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_minDetectionBoxLength_Sprite
static int tolua_minDetectionBoxLength_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "minDetectionBoxLength");
	
	if (self != 0)
	{
		tolua_push_float(L, self->MinDetectionBoxLength());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_minDetectionBoxLength_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_waypointSeekDist_Sprite
static int tolua_waypointSeekDist_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "waypointSeekDist");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WaypointSeekDist());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_waypointSeekDist_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_maxFleeDistance_Sprite
static int tolua_maxFleeDistance_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "maxFleeDistance");
	
	if (self != 0)
	{
		tolua_push_float(L, self->MaxFleeDistance());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_maxFleeDistance_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightSeek_Sprite
static int tolua_weightSeek_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightSeek");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightSeek());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightSeek_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightFlee_Sprite
static int tolua_weightFlee_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightFlee");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightFlee());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightFlee_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightArrive_Sprite
static int tolua_weightArrive_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightArrive");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightArrive());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightArrive_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightSpriteAvoidance_Sprite
static int tolua_weightSpriteAvoidance_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightSpriteAvoidance");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightSpriteAvoidance());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightSpriteAvoidance_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightSeparation_Sprite
static int tolua_weightSeparation_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightSeparation");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightSeparation());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightSeparation_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightAlignment_Sprite
static int tolua_weightAlignment_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightAlignment");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightAlignment());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightAlignment_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightCohesion_Sprite
static int tolua_weightCohesion_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightCohesion");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightCohesion());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightCohesion_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightWander_Sprite
static int tolua_weightWander_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightWander");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightWander());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightWander_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightFollowPath_Sprite
static int tolua_weightFollowPath_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightFollowPath");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightFollowPath());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightFollowPath_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightPursueTarget_Sprite
static int tolua_weightPursueTarget_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightPursueTarget");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightPursueTarget());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightPursueTarget_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightEvadeTarget_Sprite
static int tolua_weightEvadeTarget_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightEvadeTarget");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightEvadeTarget());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightEvadeTarget_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightInterpose_Sprite
static int tolua_weightInterpose_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightInterpose");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightInterpose());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightInterpose_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightHide_Sprite
static int tolua_weightHide_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightHide");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightHide());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightHide_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_weightOffsetPursuit_Sprite
static int tolua_weightOffsetPursuit_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "weightOffsetPursuit");
	
	if (self != 0)
	{
		tolua_push_float(L, self->WeightOffsetPursuit());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_weightOffsetPursuit_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canSeek_Sprite
static int tolua_canSeek_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canSeek");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanSeek());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canSeek_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canFlee_Sprite
static int tolua_canFlee_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canFlee");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanFlee());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canFlee_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canArrive_Sprite
static int tolua_canArrive_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canArrive");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanArrive());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canArrive_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canAvoidSprites_Sprite
static int tolua_canAvoidSprites_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canAvoidSprites");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanAvoidSprites());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canAvoidSprites_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canFlockSeparation_Sprite
static int tolua_canFlockSeparation_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canFlockSeparation");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanFlockSeparation());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canFlockSeparation_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canFlockAlignment_Sprite
static int tolua_canFlockAlignment_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canFlockAlignment");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanFlockAlignment());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canFlockAlignment_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canFlockCohesion_Sprite
static int tolua_canFlockCohesion_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canFlockCohesion");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanFlockCohesion());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canFlockCohesion_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canWander_Sprite
static int tolua_canWander_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canWander");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanWander());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canWander_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canFollowPath_Sprite
static int tolua_canFollowPath_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canFollowPath");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanFollowPath());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canFollowPath_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canPursueTarget_Sprite
static int tolua_canPursueTarget_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canPursueTarget");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanPursueTarget());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canPursueTarget_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canEvadeTarget_Sprite
static int tolua_canEvadeTarget_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canEvadeTarget");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanEvadeTarget());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canEvadeTarget_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canInterpose_Sprite
static int tolua_canInterpose_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canInterpose");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanInterpose());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canInterpose_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canHide_Sprite
static int tolua_canHide_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canHide");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanHide());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canHide_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_canOffsetPursuit_Sprite
static int tolua_canOffsetPursuit_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "canOffsetPursuit");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->CanOffsetPursuit());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_canOffsetPursuit_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_evader_Sprite
static int tolua_evader_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "evader");
	
	if (self != 0)
	{
		tolua_push_sprite(L, self->Evader());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_evader_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_pursuer_Sprite
static int tolua_pursuer_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "pursuer");
	
	if (self != 0)
	{
		tolua_push_sprite(L, self->Pursuer());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_pursuer_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_hideTarget_Sprite
static int tolua_hideTarget_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "hideTarget");
	
	if (self != 0)
	{
		tolua_push_sprite(L, self->HideTarget());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_hideTarget_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_spriteA_Sprite
static int tolua_spriteA_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "spriteA");
	
	if (self != 0)
	{
		tolua_push_sprite(L, self->SpriteA());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_spriteA_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_spriteB_Sprite
static int tolua_spriteB_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "spriteB");
	
	if (self != 0)
	{
		tolua_push_sprite(L, self->SpriteB());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_spriteB_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_leader_Sprite
static int tolua_leader_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "leader");
	
	if (self != 0)
	{
		tolua_push_sprite(L, self->Leader());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_leader_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_leaderOffsetX_Sprite
static int tolua_leaderOffsetX_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "leaderOffsetX");
	
	if (self != 0)
	{
		tolua_push_float(L, self->LeaderOffsetX());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_leaderOffsetX_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_leaderOffsetY_Sprite
static int tolua_leaderOffsetY_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "leaderOffsetY");
	
	if (self != 0)
	{
		tolua_push_float(L, self->LeaderOffsetY());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_leaderOffsetY_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_leaderOffsetZ_Sprite
static int tolua_leaderOffsetZ_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "leaderOffsetZ");
	
	if (self != 0)
	{
		tolua_push_float(L, self->LeaderOffsetZ());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_leaderOffsetZ_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_targetPointX_Sprite
static int tolua_targetPointX_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "targetPointX");
	
	if (self != 0)
	{
		tolua_push_float(L, self->TargetPointX());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_targetPointX_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_targetPointY_Sprite
static int tolua_targetPointY_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "targetPointY");
	
	if (self != 0)
	{
		tolua_push_float(L, self->TargetPointY());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_targetPointY_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_targetPointZ_Sprite
static int tolua_targetPointZ_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "targetPointZ");
	
	if (self != 0)
	{
		tolua_push_float(L, self->TargetPointZ());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_targetPointZ_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_targetDecelerationType_Sprite
static int tolua_targetDecelerationType_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "targetDecelerationType");
	
	if (self != 0)
	{
		tolua_push_float(L, self->TargetDecelerationType());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_targetDecelerationType_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_findNearestSpriteIDInMap_Sprite
static int tolua_findNearestSpriteIDInMap_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "findNearestSpriteIDInMap");
	
	if (self != 0)
	{
		tolua_push_int(L, self->FindNearestSpriteIDInMap());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_findNearestSpriteIDInMap_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_addAllNearestSpriteIDsToCollection_Sprite
static int tolua_addAllNearestSpriteIDsToCollection_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "addAllNearestSpriteIDsToCollection");
	
	if (self != 0)
	{
		self->AddAllNearestSpriteIDsToCollection();
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_addAllNearestSpriteIDsToCollection_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_amountOfSpritesInRange_Sprite
static int tolua_amountOfSpritesInRange_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "amountOfSpritesInRange");
	
	if (self != 0)
	{
		tolua_push_int(L, self->AmountOfSpritesInRange());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_amountOfSpritesInRange_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_findNearestSpriteIDInCollection_Sprite
static int tolua_findNearestSpriteIDInCollection_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "findNearestSpriteIDInCollection");
	
	if (self != 0)
	{
		tolua_push_int(L, self->FindNearestSpriteIDInCollection(tolua_get_int(L, 2)));
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_findNearestSpriteIDInCollection_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_setMinimumAttackDistance_Sprite
static int tolua_setMinimumAttackDistance_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "setMinimumAttackDistance");
	
	if (self != 0)
	{
		self->MinimumAttackDistance() = tolua_get_float(L, 2);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_setMinimumAttackDistance_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_minimumAttackDistance_Sprite
static int tolua_minimumAttackDistance_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "minimumAttackDistance");
	
	if (self != 0)
	{
		tolua_push_float(L, self->MinimumAttackDistance());
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_minimumAttackDistance_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_withinAttackDistanceByID_Sprite
static int tolua_withinAttackDistanceByID_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_number(L, "withinAttackDistanceByID");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->WithinAttackDistance(tolua_get_float(L, 2)));
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_withinAttackDistanceByID_Sprite
//==================================================================================================================================
#ifndef TOLUA_DISABLE_tolua_withinAttackDistanceBySprite_Sprite
static int tolua_withinAttackDistanceBySprite_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe_sprite(L, "withinAttackDistanceBySprite");
	
	if (self != 0)
	{
		tolua_push_bool(L, self->WithinAttackDistance(tolua_get_sprite(L, 2)));
	}
	
	return 1;
}
#endif //TOLUA_DISABLE_tolua_withinAttackDistanceBySprite_Sprite
//==================================================================================================================================
//==================================================================================================================================
TOLUA_API int tolua_Sprite_open()
{
	lua_State* L = Scripting::LuaState();
	
	if (L != NULL)
	{
		tolua_open(L);
		tolua_reg_types(L);
		tolua_module(L, NULL, 0);
		tolua_beginmodule(L, NULL);
		
		#ifdef __cplusplus
			tolua_cclass(L, "Sprite", "Sprite", "", tolua_collect_Sprite);
		#else
			tolua_cclass(L, "Sprite", "Sprite", "", NULL);
		#endif
		
		tolua_beginmodule(L, "Sprite");
		tolua_function(L, "new", tolua_new_Sprite);
		//tolua_function(L, "new_local", tolua_new_local_Sprite);
		//tolua_function(L, ".call", tolua_new_local_Sprite);
		tolua_function(L, "delete", tolua_delete_Sprite);
		tolua_function(L, "set_x", tolua_setX_Sprite);
		tolua_function(L, "get_x", tolua_getX_Sprite);
		tolua_function(L, "set_y", tolua_setY_Sprite);
		tolua_function(L, "get_y", tolua_getY_Sprite);
		tolua_function(L, "set_width", tolua_setWidth_Sprite);
		tolua_function(L, "get_width", tolua_getWidth_Sprite);
		tolua_function(L, "set_height", tolua_setHeight_Sprite);
		tolua_function(L, "get_height", tolua_getHeight_Sprite);
		tolua_function(L, "create_texture", tolua_createTexture_Sprite);
		tolua_function(L, "set_name", tolua_setName_Sprite);
		tolua_function(L, "get_name", tolua_getName_Sprite);
		tolua_function(L, "set_damage_amount", tolua_setDamageAmount_Sprite);
		tolua_function(L, "get_damage_amount", tolua_getDamageAmount_Sprite);
		tolua_function(L, "set_vision", tolua_setVision_Sprite);
		tolua_function(L, "get_vision", tolua_getVision_Sprite);
		tolua_function(L, "set_base_texture_name", tolua_setBaseTextureName_Sprite);
		tolua_function(L, "get_base_texture_name", tolua_getBaseTextureName_Sprite);
		tolua_function(L, "set_hard", tolua_setHard_Sprite);
		tolua_function(L, "is_hard", tolua_isHard_Sprite);
		tolua_function(L, "set_animation_profile", tolua_setAnimationProfileName_Sprite);
		tolua_function(L, "get_animation_profile", tolua_getAnimationProfileName_Sprite);
		tolua_function(L, "set_hp", tolua_setHealth_Sprite);
		tolua_function(L, "get_hp", tolua_getHealth_Sprite);
		tolua_function(L, "set_hp_max", tolua_setHealthMax_Sprite);
		tolua_function(L, "get_hp_max", tolua_getHealthMax_Sprite);
		tolua_function(L, "set_mana", tolua_setMana_Sprite);
		tolua_function(L, "get_mana", tolua_getMana_Sprite);
		tolua_function(L, "set_gold", tolua_setGold_Sprite);
		tolua_function(L, "get_gold", tolua_getGold_Sprite);
		tolua_function(L, "set_strength", tolua_setStrength_Sprite);
		tolua_function(L, "get_strength", tolua_getStrength_Sprite);
		tolua_function(L, "set_speed", tolua_setSpeed_Sprite);
		tolua_function(L, "get_speed", tolua_getSpeed_Sprite);
		tolua_function(L, "set_defense", tolua_setDefense_Sprite);
		tolua_function(L, "get_defense", tolua_getDefense_Sprite);
		tolua_function(L, "set_exp", tolua_setExperience_Sprite);
		tolua_function(L, "get_exp", tolua_getExperience_Sprite);
		tolua_function(L, "set_level", tolua_setLevel_Sprite);
		tolua_function(L, "get_level", tolua_getLevel_Sprite);
		tolua_function(L, "set_static", tolua_setStatic_Sprite);
		tolua_function(L, "is_static", tolua_isStatic_Sprite);
		tolua_function(L, "set_no_hit", tolua_setNoHit_Sprite);
		tolua_function(L, "get_no_hit", tolua_getNoHit_Sprite);
		tolua_function(L, "set_touch", tolua_setTouch_Sprite);
		tolua_function(L, "get_touch", tolua_getTouch_Sprite);
		tolua_function(L, "set_script_name", tolua_setScriptName_Sprite);
		tolua_function(L, "get_script_name", tolua_getScriptName_Sprite);
		tolua_function(L, "set_can_teleport", tolua_setCanTeleport_Sprite);
		tolua_function(L, "can_teleport", tolua_getCanTeleport_Sprite);
		tolua_function(L, "set_teleport_map_name", tolua_setTeleportMapName_Sprite);
		tolua_function(L, "get_teleport_map_name", tolua_getTeleportMapName_Sprite);
		tolua_function(L, "set_teleport_x", tolua_setTeleportX_Sprite);
		tolua_function(L, "get_teleport_x", tolua_getTeleportX_Sprite);
		tolua_function(L, "set_teleport_y", tolua_setTeleportY_Sprite);
		tolua_function(L, "get_teleport_y", tolua_getTeleportY_Sprite);
		tolua_function(L, "set_background_sprite", tolua_setBackgroundSprite_Sprite);
		tolua_function(L, "is_background_sprite", tolua_isBackgroundSprite_Sprite);
		tolua_function(L, "set_entity_sprite", tolua_setEntitySprite_Sprite);
		tolua_function(L, "is_entity_sprite", tolua_isEntitySprite_Sprite);
		tolua_function(L, "set_invisible_sprite", tolua_setInvisibleSprite_Sprite);
		tolua_function(L, "is_invisible_sprite", tolua_isInvisibleSprite_Sprite);
		tolua_function(L, "set_default_sequence_id", tolua_setDefaultSequenceID_Sprite);
		tolua_function(L, "get_default_sequence_id", tolua_getDefaultSequenceID_Sprite);
		tolua_function(L, "set_animation_speed", tolua_setAnimationSpeed_Sprite);
		tolua_function(L, "get_animation_speed", tolua_getAnimationSpeed_Sprite);
		tolua_function(L, "collide_msg", tolua_collideMsg_Sprite);
		tolua_function(L, "collided_sprite_info", tolua_collidedSpriteInfo_Sprite);
		tolua_function(L, "say1", tolua_say1_Sprite);
		tolua_function(L, "say2", tolua_say2_Sprite);
		tolua_function(L, "say3", tolua_say3_Sprite);
		tolua_function(L, "say4", tolua_say4_Sprite);
		tolua_function(L, "say5", tolua_say5_Sprite);
		tolua_function(L, "say_stop1", tolua_sayStop1_Sprite);
		tolua_function(L, "say_stop2", tolua_sayStop2_Sprite);
		tolua_function(L, "say_stop3", tolua_sayStop3_Sprite);
		tolua_function(L, "say_stop4", tolua_sayStop4_Sprite);
		tolua_function(L, "say_stop5", tolua_sayStop5_Sprite);
		tolua_function(L, "set_max_speed", tolua_getMaxSpeed_Sprite);
		tolua_function(L, "get_max_speed", tolua_setMaxSpeed_Sprite);
		tolua_function(L, "set_inventory_message", tolua_setInventoryMessage_Sprite);
		tolua_function(L, "get_inventory_message", tolua_getInventoryMessage_Sprite);
		tolua_function(L, "set_always_seen_by_player", tolua_setAlwaysSeenByPlayer_Sprite);
		tolua_function(L, "is_always_seen_by_player", tolua_isAlwaysSeenByPlayer_Sprite);
		// Sprite lua built in function attrib
		tolua_function(L, "set_moving", tolua_setMoving_Sprite);
		tolua_function(L, "is_moving", tolua_isMoving_Sprite);
		tolua_function(L, "set_thinking", tolua_setThinking_Sprite);
		tolua_function(L, "is_thinking", tolua_isThinking_Sprite);
		tolua_function(L, "set_talking", tolua_setTalking_Sprite);
		tolua_function(L, "is_talking", tolua_isTalking_Sprite);
		tolua_function(L, "set_dying", tolua_setDying_Sprite);
		tolua_function(L, "is_dying", tolua_isDying_Sprite);
		tolua_function(L, "set_hit", tolua_setHit_Sprite);
		tolua_function(L, "is_hit", tolua_isHit_Sprite);
		tolua_function(L, "set_touched", tolua_setTouched_Sprite);
		tolua_function(L, "is_touched", tolua_isTouched_Sprite);
		tolua_function(L, "set_attacking", tolua_setAttacking_Sprite);
		tolua_function(L, "is_attacking", tolua_isAttacking_Sprite);
		tolua_function(L, "set_targeted", tolua_setTargeted_Sprite);
		tolua_function(L, "is_targeted", tolua_isTargeted_Sprite);
		// Sprite animation lua functions
		tolua_function(L, "set_animation_sequence", tolua_setAnimationSequence_Sprite);
		tolua_function(L, "set_direction_moved", tolua_setDirectionMoved_Sprite);
		tolua_function(L, "get_direction_moved", tolua_getDirectionMoved_Sprite);
		tolua_function(L, "current_sequence_exists", tolua_currentSequenceExists_Sprite);
		tolua_function(L, "is_last_frame", tolua_isLastFrame_Sprite);
		tolua_function(L, "set_not_on_last_frame", tolua_setNotOnLastFrame_Sprite);
		tolua_function(L, "add_animation", tolua_addAnimation_Sprite);
		// Sprite move commands (Do not need)
		/*
		tolua_function(L, "is_moving_up", tolua_isMovingUp_Sprite);
		tolua_function(L, "is_moving_down", tolua_isMovingDown_Sprite);
		tolua_function(L, "is_moving_left", tolua_isMovingLeft_Sprite);
		tolua_function(L, "is_moving_right", tolua_isMovingRight_Sprite);
		tolua_function(L, "move_up", tolua_moveUp_Sprite);
		tolua_function(L, "move_down", tolua_moveDown_Sprite);
		tolua_function(L, "move_left", tolua_moveLeft_Sprite);
		tolua_function(L, "move_right", tolua_moveRight_Sprite);
		*/
		// Behavior Tree commands
		tolua_function(L, "add_default_behavior", tolua_addDefaultBehavior_Sprite);
		tolua_function(L, "add_behavior_sequence", tolua_addBehaviorSequence_Sprite);
		tolua_function(L, "register_behavior", tolua_registerBehavior_Sprite);
		tolua_function(L, "create_behavior_tree", tolua_createBehaviorTree_Sprite);
		tolua_function(L, "set_run_behavior", tolua_canRunBehavior_Sprite);
		tolua_function(L, "set_run_behavior_name", tolua_setRunBehaviorName_Sprite);
		tolua_function(L, "set_run_default_behavior", tolua_canRunDefaultBehavior_Sprite);
		// Sprite AI steering commands
		tolua_function(L, "create_waypoint_system", tolua_createWaypointSystem_Sprite);
		tolua_function(L, "waypoint_system_size", tolua_waypointSystemSize_Sprite);
		tolua_function(L, "waypoint_system_empty", tolua_waypointSystemEmpty_Sprite);
		tolua_function(L, "clear_waypoint_system", tolua_clearWaypoints_Sprite);
		tolua_function(L, "remove_waypoint", tolua_removeWaypoint_Sprite);
		tolua_function(L, "add_waypoint", tolua_addWaypoint_Sprite);
		tolua_function(L, "current_waypoint_x", tolua_currentWaypointX_Sprite);
		tolua_function(L, "current_waypoint_y", tolua_currentWaypointY_Sprite);
		tolua_function(L, "current_waypoint_z", tolua_currentWaypointZ_Sprite);
		tolua_function(L, "set_next_waypoint", tolua_setNextWaypoint_Sprite);
		tolua_function(L, "waypoint_system_finished", tolua_waypointSystemFinished_Sprite);
		tolua_function(L, "set_waypoint_system_can_loop", tolua_setWaypointSystemCanLoop_Sprite);
		tolua_function(L, "waypoint_system_can_loop", tolua_waypointSystemCanLoop_Sprite);
		tolua_function(L, "current_waypoint_index", tolua_currentWaypointIndex_Sprite);
		tolua_function(L, "set_wandering_radius", tolua_setWanderingRadius_Sprite);
		tolua_function(L, "set_wandering_distance", tolua_setWanderingDistance_Sprite);
		tolua_function(L, "set_wandering_jitter", tolua_setWanderingJitter_Sprite);
		tolua_function(L, "set_min_detection_box_length", tolua_setMinDetectionBoxLength_Sprite);
		tolua_function(L, "set_waypoint_seek_dist", tolua_setWaypointSeekDist_Sprite);
		tolua_function(L, "set_max_flee_dist", tolua_setMaxFleeDistance_Sprite);
		tolua_function(L, "set_weight_seek", tolua_setWeightSeek_Sprite);
		tolua_function(L, "set_weight_flee", tolua_setWeightFlee_Sprite);
		tolua_function(L, "set_weight_arrive", tolua_setWeightArrive_Sprite);
		tolua_function(L, "set_weight_sprite_avoidance", tolua_setWeightSpriteAvoidance_Sprite);
		tolua_function(L, "set_weight_separation", tolua_setWeightSeparation_Sprite);
		tolua_function(L, "set_weight_alignment", tolua_setWeightAlignment_Sprite);
		tolua_function(L, "set_weight_cohesion", tolua_setWeightCohesion_Sprite);
		tolua_function(L, "set_weight_wander", tolua_setWeightWander_Sprite);
		tolua_function(L, "set_weight_follow_path", tolua_setWeightFollowPath_Sprite);
		tolua_function(L, "set_weight_pursue_target", tolua_setWeightPursueTarget_Sprite);
		tolua_function(L, "set_weight_evade_target", tolua_setWeightEvadeTarget_Sprite);
		tolua_function(L, "set_weight_interpose", tolua_setWeightInterpose_Sprite);
		tolua_function(L, "set_weight_hide", tolua_setWeightHide_Sprite);
		tolua_function(L, "set_weight_offset_pursuit", tolua_setWeightOffsetPursuit_Sprite);
		tolua_function(L, "set_can_seek", tolua_setCanSeek_Sprite);
		tolua_function(L, "set_can_flee", tolua_setCanFlee_Sprite);
		tolua_function(L, "set_can_arrive", tolua_setCanArrive_Sprite);
		tolua_function(L, "set_can_avoid_sprites", tolua_setCanAvoidSprites_Sprite);
		tolua_function(L, "set_can_flock_separation", tolua_setCanFlockSeparation_Sprite);
		tolua_function(L, "set_can_flock_alignment", tolua_setCanFlockAlignment_Sprite);
		tolua_function(L, "set_can_flock_cohesion", tolua_setCanFlockCohesion_Sprite);
		tolua_function(L, "set_can_wander", tolua_setCanWander_Sprite);
		tolua_function(L, "set_can_follow_path", tolua_setCanFollowPath_Sprite);
		tolua_function(L, "set_can_pursue_target", tolua_setCanPursueTarget_Sprite);
		tolua_function(L, "set_can_evade_target", tolua_setCanEvadeTarget_Sprite);
		tolua_function(L, "set_can_interpose", tolua_setCanInterpose_Sprite);
		tolua_function(L, "set_can_hide", tolua_setCanHide_Sprite);
		tolua_function(L, "set_can_offset_pursuit", tolua_setCanOffsetPursuit_Sprite);
		tolua_function(L, "set_evader", tolua_setEvader_Sprite);
		tolua_function(L, "set_pursuer", tolua_setPursuer_Sprite);
		tolua_function(L, "set_hide_target", tolua_setHideTarget_Sprite);
		tolua_function(L, "set_interpose_target_a", tolua_setSpriteA_Sprite);
		tolua_function(L, "set_interpose_target_b", tolua_setSpriteB_Sprite);
		tolua_function(L, "set_leader", tolua_setLeader_Sprite);
		tolua_function(L, "set_leader_offset", tolua_setLeaderOffset_Sprite);
		tolua_function(L, "set_target_point", tolua_setTargetPoint_Sprite);
		tolua_function(L, "set_target_deceleration_type", tolua_setTargetDecelerationType_Sprite);
		tolua_function(L, "wandering_radius", tolua_wanderingRadius_Sprite);
		tolua_function(L, "wandering_distance", tolua_wanderingDistance_Sprite);
		tolua_function(L, "wandering_jitter", tolua_wanderingJitter_Sprite);
		tolua_function(L, "min_detection_box_length", tolua_minDetectionBoxLength_Sprite);
		tolua_function(L, "waypoint_seek_dist", tolua_waypointSeekDist_Sprite);
		tolua_function(L, "max_flee_dist", tolua_maxFleeDistance_Sprite);
		tolua_function(L, "weight_seek", tolua_weightSeek_Sprite);
		tolua_function(L, "weight_flee", tolua_weightFlee_Sprite);
		tolua_function(L, "weight_arrive", tolua_weightArrive_Sprite);
		tolua_function(L, "weight_sprite_avoidance", tolua_weightSpriteAvoidance_Sprite);
		tolua_function(L, "weight_separation", tolua_weightSeparation_Sprite);
		tolua_function(L, "weight_alignment", tolua_weightAlignment_Sprite);
		tolua_function(L, "weight_cohesion", tolua_weightCohesion_Sprite);
		tolua_function(L, "weight_wander", tolua_weightWander_Sprite);
		tolua_function(L, "weight_follow_path", tolua_weightFollowPath_Sprite);
		tolua_function(L, "weight_pursue_target", tolua_weightPursueTarget_Sprite);
		tolua_function(L, "weight_evade_target", tolua_weightEvadeTarget_Sprite);
		tolua_function(L, "weight_interpose", tolua_weightInterpose_Sprite);
		tolua_function(L, "weight_hide", tolua_weightHide_Sprite);
		tolua_function(L, "weight_offset_pursuit", tolua_weightOffsetPursuit_Sprite);
		tolua_function(L, "can_seek", tolua_canSeek_Sprite);
		tolua_function(L, "can_flee", tolua_canFlee_Sprite);
		tolua_function(L, "can_arrive", tolua_canArrive_Sprite);
		tolua_function(L, "can_avoid_sprites", tolua_canAvoidSprites_Sprite);
		tolua_function(L, "can_flock_separation", tolua_canFlockSeparation_Sprite);
		tolua_function(L, "can_flock_alignment", tolua_canFlockAlignment_Sprite);
		tolua_function(L, "can_flock_cohesion", tolua_canFlockCohesion_Sprite);
		tolua_function(L, "can_wander", tolua_canWander_Sprite);
		tolua_function(L, "can_follow_path", tolua_canFollowPath_Sprite);
		tolua_function(L, "can_pursue_target", tolua_canPursueTarget_Sprite);
		tolua_function(L, "can_evade_target", tolua_canEvadeTarget_Sprite);
		tolua_function(L, "can_interpose", tolua_canInterpose_Sprite);
		tolua_function(L, "can_hide", tolua_canHide_Sprite);
		tolua_function(L, "can_offset_pursuit", tolua_canOffsetPursuit_Sprite);
		tolua_function(L, "evader", tolua_evader_Sprite);
		tolua_function(L, "pursuer", tolua_pursuer_Sprite);
		tolua_function(L, "hide_target", tolua_hideTarget_Sprite);
		tolua_function(L, "interpose_target_a", tolua_spriteA_Sprite);
		tolua_function(L, "interpose_target_b", tolua_spriteB_Sprite);
		tolua_function(L, "leader", tolua_leader_Sprite);
		tolua_function(L, "leader_offset_x", tolua_leaderOffsetX_Sprite);
		tolua_function(L, "leader_offset_y", tolua_leaderOffsetY_Sprite);
		tolua_function(L, "leader_offset_z", tolua_leaderOffsetZ_Sprite);
		tolua_function(L, "target_point_x", tolua_targetPointX_Sprite);
		tolua_function(L, "target_point_y", tolua_targetPointY_Sprite);
		tolua_function(L, "target_point_z", tolua_targetPointZ_Sprite);
		tolua_function(L, "target_deceleration_type", tolua_targetDecelerationType_Sprite);
		tolua_function(L, "find_nearest_sprite_id_in_map", tolua_findNearestSpriteIDInMap_Sprite);
		tolua_function(L, "add_all_nearest_sprite_ids_to_collection", tolua_addAllNearestSpriteIDsToCollection_Sprite);
		tolua_function(L, "amount_of_sprites_in_range", tolua_amountOfSpritesInRange_Sprite);
		tolua_function(L, "find_nearest_sprite_id_in_collection", tolua_findNearestSpriteIDInCollection_Sprite);
		tolua_function(L, "set_minimum_attack_dist", tolua_setMinimumAttackDistance_Sprite);
		tolua_function(L, "minimum_attack_dist", tolua_minimumAttackDistance_Sprite);
		tolua_function(L, "within_attack_dist_by_id", tolua_withinAttackDistanceByID_Sprite);
		tolua_function(L, "within_attack_dist_by_sprite", tolua_withinAttackDistanceBySprite_Sprite);
		tolua_endmodule(L);
		tolua_endmodule(L);
	}
	
	return 1;
}
//==================================================================================================================================
#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
TOLUA_API int luaopen_Sprite()
{
	return tolua_Sprite_open();
}
#endif
//==================================================================================================================================