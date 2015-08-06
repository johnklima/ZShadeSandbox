//==================================================================================================================================
// Sprite.h
//
//==================================================================================================================================
// History
//
// -Created in 2013 by Dustin Watson
// -Updated on 1/16/2015 by Dustin Watson
// -Updated on 7/14/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __SPRITE_H
#define __SPRITE_H
//==================================================================================================================================
//==================================================================================================================================

//
//Includes
//

#include "D3DIncludes.h"
#include "GameDirectory.h"
#include "D3D.h"
#include "QuadMesh.h"
#include "Animation2D.h"
#include <string>
#include "MemoryMgr.h"
#include "PhysicsBody2D.h"
#include "Input.h"
#include "Text.h"
using namespace std;
class Tile;
class Section;
class Script;
class AISprite;

//==================================================================================================================================
//==================================================================================================================================
class Sprite : public PhysicsBody2D
{
public:
	
	Sprite(D3D* d3d);
	~Sprite();

	bool Initialize(string base_path, string textureFilename, XMFLOAT3 position, int bitmapWidth, int bitmapHeight, PhysicsType pt);
	void InitializeStats();

	// Update sprite physics body
	void Update(float dt, Keyboard* keyboard);
	void PlayerPlatformerInput(float dt, Keyboard* keyboard);
	void PlayerTopdownInput(float dt, Keyboard* keyboard);
	
	void CreateTexture(string base_path, string windowName, int screenWidth, int screenHeight);
	
	void UpdateMeshTexture(string base_sprite_path);
	void UpdateMeshTexture(string base_sprite_path, string textureFilename);

	// Render non-transparent texture
	bool Render(Camera* camera);
	bool Render(Camera* camera, XMFLOAT2 offset);
	
	// Render transparent texture
	bool Render(Camera* camera, float blendAmount);
	bool Render(Camera* camera, float blendAmount, XMFLOAT2 offset);
	
	// Render non-transparent animation
	bool RenderAnimation(Camera* camera);
	bool RenderAnimation(Camera* camera, XMFLOAT2 offset);
	
	// Render transparent animation
	bool RenderAnimation(Camera* camera, float blendAmount);
	bool RenderAnimation(Camera* camera, float blendAmount, XMFLOAT2 offset);
	
	void RenderDisplayText();
	
	void UpdateAnimation();
	
	// Interfaces with the collision system of the body
	void OnCollision(bool bTop, bool bBottom, bool bLeft, bool bRight, int damageAmount);
	
	void say(string text);
	void say(string text, int font_size);
	void say(string text, int x, int y, int font_size);
	void say(string text, int r, int g, int b, int font_size);
	void say(string text, int x, int y, int r, int g, int b, int font_size);
	
	void say_stop(string text);
	void say_stop(string text, int font_size);
	void say_stop(string text, int x, int y, int font_size);
	void say_stop(string text, int r, int g, int b, int font_size);
	void say_stop(string text, int x, int y, int r, int g, int b, int font_size);
	
	bool CollidesRect(Sprite* spr);
	bool CollidesWith(Sprite* spr);
	
	void Move(float newPosX, float newPosY, float newPosHBX, float newPosHBY);
	
	void ClearAnimationList();
	void AddAnimationSequence(Animation2D* sequence);
	void SetSequence(int i);
	void NullSequence() { m_CurrentSequence = NULL; ClearAnimationList(); }
	Animation2D* CurrentSequenceExists() { return m_CurrentSequence; }
	int GetCurrentFrameIndex() { if (m_CurrentSequence != NULL) return m_CurrentSequence->GetCurrentFrameIndex(); else return -1; }
	bool IsCurrentSequenceOnLastFrame() { if (m_CurrentSequence != NULL) return m_CurrentSequence->IsOnLastFrame(); else return false; }
	void SetCurrentSequenceNotOnLastFrame() { if (m_CurrentSequence != NULL) m_CurrentSequence->SetNotOnLastFrame(); }
	
	//void CreateHardBoxSpr(string editor_base);
	//Sprite* GetHardBoxSpr() { return m_hardBoxSpr; }
	
