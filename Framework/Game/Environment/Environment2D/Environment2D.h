//================================================================================================================
// Environment2D.h
//
//================================================================================================================
// History
//
// Created on 10/3/2014 by Dustin Watson
//================================================================================================================
#ifndef __ENVIRONMENT2D_H
#define __ENVIRONMENT2D_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include <memory>
#include <sstream>
#include <Windows.h>
#include <windowsx.h>
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
#include "Window.h"
#include "TextureManager.h"
#include "CreateGameDirectory.h"
#include "ZShadeMessageCenter.h"
#include "MemoryMgr.h"
#include "ShapeContact.h"
#include "WinToolbar.h"
#include "ZShadeResource.h"
#include "PlatformerBackground.h"
#include "Timer.h"
#include "MenuSystem.h"
#include "HUD.h"
#include "TopdownWorld.h"
#include "PlatformerWorld.h"
#include "DirectionalLight.h"
#include "PlayerInventory2D.h"
//class TabEditWindow2D;
class MapEditorSystem2D;
class MenuEditorSystem;
class HUDEditorSystem;
class InventoryEditorSystem2D;
using namespace std;
using namespace GUIComponents;

//===============================================================================================================================
//===============================================================================================================================
class Environment2D
{
public:
	
	Environment2D(EngineOptions* engineOptions);
	~Environment2D();
	
	bool Init();
	void Shutdown();
	int Run();
	
	// Get the specified world
	TopdownWorld* GetTopdownWorld(string world_name);
	PlatformerWorld* GetPlatformerWorld(string world_name);
	STopdownWorld* GetSTopdownWorld(string world_name);
	SPlatformerWorld* GetSPlatformerWorld(string world_name);
	// Take a map off the active world
	void RemoveMap(string mapName);
	// Gets the name of all worlds and maps available
	void GrabWorldNames(vector<string>& names);
	void GrabMapNames(string world_name, vector<string>& names);
	void LoadWorld(string world_name, STopdownWorld* sw);
	void LoadWorld(string world_name, SPlatformerWorld* sw);
	void UpdateNewMapName(string world_name, string old_map_name, string new_map_name);
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	
	MapEditorSystem2D* GetMapEditorSystem2D() { return m_MapEditorSystem2D; }
	MenuEditorSystem* GetMenuEditorSystem() { return m_MenuEditorSystem; }
	HUDEditorSystem* GetHUDEditorSystem() { return m_HUDEditorSystem; }
	InventoryEditorSystem2D* GetInventoryEditorSystem2D() { return mInventoryEditorSystem2D; }
	
	void SetMapEditorSystem(MapEditorSystem2D* mapEditorSystem) { m_MapEditorSystem2D = mapEditorSystem; }
	void SetMenuEditorSystem(MenuEditorSystem* menuEditorSystem) { m_MenuEditorSystem = menuEditorSystem; }
	void SetHUDEditorSystem(HUDEditorSystem* hudEditorSystem) { m_HUDEditorSystem = hudEditorSystem; }
	void SetInventoryEditorSystem2D(InventoryEditorSystem2D* inventoryEditorSystem) { mInventoryEditorSystem2D = inventoryEditorSystem; }
	
	map<string, STopdownWorld*> WorldsTopdown() { return m_WorldsTopdown; }
	map<string, SPlatformerWorld*> WorldsPlatformer() { return m_WorldsPlatformer; }
	
	string& ActiveMenuName() { return m_ActiveMenuName; }
	string ActiveMenuName() const { return m_ActiveMenuName; }
	string& ActiveHUDName() { return m_ActiveHUDName; }
	string ActiveHUDName() const { return m_ActiveHUDName; }
	
	string& ActiveWorldName() { return m_ActiveWorldName; }
	string ActiveWorldName() const { return m_ActiveWorldName; }
	
	string& ActiveMapName() { return m_ActiveMapName; }
	string ActiveMapName() const { return m_ActiveMapName; }
	
	bool& WorldLoaded() { return m_WorldLoaded; }
	bool WorldLoaded() const { return m_WorldLoaded; }
	
	bool& MapLoaded() { return m_MapLoaded; }
	bool MapLoaded() const { return m_MapLoaded; }
	
	bool& InSectionPicker() { return m_InSectionPicker; }
	bool InSectionPicker() const { return m_InSectionPicker; }
	
	HUD*& GetHUDSystem() { return m_HUDSystem; }
	HUD* GetHUDSystem() const { return m_HUDSystem; }
	MenuSystem*& GetMenuSystem() { return m_MenuSystem; }
	MenuSystem* GetMenuSystem() const { return m_MenuSystem; }
	void SetGameDirectory2D(GameDirectory2D* gd) { m_GameDirectory2D = gd; }
	GameDirectory2D* GetGD2D() { return m_GameDirectory2D; }
	EngineOptions* GetEngineOptions()  { return mEngineOptions; }
	D3D* GetD3D() { return m_D3DSystem; }
	Camera* GetCamera() { return m_CameraSystem; }

	void SetBackground(PlatformerBackground* background) { m_Background = background; }

	bool& EditorTypeMap() { return bInEditorMap; }
	bool& EditorTypeHUD() { return bInEditorHUD; }
	bool& EditorTypeMenu() { return bInEditorMenu; }
	
	PlayerInventory2D*& PlayerInventory() { return m_Inventory; }
	PlayerInventory2D* PlayerInventory() const { return m_Inventory; }
	bool& InventoryActive() { return bInventoryActive; }
	bool InventoryActive() const { return bInventoryActive; }
	
	//Engine stuff
protected:
	
	//TabEditWindow2D* m_TabEditWindow2D;
	MapEditorSystem2D* m_MapEditorSystem2D;
	MenuEditorSystem* m_MenuEditorSystem;
	HUDEditorSystem* m_HUDEditorSystem;
	InventoryEditorSystem2D* mInventoryEditorSystem2D;
	
	bool Update();
	bool RenderMaster();
	
	//Game related stuff
protected:
	
	Keyboard* keyboard;
	Mouse* mouse;
	D3D* m_D3DSystem;
	Camera* m_CameraSystem;
	ZShadeSandboxGlobal::Timer m_Timer;
	GameDirectory2D* m_GameDirectory2D;
	EngineOptions* mEngineOptions;
	HUD* m_HUDSystem;
	MenuSystem* m_MenuSystem;
	PlayerInventory2D* m_Inventory;
	bool m_GameInitialized;
	
	// If the empty constructor is called then we need to call InitBase from Init
	bool m_EnvBaseInit;
	
	bool bInventoryActive;
	bool bInEditorMap;
	bool bInEditorMenu;
	bool bInEditorHUD;
	
	string m_ActiveMenuName;
	string m_ActiveHUDName;
	string m_ActiveWorldName;
	string m_ActiveMapName;
	bool m_WorldLoaded;
	bool m_MapLoaded;
	bool m_InSectionPicker;
	PlatformerBackground* m_Background;
	
	TopdownWorld* m_ActiveTopdownWorld;
	TopdownMap* m_ActiveTopdownMap;

	PlatformerWorld* m_ActivePlatformerWorld;
	PlatformerMap* m_ActivePlatformerMap;

	map<string, STopdownWorld*> m_WorldsTopdown;
	map<string, SPlatformerWorld*> m_WorldsPlatformer;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__ENVIRONMENT2D_H