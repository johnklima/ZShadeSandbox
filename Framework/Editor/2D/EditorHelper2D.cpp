#include "EditorHelper2D.h"
#include "Environment.h"
#include "DefaultMaterials.h"
#include "InventoryEditorSystem2D.h"
#include "AudioSystem.h"
#include "Scripting.h"
#include <Windows.h>
//===============================================================================================================================
//===============================================================================================================================
GameDirectory2D* EditorHelper2D::mGameDirectory = 0;
bool EditorHelper2D::bGameDirectoryCreated = false;
//===============================================================================================================================
EditorHelper2D::EditorHelper2D()
{
	Init();
}
//===============================================================================================================================
EditorHelper2D::~EditorHelper2D()
{
}
//===============================================================================================================================
void EditorHelper2D::Init()
{
	mD3DSystem = ZShadeSandboxEnvironment::Environment::Env2D()->GetD3D();
	mEngineOptions = ZShadeSandboxEnvironment::Environment::Env2D()->GetEngineOptions();
	mMapEditorSystem2D = new MapEditorSystem2D(ZShadeSandboxEnvironment::Environment::Env2D());
	mHUDEditorSystem = new HUDEditorSystem(DXGameWindow::Env());
	mMenuEditorSystem = new MenuEditorSystem(DXGameWindow::Env());
	mInventoryEditorSystem2D = new InventoryEditorSystem2D(ZShadeSandboxEnvironment::Environment::Env2D());
}
//===============================================================================================================================
void EditorHelper2D::FillTileCache(vector<string>& cacheList)
{
	cacheList = CGlobal::CacheNames(mEngineOptions->m_GD2D->m_tiles_path);
}
//===============================================================================================================================
void EditorHelper2D::FillSpriteCache(vector<string>& cacheList)
{
	cacheList = CGlobal::CacheNames(mEngineOptions->m_GD2D->m_sprites_path);
}
//===============================================================================================================================
void EditorHelper2D::FillMenuCache(vector<string>& cacheList)
{
	cacheList = CGlobal::CacheNames(mEngineOptions->m_GD2D->m_menu_sprites_path);
}
//===============================================================================================================================
void EditorHelper2D::FillHUDCache(vector<string>& cacheList)
{
	cacheList = CGlobal::CacheNames(mEngineOptions->m_GD2D->m_hud_path);
}
//===============================================================================================================================
string EditorHelper2D::FullTileImagePath(string imageName)
{
	string imgPath = mEngineOptions->m_GD2D->m_tiles_path;
	imgPath += "\\";
	imgPath += imageName;
	return imgPath;
}
//===============================================================================================================================
string EditorHelper2D::FullSpriteImagePath(string imageName)
{
	string imgPath = mEngineOptions->m_GD2D->m_sprites_path;
	imgPath += "\\";
	imgPath += imageName;
	return imgPath;
}
//===============================================================================================================================
string EditorHelper2D::FullMenuImagePath(string imageName)
{
	string imgPath = mEngineOptions->m_GD2D->m_menu_sprites_path;
	imgPath += "\\";
	imgPath += imageName;
	return imgPath;
}
//===============================================================================================================================
string EditorHelper2D::FullHUDImagePath(string imageName)
{
	string imgPath = mEngineOptions->m_GD2D->m_hud_path;
	imgPath += "\\";
	imgPath += imageName;
	return imgPath;
}
//===============================================================================================================================
string EditorHelper2D::SelectToolNormalImageName()
{
	return mGameDirectory->m_required_textures + "\\stamp_normal.png";
}
//===============================================================================================================================
string EditorHelper2D::SelectToolHighlightImageName()
{
	return mGameDirectory->m_required_textures + "\\stamp_highlight.png";
}
//===============================================================================================================================
bool EditorHelper2D::GameWindowInForeground()
{
	return (GetForegroundWindow() == mEngineOptions->hwnd);
}
//===============================================================================================================================
void EditorHelper2D::SaveGameDirectory()
{
	// Update the ini file with the game folder
	ZShadeINIParser iniSandboxParser("ZShadeSandbox.ini", true);
	iniSandboxParser.WriteString("Editor", "GameFolder", mGameDirectory->GAME_FOLDER);
}
//===============================================================================================================================
void EditorHelper2D::CloseGameDirectory()
{
	EditorHelper2D::bGameDirectoryCreated = false;
	delete mGameDirectory;
	mGameDirectory = 0;
	
	// Make sure we no longer have a map or world
}
//===============================================================================================================================
void EditorHelper2D::LoadINIGameDirectory()
{
	ZShadeINIParser iniSandboxParser("ZShadeSandbox.ini", true);
	string gameType = iniSandboxParser.GetString("Editor", "Type2D");
	string gameName = iniSandboxParser.GetString("Editor", "GameFolder");
	
	//Full path of game directory
	string username = CGlobal::GrabUserName();
	BetterString gameFolder = "C:\\Users\\";
	gameFolder += username;
	gameFolder += "\\AppData\\Roaming\\";
	gameFolder += mEngineOptions->m_GameFolderName;

	mGameDirectory = CGlobal::CreateGD2D(gameFolder, mEngineOptions->m_GameType2D);

	mEngineOptions->m_GD2D = mGameDirectory;

	mMenuEditorSystem->SetGameDirectory2D(mGameDirectory);
	mHUDEditorSystem->SetGameDirectory2D(mGameDirectory);
	mMapEditorSystem2D->SetGameDirectory(mGameDirectory);

	EditorHelper2D::bGameDirectoryCreated = true;

	// Load the stamp tools and display sprites
	mMenuEditorSystem->LoadRequiredSprites();
	mHUDEditorSystem->LoadRequiredSprites();
	mMapEditorSystem2D->LoadRequiredSprites();

	DefaultMaterials materials;
	materials.Build(mD3DSystem, mGameDirectory->m_required_textures);
	
	/*  Test out the inventory system
		Create an inventory that is 5x5 grid for regular and magic items
		
		  Items      Magic
		* * * * *  * * * * *
		* * * * *  * * * * *
		* * * * *  * * * * *
		* * * * *  * * * * *
		* * * * *  * * * * *
	*
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	if (env2D->PlayerInventory())
	{
		env2D->PlayerInventory()->Init(
			mGameDirectory->m_required_textures,// path of the stamp tool
			mGameDirectory->m_required_textures,// path of the background image
			"InventoryImage1366x768.png",
			mEngineOptions->m_screenWidth,
			mEngineOptions->m_screenHeight,
			64,
			64,
			64,
			5, 5,// Amount of regular items per row/col
			5, 5,// Amount of magic items per row/col
			0, 0,// Padding of regular items (amount of space between items)
			0, 0,// Padding of magic items (amount of space between items)
			XMFLOAT2(64, 64),
			XMFLOAT2(64 + (64 * 7), 64)
		);
		
		// Add some test items to the inventory
		env2D->PlayerInventory()->AddItemRegular(
			mGameDirectory->m_sprites_path,
			"sword.png",
			"sword"
		);
		env2D->PlayerInventory()->AddItemRegular(
			mGameDirectory->m_sprites_path,
			"heart.png",
			"heart"
		);
		env2D->PlayerInventory()->AddItemMagic(
			mGameDirectory->m_sprites_path,
			"fireball.png",
			"fireball"
		);
		
		// Position the items on the grid when finished
		env2D->PlayerInventory()->PositionItemsOnGrid();
	}*/
	
	if (BetterString(gameType) == "Topdown")
	{
		mEngineOptions->m_GameType2D = TOPDOWN;

		//Load the Fog of War data
		FogData::NewInstance(mD3DSystem);
		FogData::Instance()->InitializeTable();
		//FogData::Instance()->LoadTextures(mGameDirectory->m_required_textures);
	}
	else if (BetterString(gameType) == "Platformer")
	{
		mEngineOptions->m_GameType2D = PLATFORMER;

		PlatformerBackground* background = new PlatformerBackground(mD3DSystem);
		background->AddBackground(mGameDirectory->m_sprites_path, "frontBackground.png", 1600, 768, false, true, 3, 0);
		ZShadeSandboxEnvironment::Environment::Env2D()->SetBackground(background);
	}

	// Initialize Audio System
	AudioSystem::NewInstance(mGameDirectory, mEngineOptions->hwnd);

	// Initialize lua scripting system
	Scripting::NewInstance(mD3DSystem, mGameDirectory);
}
//===============================================================================================================================
void EditorHelper2D::LoadGameDirectory(string gameName, bool isNew)
{
	if (isNew)
	{
		//Full path of game directory
		string username = CGlobal::GrabUserName();
		BetterString gameFolder = "C:\\Users\\";
		gameFolder += username;
		gameFolder += "\\AppData\\Roaming\\";
		gameFolder += gameName;
		mGameDirectory = CGlobal::CreateGD2D(gameFolder, mEngineOptions->m_GameType2D);
		mEngineOptions->m_GameFolderName = gameName;
	}
	else
	{
		mGameDirectory = CGlobal::CreateGD2D(gameName, mEngineOptions->m_GameType2D);

		// Strip the game folder name from the path
		BetterString bs(gameName);
		vector<string> strList = bs.split('\\');
		mEngineOptions->m_GameFolderName = strList[strList.size() - 1];
	}

	mEngineOptions->m_GD2D = mGameDirectory;
	
	mMenuEditorSystem->SetGameDirectory2D(mGameDirectory);
	mHUDEditorSystem->SetGameDirectory2D(mGameDirectory);
	mMapEditorSystem2D->SetGameDirectory(mGameDirectory);

	EditorHelper2D::bGameDirectoryCreated = true;

	// Load the stamp tools and display sprites
	mMenuEditorSystem->LoadRequiredSprites();
	mHUDEditorSystem->LoadRequiredSprites();
	mMapEditorSystem2D->LoadRequiredSprites();
	
	DefaultMaterials materials;
	materials.Build(mD3DSystem, mGameDirectory->m_required_textures);
	
	/*  Test out the inventory system
		Create an inventory that is 5x5 grid for regular and magic items
		
		  Items      Magic
		* * * * *  * * * * *
		* * * * *  * * * * *
		* * * * *  * * * * *
		* * * * *  * * * * *
		* * * * *  * * * * *
	*
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	if (env2D->PlayerInventory())
	{
		env2D->PlayerInventory()->Init(
			mGameDirectory->m_required_textures,// path of the stamp tool
			mGameDirectory->m_required_textures,// path of the background image
			"InventoryImage1366x768.png",
			mEngineOptions->m_screenWidth,
			mEngineOptions->m_screenHeight,
			64,
			64,
			64,
			5, 5,// Amount of regular items per row/col
			5, 5,// Amount of magic items per row/col
			0, 0,// Padding of regular items (amount of space between items)
			0, 0, // Padding of magic items (amount of space between items)
			XMFLOAT2(64, 64),
			XMFLOAT2(64 + (64 * 7), 64)
		);
		
		// Add some test items to the inventory
		env2D->PlayerInventory()->AddItemRegular(
			mGameDirectory->m_sprites_path,
			"sword.png",
			"sword"
		);
		env2D->PlayerInventory()->AddItemRegular(
			mGameDirectory->m_sprites_path,
			"heart.png",
			"heart"
		);
		env2D->PlayerInventory()->AddItemMagic(
			mGameDirectory->m_sprites_path,
			"fireball.png",
			"fireball"
		);
		
		// Position the items on the grid when finished
		env2D->PlayerInventory()->PositionItemsOnGrid();
	}*/
	
	if (isNew)
	{
		string gameType;
		switch (mEngineOptions->m_GameType2D)
		{
			case TOPDOWN:
			{
				gameType = "Topdown";

				//Load the Fog of War data
				FogData::NewInstance(mD3DSystem);
				FogData::Instance()->InitializeTable();
				//FogData::Instance()->LoadTextures(mGameDirectory->m_required_textures);
			}
			break;
			case PLATFORMER:
			{
				gameType = "Platformer";

				PlatformerBackground* background = new PlatformerBackground(mD3DSystem);
				background->AddBackground(mGameDirectory->m_sprites_path, "frontBackground.png", 1600, 768, false, true, 3, 0);
				ZShadeSandboxEnvironment::Environment::Env2D()->SetBackground(background);
			}
			break;
		}
		//iniSandboxParser.WriteString("Editor", "Type2D", gameType);
	}
	else
	{
		// Read the Game.ini file
		ZShadeINIParser iniParser(mGameDirectory->m_game_ini_path, false);
		string gameType = iniParser.GetString("GAME", "GameType");

		if (BetterString(gameType) == "Topdown")
		{
			mEngineOptions->m_GameType2D = TOPDOWN;

			//Load the Fog of War data
			FogData::NewInstance(mD3DSystem);
			FogData::Instance()->InitializeTable();
			//FogData::Instance()->LoadTextures(mGameDirectory->m_required_textures);
		}
		else if (BetterString(gameType) == "Platformer")
		{
			mEngineOptions->m_GameType2D = PLATFORMER;

			PlatformerBackground* background = new PlatformerBackground(mD3DSystem);
			background->AddBackground(mGameDirectory->m_sprites_path, "frontBackground.png", 1600, 768, false, true, 3, 0);
			ZShadeSandboxEnvironment::Environment::Env2D()->SetBackground(background);
		}
	}

	// Initialize Audio System
	AudioSystem::NewInstance(mGameDirectory, mEngineOptions->hwnd);

	// Initialize lua scripting system
	Scripting::NewInstance(mD3DSystem, mGameDirectory);
}
//===============================================================================================================================
void EditorHelper2D::CreateWorld(string world_name)
{
	mMapEditorSystem2D->CreateWorld(world_name);
}
//===============================================================================================================================
void EditorHelper2D::LoadWorld(string& worldName, string filename, bool InEditor)
{
	mMapEditorSystem2D->LoadWorld(worldName, filename, InEditor);
}
//===============================================================================================================================
void EditorHelper2D::SaveWorld(string filename)
{
	mMapEditorSystem2D->SaveWorld(filename);
}
//===============================================================================================================================
void EditorHelper2D::RemoveMap(string mapName)
{
	mMapEditorSystem2D->RemoveMap(mapName);
}
//===============================================================================================================================
void EditorHelper2D::LoadMap(string& mapName, string filename, bool InEditor)
{
	mMapEditorSystem2D->LoadMap(mapName, filename, InEditor);
}
//===============================================================================================================================
void EditorHelper2D::SaveMap(string filename)
{
	mMapEditorSystem2D->SaveMap(filename);
}
//===============================================================================================================================
void EditorHelper2D::NewMapTopdown(string world_name, string name, int x, int y, bool fow, int fowRadius, bool fowFlashlight, bool bIso, bool InEditor)
{
	mMapEditorSystem2D->NewMapTopdown(world_name, name, x, y, fow, fowRadius, fowFlashlight, bIso, InEditor);
}
//===============================================================================================================================
void EditorHelper2D::NewMapPlatformer(string world_name, string name, int x, int y, bool InEditor)
{
	mMapEditorSystem2D->NewMapPlatformer(world_name, name, x, y, InEditor);
}
//===============================================================================================================================
void EditorHelper2D::SetActiveRenderedMap(string world_name, string map_name)
{
	mMapEditorSystem2D->SetActiveRenderedMap(world_name, map_name);
	mMapEditorSystem2D->SetMapCreated(true);
}
//===============================================================================================================================