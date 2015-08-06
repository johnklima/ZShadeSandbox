//======================================================================================================
// TopdownWorld.h
//
//======================================================================================================
// History
//
// Created 7/13/2015 by Dustin Watson
//======================================================================================================
#ifndef __TOPDOWNWORLD_H
#define __TOPDOWNWORLD_H
//=================================================================================================================
//=================================================================================================================
//C/C++ Includes
#include <Windows.h>
#include <string>
#include <map>
#include "D3D.h"
#include "Input.h"
#include "Camera.h"
#include "TopdownMap.h"
#include "GameDirectory.h"
#include "MemoryMgr.h"
#include "PhysicsWorld2D.h"
using namespace std;
//=================================================================================================================
//=================================================================================================================
class TopdownWorld
{
public:

	TopdownWorld(D3D* d3d);
	~TopdownWorld();

	void NewMap(string map_name, GameDirectory2D* gd, Map2DType mt, bool InEditor);
	void LoadMap(string& map_name, string filename, GameDirectory2D* gd, bool InEditor);
	void RemoveMap(string activeMapName);
	
	void UpdateScrollOffset(XMFLOAT2 offset);
	void Update(float dt, Keyboard* keyboard);
	
	bool Render(Camera* camera);
	bool RenderSectionPicker(Camera* camera);

	void ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY);
	
	void AddSprite(AISprite* s);
	void AddSpriteAnimation(string profile_name, GameDirectory2D* gd, AISprite* s);
	void LoadSpritesMain();
	void RunSpritesMain();
	void GetPlayerPos(float& x, float &y);
	void SetMapToRender(string p_map_to_render);
	void InitAISprites(GameDirectory2D* gd);
	void RemoveSprite(AISprite* spr);
	//void CreateSpritesText(GameDirectory2D* gd, HWND hwnd);
	int NumSpritesInMap();
	
	AISprite* GetSpriteByID(int id);

	TopdownMap* GetMap(string mapName);

	// Updates the first key value in the std::map to the new map name
	// by removing the old map and adding a copy in its place with the new map name
	void UpdateMapName(string mapName);

	map<string, TopdownMap*> GetWorldMaps() { return m_world_maps; }

	void SetD3DSystem(D3D* d3d) { m_D3DSystem = d3d; }

	D3D* D3DSystem() { return m_D3DSystem; }

	string MapToRender() const { return m_map_to_render; }

private:

	D3D* m_D3DSystem;
	unique_ptr<PhysicsWorld2D> m_physicsWorld;

	string m_map_to_render;
	
	bool m_sprites_main_init;

	map<string, TopdownMap*> m_world_maps;
};
//=================================================================================================================
struct STopdownWorld
{
	vector<string> m_mapNames;
	TopdownWorld* m_World;
	string sWorldName;
};
//=================================================================================================================
//=================================================================================================================
#endif//__TOPDOWNWORLD_H