	//LuaSpriteScript* GetSpriteScript() { return m_spriteScript; }
	Script* GetScript() { return m_script; }
	
	void AddScript(string name);

	bool MoveSpriteUp() { return m_move_up; }
	bool MoveSpriteDown() { return m_move_down; }
	bool MoveSpriteLeft() { return m_move_left; }
	bool MoveSpriteRight() { return m_move_right; }

	void SetMoveSpriteUp(bool up) { m_move_up = up; bMovingUp = up; }
	void SetMoveSpriteDown(bool down) { m_move_down = down; bMovingDown = down; }
	void SetMoveSpriteLeft(bool left) { m_move_left = left; bMovingLeft = left; }
	void SetMoveSpriteRight(bool right) { m_move_right = right; bMovingRight = right; }
	
	AISprite* Clone(GameDirectory2D* gd);
	AISprite* Clone(GameDirectory2D* gd, float x, float y);
	void CloneMe(GameDirectory2D* gd, float x, float y, int w, int h);

	void AddAnimation();
	
	// Special Methods
	
	//bool m_HasWorldCollision() const { return true; }
	//bool m_ApplyGravity() const { return true; }
	//bool m_ApplyFriction() const { return !bTryToMove; }
	
	void MakeTemporarilyInvincible() { iHurtTimer = kHurtFrames; }
	
	bool IsHurt() { return iHurtTimer != 0; }
	
	XMMATRIX& SpriteWorld() { return mWorld; }
	XMMATRIX SpriteWorld() const { return mWorld; }
	
	void Tag() { m_tag = true; }
	bool IsTagged() { return m_tag; }
	void UnTag() { m_tag = false; }
	
public:
	
	//--------------------------------------------------------------------
	//Attribute assignment
	//--------------------------------------------------------------------
	
	void SetCollidedSprite(Sprite* spr) { m_collided_with_sprite = spr; }
	Sprite* GetCollidedSprite() { return m_collided_with_sprite; }
	
	int& OldSpeed() { return m_old_speed; }
	int OldSpeed() const { return m_old_speed; }

	bool& CanDisplayHardbox() { return m_displayHardbox; }
	bool CanDisplayHardbox() const { return m_displayHardbox; }
	
	int& Width() { return m_imageWidth; }
	int Width() const { return m_imageWidth; }
	
	int& Height() { return m_imageHeight; }
	int Height() const { return m_imageHeight; }
	
	int& DirectionMoved() { return m_DirectionMoved; }
	int DirectionMoved() const { return m_DirectionMoved; }
	
	bool& Attacking() { return mAttacking; }
	bool Attacking() const { return mAttacking; }
	
	bool& IsPlayer() { return m_player; }
	bool IsPlayer() const { return m_player; }
	
	bool& IsDisplaySprite() { return m_displaySprite; }
	bool IsDisplaySprite() const { return m_displaySprite; }

	bool& IsCloneDisplaySprite() { return m_cloneDisplaySprite; }
	bool IsCloneDisplaySprite() const { return m_cloneDisplaySprite; }
	
	int& ID() { return m_id; }
	int ID() const { return m_id; }
	
	float& X() { return m_TopLeftPosition.x; }
	float X() const { return m_TopLeftPosition.x; }
	
	float& Y() { return m_TopLeftPosition.y; }
	float Y() const { return m_TopLeftPosition.y; }
	
	int& Vision() { return m_vision; }
	int Vision() const { return m_vision; }
	
	int& Depth() { return m_depth; }
	int Depth() const { return m_depth; }
	
	string& BaseTextureFilename() { return m_baseTextureFilename; }
	string BaseTextureFilename() const { return m_baseTextureFilename; }
	
	int& IsHard() { return m_hard; }
	int IsHard() const { return m_hard; }
	
	string& AnimationProfileName() { return m_animationProfileName; }
	string AnimationProfileName() const { return m_animationProfileName; }
	
	int& HP() { return m_hp; }
	int HP() const { return m_hp; }
	
