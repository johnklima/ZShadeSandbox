//================================================================================================================
// Environment2DMapHelper.h
//
// Functions needed by LUA scripting engine and map editor as well as creating the game
//================================================================================================================
// History
//
// Created on 3/10/2015 by Dustin Watson
//================================================================================================================
#ifndef __ENVIRONMENT2DMAPHELPER_H
#define __ENVIRONMENT2DMAPHELPER_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include <memory>
#include <sstream>
#include <Windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <string>
#include <map>
#include <vector>
#include "D3D.h"
#include "Input.h"
#include "Camera.h"
#include "TopdownMap.h"
#include "FogData.h"
#include "GameState.h"
#include "GameDirectory.h"
#include "ZShadeINIParser.h"
#include "TextureManager.h"
#include "CreateGameDirectory.h"
#include "ZShadeMessageCenter.h"
#include "MemoryMgr.h"
#include "ShapeContact.h"
#include "PlatformerBackground.h"
#include "Timer.h"
#include "MenuSystem.h"
#include "HUD.h"
class TabEditWindow2D;
class MapEditorSystem2D;
class MenuEditorSystem;
using namespace std;

#pragma comment(lib, "shell32.lib")

//================================================================================================================
//================================================================================================================
class Environment2DMapHelper
{
public:
	
	Environment2DMapHelper();
	~Environment2DMapHelper();
	
	//
	// World Updating (Creating, Saving, Loading)
	//
	
	static void AddMapName(string world_name, string name);
	static void LoadWorld(string world_name, bool InEditor);
	static void LoadMapInWorld(string world_name, string name, bool InEditor);
	static void LoadMap(string& mapName, string filename, bool InEditor);
	static void LoadWorld(string& worldName, string filename, bool InEditor); // Loads a world from an xml file and then loads all the maps
	static void CreateWorld(string world_name);
	static void CreateWorld();
	static void NewMapTopdown(string world_name, string name, Map2DType mt, bool InEditor);
	static void NewMapPlatformer(string world_name, string name, bool InEditor);
	static void SaveMap(string filename);
	static void RemoveMap(string mapName);
	static void CreateMapTopdown(int x, int y, bool fow, int fowRadius, bool fowFlashlight, string world_name, string name, bool bIso, bool InEditor);
	static void CreateMapPlatformer(int x, int y, string world_name, string name, bool InEditor);
	static void SaveWorld(string filename);
	
	//
	// Sprite Updating
	//
	
	// Attempts to locate a sprite based on the x,y position and returns if a sprite was clicked
	static bool SpriteClicked(AISprite*& spr, int x, int y);
	static bool GetSpriteID(int& spriteID, int x, int y);
	static bool MoveSpriteStart(AISprite*& spr, int& spriteID, int x, int y);
	static void MoveSpriteEnd(AISprite*& spr, int spriteID, int x, int y);
	static bool CloneSpriteStart(AISprite*& spr, int x, int y);
	static void MoveSprite(AISprite*& spr, int x, int y);
	static void CloneSpriteEnd(AISprite*& spr, int x, int y);
	static void AddSprite(string spriteBaseTextureName, int x, int y);
	static void DeleteSprite(AISprite* spr);
	static void DeleteSprite(int id);
	// ResizeSpriteUpdate goes in update loop to resize the sprite with the keyboard
	// I don't like pressing the keys on the keyboard for this change to use GUI
	static void ResizeSpriteUpdate(
		AISprite*& spr,
		bool updatePosition,
		bool updateSize,
		bool goFast,
		bool pressedLeftKey,
		bool pressedRightKey,
		bool pressedUpKey,
		bool pressedDownKey
	);
	static void HighlightSprite(AISprite*& spriteWithBox, Sprite*& highlightBox, bool& boxCreated, int x, int y);
	//static AISprite* ClickedSprite(int x, int y, bool& clicked);
	static AISprite* GetSpriteByID(int spriteID);
	static void RemoveDisplaySprite();
	static void RemoveCloneDisplaySprite();
	static void SetCanRenderDisplaySprite(bool canRender);
	static void SetCanRenderCloneDisplaySprite(bool canRender);
	static void ToggleSelectedSpriteBackground(bool background, int spriteID);
	static void ToggleSelectedSpriteEntity(bool entity, int spriteID);
	static void ToggleSelectedSpriteInvisible(bool invisible, int spriteID);
	static void ToggleSelectedSpriteHard(bool hard, int spriteID);
	static void ToggleSelectedSpriteNoHit(bool nohit, int spriteID);
	static void ToggleSelectedSpritePhysicsType(bool isstatic, int spriteID);
	static void ToggleSelectedSpriteTouch(bool touch, int spriteID);
	static void ToggleSelectedSpriteTeleport(bool teleport, int spriteID);
	static bool SelectedSpriteIsBackground(int spriteID);
	static bool SelectedSpriteIsEntity(int spriteID);
	static bool SelectedSpriteIsInvisible(int spriteID);
	static bool SelectedSpriteIsHard(int spriteID);
	static bool SelectedSpriteIsNoHit(int spriteID);
	static bool SelectedSpriteIsStatic(int spriteID);
	static bool SelectedSpriteIsTouch(int spriteID);
	static bool SelectedSpriteIsTeleportSprite(int spriteID);
	static void SetSelectedSpriteVision(int vision, int spriteID);
	static void SetSelectedSpriteDepth(int depth, int spriteID);
	static void SetSelectedSpriteTextureName(string textureName, int spriteID);
	static void SetSelectedSpriteAnimationName(string animationName, int spriteID);
	static void SetSelectedSpriteDefaultSequence(int sequenceNum, int spriteID);
	static void SetSelectedSpriteAnimationSpeed(int speed, int spriteID);
	static void SetSelectedSpriteCollideMsg(string collideMsg, int spriteID);
	static void SetSelectedSpriteHPMax(int value, int spriteID);
	static void SetSelectedSpriteHP(int value, int spriteID);
	static void SetSelectedSpriteMana(int value, int spriteID);
	static void SetSelectedSpriteGold(int value, int spriteID);
	static void SetSelectedSpriteStrength(int value, int spriteID);
	static void SetSelectedSpriteSpeed(int value, int spriteID);
	static void SetSelectedSpriteDefense(int value, int spriteID);
	static void SetSelectedSpriteExperience(int value, int spriteID);
	static void SetSelectedSpriteLevel(int value, int spriteID);
	static void SetSelectedSpriteScriptName(string scriptName, int spriteID);
	static void SetSelectedSpriteTeleportMapName(string mapName, int spriteID);
	static void SetSelectedSpriteTeleportMapX(int value, int spriteID);
	static void SetSelectedSpriteTeleportMapY(int value, int spriteID);
	static void SetSelectedSpriteAlwaysSeenByPlayer(bool value, int spriteID);
	static float GetSelectedSpriteXPos(int spriteID);
	static float GetSelectedSpriteYPos(int spriteID);
	static int GetSelectedSpriteVision(int spriteID);
	static int GetSelectedSpriteDepth(int spriteID);
	static string GetSelectedSpriteTextureName(int spriteID);
	static string GetSelectedSpriteAnimationName(int spriteID);
	static int GetSelectedSpriteDefaultSequence(int spriteID);
	static int GetSelectedSpriteAnimationSpeed(int spriteID);
	static string GetSelectedSpriteCollideMsg(int spriteID);
	static int GetSelectedSpriteHPMax(int spriteID);
	static int GetSelectedSpriteHP(int spriteID);
	static int GetSelectedSpriteMana(int spriteID);
	static int GetSelectedSpriteGold(int spriteID);
	static int GetSelectedSpriteStrength(int spriteID);
	static int GetSelectedSpriteSpeed(int spriteID);
	static int GetSelectedSpriteDefense(int spriteID);
	static int GetSelectedSpriteExperience(int spriteID);
	static int GetSelectedSpriteLevel(int spriteID);
	static string GetSelectedSpriteScriptName(int spriteID);
	static string GetSelectedSpriteTeleportMapName(int spriteID);
	static int GetSelectedSpriteTeleportMapX(int spriteID);
	static int GetSelectedSpriteTeleportMapY(int spriteID);
	static bool GetSelectedSpriteAlwaysSeenByPlayer(int spriteID);
	static void RevertSelectedSpriteAnimation(int spriteID);
	static void UpdateSelectedSpriteAnimation(int spriteID);
	static void UpdateSelectedSpriteMeshTexture(int spriteID);
	static void OpenSelectedSpriteScript(string scriptName);

