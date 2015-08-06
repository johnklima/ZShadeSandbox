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
/*#ifndef TOLUA_DISABLE_tolua_doCreateHardbox_Sprite
static int tolua_doCreateHardbox_Sprite(lua_State* L)
{
	AISprite* self = tolua_Sprite_Safe(L, "doCreateHardbox");
	
	if (self != 0)
	{
		self->CreateHardBoxSpr(Scripting::GetGameDirectory()->m_required_textures);
	}
	
	return 0;
}
#endif //TOLUA_DISABLE_tolua_doCreateHardbox_Sprite
*/
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
		//tolua_function(L, "do_create_hardbox", tolua_doCreateHardbox_Sprite);
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
		/*
		tolua_function(L, "create_waypoint_system", tolua_createWaypointSystem_Sprite);
		tolua_function(L, "waypoint_system_size", tolua_waypointSystemSize_Sprite);
		tolua_function(L, "waypoint_system_empty", tolua_waypointSystemEmpty_Sprite);
		tolua_function(L, "clear_waypoint_system", tolua_clearWaypoints_Sprite);
		tolua_function(L, "remove_waypoint", tolua_removeWaypoint_Sprite);
		tolua_function(L, "add_waypoint", tolua_addWaypoint_Sprite);
		tolua_function(L, "current_waypoint", tolua_currentWaypoint_Sprite);
		tolua_function(L, "set_next_waypoint", tolua_setNextWaypoint_Sprite);
		tolua_function(L, "waypoint_system_finished", tolua_waypointSystemFinished_Sprite);
		tolua_function(L, "set_waypoint_system_can_loop", tolua_setWaypointSystemCanLoop_Sprite);
		tolua_function(L, "waypoint_system_can_loop", tolua_waypointSystemCanLoop_Sprite);
		tolua_function(L, "current_waypoint_index", tolua_currentWaypointIndex_Sprite);
		
		*/
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