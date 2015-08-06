//==================================================================================================================================
// Script.h
//
//==================================================================================================================================
// History
//
// -Created on 2/3/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __SCRIPT_H
#define __SCRIPT_H
//==================================================================================================================================
//==================================================================================================================================
#include "ScriptBase.h"
//==================================================================================================================================
class Script : public ScriptBase
{
public:
	
	Script(D3D* d3d, GameDirectory2D* gd, lua_State* L, string name);
	
	// Default Script Functions
	void Main(int args = 0, AISprite* spr = NULL, AISprite* target_spr = NULL);
	void Think(int args = 0, AISprite* spr = NULL, AISprite* target_spr = NULL);
	void Move(int args = 0, AISprite* spr = NULL, AISprite* target_spr = NULL);
	void Talk(int args = 0, AISprite* spr = NULL, AISprite* target_spr = NULL);
	void Die(int args = 0, AISprite* spr = NULL, AISprite* target_spr = NULL);
	void Hit(int args = 0, AISprite* spr = NULL, AISprite* target_spr = NULL);
	void Touch(int args = 0, AISprite* spr = NULL, AISprite* target_spr = NULL);
	void Attack(int args = 0, AISprite* spr = NULL, AISprite* target_spr = NULL);
	void Target(int args = 0, AISprite* spr = NULL, AISprite* target_spr = NULL);
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__SCRIPT_H