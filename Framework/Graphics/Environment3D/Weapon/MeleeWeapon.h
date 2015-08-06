//==================================================================================================================================
// MeleeWeapon
//
// This class will contain data for a close combat melee weapon
//==================================================================================================================================
// History
//
// -Created on 7/17/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __MELEEWEAPON_H
#define __MELEEWEAPON_H
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

Need PhysicsBody3D for this weapon

There are 2 categories of weapons
melee weapons - fist, sword
ranged weapon - magic, guns

What is a Melee Weapon
-hits only when it touches something


What does a sword do?
-hits only when it touches something
-can have durability
-has a sharpness
-could break

*/
class MeleeWeapon
{
public:
	
	MeleeWeapon();
	
	
	
public:
	
	int& DamageCount() { return iDamageCount; }
	int DamageCount() const { return iDamageCount; }
	int& Durability() { return iDurability; }
	int Durability() const { return iDurability; }
	int& Sharpness() { return iSharpness; }
	int Sharpness() const { return iSharpness; }
	bool& Hit() { return bHit; }
	bool Hit() const { return bHit; }
	bool& Broken() { return bBroken; }
	bool Broken() const { return bBroken; }
	bool& Throw() { return bThrow; }
	bool Throw() const { return bThrow; }
	bool& MeshAssociated() { return bMeshAssociated; }
	bool MeshAssociated() const { return bMeshAssociated; }
	
private:
	
	// how much damage does this weapon do if it hits something
	int iDamageCount;
	
	// Life of the weapon, how long this weapon can last until it is broken and unusable
	int iDurability;
	
	// Defines the edge for the weapon and how powerful it is
	int iSharpness;
	
	// Has this weapon collided with something
	bool bHit;
	
	// Has the durability worn off and the weapon is now broken and in need of repair
	bool bBroken;
	
	// If the weapon is a spear then it is a melee weapon that can be thrown
	bool bThrow;
	
	// If the melee weapon is fists then there is no mesh
	bool bMeshAssociated;
	
	// Starting out with obj Mesh that represents this weapon
	ZShadeSandboxMesh::OBJMesh* mMesh;
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__MELEEWEAPON_H