	//
	// Tile Updating
	//
	
	static void UpdateTileTexture(string textureName, int x, int y);
	static void NullTileAnimation(int x, int y);
	static void UpdateTileAnimation(string animProfileName, int animSpeed, int x, int y);
	static void UpdateTileHardness(bool bHard, int x, int y);
	static void MakeTileHard(int x, int y);
	static void RemoveTileTexture(int x, int y);
	
	//
	// Functions for getting stuff from a map or setting things in a map
	//
	
	static XMFLOAT3 GetPlayerPosition();
	static XMFLOAT2 GetSectionScrollOffset(int sectionID);
	static void ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY);
	static void SelectTopdownSectionID(int& sectionID, int x, int y);
	static TopdownTile* GetSelectedTopdownTile(int x, int y);
	static PlatformerTile* GetSelectedPlatformerTile(int x, int y);
	static void GetMapWidthAttribute(int& width);
	static void GetMapHeightAttribute(int& height);
	static void GetMapVisionAttribute(int& vision);
	static void GetMapMusicAttribute(string& music);
	static void GetMapFOWAttribute(bool& fow);
	static void GetMapFOWRadiusAttribute(int& fowRadius);
	static void GetMapFOWFlashlightAttribute(bool& fowFlashlight);
	static void GetMapFOWUncoveredAttribute(bool& fowUncovered);
	static void GetNumSpritesRendered(int& spritesRendered);
	static void VisibleTileCount(int& visibleTileCount);
	static void GetSpriteTotal(int& spriteTotal);
	static void GetWorldAndMapName(string& activeMapName, string& activeWorldName);
	static void SetActivePlatformerWorld();
	static void SetActiveRenderedMap(string world_name, string map_name);
	static void SetActiveMapName(string name);
	static bool HasMaps();
	static int GetNumSpritesInMap();
	static AISprite* GetSpriteInMap(int i);
	static void AddSpriteInMap(AISprite* spr, bool noIndex = false);
	static void AddPlayerInMap(AISprite* spr, bool noIndex = false);
	static void RemoveSpriteInMap(AISprite* spr);
	static void ToggleMapHardness(bool bEnable);
	static void SetToolWindowWidth(int width);
	static void SetMapVisionAttribute(int vision);
	static void SetMapMusicAttribute(string music);
	static void SetMapFOWAttribute(bool fow);
	static void SetMapFOWRadiusAttribute(int fowRadius);
	static void SetMapFOWFlashlightAttribute(bool fowFlashlight);
	static void SetMapFOWUncoveredAttribute(bool fowUncovered);
	static void SetMapNameAttribute(string mapName);
	static void UpdateNewMapName(string world_name, string old_map_name, string new_map_name);
	
	static TopdownMap* ActiveTopdownMap();
	static PlatformerMap* ActivePlatformerMap();
};
//================================================================================================================
//================================================================================================================
#endif//__ENVIRONMENT2DMAPHELPER_H