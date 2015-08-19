#include "Environment2D.h"
#include "TopdownWorld.h"
#include "Convert.h"
#include "MapEditorSystem2D.h"
#include "MenuEditorSystem.h"
#include "HUDEditorSystem.h"
#include "InventoryEditorSystem2D.h"
#include "GDI.h"
#include "SurfaceDefinition.h"
#include "Scripting.h"
#include "AudioSystem.h"
#include "Environment2DMapHelper.h"
#include "ShaderCompiler.h"
#include "DefaultMaterials.h"
#include "TextureManager.h"
#include "Shaders.h"
//================================================================================================================
//================================================================================================================
Environment2D::Environment2D(EngineOptions* engineOptions)
:   m_EnvBaseInit(true)
,   m_MapEditorSystem2D(0)
,   m_HUDEditorSystem(0)
,	mEngineOptions(engineOptions)
{
	Init();
}
//================================================================================================================
bool Environment2D::Init()
{
	m_WorldLoaded = false;
	m_MapLoaded = false;
	bInEditorMap = true;
	bInEditorMenu = false;
	bInEditorHUD = false;
	
	m_D3DSystem = new D3D();
	if (!m_D3DSystem) return false;
	
	if (!m_D3DSystem->Initialize11(mEngineOptions))
	{
		MessageBox(NULL, "Direct3D Init Failed", "Error", MB_OK);
		return false;
	}
	
	TextureManager::NewInstance(m_D3DSystem);
	
	ZShadeSandboxShader::Shaders::Build(m_D3DSystem);

	SSurfaceDefinition::Build();
	
	m_Inventory = new PlayerInventory2D(m_D3DSystem);
	
	bInventoryActive = false;

	//Initialize the Camera
	m_CameraSystem = new Camera(mEngineOptions);
	if (!m_CameraSystem) return false;
	m_CameraSystem->SetPosition(0.0f, 0.0f, -15.0f);
	ZShadeSandboxMath::XMMatrix world(XMMatrixIdentity());
	m_CameraSystem->SetWorld(world);
	m_CameraSystem->CreateOrtho(mEngineOptions->m_screenWidth, mEngineOptions->m_screenHeight, 0.1f, 1000.0f);

	//Initialize the menu system
	m_MenuSystem = new MenuSystem(m_D3DSystem);

	//Initialize the HUD system
	m_HUDSystem = new HUD(m_D3DSystem);

	//The game state will be set to GS_NONE by default
	GameState::NewInstance();
	
	GDI::Init();
	
	m_Timer.Initialize();

	//Full path of game directory
	string username = CGlobal::GrabUserName();
	string path = "C:\\Users\\";
	path.append(username);
	path.append("\\AppData\\Roaming\\");
	path.append(mEngineOptions->m_GameFolderName);
	
	if (!CGlobal::DoesFolderExist(path))
	{
		mEngineOptions->m_GameFolderName = "ZShadeSandboxDefaultGame2D";
		
		// If the GameFolder does not exist then create a default game folder
		CGlobal::CreateGD2D(mEngineOptions->m_GameFolderName, mEngineOptions->m_GameType2D);
		
		// And update the ini file
		ZShadeINIParser iniSandboxParser("ZShadeSandbox.ini", true);
		iniSandboxParser.WriteString("Editor", "GameFolder", mEngineOptions->m_GameFolderName);
	}
	
	// Load the Game Directory since the game is starting from here
	if (!mEngineOptions->m_inEditor)
	{
		mEngineOptions->m_inEditor = false;

		//Load the game directory
		m_GameDirectory2D = CGlobal::LoadGameDirectory2D(mEngineOptions->m_GameFolderName);
		
		mEngineOptions->m_GD2D = m_GameDirectory2D;
		
		DefaultMaterials materials;
		materials.Build(m_D3DSystem, m_GameDirectory2D->m_required_textures);
		
		// Read the Game.ini file
		ZShadeINIParser iniParser(m_GameDirectory2D->m_game_ini_path, false);
		string gameType = iniParser.GetString("GAME", "GameType");

		if (BetterString(gameType) == "Topdown")
		{
			mEngineOptions->m_GameType2D = TOPDOWN;
			
			//Load the Fog of War data
			FogData::NewInstance(m_D3DSystem);
			FogData::Instance()->InitializeTable();
			//FogData::Instance()->LoadTextures(m_GameDirectory2D->m_required_textures);
		}
		else if (BetterString(gameType) == "Platformer")
		{
			mEngineOptions->m_GameType2D = PLATFORMER;
			
			m_Background = new PlatformerBackground(m_D3DSystem);
			m_Background->AddBackground(m_GameDirectory2D->m_sprites_path, "frontBackground.png", 1600, 768, false, true, 3, 0);
		}
		
		/*  Test out the inventory system
			Create an inventory that is 5x5 grid for regular and magic items
			
		      Items      Magic
			* * * * *  * * * * *
			* * * * *  * * * * *
			* * * * *  * * * * *
			* * * * *  * * * * *
			* * * * *  * * * * *
		*
		if (m_Inventory)
		{
			m_Inventory->Init(
				m_GameDirectory2D->m_required_textures,// path of the stamp tool
				m_GameDirectory2D->m_required_textures,// path of the background image
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
			m_Inventory->AddItemRegular(
				m_GameDirectory2D->m_sprites_path,
				"sword.png",
				"sword"
			);
			m_Inventory->AddItemRegular(
				m_GameDirectory2D->m_sprites_path,
				"heart.png",
				"heart"
			);
			m_Inventory->AddItemMagic(
				m_GameDirectory2D->m_sprites_path,
				"fireball.png",
				"fireball"
			);
			
			// Position the items on the grid when finished
			m_Inventory->PositionItemsOnGrid();
		}*/
		
		// Initialize Audio System
		AudioSystem::NewInstance(m_GameDirectory2D, mEngineOptions->hwnd);
		
		// Initialize lua scripting system
		Scripting::NewInstance(m_D3DSystem, m_GameDirectory2D);
	}
	else
	{
		m_MapEditorSystem2D = 0;
		mEngineOptions->m_GD2D = 0;
		mEngineOptions->m_inEditor = true;
		bInEditorMap = true;
	}
	
	return true;
}
//================================================================================================================
Environment2D::~Environment2D()
{
//	Shutdown();
}
//================================================================================================================
void Environment2D::Shutdown()
{
	GDI::Shutdown();

	m_D3DSystem->Shutdown();
}
//================================================================================================================
void Environment2D::OnMouseDown(WPARAM btnState, int x, int y)
{
	if (GameState::GetInstance()->GetState() == GS_MAINMENU ||
		GameState::GetInstance()->GetState() == GS_INGAMEMENU)
	{
		if (m_MenuSystem != NULL)
		{
			m_MenuSystem->UpdateButtonMouseOn(btnState, x, y);
			m_MenuSystem->UpdateButtonMouseDown(btnState, x, y);
		}

		if (mEngineOptions->m_inEditor && m_MenuEditorSystem != NULL)
		{
			m_MenuEditorSystem->OnMouseDown(btnState, x, y);
		}
	}
	else if (GameState::GetInstance()->GetState() == GS_INGAME)
	{
		if (mEngineOptions->m_inEditor && bInEditorHUD && m_HUDEditorSystem != NULL)
		{
			m_HUDEditorSystem->OnMouseDown(btnState, x, y);
		}
		
		if (mEngineOptions->m_inEditor && bInEditorMap && m_MapEditorSystem2D != NULL)
		{
			m_MapEditorSystem2D->OnMouseDown(btnState, x, y);
		}
		
		if (bInventoryActive && m_Inventory)
		{
			m_Inventory->OnMouseDown(btnState, x, y);
		}
	}
}
//================================================================================================================
void Environment2D::OnMouseUp(WPARAM btnState, int x, int y)
{
	if (GameState::GetInstance()->GetState() == GS_MAINMENU ||
		GameState::GetInstance()->GetState() == GS_INGAMEMENU)
	{
		if (m_MenuSystem != NULL)
		{
			m_MenuSystem->UpdateButtonMouseOn(btnState, x, y);
			m_MenuSystem->UpdateButtonMouseUp(btnState, x, y);
		}

		if (mEngineOptions->m_inEditor && m_MenuEditorSystem != NULL)
		{
			m_MenuEditorSystem->OnMouseUp(btnState, x, y);
		}
	}
	else if (GameState::GetInstance()->GetState() == GS_INGAME)
	{
		if (mEngineOptions->m_inEditor && bInEditorHUD && m_HUDEditorSystem != NULL)
		{
			m_HUDEditorSystem->OnMouseUp(btnState, x, y);
		}
		
		if (mEngineOptions->m_inEditor && bInEditorMap && m_MapEditorSystem2D != NULL)
		{
			m_MapEditorSystem2D->OnMouseUp(btnState, x, y);
		}
		
		if (bInventoryActive && m_Inventory)
		{
			m_Inventory->OnMouseUp(btnState, x, y);
		}
	}
}
//================================================================================================================
void Environment2D::OnMouseMove(WPARAM btnState, int x, int y)
{
	if (GameState::GetInstance()->GetState() == GS_MAINMENU ||
		GameState::GetInstance()->GetState() == GS_INGAMEMENU)
	{
		if (m_MenuSystem != NULL)
		{
			m_MenuSystem->UpdateButtonMouseOn(btnState, x, y);
			m_MenuSystem->UpdateButtonMouseMove(btnState, x, y);
		}

		if (mEngineOptions->m_inEditor && m_MenuEditorSystem != NULL)
		{
			m_MenuEditorSystem->OnMouseMove(btnState, x, y);
		}
	}
	else if (GameState::GetInstance()->GetState() == GS_INGAME)
	{
		if (mEngineOptions->m_inEditor && bInEditorHUD && m_HUDEditorSystem != NULL)
		{
			m_HUDEditorSystem->OnMouseMove(btnState, x, y);
		}
		
		if (mEngineOptions->m_inEditor && bInEditorMap && m_MapEditorSystem2D != NULL)
		{
			m_MapEditorSystem2D->OnMouseMove(btnState, x, y);
		}
		
		if (bInventoryActive && m_Inventory)
		{
			m_Inventory->OnMouseMove(btnState, x, y);
		}
	}
}
//================================================================================================================
bool Environment2D::Update()
{
	m_Timer.Frame();
	
	m_CameraSystem->Update();
	
	keyboard = Keyboard::GetKeyboard();
	mouse = Mouse::GetMouse();
	
	if (Scripting::Instance() != NULL)
		Scripting::Instance()->SetActiveWorldName(m_ActiveWorldName);
	if (Scripting::Instance() != NULL)
		Scripting::Instance()->SetActiveMapName(m_ActiveMapName);

	bool update = true;
	
	if (mEngineOptions->m_inEditor)
	{
		if (bInEditorMap || bInEditorHUD)
		{
			GameState::GetInstance()->SetState(GS_INGAME);
		}

		if (bInEditorMenu)
		{
			GameState::GetInstance()->SetState(GS_MAINMENU);
		}
	}

	if (GameState::GetInstance()->GetState() == GS_QUIT)
	{
		ExitProcess(0);
	}
	else if (GameState::GetInstance()->GetState() == GS_MAINMENU ||
			 GameState::GetInstance()->GetState() == GS_INGAMEMENU)
	{
		if (mEngineOptions->m_inEditor)
		{
			//if (m_TabEditWindow2D->IsEditorTypeMenu())
			//	;// m_MenuEditorSystem->UpdateInput(keyboard);
		}
	}
	else if (GameState::GetInstance()->GetState() == GS_INGAME)
	{
		// If a button is pressed make the inventory active or inactive
		if( keyboard->RisingEdge(Keyboard::Key::I) )
		{
			bInventoryActive = !bInventoryActive;
		}
		
		if (bInEditorHUD || bInventoryActive)
		{
			update = false;
		}
		
		// If we are in the editor in a map and a topdown map is in section picker mode
		// then disable updating the map
		if (mEngineOptions->m_inEditor)
		{
			if (bInEditorMap)
			{
				if (m_MapEditorSystem2D->IsInSectionPicker())
				{
					update = false;
				}

				m_MapEditorSystem2D->UpdateInput(keyboard);
			}
		}
		
		// MAKING SURE THAT IF I'M IN MAP, MENU OR HUD MODE THAT EACH GET UPDATED AND RENDERED ACCORDINGLY
		
		// Update all the world and physics objects
		if (update)
		{
			if (m_WorldLoaded && m_MapLoaded)
			{
				if (mEngineOptions->m_GameType2D == TOPDOWN)
				{
					TopdownWorld* world = GetTopdownWorld(m_ActiveWorldName);

					world->LoadSpritesMain();

					if (mEngineOptions->m_inEditor)
						world->UpdateScrollOffset(m_MapEditorSystem2D->GetScrollOffset());

					world->Update(m_Timer.DeltaSecondsF(), keyboard);
				}
				else if (mEngineOptions->m_GameType2D == PLATFORMER)
				{
					PlatformerWorld* world = GetPlatformerWorld(m_ActiveWorldName);

					world->LoadSpritesMain();

					if (mEngineOptions->m_inEditor)
						world->UpdateScrollOffset(m_MapEditorSystem2D->GetScrollOffset());

					world->Update(m_Timer.DeltaSecondsF(), keyboard, mEngineOptions->hwnd);

					if (m_Background != NULL)
					{
						m_Background->Update();
					}
				}
			}
		}
	}

	return true;
}
//================================================================================================================
bool Environment2D::RenderMaster()
{
	bool result = true;

	//If the game directory is not created yet make sure I cannot do anything until I create a game directory
	if (m_GameDirectory2D == NULL)
		return true;
	if (m_GameDirectory2D->GAME_FOLDER == "")
		return true;

	if (GameState::GetInstance()->GetState() == GS_MAINMENU ||
		GameState::GetInstance()->GetState() == GS_INGAMEMENU)
	{
		if (m_MenuSystem != NULL)
		{
			m_D3DSystem->TurnOffZBuffer();
			{
				if (m_MenuSystem != 0)
				{
					m_MenuSystem->Render(m_CameraSystem, 0);
				}
				
				if (mEngineOptions->m_inEditor)
				{
					if (m_MenuEditorSystem != NULL)
					{
						m_MenuEditorSystem->Render(m_CameraSystem);
					}
				}
			}
			m_D3DSystem->TurnOnZBuffer();
		}
	}
	else if (GameState::GetInstance()->GetState() == GS_INGAME)
	{
		//Turn z-buffer off for 2D Rendering
		m_D3DSystem->TurnOffZBuffer();
		{
			bool inInventory = false;
			
			// If not in the editor then normal map rendering will happen
			if (!mEngineOptions->m_inEditor)
			{
				bInEditorMap = true;
				bInEditorHUD = false;
			}
			
			if (bInventoryActive && m_Inventory)
			{
				inInventory = true;
				
				m_Inventory->Render(m_CameraSystem);
			}
			
			if (bInEditorHUD)
			{
				if (m_HUDSystem != 0)
				{
					m_HUDSystem->Render(m_CameraSystem, 0);
				}
				
				m_HUDEditorSystem->Render(m_CameraSystem);
			}
			
			if (m_WorldLoaded && m_MapLoaded && bInEditorMap && !inInventory)
			{
				if (mEngineOptions->m_GameType2D == TOPDOWN)
				{
					TopdownWorld* world = GetTopdownWorld(m_ActiveWorldName);

					// If we are in the editor
					if (mEngineOptions->m_inEditor)
					{
						world->UpdateScrollOffset(m_MapEditorSystem2D->GetScrollOffset());

						if (m_MapEditorSystem2D->IsInSectionPicker())
						{
							world->RenderSectionPicker(m_CameraSystem);
							m_MapEditorSystem2D->RenderStampSprites(m_CameraSystem);
						}
						else
						{
							world->Render(m_CameraSystem);
							m_MapEditorSystem2D->RenderStampSprites(m_CameraSystem);
						}
					}
					else// Just render the game
					{
						world->Render(m_CameraSystem);
					}
				}
				else if (mEngineOptions->m_GameType2D == PLATFORMER)
				{
					PlatformerWorld* world = GetPlatformerWorld(m_ActiveWorldName);

					int mapWidth = 0;

					if (world->MapToRender() != "")
					{
						if (world->GetMap(world->MapToRender()) != NULL)
							mapWidth = world->GetMap(world->MapToRender())->Width();
					}

					if (m_Background != NULL)
					{
						m_Background->Render(m_CameraSystem, mapWidth, 0);
					}
					
					world->SetBackgroundScreenWidth(m_Background->BackgroundWidth());
					
					world->Render(m_CameraSystem);

					if (mEngineOptions->m_inEditor)
						m_MapEditorSystem2D->RenderStampSprites(m_CameraSystem);
				}
				
				if (m_HUDSystem != 0)
				{
					m_HUDSystem->Render(m_CameraSystem, 0);
				}
			}
		}
		m_D3DSystem->TurnOnZBuffer();
	}

	return result;
}
//================================================================================================================
int Environment2D::Run()
{
	Update();

	if (m_D3DSystem->BeginScene(0.0f, 0.2f, 0.4f, 1.0f))
	{
		//Rendering Happens Here
		RenderMaster();

		m_D3DSystem->EndScene();
	}

	return 1;
}
//================================================================================================================
void Environment2D::RemoveMap(string mapName)
{
	if (mEngineOptions->m_GameType2D == TOPDOWN)
	{
		auto iter = m_WorldsTopdown.begin();
		for (; iter != m_WorldsTopdown.end(); iter++)
		{
			if ((*iter).first == m_ActiveWorldName)
			{
				auto current = (*iter).second->m_mapNames.begin();
				while (current != (*iter).second->m_mapNames.end())
				{
					if ((*current) == mapName)
					{
						current = (*iter).second->m_mapNames.erase(current);
					}
					else
					{
						++current;
					}
				}

				break;
			}
		}

		/*map<string, STopdownWorld*>::iterator iter = m_WorldsTopdown.begin();

		bool found = false;

		for (; iter != m_WorldsTopdown.end(); iter++)
		{
			if ((*iter).first == m_ActiveWorldName)
			{
				TopdownWorld* world = (*iter).second->m_World;

				world->RemoveMap(mapName);

				vector<string, string>::iterator MapNameIter = (*iter).second->m_mapNames.begin();
				vector<string, string>::iterator KillIter;

				for (; MapNameIter != (*iter).second->m_mapNames.end(); MapNameIter++)
				{
					string name = (*MapNameIter);

					if (BetterString(name) == BetterString(mapName))
					{
						KillIter = MapNameIter;
						found = true;
						break;
					}
				}

				if (found)
				{
					(*iter).second->m_mapNames.erase(KillIter);
				}

				break;
			}
		}*/
	}
	else if (mEngineOptions->m_GameType2D == PLATFORMER)
	{
		auto iter = m_WorldsPlatformer.begin();
		for (; iter != m_WorldsPlatformer.end(); iter++)
		{
			if ((*iter).first == m_ActiveWorldName)
			{
				auto current = (*iter).second->m_mapNames.begin();
				while (current != (*iter).second->m_mapNames.end())
				{
					if ((*current) == mapName)
					{
						current = (*iter).second->m_mapNames.erase(current);
					}
					else
					{
						++current;
					}
				}

				break;
			}
		}

		/*map<string, SPlatformerWorld*>::iterator iter = m_WorldsPlatformer.begin();

		bool found = false;

		for (; iter != m_WorldsPlatformer.end(); iter++)
		{
			if ((*iter).first == m_ActiveWorldName)
			{
				PlatformerWorld* world = (*iter).second->m_World;

				world->RemoveMap(mapName);

				vector<string, string>::iterator MapNameIter = (*iter).second->m_mapNames.begin();
				vector<string, string>::iterator KillIter;

				for (; MapNameIter != (*iter).second->m_mapNames.end(); MapNameIter++)
				{
					string name = (*MapNameIter);

					if (BetterString(name) == BetterString(mapName))
					{
						KillIter = MapNameIter;
						found = true;
						break;
					}
				}

				if (found)
				{
					(*iter).second->m_mapNames.erase(KillIter);
				}

				break;
			}
		}*/
	}
}
//================================================================================================================
void Environment2D::GrabWorldNames(vector<string>& names)
{
	if (mEngineOptions->m_GameType2D == TOPDOWN)
	{
		map<string, STopdownWorld*>::iterator iter = m_WorldsTopdown.begin();

		for (; iter != m_WorldsTopdown.end(); iter++)
		{
			names.push_back((*iter).first);
		}
	}
	else if (mEngineOptions->m_GameType2D == PLATFORMER)
	{
		map<string, SPlatformerWorld*>::iterator iter = m_WorldsPlatformer.begin();

		for (; iter != m_WorldsPlatformer.end(); iter++)
		{
			names.push_back((*iter).first);
		}
	}
}
//================================================================================================================
void Environment2D::GrabMapNames(string world_name, vector<string>& names)
{
	if (mEngineOptions->m_GameType2D == TOPDOWN)
	{
		map<string, STopdownWorld*>::iterator iter = m_WorldsTopdown.begin();

		for (; iter != m_WorldsTopdown.end(); iter++)
		{
			if ((*iter).first == world_name)
			{
				for (int i = 0; i < (*iter).second->m_mapNames.size(); i++)
					names.push_back((*iter).second->m_mapNames[i]);
			}
		}
	}
	else if (mEngineOptions->m_GameType2D == PLATFORMER)
	{
		map<string, SPlatformerWorld*>::iterator iter = m_WorldsPlatformer.begin();

		for (; iter != m_WorldsPlatformer.end(); iter++)
		{
			if ((*iter).first == world_name)
			{
				for (int i = 0; i < (*iter).second->m_mapNames.size(); i++)
					names.push_back((*iter).second->m_mapNames[i]);
			}
		}
	}
}
//================================================================================================================
void Environment2D::UpdateNewMapName(string world_name, string old_map_name, string new_map_name)
{
	if (mEngineOptions->m_GameType2D == TOPDOWN)
	{
		map<string, STopdownWorld*>::iterator iter = m_WorldsTopdown.begin();

		for (; iter != m_WorldsTopdown.end(); iter++)
		{
			if ((*iter).first == world_name)
			{
				RemoveMap(old_map_name);
				(*iter).second->m_mapNames.push_back(new_map_name);

				(*iter).second->m_World->UpdateMapName(new_map_name);
			}
		}
	}
	else if (mEngineOptions->m_GameType2D == PLATFORMER)
	{
		map<string, SPlatformerWorld*>::iterator iter = m_WorldsPlatformer.begin();

		for (; iter != m_WorldsPlatformer.end(); iter++)
		{
			if ((*iter).first == world_name)
			{
				RemoveMap(old_map_name);
				(*iter).second->m_mapNames.push_back(new_map_name);

				// Implement this
				// (*iter).second->m_World->UpdateMapName(new_map_name);
			}
		}
	}
}
//================================================================================================================
void Environment2D::LoadWorld(string world_name, STopdownWorld* sw)
{
	m_WorldsTopdown[world_name] = sw;
}
//================================================================================================================
void Environment2D::LoadWorld(string world_name, SPlatformerWorld* sw)
{
	m_WorldsPlatformer[world_name] = sw;
}
//================================================================================================================
TopdownWorld* Environment2D::GetTopdownWorld(string world_name)
{
	STopdownWorld* sw = NULL;

	//Get a copy of the world if it exists
	if (m_WorldsTopdown[world_name] != NULL)
	{
		sw = m_WorldsTopdown[world_name];
	}

	if (sw == NULL) return NULL;

	return sw->m_World;
}
//================================================================================================================
PlatformerWorld* Environment2D::GetPlatformerWorld(string world_name)
{
	SPlatformerWorld* sw = NULL;

	//Get a copy of the world if it exists
	if (m_WorldsPlatformer[world_name] != NULL)
	{
		sw = m_WorldsPlatformer[world_name];
	}

	if (sw == NULL) return NULL;

	return sw->m_World;
}
//================================================================================================================
STopdownWorld* Environment2D::GetSTopdownWorld(string world_name)
{
	STopdownWorld* sw = NULL;

	//Get a copy of the world if it exists
	if (m_WorldsTopdown[world_name] != NULL)
	{
		sw = m_WorldsTopdown[world_name];
	}

	if (sw == NULL) return NULL;

	return sw;
}
//================================================================================================================
SPlatformerWorld* Environment2D::GetSPlatformerWorld(string world_name)
{
	SPlatformerWorld* sw = NULL;

	//Get a copy of the world if it exists
	if (m_WorldsPlatformer[world_name] != NULL)
	{
		sw = m_WorldsPlatformer[world_name];
	}

	if (sw == NULL) return NULL;

	return sw;
}
//================================================================================================================