	int& HPMax() { return m_hp_max; }
	int HPMax() const { return m_hp_max; }
	
	//bool& IsStatic() { return m_static; }
	//bool IsStatic() const { return m_static; }
	
	int& Gold() { return m_gold; }
	int Gold() const { return m_gold; }

	int& Strength() { return m_strength; }
	int Strength() const { return m_strength; }
	
	//float& Speed() { return m_speed; }
	//float Speed() const { return m_speed; }
	
	int& Mana() { return m_mana; }
	int Mana() const { return m_mana; }
	
	int& Defense() { return m_defense; }
	int Defense() const { return m_defense; }
	
	int& Experience() { return m_experience; }
	int Experience() const { return m_experience; }
	
	int& Level() { return m_level; }
	int Level() const { return m_level; }
	
	bool& IsNoHit() { return m_nohit; }
	bool IsNoHit() const { return m_nohit; }
	
	bool& IsTouch() { return m_touch; }
	bool IsTouch() const { return m_touch; }
	
	bool& IsTouched() { return m_touched; }
	bool IsTouched() const { return m_touched; }
	
	string& ScriptName() { return m_script_name; }
	string ScriptName() const { return m_script_name; }
	
	bool& CanTeleport() { return m_can_teleport; }
	bool CanTeleport() const { return m_can_teleport; }
	
	string& TeleportMapName() { return m_teleport_map; }
	string TeleportMapName() const { return m_teleport_map; }
	
	int& TeleportX() { return m_teleport_x; }
	int TeleportX() const { return m_teleport_x; }
	
	int& TeleportY() { return m_teleport_y; }
	int TeleportY() const { return m_teleport_y; }
	
	bool& IsBackgroundSprite() { return m_background; }
	bool IsBackgroundSprite() const { return m_background; }
	
	bool& IsEntitySprite() { return m_entity; }
	bool IsEntitySprite() const { return m_entity; }
	
	bool& IsInvisibleSprite() { return m_invisible; }
	bool IsInvisibleSprite() const { return m_invisible; }
	
	int& DefaultSequence() { return default_sequence; }
	int DefaultSequence() const { return default_sequence; }
	
	int& AnimationSpeed() { return m_animationSpeed; }
	int AnimationSpeed() const { return m_animationSpeed; }
	
	int& DamageAmount() { return m_damageAmount; }
	int DamageAmount() const { return m_damageAmount; }
	
	string& Name() { return m_name; }
	string Name() const { return m_name; }
	
	bool& Moving() { return m_moving; }
	bool Moving() const { return m_moving; }
	
	bool& Thinking() { return m_thinking; }
	bool Thinking() const { return m_thinking; }
	
	bool& Talking() { return m_talking; }
	bool Talking() const { return m_talking; }
	
	bool& IsDying() { return m_dying; }
	bool IsDying() const { return m_dying; }
	
	bool& IsHit() { return m_hit; }
	bool IsHit() const { return m_hit; }
	
	bool& Attacked() { return m_attacked; }
	bool Attacked() const { return m_attacked; }
	
	bool& Targeted() { return m_targeted; }
	bool Targeted() const { return m_targeted; }
	
	string& CollideMsg() { return m_collide_msg; }
	string CollideMsg() const { return m_collide_msg; }
	
	string& InventoryMessage() { return m_inventoryMessage; }
	string InventoryMessage() const { return m_inventoryMessage; }
	
	bool& PlatformerSprite() { return m_platformerSprite; }
	bool PlatformerSprite() const { return m_platformerSprite; }
	
	bool& InFowCircle() { return m_inFowCircle; }
	bool InFowCircle() const { return m_inFowCircle; }
	
	bool& SeenByPlayer() { return m_seenByPlayer; }
	bool SeenByPlayer() const { return m_seenByPlayer; }

	bool& AlwaysSeenByPlayer() { return m_alwaysSeenByPlayer; }
	bool AlwaysSeenByPlayer() const { return m_alwaysSeenByPlayer; }
	
protected:
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;

