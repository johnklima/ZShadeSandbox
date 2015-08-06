//==================================================================================================================================
// RangedWeapon
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

#include "D3D.h"
#include "ZMath.h"

//==================================================================================================================================
//==================================================================================================================================
/*
Brainstorming

What is a Ranged Weapon
-hits only when projectile type collides with something
-fires a projectile
-has limited supply of ammunition

*/
class RangedWeapon
{
public:
	
	RangedWeapon();
	
	void Fire();
	
public:
	
	
	
private:
	
	// How much damage can a projectile fired from this weapon deal
	int iProjectileDamage;
	
	// How fast can this weapon fire
	int iFireRate;
	
	// Has this magic collided with something
	bool bHit;
	
	// Has all the ammunition that fuels this weapon been used up
	bool bOutOfAmmo;
	
	// Starting out with obj Mesh that represents this weapon
	ZShadeSandboxMesh::OBJMesh* mMesh;
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__RANGEDWEAPON_H