//==================================================================================================================================
// PlatformerWorld.h
//
//==================================================================================================================================
// History
//
// -Created on 1/16/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __PLATFORMERWORLD_H
#define __PLATFORMERWORLD_H
//==================================================================================================================================
//==================================================================================================================================
//C/C++ Includes
#include <Windows.h>
#include <string>
#include <map>
#include "D3D.h"
#include "Input.h"
#include "Camera.h"
#include "PlatformerMap.h"
#include "GameDirectory.h"
#include "MemoryMgr.h"
#include "PhysicsWorld2D.h"
using namespace std;
//==================================================================================================================================
//==================================================================================================================================
class PlatformerWorld
{
public:
	
	PlatformerWorld(D3D* d3d);
	~PlatformerWorld();
	
	void InitPhysicsWorld();
	
	void NewMap(string map_name, GameDirectory2D* gd, bool InEditor);
	void LoadMap(string& map_name, string filename, GameDirectory2D* gd, bool InEditor);
	void RemoveMap(string mapName);
	
	void Update(float dt, Keyboard* keyboard, HWND hwnd);
	
	void UpdateScrollOffset(XMFLOAT2 offset);
	void UpdateScrollOffset(float x, float y);
	
	void SetBackgroundScreenWidth(int backgroundScreenWidth);
	
	bool Render(Camera* camera);
	
	void PlayerMovingDir(bool& moved, int& dirUD, int& dirLR);
	XMFLOAT3 MatchingTilePos(float x, float y);
	void ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY);
	
	void AddSprite(AISprite* s);
	void AddSpriteAnimation(string profile_name, GameDirectory2D* gd, AISprite* s);
	void LoadSpritesMain();
	void RunSpritesMain();
	void GetPlayerPos(float& x, float &y);
	void SetMapToRender(string p_map_to_render);
	void InitAISprites(GameDirectory2D* gd);
	void RemoveSprite(AISprite* spr);
	void CreateSpritesText(GameDirectory2D* gd, HWND hwnd);
	int NumSpritesInMap();
	
	AISprite* GetSpriteByID(int id);
	PlatformerMap* GetMap(string mapName);
	map<string, PlatformerMap*> GetWorldMaps() { return m_world_maps; }
	void SetD3DSystem(D3D* d3d) { m_D3DSystem = d3d; }
	D3D* D3DSystem() { return m_D3DSystem; }
	string MapToRender() const { return m_map_to_render; }

private:
	
	D3D* m_D3DSystem;
	unique_ptr<PhysicsWorld2D> m_physicsWorld;
	bool m_sprites_main_init;
	string m_map_to_render;
	map<string, PlatformerMap*> m_world_maps;
};
//==================================================================================================================================
struct SPlatformerWorld
{
	// Sub parts of the map mapped to a map name
	map<string, string> m_SubMapNames;
	vector<string> m_mapNames;
	PlatformerWorld* m_World;
	string sWorldName;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PLATFORMERWORLD_H