	ZShadeSandboxGraphics::Text m_text;
	XMMATRIX m_text_transform;
	XMFLOAT4 m_text_color;
	bool m_display_text;
	
	SleepTimer* m_text_timer;
	int m_old_speed;
	
	XMMATRIX mWorld;
	
	//is it static or dynamic
	//bool m_static;

	string m_collide_msg;

	int m_id;
	
	//Depth que is based on sorting the y position of the sprites on the map if it has a default depth of 0
	//otherwise use assigned sprite depth (range 1 - 2000)(default 0)

	//Does the sprite exist
	bool m_exists;

	bool m_move_up;
	bool m_move_down;
	bool m_move_left;
	bool m_move_right;
	
	// The sprite that this sprite has collided with
	Sprite* m_collided_with_sprite;

	//--------------------------------------------------------------------
	//Sprite Attributes
	//--------------------------------------------------------------------
	//The vision of this sprite
	int m_vision;

	//The depth of this sprite
	int m_depth;

	//Base texture of the sprite
	string m_baseTextureFilename;
	
	bool m_platformerSprite;
	
	//Hardness of the sprite - 1 for hard, 0 for not hard
	int m_hard;

	string m_animationProfileName;
	
	// This will notify the player what kind of inventory item he has selected
	string m_inventoryMessage;
	
	// The physics body is the one that uses this, add to the PhysicsBody2D instead
	//float m_speed;
	
	int m_hp;
	int m_hp_max;
	int m_mana;
	int m_gold;
	int m_strength;
	int m_defense;
	int m_experience;
	int m_level;
	int m_animationSpeed;
	int m_damageAmount;
	bool m_nohit;
	bool m_background;
	bool m_entity;
	bool m_invisible;
	int default_sequence;
	
	//Is this a touchable sprite
	bool m_touch;
	
	//The script the sprite will use
	string m_script_name;
	
	string m_name;
	bool m_moving;
	bool m_thinking;
	bool m_talking;
	bool m_dying;
	bool m_hit;
	//Was this sprite touched
	bool m_touched;
	bool m_attacked;
	bool m_targeted;
	
	//The sprite script for the sprite
	//LuaSpriteScript* m_spriteScript;
	Script* m_script;

	//Teleport
	bool m_can_teleport;
	string m_teleport_map;
	int m_teleport_x;
	int m_teleport_y;
	
	//--------------------------------------------------------------------
	
	// This needs to go into the editor (allows for this sprite to be an ally to the player to see what they are doing)
	bool m_seenByPlayer; // Need to set and save this for fow so a sprite can be seen by the player with fow or hidden until in the fow radius
	bool m_alwaysSeenByPlayer;

	bool m_inFowCircle;
	bool m_displayHardbox;
	bool m_displaySprite;
	bool m_cloneDisplaySprite;
	bool m_player;
	
	// This is used by the AI system to tag this sprite to be used in calculating
	// different AI behaviours
	bool m_tag;
	
	//The current rendered bitmap for the sprite
	ZShadeSandboxMesh::QuadMesh* m_bitmap;
	
	int m_imageWidth;
	int m_imageHeight;
	
	int m_DirectionMoved;
	bool mAttacking;
	
	Animation2D* m_CurrentSequence;

	//The animation sequences for the sprite
	vector<Animation2D*> mAnimationSequences;
	
	// Special Attributes
	
	// How high can the sprite jump
	float kPlatformerJumpVelocity;// = 900.0f * 1.2f;
	
	// Sprite flashes for 120 frames if hurt
	const static int kHurtFrames = 120;
	const static int kEnemyHurtFrames = 10;
	
	// Controls how fast the sprites velocity moves towards the target velocity
	// 1 = in one frame and 0 = never
	float kReachTargetScale;// = 0.7f;
	
	// How fast can the sprite walk
	const static int kWalkSpeed = 80;
	
	int iHurtTimer;
	
	bool bVisible; // This pertains to when the sprite is hit
	bool bTryToMove;
	bool bFlyMode;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__SPRITE_H