//===============================================================================================================================
// EditorHelper2D
//
//===============================================================================================================================
// History
//
// -Created 6/22/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __EDITORHELPER2D_H
#define __EDITORHELPER2D_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include <Windows.h>
#include "Window.h"
#include "EngineOptions.h"
#include "MapEditorSystem2D.h"
#include "HUDEditorSystem.h"
#include "MenuEditorSystem.h"

//===============================================================================================================================
//===============================================================================================================================

class EditorHelper2D
{
public:

	EditorHelper2D();
	~EditorHelper2D();

	void Init();
	
	// Loads the game directory directly from the INI file
	void LoadINIGameDirectory();
	
	// Loads the game directory from the open and create game directory buttons
	void LoadGameDirectory(string gameName, bool isNew);
	
	void SaveGameDirectory();
	void CloseGameDirectory();
	
	void CreateWorld(string world_name);
	void LoadWorld(string& worldName, string filename, bool InEditor);
	void SaveWorld(string filename);
	void RemoveMap(string mapName);
	void LoadMap(string& mapName, string filename, bool InEditor);
	void SaveMap(string filename);
	void NewMapTopdown(string world_name, string name, int x, int y, bool fow, int fowRadius, bool fowFlashlight, bool bIso, bool InEditor);
	void NewMapPlatformer(string world_name, string name, int x, int y, bool InEditor);
	void SetActiveRenderedMap(string world_name, string map_name);
	void FillTileCache(vector<string>& cacheList);
	void FillSpriteCache(vector<string>& cacheList);
	void FillMenuCache(vector<string>& cacheList);
	void FillHUDCache(vector<string>& cacheList);
	
	string FullTileImagePath(string imageName);
	string FullSpriteImagePath(string imageName);
	string FullMenuImagePath(string imageName);
	string FullHUDImagePath(string imageName);
	
	string SelectToolNormalImageName();
	string SelectToolHighlightImageName();

	HWND& ToolHwnd() { return mToolHwnd; }
	HWND ToolHwnd() const { return mToolHwnd; }

	D3D* GetD3D() { return mD3DSystem; }

	bool GameWindowInForeground();
	
	// Getter Methods
public:
	
	EngineOptions* GetEngineOptions() { return mEngineOptions; }
	static bool GameDirectoryCreated() { return bGameDirectoryCreated; }
	static GameDirectory2D* GetGameDirectory2D() { return mGameDirectory; }
	MapEditorSystem2D*& GetMapEditorSystem2D() { return mMapEditorSystem2D; }
	MapEditorSystem2D* GetMapEditorSystem2D() const { return mMapEditorSystem2D; }
	HUDEditorSystem*& GetHUDEditorSystem() { return mHUDEditorSystem; }
	HUDEditorSystem* GetHUDEditorSystem() const { return mHUDEditorSystem; }
	MenuEditorSystem*& GetMenuEditorSystem() { return mMenuEditorSystem; }
	MenuEditorSystem* GetMenuEditorSystem() const { return mMenuEditorSystem; }
	InventoryEditorSystem2D*& GetInventoryEditorSystem2D() { return mInventoryEditorSystem2D; }
	InventoryEditorSystem2D* GetInventoryEditorSystem2D() const { return mInventoryEditorSystem2D; }
	
private:

	HWND mToolHwnd;
	static GameDirectory2D* mGameDirectory;
	HUDEditorSystem* mHUDEditorSystem;
	MenuEditorSystem* mMenuEditorSystem;
	MapEditorSystem2D* mMapEditorSystem2D;
	InventoryEditorSystem2D* mInventoryEditorSystem2D;
	D3D* mD3DSystem;
	EngineOptions* mEngineOptions;
	static bool bGameDirectoryCreated;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__EDITORHELPER2D_H