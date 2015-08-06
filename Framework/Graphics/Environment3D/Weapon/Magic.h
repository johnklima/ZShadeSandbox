//==================================================================================================================================
// Magic
//
// This class will contain data for a ranged weapon
//==================================================================================================================================
// History
//
// -Created on 7/17/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __RANGEDWEAPON_H
#define __RANGEDWEAPON_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "Weapon.h"

//==================================================================================================================================
//==================================================================================================================================
/*
Brainstorming

What is Magic
-Hits something when cast
-has a rate of fire for how fast the magic can be casted
-has a limited amount of mana associated with it

*/
class Magic
{
public:
	
	Magic();
	
	void Cast();
	
public:
	
	int& DamageCount() { return iDamageCount; }
	int DamageCount() const { return iDamageCount; }
	int& FireRate() { return iFireRate; }
	int FireRate() const { return iFireRate; }
	bool& Hit() { return bHit; }
	bool Hit() const { return bHit; }
	bool& OutOfMana() { return bOutOfMana; }
	bool OutOfMana() const { return bOutOfMana; }
	
private:
	
	// How much damage can a projectile fired from this weapon deal
	int iDamageCount;
	
	// How fast can this magic fire
	int iFireRate;
	
	// Has this magic collided with something
	bool bHit;
	
	// Has all the mana ran out to be able to cast magic
	bool bOutOfMana;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__RANGEDWEAPON_H