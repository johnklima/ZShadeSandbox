//==================================================================================================================================
// Entity3D
//
// This will represent a 3D entity that can be hostile or passive
//==================================================================================================================================
// History
//
// -Created on 6/22/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __ENTITY3D_H
#define __ENTITY3D_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "ZMath.h"
#include "XMMath.h"
#include "OBJMesh.h"
#include "SleepTimer.h"
#include "Text.h"

//==================================================================================================================================
//==================================================================================================================================
class Entity3D
{
public:
	
	Entity3D(D3D* d3d);
	
	void Init();
	
	// dt is for animation when skeletal animation is ready
	void Update(float dt);
	void Render(Camera* camera);
	
	void MakeTemporarilyInvincible() { iHurtTimer = kHurtFrames; }
	
	void say(string text);
	void say(string text, int font_size);
	void say(string text, int r, int g, int b, int font_size);
	
	void say_stop(string text);
	void say_stop(string text, int font_size);
	void say_stop(string text, int r, int g, int b, int font_size);
	
	// Getter methods
public:
	
	XMFLOAT3& Position()				{ return mPosition; }
	XMFLOAT3  Position() const			{ return mPosition; }
	XMFLOAT3& Rotation()				{ return mRotation; }
	XMFLOAT3  Rotation() const			{ return mRotation; }
	XMFLOAT3& Scale()					{ return mScale; }
	XMFLOAT3  Scale() const				{ return mScale; }
	
	bool IsHurt()						{ return iHurtTimer != 0; }
	bool& IsPlayer()					{ return bPlayer; }
	bool  IsPlayer() const				{ return bPlayer; }
	bool& IsDead()						{ return bDead; }
	bool  IsDead() const				{ return bDead; }
	bool& IsHostile()					{ return bHostile; }
	bool  IsHostile() const				{ return bHostile; }
	bool& IsMoving()					{ return bMoving; }
	bool  IsMoving() const				{ return bMoving; }
	bool& IsThinking()					{ return bThinking; }
	bool  IsThinking() const			{ return bThinking; }
	bool& IsTalking()					{ return bTalking; }
	bool  IsTalking() const				{ return bTalking; }
	bool& IsDying()						{ return bDying; }
	bool  IsDying() const				{ return bDying; }
	bool& IsHit()						{ return bHit; }
	bool  IsHit() const					{ return bHit; }
	bool& IsTouched()					{ return bTouched; }
	bool  IsTouched() const				{ return bTouched; }
	bool& IsAttacked()					{ return bAttacked; }
	bool  IsAttacked() const			{ return bAttacked; }
	bool& IsTargeted()					{ return bTargeted; }
	bool  IsTargeted() const			{ return bTargeted; }
	
	float& InflictingDamage()			{ return fInflictingDamage; }
	float  InflictingDamage() const		{ return fInflictingDamage; }
	float& Damage()						{ return fDamage; }
	float  Damage() const				{ return fDamage; }
	float& Health()						{ return fHealth; }
	float  Health() const				{ return fHealth; }
	float& MaxHealth()					{ return fMaxHealth; }
	float  MaxHealth() const			{ return fMaxHealth; }
	float& Speed()						{ return fSpeed; }
	float  Speed() const				{ return fSpeed; }
	
	string& ScriptName()				{ return sScriptName; }
	string  ScriptName() const			{ return sScriptName; }
	string& Name()						{ return sName; }
	string  Name() const				{ return sName; }
	
	int& Mana()							{ return iMana; }
	int  Mana() const					{ return iMana; }
	int& Gold()							{ return iGold; }
	int  Gold() const					{ return iGold; }
	int& Coin()							{ return iCoin; }
	int  Coin() const					{ return iCoin; }
	int& Strength()						{ return iStrength; }
	int  Strength() const				{ return iStrength; }
	int& Defense()						{ return iDefense; }
	int  Defense() const				{ return iDefense; }
	int& Experience()					{ return iExperience; }
	int  Experience() const				{ return iExperience; }
	int& Level()						{ return iLevel; }
	int  Level() const					{ return iLevel; }
	
protected:
	
	static int iCounter;
	
	ZShadeSandboxGraphics::Text m_text;
	XMMATRIX m_text_transform;
	XMFLOAT4 m_text_color;
	bool m_display_text;
	SleepTimer* m_text_timer;
	
	D3D* mD3DSystem;
	EngineOptions* mEngineOptions;
	
	// Starting out with obj Mesh that represents this entity (Skeletal Mesh Animation not done yet)
	ZShadeSandboxMesh::OBJMesh* mMesh;
	
	// The main transformation attributes for the entity in 3D space
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;
	
	// Flashes for 120 frames if the entity takes damage
	const static int kHurtFrames = 120;
	const static int kEnemyHurtFrames = 10;
	
	// When the entity takes damage this counts down from the amount of hurt frames
	int iHurtTimer;
	
	// Allows this entity to be seen or hidden
	bool bVisible;
	
	// Lets the entity system know that this is the player so processing will be done by
	// the human with a keyboard, mouse, etc..
	bool bPlayer;
	
	// Has this entity been killed
	bool bDead;
	
	// Tells the entity that it will attack something or just be passive
	bool bHostile;
	
	//The script the entity will use
	string sScriptName;
	
	string sName;
	
	bool bMoving;
	bool bThinking;
	bool bTalking;
	bool bDying;
	bool bHit;
	bool bTouched;
	bool bAttacked;
	bool bTargeted;
	
	// How much damage will this entity do when it is hostile
	float fInflictingDamage;
	
	// How much damage is this entity currently taking
	float fDamage;
	
	// Amount of health this entity has until it is dead
	float fHealth;
	
	// Maximum amount of health this entity can have to fill his container
	float fMaxHealth;
	
	// Defines how fast this entity can travel
	float fSpeed;
	float fOldSpeed;
	
	int iMana;
	int iGold;
	int iCoin;
	int iStrength;
	int iDefense;
	int iExperience;
	int iLevel;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__ENTITY3D_H