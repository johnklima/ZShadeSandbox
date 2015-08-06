#include "MapEditorSystem2D.h"
#include "Environment2DMapHelper.h"
//================================================================================================================
MapEditorSystem2D::MapEditorSystem2D(Environment2D* env2D)
: m_Env2D(env2D)
{
	m_mainGameDirectory = m_Env2D->GetGD2D();
	m_D3DSystem = m_Env2D->GetD3D();
	m_EngineOptions = m_Env2D->GetEngineOptions();

	m_ClickDelayTimer = new SleepTimer();
	m_SelectionTimer = new SleepTimer();
	
	m_originalSpriteID = -1;
	m_sprite_cache_selection_changed = false;
	m_resizing_sprite = false;
	m_moving_sprite = false;
	m_cloning_sprite = false;
	m_selected_sprite_cover = false;
	m_display_sprite_box_created = false;
	m_display_selected_sprite_box = false;
	m_sprite_with_box = 0;
	m_sprite_box = 0;
	m_select_sprite_box = 0;
	m_SelectedSprite = 0;
	m_EditSizeSprite = 0;
	m_displaySprite = 0;
	m_cloneDisplaySprite = 0;
	m_selectModeSpriteID = -1;

	m_Updated = false;
	
	iToolWindowWidth = 0;

	m_SelectedTilePos.x = 0;
	m_SelectedTilePos.y = 0;

	// Set defaults
	SwitchToTileModeSelect();
	SwitchToStampSize1x1();
	
	// Start at world origin
	ScrollOffset.x = 0;
	ScrollOffset.y = 0;
	
	// No drag in progress
	dragStartX = dragStartY = dragPrevX = dragPrevY = -1;

	// No pan in progress
	panning = false;

	stampPressed = false;
	
	bWasClicked = false;

	// Grid size
	gridSizeX = m_EngineOptions->TILE_SIZE;
	gridSizeY = m_EngineOptions->TILE_SIZE;
	usingGrid = true;
}
//================================================================================================================
MapEditorSystem2D::~MapEditorSystem2D()
{
}
//================================================================================================================
void MapEditorSystem2D::SwitchToStampSize1x1()
{
	multiStampSize = MSS_1X1;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToStampSize2x2()
{
	multiStampSize = MSS_2X2;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToStampSize3x3()
{
	multiStampSize = MSS_3X3;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToStampSize4x4()
{
	multiStampSize = MSS_4X4;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToStampSize5x5()
{
	multiStampSize = MSS_5X5;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToStampSize6x6()
{
	multiStampSize = MSS_6X6;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToStampSize7x7()
{
	multiStampSize = MSS_7X7;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToStampSize8x8()
{
	multiStampSize = MSS_8X8;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToStampSize9x9()
{
	multiStampSize = MSS_9X9;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToStampSize10x10()
{
	multiStampSize = MSS_10X10;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToSpriteModeMove()
{
	spriteAction = SA_Move;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToSpriteModeDelete()
{
	spriteAction = SA_Delete;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToSpriteModePlace()
{
	spriteAction = SA_Place;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToSpriteModeClone()
{
	spriteAction = SA_Clone;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToSpriteModeSize()
{
	spriteAction = SA_EditSize;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToSpriteModeSelect()
{
	spriteAction = SA_Select;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToTileModeSelect()
{
	tileAction = TA_Select;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToTileModeRegular()
{
	tileAction = TA_Regular;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToTileModeMultiStamp()
{
	tileAction = TA_MultiStamp;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToTileModeRemoveTexture()
{
	tileAction = TA_RemoveTexture;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToHardnessModeTiles()
{
	hardnessAction = HA_Tiles;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToHardnessModeSprites()
{
	hardnessAction = HA_Sprites;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToGridModeFree()
{
	gridMode = GM_Free;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToGridModeSnap()
{
	gridMode = GM_Snap;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToEditModeTiles()
{
	editMode = EM_Tiles;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToEditModeSprites()
{
	editMode = EM_Sprites;
}
//================================================================================================================
void MapEditorSystem2D::SwitchToEditModeHardness()
{
	editMode = EM_Hardness;
}
//================================================================================================================
void MapEditorSystem2D::SetActiveRenderedMap(string world_name, string map_name)
{
	Environment2DMapHelper::SetActiveRenderedMap(world_name, map_name);
}
//================================================================================================================
void MapEditorSystem2D::GrabWorldNames(vector<string>& names)
{
	m_Env2D->GrabWorldNames(names);
}
//================================================================================================================
void MapEditorSystem2D::GrabMapNames(string world_name, vector<string>& names)
{
	m_Env2D->GrabMapNames(world_name, names);
}
//================================================================================================================
void MapEditorSystem2D::UpdateNewMapName(string world_name, string old_map_name, string new_map_name)
{
	Environment2DMapHelper::UpdateNewMapName(world_name, old_map_name, new_map_name);
}
//================================================================================================================
void MapEditorSystem2D::GoFullscreen(bool bEnable)
{
	//m_Env2D->GoFullscreen(bEnable);
}
//================================================================================================================
void MapEditorSystem2D::ToggleMapHardness(bool bEnable)
{
	Environment2DMapHelper::ToggleMapHardness(bEnable);
}
//================================================================================================================
bool MapEditorSystem2D::IsWorldLoaded()
{
	// Find out if the world is loaded
	m_WorldLoaded = m_Env2D->WorldLoaded();
	return m_WorldLoaded;
}
//================================================================================================================
TopdownTile* MapEditorSystem2D::GetSelectedTopdownTile()
{
	if (m_SelectedTilePos.x == -1 && m_SelectedTilePos.y == -1) return NULL;

	return Environment2DMapHelper::GetSelectedTopdownTile(m_SelectedTilePos.x, m_SelectedTilePos.y);
}
//================================================================================================================
PlatformerTile* MapEditorSystem2D::GetSelectedPlatformerTile()
{
	if (m_SelectedTilePos.x == -1 && m_SelectedTilePos.y == -1) return NULL;

	return Environment2DMapHelper::GetSelectedPlatformerTile(m_SelectedTilePos.x, m_SelectedTilePos.y);
}
//================================================================================================================
void MapEditorSystem2D::LoadRequiredSprites()
{
	string base_path = m_mainGameDirectory->m_required_textures;
	
	int tileSize = m_EngineOptions->TILE_SIZE;
	
	m_StampNormal = new Sprite(m_D3DSystem);
	m_StampNormal->Initialize(base_path, "stamp_normal.png", XMFLOAT3(0, 0, 0), tileSize, tileSize, STATIC);
	m_StampHighlight = new Sprite(m_D3DSystem);
	m_StampHighlight->Initialize(base_path, "stamp_highlight.png", XMFLOAT3(0, 0, 0), tileSize, tileSize, STATIC);

	m_StampNormal32x32 = new Sprite(m_D3DSystem);
	m_StampNormal32x32->Initialize(base_path, "stamp_normal_32x32.png", XMFLOAT3(0, 0, 0), tileSize / 2, tileSize / 2, STATIC);
	m_StampHighlight32x32 = new Sprite(m_D3DSystem);
	m_StampHighlight32x32->Initialize(base_path, "stamp_highlight_32x32.png", XMFLOAT3(0, 0, 0), tileSize / 2, tileSize / 2, STATIC);

	int tx = tileSize;
	int ty = tileSize;
	m_TileEdit1x1Sprite = new Sprite(m_D3DSystem);
	m_TileEdit1x1Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_1x1.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit2x2Sprite = new Sprite(m_D3DSystem);
	m_TileEdit2x2Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_2x2.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit3x3Sprite = new Sprite(m_D3DSystem);
	m_TileEdit3x3Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_3x3.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit4x4Sprite = new Sprite(m_D3DSystem);
	m_TileEdit4x4Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_4x4.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit5x5Sprite = new Sprite(m_D3DSystem);
	m_TileEdit5x5Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_5x5.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit6x6Sprite = new Sprite(m_D3DSystem);
	m_TileEdit6x6Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_6x6.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit7x7Sprite = new Sprite(m_D3DSystem);
	m_TileEdit7x7Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_7x7.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit8x8Sprite = new Sprite(m_D3DSystem);
	m_TileEdit8x8Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_8x8.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit9x9Sprite = new Sprite(m_D3DSystem);
	m_TileEdit9x9Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_9x9.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit10x10Sprite = new Sprite(m_D3DSystem);
	m_TileEdit10x10Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_10x10.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);

	tx = tileSize;
	ty = tileSize;
	m_TileEdit1x1Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit1x1Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_1x1.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit2x2Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit2x2Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_2x2.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit3x3Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit3x3Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_3x3.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit4x4Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit4x4Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_4x4.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit5x5Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit5x5Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_5x5.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit6x6Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit6x6Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_6x6.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit7x7Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit7x7Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_7x7.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit8x8Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit8x8Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_8x8.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit9x9Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit9x9Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_9x9.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += tileSize;
	ty += tileSize;
	m_TileEdit10x10Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit10x10Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_10x10.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);

	//int size = 150;
	//if (m_EngineOptions->m_screenWidth == 1920 && m_EngineOptions->m_screenHeight == 1080)
	//	size += 50;

	//m_displaySpriteCover = new Sprite(m_D3DSystem);
	//m_displaySpriteCover->Initialize(base_path, "sprite_display_cover.png",
	//	XMFLOAT3(m_EngineOptions->m_screenWidth - size, m_EngineOptions->m_screenHeight - size, 0), 74, 74, STATIC);

	// Load FOW textures
	//Load the Fog of War data
	FogData::NewInstance(m_D3DSystem);
	FogData::Instance()->InitializeTable();
	//FogData::Instance()->LoadTextures(base_path);
}
//================================================================================================================
void MapEditorSystem2D::NewMapTopdown(string world_name, string name, int x, int y, bool fow, int fowRadius, bool fowFlashlight, bool bIso, bool InEditor)
{
	//Create a new map
	if (m_Env2D != NULL)
	{
		Environment2DMapHelper::CreateMapTopdown(x, y, fow, fowRadius, fowFlashlight, world_name, name, bIso, InEditor);

		if (InEditor)
		{
			m_InSectionPicker = true;
			m_currentSection = 0;
		}

		m_MapCreated = true;
	}
}
//================================================================================================================
void MapEditorSystem2D::NewMapPlatformer(string world_name, string name, int x, int y, bool InEditor)
{
	//Create a new map
	if (m_Env2D != NULL)
	{
		Environment2DMapHelper::CreateMapPlatformer(x, y, world_name, name, InEditor);

		m_MapCreated = true;
	}
}
//================================================================================================================
void MapEditorSystem2D::RemoveMap(string mapName)
{
	m_Env2D->RemoveMap(mapName);
}
//================================================================================================================
void MapEditorSystem2D::CreateWorld(string world_name)
{
	Environment2DMapHelper::CreateWorld(world_name);

	m_InSectionPicker = true;
	m_currentSection = 0;
}
//================================================================================================================
string MapEditorSystem2D::UpdateTitleBarText()
{
	if (m_Updated)
	{
		//Place an [*] on the title bar of the window
		string path = "ZShadeSandbox [*]";
		
		if (m_mainGameDirectory->GAME_FOLDER != "")
		{
			path = "ZShadeSandbox (";
			path.append(m_mainGameDirectory->GAME_FOLDER);
			path.append(") [*]");
		}

		return path;
	}
	else
	{
		string path = "ZShadeSandbox";
		
		if (m_mainGameDirectory->GAME_FOLDER != "")
		{
			path = "ZShadeSandbox (";
			path.append(m_mainGameDirectory->GAME_FOLDER);
			path.append(")");
		}

		return path;
	}
}
//================================================================================================================
void MapEditorSystem2D::SetGameDirectory(GameDirectory2D* gd)
{
	m_mainGameDirectory = gd;
	m_Env2D->SetGameDirectory2D(gd);
}
//================================================================================================================
bool MapEditorSystem2D::HasMaps(string worldName)
{
	return Environment2DMapHelper::HasMaps();
}
//================================================================================================================
void MapEditorSystem2D::LoadWorld(string& worldName, string filename, bool InEditor)
{
	Environment2DMapHelper::LoadWorld(worldName, filename, InEditor);

	if (InEditor)
	{
		m_InSectionPicker = true;
		m_currentSection = 0;
	}
}
//================================================================================================================
void MapEditorSystem2D::SaveWorld(string filename)
{
	Environment2DMapHelper::SaveWorld(filename);
}
//================================================================================================================
void MapEditorSystem2D::LoadMap(string& mapName, string filename, bool InEditor)
{
	Environment2DMapHelper::LoadMap(mapName, filename, InEditor);

	if (InEditor)
	{
		m_InSectionPicker = true;
		m_currentSection = 0;
	}

	m_MapCreated = true;
}
//================================================================================================================
void MapEditorSystem2D::SaveMap(string filename)
{
	Environment2DMapHelper::SaveMap(filename);
}
//================================================================================================================
void MapEditorSystem2D::GetMapWidthAttribute(int& width)
{
	Environment2DMapHelper::GetMapWidthAttribute(width);
}
//================================================================================================================
void MapEditorSystem2D::GetMapHeightAttribute(int& height)
{
	Environment2DMapHelper::GetMapHeightAttribute(height);
}
//================================================================================================================
void MapEditorSystem2D::GetMapVisionAttribute(int& vision)
{
	Environment2DMapHelper::GetMapVisionAttribute(vision);
}
//================================================================================================================
void MapEditorSystem2D::GetMapMusicAttribute(string& music)
{
	Environment2DMapHelper::GetMapMusicAttribute(music);
}
//================================================================================================================
void MapEditorSystem2D::GetMapFOWAttribute(bool& fow)
{
	Environment2DMapHelper::GetMapFOWAttribute(fow);
}
//================================================================================================================
void MapEditorSystem2D::GetMapFOWRadiusAttribute(int& fowRadius)
{
	Environment2DMapHelper::GetMapFOWRadiusAttribute(fowRadius);
}
//================================================================================================================
void MapEditorSystem2D::GetMapFOWFlashlightAttribute(bool& fowFlashlight)
{
	Environment2DMapHelper::GetMapFOWFlashlightAttribute(fowFlashlight);
}
//================================================================================================================
void MapEditorSystem2D::GetMapFOWUncoveredAttribute(bool& fowUncovered)
{
	Environment2DMapHelper::GetMapFOWUncoveredAttribute(fowUncovered);
}
//================================================================================================================
void MapEditorSystem2D::VisibleTileCount(int& visibleTileCount)
{
	Environment2DMapHelper::VisibleTileCount(visibleTileCount);
}
//================================================================================================================
void MapEditorSystem2D::GetNumSpritesRendered(int& spritesRendered)
{
	Environment2DMapHelper::GetNumSpritesRendered(spritesRendered);
}
//================================================================================================================
void MapEditorSystem2D::GetSpriteTotal(int& spriteTotal)
{
	Environment2DMapHelper::GetSpriteTotal(spriteTotal);
}
//================================================================================================================
void MapEditorSystem2D::GetWorldAndMapName(string& activeMapName, string& activeWorldName)
{
	Environment2DMapHelper::GetWorldAndMapName(activeMapName, activeWorldName);
}
//================================================================================================================
void MapEditorSystem2D::UpdateInput(Keyboard* keyboard)
{
	/*m_ClickDelayTimer->Sleep(50);
	if (m_ClickDelayTimer->IsTimeUp())
	{
		if (keyboard->IsKeyDown(Keyboard::Key::Left))// || keyboard->IsKeyDown(Keyboard::Key::A))
		{
			if (ScrollOffset.x > 0)
				ScrollOffset.x -= edgeScrollX;
		}

		if (keyboard->IsKeyDown(Keyboard::Key::Right))// || keyboard->IsKeyDown(Keyboard::Key::D))
		{
			//if (ScrollOffset.x < 16)
				ScrollOffset.x += edgeScrollX;
		}

		if (keyboard->IsKeyDown(Keyboard::Key::Up))// || keyboard->IsKeyDown(Keyboard::Key::W))
		{
			if (ScrollOffset.y > 0)
				ScrollOffset.y -= edgeScrollY;
		}

		if (keyboard->IsKeyDown(Keyboard::Key::Down))// || keyboard->IsKeyDown(Keyboard::Key::S))
		{
			//if (ScrollOffset.y < 16)
				ScrollOffset.y += edgeScrollY;
		}
	}*/
	
	// These values should go into the sprite tab tool window as radio buttons
	// 3 radio buttons and a checkbox
	// reposition radio button
	// resize radio button
	// noupdate radio button
	// goFast should be a checkbox
	bool updatePosition = keyboard->IsKeyDown(Keyboard::Key::K);
	bool updateSize = keyboard->IsKeyDown(Keyboard::Key::L);
	bool goFast = keyboard->IsKeyDown(Keyboard::Key::O);
	if (keyboard->IsKeyDown(Keyboard::Key::N))
	{
		updatePosition = updateSize = false;
	}
	
	bool pressedLeftKey = keyboard->IsKeyDown(Keyboard::Key::Left);
	bool pressedRightKey = keyboard->IsKeyDown(Keyboard::Key::Right);
	bool pressedUpKey = keyboard->IsKeyDown(Keyboard::Key::Up);
	bool pressedDownKey = keyboard->IsKeyDown(Keyboard::Key::Down);
	
	ResizeSpriteUpdate(
		updatePosition,
		updateSize,
		goFast,
		pressedLeftKey,
		pressedRightKey,
		pressedUpKey,
		pressedDownKey
	);

	ScrollOffsetToBounds();

	if (m_MapCreated) Environment2DMapHelper::SetToolWindowWidth(iToolWindowWidth);

	//Add this delay so that we do not automatically stamp an image
	//to the tile when we click on a section of the map in the section picker
	if (m_startDelayTimer && m_EngineOptions->m_GameType2D == GameType2D::TOPDOWN)
	{
		m_ClickDelayTimer->Sleep(50);
		if (!m_ClickDelayTimer->IsTimeUp())
		{
			stampPressed = false;
		}
		else
		{
			m_startDelayTimer = false;
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::ScrollMapUp()
{
	ScrollOffset.y += m_EngineOptions->TILE_SIZE;
}
//================================================================================================================
void MapEditorSystem2D::ScrollMapDown()
{
	ScrollOffset.y -= m_EngineOptions->TILE_SIZE;
}
//================================================================================================================
void MapEditorSystem2D::ScrollMapLeft()
{
	ScrollOffset.x += m_EngineOptions->TILE_SIZE;
}
//================================================================================================================
void MapEditorSystem2D::ScrollMapRight()
{
	ScrollOffset.x -= m_EngineOptions->TILE_SIZE;
}
//================================================================================================================
void MapEditorSystem2D::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Start a left button drag of an item
	if ((btnState & MK_LBUTTON) != 0)
	{
		if (dragStartX == -1)
		{
			dragStartX = x;
			dragStartY = y;
		}

		dragPrevX = x;
		dragPrevY = y;

		stampPressed = true;

		// Change mouse co-ordinates to current world co-ordinates
		x -= ScrollOffset.x;
		y -= ScrollOffset.y;
		
		CalculateNewTilePosition(x, y);
		
		// If is section picker mode on a topdown map this will find the scrolling offset value
		// so that camera can be placed in the selected section
		SelectTopdownSection();
		
		// Updates the tile with a selected tile image from the tile cache if in regular or multistamp
		// tile mode
		UpdateTileStamp();
		
		// If in a platformer map this will make the tile invisible so that it becomes empty space and
		// sprites can pass through it as if it does not exist
		RemoveTexture();
		
		// Adds a new sprite to the map from the selected sprite image in the sprite cache if in sprite
		// place mode
		AddSprite();
		
		// Selects a sprite so it can be moved, if it is moving then this will be skipped
		// so that the sprite can be placed back on the map with a left click
		bool goingToMove = MoveSpriteStart();
		// If a sprite is being moved place the sprite at the clicked position if in sprite move mode
		if (!goingToMove) MoveSpriteEnd();
		
		// Selects a sprite so it can be cloned, if it is moving then this will be skipped
		// so that the sprite clone can be added to the map with a left click
		bool goingToClone = CloneSpriteStart();
		// If a sprite is being moved place the sprite at the clicked position if in sprite clone mode
		if (!goingToClone) CloneSpriteEnd();
		
		// Selects a sprite on the map and deletes it if in sprite delete mode
		DeleteSprite();
		
		// Selects the sprite to be resized if in sprite resize mode
		ResizeSpriteStart();
		
		// Select a sprite if in sprite select mode
		SelectModeSpriteClicked();
	}
	
	// Start panning the game world
	if ((btnState & MK_RBUTTON) != 0)
	{
		dragPrevX = x;
		dragPrevY = y;

		panning = true;
	}
}
//================================================================================================================
void MapEditorSystem2D::OnMouseUp(WPARAM btnState, int x, int y)
{
	// Stop dragging of item
	dragStartX = dragStartY = dragPrevX = dragPrevY = -1;
	stampPressed = false;
	
	// Stop panning the game world
	XMFLOAT2 p = SnapToGrid(ScrollOffset.x, ScrollOffset.y);
	ScrollOffset.x = p.x;
	ScrollOffset.y = p.y;
	panning = false;
}
//================================================================================================================
void MapEditorSystem2D::OnMouseMove(WPARAM btnState, int x, int y)
{
	int dX, dY, tdX, tdY;
	
	// Calculate movement delta from previous frame
	if (dragPrevX != -1)
	{
		dX = x - dragPrevX;
		dY = y - dragPrevY;
		tdX = x - dragStartX;
		tdY = y - dragStartY;
	}

	// Move the selection tool
	if ((editMode == EM_Tiles || (editMode == EM_Tiles && editMode == EM_Hardness)) && m_StampNormal != NULL)
	{
		XMFLOAT2 selToolPoint = SnapToGrid(x, y);
		
		if (tileAction == TA_MultiStamp)
		{
#pragma region "MoveMultiStampTool"
			switch (multiStampSize)
			{
				case MSS_1X1:
				{
					m_TileEdit1x1Sprite->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					m_TileEdit1x1Sprite2->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
				}
				break;
				case MSS_2X2:
				{
					m_TileEdit2x2Sprite->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					m_TileEdit2x2Sprite2->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
				}
				break;
				case MSS_3X3:
				{
					m_TileEdit3x3Sprite->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					m_TileEdit3x3Sprite2->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
				}
				break;
				case MSS_4X4:
				{
					m_TileEdit4x4Sprite->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					m_TileEdit4x4Sprite2->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
				}
				break;
				case MSS_5X5:
				{
					m_TileEdit5x5Sprite->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					m_TileEdit5x5Sprite2->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
				}
				break;
				case MSS_6X6:
				{
					m_TileEdit6x6Sprite->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					m_TileEdit6x6Sprite2->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
				}
				break;
				case MSS_7X7:
				{
					m_TileEdit7x7Sprite->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					m_TileEdit7x7Sprite2->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
				}
				break;
				case MSS_8X8:
				{
					m_TileEdit8x8Sprite->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					m_TileEdit8x8Sprite2->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
				}
				break;
				case MSS_9X9:
				{
					m_TileEdit9x9Sprite->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					m_TileEdit9x9Sprite2->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
				}
				break;
				case MSS_10X10:
				{
					m_TileEdit10x10Sprite->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					m_TileEdit10x10Sprite2->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
				}
				break;
			}
#pragma endregion
		}
		else
		{
			if (m_EngineOptions->m_GameType2D == GameType2D::TOPDOWN)
			{
				if (m_InSectionPicker)
				{
					m_StampNormal32x32->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					m_StampHighlight32x32->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					BoundSectionPicker();
				}
				else
				{
					m_StampNormal->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
					m_StampHighlight->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
				}
			}
			else if (m_EngineOptions->m_GameType2D == GameType2D::PLATFORMER)
			{
				m_StampNormal->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
				m_StampHighlight->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
			}
		}
	}
	
	if (editMode == EM_Sprites && gridMode == GM_Snap)
	{
		XMFLOAT2 selToolPoint = SnapToGrid(x, y);

		m_StampNormal->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
		m_StampHighlight->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);

		// If a sprite is being placed on the map we need to render a sprite over the position the mouse is moving to
		UpdateDisplaySprite(selToolPoint.x, selToolPoint.y);

		// If a sprite is being moved then continue to move it
		MoveSprite(selToolPoint.x - ScrollOffset.x, selToolPoint.y - ScrollOffset.y);

		// Create a highlight box over the sprite the mouse has hovered over
		HighlightSprite(selToolPoint.x - ScrollOffset.x, selToolPoint.y - ScrollOffset.y);
	}
	else
	{
		// If a sprite is being placed on the map we need to render a sprite over the position the mouse is moving to
		UpdateDisplaySprite(x, y);

		// If a sprite is being moved then continue to move it
		MoveSprite(x - ScrollOffset.x, y - ScrollOffset.y);

		// Create a highlight box over the sprite the mouse has hovered over
		HighlightSprite(x - ScrollOffset.x, y - ScrollOffset.y);
	}

	if ((btnState & MK_LBUTTON) != 0)
	{
		stampPressed = true;
		
		// Change mouse co-ordinates to current world co-ordinates
		CalculateNewTilePosition(x - ScrollOffset.x, y - ScrollOffset.y);
		
		// Selects a sprite on the map and deletes it if in sprite delete mode
		DeleteSprite();
		
		// Updates the tile with a selected tile image from the tile cache if in regular or multistamp
		// tile mode
		UpdateTileStamp();
		
		// If in a platformer map this will make the tile invisible so that it becomes empty space and
		// sprites can pass through it as if it does not exist
		RemoveTexture();
		
		// Adds a new sprite to the map from the selected sprite image in the sprite cache if in sprite
		// place mode
		AddSprite();
	}

	// Pan around game world if dragging with RMB
	if ((btnState & MK_RBUTTON) != 0 && panning)
	{
		ScrollOffset.x += dX * 2;
		ScrollOffset.y += dY * 2;
	}

	// Remember current mouse co-ordinates for next frame
	if (dragPrevX != -1)
	{
		dragPrevX = x;
		dragPrevY = y;
	}
}
//================================================================================================================
void MapEditorSystem2D::ScrollOffsetToBounds()
{
	if (m_MapCreated
	&&	m_ActiveMapName != ""
	&&	m_ActiveWorldName != "")
	{
		if (m_EngineOptions->m_GameType2D == GameType2D::TOPDOWN)
		{
			if (m_InSectionPicker) return;
		}
		
		int width;
		int height;
		Environment2DMapHelper::GetMapWidthAttribute(width);
		Environment2DMapHelper::GetMapHeightAttribute(height);

		if (-ScrollOffset.x <= 0)
		{
			ScrollOffset.x = 0;
		}
		if (-ScrollOffset.y <= 0)
		{
			ScrollOffset.y = 0;
		}
		if (-ScrollOffset.x > ((width * m_EngineOptions->TILE_SIZE) - (m_EngineOptions->m_screenWidth - iToolWindowWidth)))
		{
			ScrollOffset.x = -((width * m_EngineOptions->TILE_SIZE) - (m_EngineOptions->m_screenWidth - iToolWindowWidth));
		}
		if (-ScrollOffset.y > ((height * m_EngineOptions->TILE_SIZE) - m_EngineOptions->m_screenHeight))
		{
			ScrollOffset.y = -((height * m_EngineOptions->TILE_SIZE) - m_EngineOptions->m_screenHeight);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::BoundSectionPicker()
{
	if (m_MapCreated
	&&	m_ActiveMapName != ""
	&&	m_ActiveWorldName != ""
	&&	m_EngineOptions->m_GameType2D == GameType2D::TOPDOWN)
	{
		if (m_InSectionPicker)
		{
			int mapSizeX = m_EngineOptions->SECTION_ROW_SIZE * (m_EngineOptions->TILE_SIZE / 2);
			int mapSizeY = m_EngineOptions->SECTION_COL_SIZE * (m_EngineOptions->TILE_SIZE / 2);

			XMFLOAT3 position = m_StampNormal32x32->TopLeftPosition();

			if (position.x > mapSizeX - (m_EngineOptions->TILE_SIZE / 2))
			{
				m_StampNormal32x32->TopLeftPosition() = XMFLOAT3(mapSizeX - (m_EngineOptions->TILE_SIZE / 2), position.y, 0);
				m_StampHighlight32x32->TopLeftPosition() = XMFLOAT3(mapSizeX - (m_EngineOptions->TILE_SIZE / 2), position.y, 0);
			}

			position = m_StampNormal32x32->TopLeftPosition();

			if (position.y > mapSizeY - (m_EngineOptions->TILE_SIZE / 2))
			{
				m_StampNormal32x32->TopLeftPosition() = XMFLOAT3(position.x, mapSizeY - (m_EngineOptions->TILE_SIZE / 2), 0);
				m_StampHighlight32x32->TopLeftPosition() = XMFLOAT3(position.x, mapSizeY - (m_EngineOptions->TILE_SIZE / 2), 0);
			}
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SelectTopdownSection()
{
	int sectionIDSelected;
	// Select the section the stamp tool is over for topdown map
	if (m_MapCreated
	&&	m_ActiveMapName != ""
	&&	m_ActiveWorldName != ""
	&&	m_EngineOptions->m_GameType2D == GameType2D::TOPDOWN)
	{
		if (m_InSectionPicker)
		{
			Environment2DMapHelper::SelectTopdownSectionID(sectionIDSelected, m_SelectedTilePos.x, m_SelectedTilePos.y);

			// Get the selected section and enter it
			if (sectionIDSelected != -1)
			{
				m_StampNormal->TopLeftPosition() = XMFLOAT3(0, 0, 0);
				m_StampHighlight->TopLeftPosition() = XMFLOAT3(0, 0, 0);

				ScrollOffset = Environment2DMapHelper::GetSectionScrollOffset(sectionIDSelected);

				m_InSectionPicker = false;
				m_startDelayTimer = true;
			}
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::ReturnToTopdownSectionPicker()
{
	if (m_MapCreated
	&&	m_ActiveMapName != ""
	&&	m_ActiveWorldName != ""
	&&	m_EngineOptions->m_GameType2D == GameType2D::TOPDOWN)
	{
		if (!m_InSectionPicker)
		{
			m_InSectionPicker = true;
			ScrollOffset.x = 0;
			ScrollOffset.y = 0;
		}
	}
}
//================================================================================================================
XMFLOAT2 MapEditorSystem2D::MultiSampleSpritePos()
{
	XMFLOAT2 retpos;

	switch (multiStampSize)
	{
		case MSS_1X1:
		{
			retpos.x = m_TileEdit1x1Sprite->TopLeftPosition().x;
			retpos.y = m_TileEdit1x1Sprite->TopLeftPosition().y;
		}
		break;
		case MSS_2X2:
		{
			retpos.x = m_TileEdit2x2Sprite->TopLeftPosition().x;
			retpos.y = m_TileEdit2x2Sprite->TopLeftPosition().y;
		}
		break;
		case MSS_3X3:
		{
			retpos.x = m_TileEdit3x3Sprite->TopLeftPosition().x;
			retpos.y = m_TileEdit3x3Sprite->TopLeftPosition().y;
		}
		break;
		case MSS_4X4:
		{
			retpos.x = m_TileEdit4x4Sprite->TopLeftPosition().x;
			retpos.y = m_TileEdit4x4Sprite->TopLeftPosition().y;
		}
		break;
		case MSS_5X5:
		{
			retpos.x = m_TileEdit5x5Sprite->TopLeftPosition().x;
			retpos.y = m_TileEdit5x5Sprite->TopLeftPosition().y;
		}
		break;
		case MSS_6X6:
		{
			retpos.x = m_TileEdit6x6Sprite->TopLeftPosition().x;
			retpos.y = m_TileEdit6x6Sprite->TopLeftPosition().y;
		}
		break;
		case MSS_7X7:
		{
			retpos.x = m_TileEdit7x7Sprite->TopLeftPosition().x;
			retpos.y = m_TileEdit7x7Sprite->TopLeftPosition().y;
		}
		break;
		case MSS_8X8:
		{
			retpos.x = m_TileEdit8x8Sprite->TopLeftPosition().x;
			retpos.y = m_TileEdit8x8Sprite->TopLeftPosition().y;
		}
		break;
		case MSS_9X9:
		{
			retpos.x = m_TileEdit9x9Sprite->TopLeftPosition().x;
			retpos.y = m_TileEdit9x9Sprite->TopLeftPosition().y;
		}
		break;
		case MSS_10X10:
		{
			retpos.x = m_TileEdit10x10Sprite->TopLeftPosition().x;
			retpos.y = m_TileEdit10x10Sprite->TopLeftPosition().y;
		}
		break;
	}

	return retpos;
}
//================================================================================================================
void MapEditorSystem2D::UpdateStampSprites()
{
	if (!m_MapCreated)
		return;

	POINT pt;

	GetCursorPos(&pt);
	//ScreenToClient(m_Env2D->GetBaseWindow()->GetHwnd(), &pt);

	//pt.y += 10;

	//bool mouseNotInRenderWindow = false;
	//if (CGlobal::IsMouseOverWindow(m_Env2D->GetBaseWindow()->GetHwnd(), pt.x, pt.y, false))
	//	mouseNotInRenderWindow = false;

	/*
	#pragma region "PickMultipleStamp"
	if (!mouseNotInRenderWindow)
	{
		if (m_InSectionPicker)
		{
			stampPressed = false;
			if (m_Env2D->GetMouse().LButton.Pressed)
				stampPressed = true;
			else
				stampPressed = false;
		}
		else
		{
			stampPressed = false;
			if (m_Env2D->GetMouse().LButton.Pressed)
				stampPressed = true;
			else
				stampPressed = false;
		}
	}

	//Build multiple tile selection list
	int currentStampX = m_StampNormal->TopLeftPosition().x;
	int currentStampY = m_StampNormal->TopLeftPosition().y;

	int stamp_size = 0;

	//Pick the stamp tool we need to use
	if (m_TileEdit1x1) { stamp_size = 64; m_StampNormal = m_TileEdit1x1Sprite; m_StampHighlight = m_TileEdit1x1Sprite2; }
	if (m_TileEdit2x2) { stamp_size = 128; m_StampNormal = m_TileEdit2x2Sprite; m_StampHighlight = m_TileEdit2x2Sprite2; }
	if (m_TileEdit3x3) { stamp_size = 192; m_StampNormal = m_TileEdit3x3Sprite; m_StampHighlight = m_TileEdit3x3Sprite2; }
	if (m_TileEdit4x4) { stamp_size = 256; m_StampNormal = m_TileEdit4x4Sprite; m_StampHighlight = m_TileEdit4x4Sprite2; }
	if (m_TileEdit5x5) { stamp_size = 320; m_StampNormal = m_TileEdit5x5Sprite; m_StampHighlight = m_TileEdit5x5Sprite2; }
	if (m_TileEdit6x6) { stamp_size = 384; m_StampNormal = m_TileEdit6x6Sprite; m_StampHighlight = m_TileEdit6x6Sprite2; }
	if (m_TileEdit7x7) { stamp_size = 448; m_StampNormal = m_TileEdit7x7Sprite; m_StampHighlight = m_TileEdit7x7Sprite2; }
	if (m_TileEdit8x8) { stamp_size = 512; m_StampNormal = m_TileEdit8x8Sprite; m_StampHighlight = m_TileEdit8x8Sprite2; }
	if (m_TileEdit9x9) { stamp_size = 576; m_StampNormal = m_TileEdit9x9Sprite; m_StampHighlight = m_TileEdit9x9Sprite2; }
	if (m_TileEdit10x10) { stamp_size = 640; m_StampNormal = m_TileEdit10x10Sprite; m_StampHighlight = m_TileEdit10x10Sprite2; }

	int stamp_x_max = m_EngineOptions->SECTION_COL_SIZE * (m_EngineOptions->ROW_SIZE * m_EngineOptions->TILE_SIZE) - stamp_size;
	int stamp_y_max = m_EngineOptions->SECTION_ROW_SIZE * (m_EngineOptions->COL_SIZE * m_EngineOptions->TILE_SIZE) - stamp_size;

	//Make sure stamp size does not go off the edge of the map
	if (currentStampY > stamp_y_max)
	{
		currentStampY -= 64;
	}

	if (currentStampX > stamp_x_max)
	{
		currentStampX -= 64;
	}

	//Update the new position
	if (m_InSectionPicker)
	{
		//m_StampNormal32x32->SetPosition(XMFLOAT3(currentStampX, currentStampY, 0));
		//m_StampHighlight32x32->SetPosition(XMFLOAT3(currentStampX, currentStampY, 0));
	}
	else
	{
		m_StampNormal->TopLeftPosition() = XMFLOAT3(currentStampX, currentStampY, 0);
		m_StampHighlight->TopLeftPosition() = XMFLOAT3(currentStampX, currentStampY, 0);
	}
	#pragma endregion

	float newX = 0;
	float newY = 0;

	//(m_InSectionPicker == true) ? m_OverworldText = true : m_OverworldText = false;

	if (m_InSectionPicker)
	{
		#pragma region section picker
		if (stampPressed)
		{
			newX = m_StampHighlight32x32->TopLeftPosition().x;
			newY = m_StampHighlight32x32->TopLeftPosition().y;
		}
		else
		{
			newX = m_StampNormal32x32->TopLeftPosition().x;
			newY = m_StampNormal32x32->TopLeftPosition().y;
		}

		m_SelectionTimer->Sleep(100);
		if (m_SelectionTimer->IsTimeUp())
		{
			//Map the stamp the key press, and fix the tool from going off the map
			//if (m_inputSystem->KeyDown(DIK_W))
			if (m_Env2D->GetKeyboard().IsKeyDown(Keyboard::W))
			{
				//Move stamp up
				newY -= m_EngineOptions->TILE_SIZE / 2;
				m_currentSection--;
				if (newY < 0)
				{
					m_currentSection++;
					newY += m_EngineOptions->TILE_SIZE / 2;
				}
			}
			//else if (m_inputSystem->KeyDown(DIK_S))
			else if (m_Env2D->GetKeyboard().IsKeyDown(Keyboard::S))
			{
				//Move stamp down
				newY += m_EngineOptions->TILE_SIZE / 2;
				m_currentSection++;
				if (newY > ((m_EngineOptions->SECTION_COL_SIZE * m_EngineOptions->TILE_SIZE / 2) - 1))
				{
					m_currentSection--;
					newY -= m_EngineOptions->TILE_SIZE / 2;
				}
			}

			//if (m_inputSystem->KeyDown(DIK_A))
			if (m_Env2D->GetKeyboard().IsKeyDown(Keyboard::A))
			{
				//Move stamp left
				newX -= m_EngineOptions->TILE_SIZE / 2;
				m_currentSection -= m_EngineOptions->SECTION_ROW_SIZE;
				if (newX < 0)
				{
					m_currentSection += m_EngineOptions->SECTION_ROW_SIZE;
					newX += m_EngineOptions->TILE_SIZE / 2;
				}
			}
			//else if (m_inputSystem->KeyDown(DIK_D))
			else if (m_Env2D->GetKeyboard().IsKeyDown(Keyboard::D))
			{
				//Move stamp right
				newX += m_EngineOptions->TILE_SIZE / 2;
				m_currentSection += m_EngineOptions->SECTION_ROW_SIZE;
				if (newX > ((m_EngineOptions->SECTION_ROW_SIZE * m_EngineOptions->TILE_SIZE / 2) - 1))
				{
					m_currentSection -= m_EngineOptions->SECTION_ROW_SIZE;
					newX -= m_EngineOptions->TILE_SIZE / 2;
				}
			}
		}

		//if (m_engineOptions->SECTION_ROW_SIZE == 20 && m_engineOptions->SECTION_COL_SIZE == 20 ||
		//m_engineOptions->SECTION_ROW_SIZE == 30 && m_engineOptions->SECTION_COL_SIZE == 30)
		//{
		//m_stampNormal32x32->Move(newX, newY);
		//m_stampHighlight32x32->Move(newX, newY);
		//m_useArrowKeys = true;
		//}
		//else m_useArrowKeys = false;
		#pragma endregion
	}
	else
	{
		#pragma region map tile picker
		if (m_ModeTile)
		{
			if (stampPressed)
			{
				newX = m_StampHighlight->TopLeftPosition().x;
				newY = m_StampHighlight->TopLeftPosition().y;
			}
			else
			{
				newX = m_StampNormal->TopLeftPosition().x;
				newY = m_StampNormal->TopLeftPosition().y;
			}

			m_SelectionTimer->Sleep(10);
			if (m_SelectionTimer->IsTimeUp())
			{
				//Map the stamp the key press, and fix the tool from going off the map
				//if (m_inputSystem->KeyDown(DIK_W))
				if (m_Env2D->GetKeyboard().IsKeyDown(Keyboard::W))
				{
					//Move stamp up
					newY = newY - m_EngineOptions->TILE_SIZE;
					if (newY < 0)
					{
						newY = newY + m_EngineOptions->TILE_SIZE;
					}
				}
				//else if (m_inputSystem->KeyDown(DIK_S))
				else if (m_Env2D->GetKeyboard().IsKeyDown(Keyboard::S))
				{
					//Move stamp down
					newY = newY + m_EngineOptions->TILE_SIZE;
					if (newY >((m_EngineOptions->SECTION_COL_SIZE * (m_EngineOptions->COL_SIZE * m_EngineOptions->TILE_SIZE)) - 1))
					{
						newY = newY - m_EngineOptions->TILE_SIZE;
					}
				}

				//if (m_inputSystem->KeyDown(DIK_A))
				if (m_Env2D->GetKeyboard().IsKeyDown(Keyboard::A))
				{
					//Move stamp left
					newX = newX - m_EngineOptions->TILE_SIZE;
					if (newX < 0)
					{
						newX = newX + m_EngineOptions->TILE_SIZE;
					}
				}
				//else if (m_inputSystem->KeyDown(DIK_D))
				else if (m_Env2D->GetKeyboard().IsKeyDown(Keyboard::D))
				{
					//Move stamp right
					newX = newX + m_EngineOptions->TILE_SIZE;
					if (newX >((m_EngineOptions->SECTION_ROW_SIZE * (m_EngineOptions->ROW_SIZE * m_EngineOptions->TILE_SIZE)) - 1))
					{
						newX = newX - m_EngineOptions->TILE_SIZE;
					}
				}
			}
		}
		#pragma endregion
	}

	#pragma region "MoveStampTool"
	bool selected = false;
	//Set the new position of the stamp tool
	if (m_InSectionPicker)// && !m_useArrowKeys)
	{
		int mxHover = pt.x;// +m_map->GetCameraPosition().x;
		int myHover = pt.y;// +-m_map->GetCameraPosition().y;

		float step = m_EngineOptions->TILE_SIZE / 2; // Grid step size.
		float xHover = step * floor((mxHover / step));// + 0.5);
		float yHover = step * floor((myHover / step));// + 0.5);

		int sec_num = 0;
		bool out = false;
		for (int x = 0; x < m_EngineOptions->SECTION_ROW_SIZE; x++)
		{
			float mx = x * m_EngineOptions->TILE_SIZE / 2;

			for (int y = 0; y < m_EngineOptions->SECTION_COL_SIZE; y++)
			{
				float my = y * m_EngineOptions->TILE_SIZE / 2;

				if (xHover == mx && yHover == my)
				{
					m_currentSection = sec_num;
					out = true;
					break;
				}

				sec_num++;
			}

			if (out) break;
		}

		//Limit the amount of movement based on the map size
		bool xHoverFalse = false;
		if (xHover > (m_EngineOptions->SECTION_ROW_SIZE * m_EngineOptions->TILE_SIZE / 2) - m_EngineOptions->TILE_SIZE / 2)
		{
			xHover = (m_EngineOptions->SECTION_ROW_SIZE * m_EngineOptions->TILE_SIZE / 2) - m_EngineOptions->TILE_SIZE / 2;
			selected = false;
			xHoverFalse = true;
		}
		else selected = true;

		if (yHover > (m_EngineOptions->SECTION_COL_SIZE * m_EngineOptions->TILE_SIZE / 2) - m_EngineOptions->TILE_SIZE / 2)
		{
			yHover = (m_EngineOptions->SECTION_COL_SIZE * m_EngineOptions->TILE_SIZE / 2) - m_EngineOptions->TILE_SIZE / 2;
			selected = false;
		}
		else
		{
			if (!xHoverFalse)
				selected = true;
		}

		m_StampNormal32x32->Move(xHover, yHover, 0, 0);
		m_StampHighlight32x32->Move(xHover, yHover, 0, 0);
	}
	else if (!m_InSectionPicker)
	{
		//if (m_ModeTile)
		{
			XMFLOAT3 tilePos = m_Env2D->MatchingTilePos(pt.x, pt.y);

			m_StampNormal->Move(tilePos.x, tilePos.y, 0, 0);
			m_StampHighlight->Move(tilePos.x, tilePos.y, 0, 0);
		}

		if (m_Env2D->GetMouse().LButton.Pressed)
		{
			m_Env2D->GetMouse().LButton.IsPressed() = false;
			m_SelectedTilePos.x = m_StampNormal->TopLeftPosition().x + 1;
			m_SelectedTilePos.y = m_StampNormal->TopLeftPosition().y + 1;
		}
	}
	#pragma endregion
	*/

	bool selected = false;
	/*
	// Select the section the mouse has clicked in the section picker
	#pragma region "PickSection"
	if (m_InSectionPicker && m_currentSection != -1)
	{
		if (m_Env2D->GetMouse().LButton.Pressed)
		{
			if (selected)
			{
				/*m_toggleTText->ChangeText("Press I to toggle this help text - use arrow keys to move around map");

				if (m_ModeTile)
				{
					m_messageText->ChangeText("Tile Edit Mode");
					m_helpText->ChangeText("In this mode use mouse to move the stamp tool and press left mouse button to stamp texture; Space to view hardness");
				}

				if (m_ModeSprite)
				{
					m_messageText->ChangeText("Sprite Edit Mode");
					m_helpText->ChangeText("In this mode you can edit sprite properties, delete a sprite, move a sprite and place sprites; Q to delete display sprite; Space to view hardness");
				}

				if (m_ModePlaceMusic)
				{
					m_messageText->ChangeText("Set Music Mode");
					m_helpText->ChangeText("In this mode you select a section with the mouse and add music to it");
				}
				*

				XMFLOAT2 stampPos = m_Env2D->SelectRenderingSection(m_currentSection);
				
				m_StampNormal->Move(stampPos.x, stampPos.y, 0, 0);
				m_StampHighlight->Move(stampPos.x, stampPos.y, 0, 0);

				m_InSectionPicker = false;
				m_startDelayTimer = true;
			}
		}
	}
	#pragma endregion
	*/

	//Add this delay so that we do not automatically stamp an image
	//to the tile when we click on a section of the map in the section picker
	//if (m_startDelayTimer && !mouseNotInRenderWindow)
	//{
	//	m_ClickDelayTimer->Sleep(100);
	//	if (!m_ClickDelayTimer->IsTimeUp())
	//	{
	//		stampPressed = false;
	//	}
	//	else
	//	{
	//		m_startDelayTimer = false;
	//	}
	//}

	//if (!m_InSectionPicker)
	//{
	//	if (m_Env2D->GetKeyboard()->IsKeyDown(Keyboard::F1))
	//	{
	//		//m_toggleTText->ChangeText("Press I to toggle this help text");
	//		//m_messageText->ChangeText("");
	//		//m_helpText->ChangeText("Use mouse to move the selector and left click to select a section");

	//		m_InSectionPicker = true;
	//	}
	//}
}
//================================================================================================================
XMFLOAT2 MapEditorSystem2D::SnapToGrid(int x, int y)
{
	XMFLOAT2 point = XMFLOAT2(x, y);
	
	if (editMode == EM_Sprites)
	{
		usingGrid = false;
	}
	
	if (editMode == EM_Tiles)
	{
		usingGrid = true;
	}
	
	if (gridMode == GM_Free)
	{
		usingGrid = false;
	}
	if (gridMode == GM_Snap)
	{
		usingGrid = true;
	}

	if (m_InSectionPicker)
	{
		gridSizeX = m_EngineOptions->TILE_SIZE / 2;
		gridSizeY = m_EngineOptions->TILE_SIZE / 2;
	}
	else
	{
		gridSizeX = m_EngineOptions->TILE_SIZE;
		gridSizeY = m_EngineOptions->TILE_SIZE;
	}

	if (usingGrid)
	{
		point.x = (x / gridSizeX) * gridSizeX;
		point.y = (y / gridSizeY) * gridSizeY;
	}
	
	return point;
}
//================================================================================================================
void MapEditorSystem2D::CalculateNewTilePosition(int x, int y)
{
	m_SelectedTilePos = SnapToGrid(x, y);
}
//================================================================================================================
void MapEditorSystem2D::RenderStampSprites(Camera* camera)
{
	if (m_EngineOptions->m_GameType2D == GameType2D::TOPDOWN)
	{
		if (m_InSectionPicker)
		{
			m_D3DSystem->TurnOnAlphaBlending();
			{
				if (stampPressed)
					m_StampHighlight32x32->Render(camera, 0);
				else
					m_StampNormal32x32->Render(camera, 0);
			}
			m_D3DSystem->TurnOffAlphaBlending();
		}
		else
		{
			if (editMode == EM_Tiles || editMode == EM_Hardness)
			{
				if (tileAction == TA_MultiStamp)
				{
					RenderMultiStampSprites(camera);
				}
				else
				{
					m_D3DSystem->TurnOnAlphaBlending();
					{
						if (stampPressed)
							m_StampHighlight->Render(camera, 0);
						else
							m_StampNormal->Render(camera, 0);
					}
					m_D3DSystem->TurnOffAlphaBlending();
				}
			}

			if (editMode == EM_Sprites && gridMode == GM_Snap)
			{
				m_D3DSystem->TurnOnAlphaBlending();
				{
					if (stampPressed)
						m_StampHighlight->Render(camera, 0);
					else
						m_StampNormal->Render(camera, 0);
				}
				m_D3DSystem->TurnOffAlphaBlending();
			}
		}
	}
	else if (m_EngineOptions->m_GameType2D == GameType2D::PLATFORMER)
	{
		if (editMode == EM_Tiles || editMode == EM_Hardness)
		{
			if (tileAction == TA_MultiStamp)
			{
				RenderMultiStampSprites(camera);
			}
			else
			{
				m_D3DSystem->TurnOnAlphaBlending();
				{
					if (stampPressed)
						m_StampHighlight->Render(camera, 0);
					else
						m_StampNormal->Render(camera, 0);
				}
				m_D3DSystem->TurnOffAlphaBlending();
			}
		}
	}
	
	// Render a box around the selected sprite from Select mode
	if (m_display_selected_sprite_box)
	{
		m_D3DSystem->TurnOnAlphaBlending();
		{
			m_select_sprite_box->Render(camera, 0, ScrollOffset);
		}
		m_D3DSystem->TurnOffAlphaBlending();
	}

	if (m_display_sprite_box_created)
	{
		m_D3DSystem->TurnOnAlphaBlending();
		{
			m_sprite_box->Render(camera, 0, ScrollOffset);
		}
		m_D3DSystem->TurnOffAlphaBlending();
	}
}
//================================================================================================================
void MapEditorSystem2D::RenderMultiStampSprites(Camera* camera)
{
	m_D3DSystem->TurnOnAlphaBlending();
	{
		if (stampPressed)
		{
			switch (multiStampSize)
			{
				case MSS_1X1: { m_TileEdit1x1Sprite2->Render(camera, 0); } break;
				case MSS_2X2: { m_TileEdit2x2Sprite2->Render(camera, 0); } break;
				case MSS_3X3: { m_TileEdit3x3Sprite2->Render(camera, 0); } break;
				case MSS_4X4: { m_TileEdit4x4Sprite2->Render(camera, 0); } break;
				case MSS_5X5: { m_TileEdit5x5Sprite2->Render(camera, 0); } break;
				case MSS_6X6: { m_TileEdit6x6Sprite2->Render(camera, 0); } break;
				case MSS_7X7: { m_TileEdit7x7Sprite2->Render(camera, 0); } break;
				case MSS_8X8: { m_TileEdit8x8Sprite2->Render(camera, 0); } break;
				case MSS_9X9: { m_TileEdit9x9Sprite2->Render(camera, 0); } break;
				case MSS_10X10: { m_TileEdit10x10Sprite2->Render(camera, 0); } break;
			}
		}
		else
		{
			switch (multiStampSize)
			{
				case MSS_1X1: { m_TileEdit1x1Sprite->Render(camera, 0); } break;
				case MSS_2X2: { m_TileEdit2x2Sprite->Render(camera, 0); } break;
				case MSS_3X3: { m_TileEdit3x3Sprite->Render(camera, 0); } break;
				case MSS_4X4: { m_TileEdit4x4Sprite->Render(camera, 0); } break;
				case MSS_5X5: { m_TileEdit5x5Sprite->Render(camera, 0); } break;
				case MSS_6X6: { m_TileEdit6x6Sprite->Render(camera, 0); } break;
				case MSS_7X7: { m_TileEdit7x7Sprite->Render(camera, 0); } break;
				case MSS_8X8: { m_TileEdit8x8Sprite->Render(camera, 0); } break;
				case MSS_9X9: { m_TileEdit9x9Sprite->Render(camera, 0); } break;
				case MSS_10X10: { m_TileEdit10x10Sprite->Render(camera, 0); } break;
			}
		}
	}
	m_D3DSystem->TurnOffAlphaBlending();
}
//================================================================================================================
/*void MapEditorSystem2D::UpdateTileProperties()
{
	if (m_MapCreated)
	{

	}
}
//================================================================================================================
void MapEditorSystem2D::AddTile(string activeMapName, string activeWorldName)
{
	if (editMode == EM_Tiles && m_MapCreated && tileAction == TA_AddTile)
	{
		//if (m_Env2D->GetMouse().LButton.Pressed)
		//if (m_leftButtonDown)
		{
			//The user has changed to image of a tile
			m_Updated = true;
			//Environment2DMapHelper::AddTile(activeMapName, activeWorldName, m_SelectedTilePos.x, m_SelectedTilePos.y);

			Environment2DMapHelper::UpdateTileTexture(activeMapName, activeWorldName, "tile_world_1_1_A.png", m_SelectedTilePos.x, m_SelectedTilePos.y);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::RemoveTile(string activeMapName, string activeWorldName)
{
	if (editMode == EM_Tiles && m_MapCreated && tileAction == TA_RemoveTile)
	{
		//if (m_Env2D->GetMouse().LButton.Pressed)
		//if (m_leftButtonDown)
		{
			//m_Env2D->GetMouse().LButton.IsPressed() = false;
			//The user has changed to image of a tile
			m_Updated = true;
			Environment2DMapHelper::RemoveTile(activeMapName, activeWorldName,
				m_SelectedTilePos.x, m_SelectedTilePos.y);
		}
	}
}*/
//================================================================================================================
void MapEditorSystem2D::RemoveTexture()
{
	if (m_MapCreated && editMode == EM_Tiles && m_MapCreated && tileAction == TA_RemoveTexture)
	{
		//The user has removed the image of a tile
		m_Updated = true;
		Environment2DMapHelper::RemoveTileTexture(m_SelectedTilePos.x, m_SelectedTilePos.y);
	}
}
//================================================================================================================
void MapEditorSystem2D::SetMapVisionAttribute(int vision)
{
	Environment2DMapHelper::SetMapVisionAttribute(vision);
}
//================================================================================================================
void MapEditorSystem2D::SetMapMusicAttribute(string music)
{
	Environment2DMapHelper::SetMapMusicAttribute(music);
}
//================================================================================================================
void MapEditorSystem2D::SetMapFOWAttribute(bool fow)
{
	Environment2DMapHelper::SetMapFOWAttribute(fow);
}
//================================================================================================================
void MapEditorSystem2D::SetMapFOWRadiusAttribute(int fowRadius)
{
	Environment2DMapHelper::SetMapFOWRadiusAttribute(fowRadius);
}
//================================================================================================================
void MapEditorSystem2D::SetMapFOWFlashlightAttribute(bool fowFlashlight)
{
	Environment2DMapHelper::SetMapFOWFlashlightAttribute(fowFlashlight);
}
//================================================================================================================
void MapEditorSystem2D::SetMapFOWUncoveredAttribute(bool fowUncovered)
{
	Environment2DMapHelper::SetMapFOWUncoveredAttribute(fowUncovered);
}
//================================================================================================================
void MapEditorSystem2D::SetMapNameAttribute(string mapName)
{
	Environment2DMapHelper::SetMapNameAttribute(mapName);
}
//================================================================================================================
void MapEditorSystem2D::UpdateTileStamp()
{
	if (m_MapCreated && editMode == EM_Tiles && (tileAction == TA_Regular || tileAction == TA_MultiStamp))
	{
		//The user has changed to image of a tile
		m_Updated = true;

		if (BetterString(m_SelectedTileName) != "")
		{
			int spotSizeX = 0;
			int spotSizeY = 0;

			//Loop through each spot in the stamp selector
			
			switch (multiStampSize)
			{
				case MSS_1X1: { spotSizeX = spotSizeY = 1; } break;
				case MSS_2X2: { spotSizeX = spotSizeY = 2; } break;
				case MSS_3X3: { spotSizeX = spotSizeY = 3; } break;
				case MSS_4X4: { spotSizeX = spotSizeY = 4; } break;
				case MSS_5X5: { spotSizeX = spotSizeY = 5; } break;
				case MSS_6X6: { spotSizeX = spotSizeY = 6; } break;
				case MSS_7X7: { spotSizeX = spotSizeY = 7; } break;
				case MSS_8X8: { spotSizeX = spotSizeY = 8; } break;
				case MSS_9X9: { spotSizeX = spotSizeY = 9; } break;
				case MSS_10X10: { spotSizeX = spotSizeY = 10; } break;
			}

			if (tileAction == TA_Regular)
			{
				spotSizeX = spotSizeY = 1;
			}

			//Stamp tile now has x spots to stamp
			int startX = m_SelectedTilePos.x;
			int startY = m_SelectedTilePos.y;

			for (int x = 0; x < spotSizeX; x++)
			{
				for (int y = 0; y < spotSizeY; y++)
				{
					//Get the previous image name from the tile
					Environment2DMapHelper::UpdateTileTexture(m_SelectedTileName, startX, startY);

					startY += m_EngineOptions->TILE_SIZE;
				}

				startX += m_EngineOptions->TILE_SIZE;
				startY = m_SelectedTilePos.y;
			}
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::RevertTileAnim()
{
	if (m_MapCreated && editMode == EM_Tiles && (tileAction == TA_Regular || tileAction == TA_MultiStamp))
	{
		int spotSizeX = 0;
		int spotSizeY = 0;

		//Loop through each spot in the stamp selector
		
		switch (multiStampSize)
		{
			case MSS_1X1: { spotSizeX = spotSizeY = 1; } break;
			case MSS_2X2: { spotSizeX = spotSizeY = 2; } break;
			case MSS_3X3: { spotSizeX = spotSizeY = 3; } break;
			case MSS_4X4: { spotSizeX = spotSizeY = 4; } break;
			case MSS_5X5: { spotSizeX = spotSizeY = 5; } break;
			case MSS_6X6: { spotSizeX = spotSizeY = 6; } break;
			case MSS_7X7: { spotSizeX = spotSizeY = 7; } break;
			case MSS_8X8: { spotSizeX = spotSizeY = 8; } break;
			case MSS_9X9: { spotSizeX = spotSizeY = 9; } break;
			case MSS_10X10: { spotSizeX = spotSizeY = 10; } break;
		}

		if (tileAction == TA_Regular)
		{
			spotSizeX = spotSizeY = 1;
		}
		
		int startX = m_SelectedTilePos.x;
		int startY = m_SelectedTilePos.y;

		for (int x = 0; x < spotSizeX; x++)
		{
			for (int y = 0; y < spotSizeY; y++)
			{
				Environment2DMapHelper::NullTileAnimation(startX, startY);

				startY += m_EngineOptions->TILE_SIZE;
			}

			startX += m_EngineOptions->TILE_SIZE;
			startY = m_SelectedTilePos.y;
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::UpdateTileAnimation(string animProfileName, int animSpeed)
{
	if (m_MapCreated && editMode == EM_Tiles && (tileAction == TA_Regular || tileAction == TA_MultiStamp))
	{
		int spotSizeX = 0;
		int spotSizeY = 0;

		//Loop through each spot in the stamp selector
		
		switch (multiStampSize)
		{
			case MSS_1X1: { spotSizeX = spotSizeY = 1; } break;
			case MSS_2X2: { spotSizeX = spotSizeY = 2; } break;
			case MSS_3X3: { spotSizeX = spotSizeY = 3; } break;
			case MSS_4X4: { spotSizeX = spotSizeY = 4; } break;
			case MSS_5X5: { spotSizeX = spotSizeY = 5; } break;
			case MSS_6X6: { spotSizeX = spotSizeY = 6; } break;
			case MSS_7X7: { spotSizeX = spotSizeY = 7; } break;
			case MSS_8X8: { spotSizeX = spotSizeY = 8; } break;
			case MSS_9X9: { spotSizeX = spotSizeY = 9; } break;
			case MSS_10X10: { spotSizeX = spotSizeY = 10; } break;
		}

		if (tileAction == TA_Regular)
		{
			spotSizeX = spotSizeY = 1;
		}
		
		int startX = m_SelectedTilePos.x;
		int startY = m_SelectedTilePos.y;

		for (int x = 0; x < spotSizeX; x++)
		{
			for (int y = 0; y < spotSizeY; y++)
			{
				Environment2DMapHelper::UpdateTileAnimation(animProfileName, animSpeed, startX, startY);

				startY += m_EngineOptions->TILE_SIZE;
			}

			startX += m_EngineOptions->TILE_SIZE;
			startY = m_SelectedTilePos.y;
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::UpdateTileHardness(bool bHard)
{
	//if (m_MapCreated && editMode == EM_Tiles && (tileAction == TA_Regular || tileAction == TA_MultiStamp))
	if (m_MapCreated && editMode == EM_Hardness && hardnessAction == HA_Tiles)
	{
		int spotSizeX = 0;
		int spotSizeY = 0;

		//Loop through each spot in the stamp selector
		
		switch (multiStampSize)
		{
			case MSS_1X1: { spotSizeX = spotSizeY = 1; } break;
			case MSS_2X2: { spotSizeX = spotSizeY = 2; } break;
			case MSS_3X3: { spotSizeX = spotSizeY = 3; } break;
			case MSS_4X4: { spotSizeX = spotSizeY = 4; } break;
			case MSS_5X5: { spotSizeX = spotSizeY = 5; } break;
			case MSS_6X6: { spotSizeX = spotSizeY = 6; } break;
			case MSS_7X7: { spotSizeX = spotSizeY = 7; } break;
			case MSS_8X8: { spotSizeX = spotSizeY = 8; } break;
			case MSS_9X9: { spotSizeX = spotSizeY = 9; } break;
			case MSS_10X10: { spotSizeX = spotSizeY = 10; } break;
		}

		if (tileAction == TA_Regular)
		{
			spotSizeX = spotSizeY = 1;
		}
		
		int startX = m_SelectedTilePos.x;
		int startY = m_SelectedTilePos.y;

		for (int x = 0; x < spotSizeX; x++)
		{
			for (int y = 0; y < spotSizeY; y++)
			{
				Environment2DMapHelper::UpdateTileHardness(bHard, startX, startY);

				startY += m_EngineOptions->TILE_SIZE;
			}

			startX += m_EngineOptions->TILE_SIZE;
			startY = m_SelectedTilePos.y;
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::MakeTileHard()
{
	if (m_MapCreated && editMode == EM_Hardness && hardnessAction == HA_Tiles)
	{
		int spotSizeX = 0;
		int spotSizeY = 0;

		//Loop through each spot in the stamp selector
		
		switch (multiStampSize)
		{
			case MSS_1X1: { spotSizeX = spotSizeY = 1; } break;
			case MSS_2X2: { spotSizeX = spotSizeY = 2; } break;
			case MSS_3X3: { spotSizeX = spotSizeY = 3; } break;
			case MSS_4X4: { spotSizeX = spotSizeY = 4; } break;
			case MSS_5X5: { spotSizeX = spotSizeY = 5; } break;
			case MSS_6X6: { spotSizeX = spotSizeY = 6; } break;
			case MSS_7X7: { spotSizeX = spotSizeY = 7; } break;
			case MSS_8X8: { spotSizeX = spotSizeY = 8; } break;
			case MSS_9X9: { spotSizeX = spotSizeY = 9; } break;
			case MSS_10X10: { spotSizeX = spotSizeY = 10; } break;
		}

		if (tileAction == TA_Regular)
		{
			spotSizeX = spotSizeY = 1;
		}
		
		int startX = m_SelectedTilePos.x;
		int startY = m_SelectedTilePos.y;

		for (int x = 0; x < spotSizeX; x++)
		{
			for (int y = 0; y < spotSizeY; y++)
			{
				Environment2DMapHelper::MakeTileHard(startX, startY);

				startY += m_EngineOptions->TILE_SIZE;
			}

			startX += m_EngineOptions->TILE_SIZE;
			startY = m_SelectedTilePos.y;
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::AddSprite()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Place)
	{
		//The user has added a sprite to the map
		m_Updated = true;
		Environment2DMapHelper::AddSprite(m_SelectedSpriteName, m_SelectedTilePos.x, m_SelectedTilePos.y);
	}
}
//================================================================================================================
void MapEditorSystem2D::MoveSprite(float x, float y)
{
	if (m_MapCreated && (m_moving_sprite || m_cloning_sprite) && editMode == EM_Sprites && (spriteAction == SA_Move || spriteAction == SA_Clone))
	{
		//The user has moved a sprite in the map
		m_Updated = true;
		
		if (m_moving_sprite)
			Environment2DMapHelper::MoveSprite(m_MovingSprite, x, y);

		if (m_cloning_sprite)
			Environment2DMapHelper::MoveSprite(m_SelectedSprite, x, y);
	}
}
//================================================================================================================
bool MapEditorSystem2D::MoveSpriteStart()
{
	if (m_MapCreated && !m_moving_sprite && editMode == EM_Sprites && spriteAction == SA_Move)
	{
		bool clicked = Environment2DMapHelper::MoveSpriteStart(m_MovingSprite, m_originalSpriteID, m_SelectedTilePos.x, m_SelectedTilePos.y);
		
		if (clicked)
		{
			m_Updated = true;
			m_moving_sprite = true;
		}
		
		return clicked;
	}
	
	// Not in sprite mode move and has not clicked on a sprite or is already moving
	return false;
}
//================================================================================================================
void MapEditorSystem2D::MoveSpriteEnd()
{
	if (m_MapCreated && m_moving_sprite && editMode == EM_Sprites && spriteAction == SA_Move)
	{
		m_Updated = true;
		m_moving_sprite = false;
		Environment2DMapHelper::MoveSpriteEnd(m_MovingSprite, m_originalSpriteID, m_SelectedTilePos.x, m_SelectedTilePos.y);
		m_originalSpriteID = -1;
	}
}
//================================================================================================================
bool MapEditorSystem2D::CloneSpriteStart()
{
	if (m_MapCreated && !m_cloning_sprite && editMode == EM_Sprites && spriteAction == SA_Clone)
	{
		bool clicked = Environment2DMapHelper::CloneSpriteStart(m_SelectedSprite, m_SelectedTilePos.x, m_SelectedTilePos.y);
		
		if (clicked)
		{
			m_Updated = true;
			m_cloning_sprite = true;
			m_selected_sprite_cover = true;
		}
		
		return clicked;
	}
	
	// Not in sprite mode clone and has not clicked on a sprite or is already moving
	return false;
}
//================================================================================================================
void MapEditorSystem2D::CloneSpriteEnd()
{
	if (m_MapCreated && m_cloning_sprite && editMode == EM_Sprites && spriteAction == SA_Clone)
	{
		m_Updated = true;
		m_cloning_sprite = false;
		Environment2DMapHelper::CloneSpriteEnd(m_SelectedSprite, m_SelectedTilePos.x, m_SelectedTilePos.y);

		if (m_cloneDisplaySprite != 0)
		{
			//Need to remove the current display sprite if there is one
			Environment2DMapHelper::RemoveCloneDisplaySprite();
			m_cloneDisplaySprite = 0;
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::DeleteSprite()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Delete)
	{
		AISprite* spr;
		bool clicked = Environment2DMapHelper::SpriteClicked(spr, m_SelectedTilePos.x, m_SelectedTilePos.y);
		if (clicked)
		{
			//The user has deleted a sprite in the map
			m_Updated = true;
			Environment2DMapHelper::DeleteSprite(spr);
		}
	}
}
//================================================================================================================
bool MapEditorSystem2D::ResizeSpriteStart()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_EditSize)
	{
		//The user has started to resize a sprite in the map
		bool clicked = Environment2DMapHelper::SpriteClicked(m_EditSizeSprite, m_SelectedTilePos.x, m_SelectedTilePos.y);
		
		if (clicked)
		{
			m_Updated = true;
			m_resizing_sprite = true;
		}
		
		return clicked;
	}
	
	return false;
}
//================================================================================================================
void MapEditorSystem2D::ResizeSpriteUpdate(
	bool updatePosition,
	bool updateSize,
	bool goFast,
	bool pressedLeftKey,
	bool pressedRightKey,
	bool pressedUpKey,
	bool pressedDownKey
)
{
	if (m_MapCreated && m_resizing_sprite && editMode == EM_Sprites && spriteAction == SA_EditSize)
	{
		//The user has resized a sprite in the map
		m_Updated = true;
		Environment2DMapHelper::ResizeSpriteUpdate(m_EditSizeSprite,
			updatePosition,
			updateSize,
			goFast,
			pressedLeftKey,
			pressedRightKey,
			pressedUpKey,
			pressedDownKey
		);
	}
}
//================================================================================================================
void MapEditorSystem2D::HighlightSprite(float x, float y)
{
	if (m_MapCreated && editMode == EM_Sprites
	&& (spriteAction == SA_Move
	||  spriteAction == SA_Delete
	||  spriteAction == SA_EditSize
	||  spriteAction == SA_Clone
	||  spriteAction == SA_Select))
	{
		//The user has selected a sprite in the map
		m_Updated = true;
		Environment2DMapHelper::HighlightSprite(m_sprite_with_box, m_sprite_box, m_display_sprite_box_created, x, y);
	}
}
//================================================================================================================
void MapEditorSystem2D::UpdateDisplaySprite(float x, float y)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Place)
	{
		//Render the selected sprite from the sprite cache with the mouse cursor
		if (BetterString(m_SelectedSpriteName) != "" && m_sprite_cache_selection_changed)
		{
			if (m_displaySprite != 0)
			{
				//Need to remove the current display sprite if there is one
				Environment2DMapHelper::RemoveDisplaySprite();
				m_displaySprite = 0;
			}
			
			string base_path = m_mainGameDirectory->m_sprites_path;
			base_path.append("\\");
			
			m_displaySprite = new AISprite(m_D3DSystem);
			m_displaySprite->Initialize(
				base_path,
				m_SelectedSpriteName,
				XMFLOAT3(x, y, 0),
				64,
				64,
				STATIC
			);
			m_displaySprite->IsDisplaySprite() = true;
			
			Environment2DMapHelper::AddSpriteInMap(m_displaySprite);
			
			m_sprite_cache_selection_changed = false;

			Environment2DMapHelper::SetCanRenderDisplaySprite(true);
		}
		
		if (m_displaySprite != 0)
		{
			m_displaySprite->TopLeftPosition().x = x;
			m_displaySprite->TopLeftPosition().y = y;
		}
	}

	if (m_MapCreated && editMode != EM_Sprites)
	{
		Environment2DMapHelper::SetCanRenderDisplaySprite(false);
	}

	if (spriteAction == SA_Select || spriteAction == SA_Move || spriteAction == SA_Delete || spriteAction == SA_EditSize || spriteAction == SA_Clone)
	{
		Environment2DMapHelper::SetCanRenderDisplaySprite(false);
	}

	if (spriteAction == SA_Place)
	{
		if (m_displaySprite != 0)
		{
			Environment2DMapHelper::SetCanRenderDisplaySprite(true);
		}
	}

	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Clone)
	{
		//Render the selected sprite from the sprite cache with the mouse cursor
		if (m_selected_sprite_cover)
		{
			if (m_cloneDisplaySprite != 0)
			{
				//Need to remove the current display sprite if there is one
				Environment2DMapHelper::RemoveCloneDisplaySprite();
				m_cloneDisplaySprite = 0;
			}

			m_cloneDisplaySprite = m_SelectedSprite->Clone(m_mainGameDirectory);
			m_cloneDisplaySprite->IsCloneDisplaySprite() = true;

			Environment2DMapHelper::AddSpriteInMap(m_cloneDisplaySprite);

			m_selected_sprite_cover = false;

			Environment2DMapHelper::SetCanRenderCloneDisplaySprite(true);
		}

		if (m_cloneDisplaySprite != 0)
		{
			m_cloneDisplaySprite->TopLeftPosition().x = x;
			m_cloneDisplaySprite->TopLeftPosition().y = y;
		}
	}

	if (m_MapCreated && editMode != EM_Sprites)
	{
		Environment2DMapHelper::SetCanRenderCloneDisplaySprite(false);
	}

	if (spriteAction == SA_Select || spriteAction == SA_Move || spriteAction == SA_Delete || spriteAction == SA_EditSize || spriteAction == SA_Move)
	{
		Environment2DMapHelper::SetCanRenderCloneDisplaySprite(false);
	}

	if (spriteAction == SA_Clone)
	{
		if (m_displaySprite != 0)
		{
			Environment2DMapHelper::SetCanRenderCloneDisplaySprite(true);
		}
	}
}
//================================================================================================================
AISprite* MapEditorSystem2D::GetSelectedSprite()
{
	if (m_MapCreated && m_selectModeSpriteID != -1)
	{
		return Environment2DMapHelper::GetSpriteByID(m_selectModeSpriteID);
	}
}
//================================================================================================================
void MapEditorSystem2D::SelectModeSpriteClicked()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		m_display_selected_sprite_box = false;
		m_select_sprite_box = 0;

		if (!Environment2DMapHelper::GetSpriteID(m_selectModeSpriteID, m_SelectedTilePos.x, m_SelectedTilePos.y))
		{
			m_selectModeSpriteID = -1;
		}
		else
		{
			bWasClicked = true;
		}

		// Create a selected sprite box around the sprite that was selected
		Environment2DMapHelper::HighlightSprite(m_sprite_with_box, m_select_sprite_box, m_display_selected_sprite_box, m_SelectedTilePos.x, m_SelectedTilePos.y);
	}
	else
	{
		m_selectModeSpriteID = -1;
		m_display_selected_sprite_box = false;
	}
}
//================================================================================================================
void MapEditorSystem2D::ToggleSelectedSpriteBackground(bool background)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if its a background sprite
			m_Updated = true;
			Environment2DMapHelper::ToggleSelectedSpriteBackground(background, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::ToggleSelectedSpriteEntity(bool entity)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if its a entity sprite
			m_Updated = true;
			Environment2DMapHelper::ToggleSelectedSpriteEntity(entity, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::ToggleSelectedSpriteInvisible(bool invisible)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if its a invisible sprite
			m_Updated = true;
			Environment2DMapHelper::ToggleSelectedSpriteInvisible(invisible, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::ToggleSelectedSpriteHard(bool hard)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if its hard
			m_Updated = true;
			Environment2DMapHelper::ToggleSelectedSpriteHard(hard, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::ToggleSelectedSpriteNoHit(bool nohit)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be hit
			m_Updated = true;
			Environment2DMapHelper::ToggleSelectedSpriteNoHit(nohit, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::ToggleSelectedSpritePhysicsType(bool isstatic)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating the physics type
			m_Updated = true;
			Environment2DMapHelper::ToggleSelectedSpritePhysicsType(isstatic, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::ToggleSelectedSpriteTouch(bool touch)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::ToggleSelectedSpriteTouch(touch, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
bool MapEditorSystem2D::SelectedSpriteIsBackground()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::SelectedSpriteIsBackground(m_selectModeSpriteID);
		}
	}
	
	return false;
}
//================================================================================================================
bool MapEditorSystem2D::SelectedSpriteIsEntity()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::SelectedSpriteIsEntity(m_selectModeSpriteID);
		}
	}

	return false;
}
//================================================================================================================
bool MapEditorSystem2D::SelectedSpriteIsInvisible()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::SelectedSpriteIsInvisible(m_selectModeSpriteID);
		}
	}

	return false;
}
//================================================================================================================
bool MapEditorSystem2D::SelectedSpriteIsHard()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::SelectedSpriteIsHard(m_selectModeSpriteID);
		}
	}

	return false;
}
//================================================================================================================
bool MapEditorSystem2D::SelectedSpriteIsNoHit()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::SelectedSpriteIsNoHit(m_selectModeSpriteID);
		}
	}

	return false;
}
//================================================================================================================
bool MapEditorSystem2D::SelectedSpriteIsStatic()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::SelectedSpriteIsStatic(m_selectModeSpriteID);
		}
	}

	return false;
}
//================================================================================================================
bool MapEditorSystem2D::SelectedSpriteIsTouch()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::SelectedSpriteIsTouch(m_selectModeSpriteID);
		}
	}

	return false;
}
//================================================================================================================
bool MapEditorSystem2D::SelectedSpriteIsTeleportSprite()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::SelectedSpriteIsTeleportSprite(m_selectModeSpriteID);
		}
	}

	return false;
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteVision(int vision)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteVision(vision, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteDepth(int depth)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteDepth(depth, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteTextureName(string textureName)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteTextureName(textureName, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteAnimationName(string animationName)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteAnimationName(animationName, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteDefaultSequence(int sequenceNum)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteDefaultSequence(sequenceNum, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteAnimationSpeed(int speed)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteAnimationSpeed(speed, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteCollideMsg(string collideMsg)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteCollideMsg(collideMsg, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteHPMax(int value)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteHPMax(value, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteHP(int value)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteHP(value, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteMana(int value)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteMana(value, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteGold(int value)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteGold(value, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteStrength(int value)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteStrength(value, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteSpeed(int value)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteSpeed(value, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteDefense(int value)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteDefense(value, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteExperience(int value)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteExperience(value, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteLevel(int value)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteLevel(value, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteScriptName(string scriptName)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteScriptName(scriptName, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteTeleportMapName(string mapName)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteTeleportMapName(mapName, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteTeleportMapX(int value)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteTeleportMapX(value, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::SetSelectedSpriteTeleportMapY(int value)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteTeleportMapY(value, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::ToggleSelectedSpriteAlwaysSeenByPlayer(bool value)
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::SetSelectedSpriteAlwaysSeenByPlayer(value, m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
float MapEditorSystem2D::GetSelectedSpriteXPos()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteXPos(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
float MapEditorSystem2D::GetSelectedSpriteYPos()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteYPos(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteVision()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteVision(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteDepth()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteDepth(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
string MapEditorSystem2D::GetSelectedSpriteTextureName()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteTextureName(m_selectModeSpriteID);
		}
	}

	return "";
}
//================================================================================================================
string MapEditorSystem2D::GetSelectedSpriteAnimationName()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteAnimationName(m_selectModeSpriteID);
		}
	}

	return "";
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteDefaultSequence()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteDefaultSequence(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteAnimationSpeed()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteAnimationSpeed(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
string MapEditorSystem2D::GetSelectedSpriteCollideMsg()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteCollideMsg(m_selectModeSpriteID);
		}
	}

	return "";
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteHPMax()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteHPMax(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteHP()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteHP(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteMana()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteMana(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteGold()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteGold(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteStrength()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteStrength(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteSpeed()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteSpeed(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteDefense()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteDefense(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteExperience()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteExperience(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteLevel()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteLevel(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
string MapEditorSystem2D::GetSelectedSpriteScriptName()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteScriptName(m_selectModeSpriteID);
		}
	}

	return "";
}
//================================================================================================================
string MapEditorSystem2D::GetSelectedSpriteTeleportMapName()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteTeleportMapName(m_selectModeSpriteID);
		}
	}

	return "";
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteTeleportMapX()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteTeleportMapX(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
int MapEditorSystem2D::GetSelectedSpriteTeleportMapY()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteTeleportMapY(m_selectModeSpriteID);
		}
	}

	return -1;
}
//================================================================================================================
bool MapEditorSystem2D::GetSelectedSpriteAlwaysSeenByPlayer()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			return Environment2DMapHelper::GetSelectedSpriteAlwaysSeenByPlayer(m_selectModeSpriteID);
		}
	}

	return true;
}
//================================================================================================================
void MapEditorSystem2D::RevertSelectedSpriteAnimation()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::RevertSelectedSpriteAnimation(m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::UpdateSelectedSpriteAnimation()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::UpdateSelectedSpriteAnimation(m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::UpdateSelectedSpriteMeshTexture()
{
	if (m_MapCreated && editMode == EM_Sprites && spriteAction == SA_Select)
	{
		if (m_selectModeSpriteID != -1)
		{
			//The user has selected a sprite and updating if it can be touched
			m_Updated = true;
			Environment2DMapHelper::UpdateSelectedSpriteMeshTexture(m_selectModeSpriteID);
		}
	}
}
//================================================================================================================
void MapEditorSystem2D::OpenSelectedSpriteScript(string scriptName)
{
	if (scriptName != "")
	{
		Environment2DMapHelper::OpenSelectedSpriteScript(scriptName);
	}
}
//================================================================================================================
/*void MapEditorSystem2D::UpdateMapProperties()
{
	if (m_MapCreated)
	{
		if (m_mapProperties != NULL)
		{
			if (m_mapProperties->AddPlayer() && !m_mapProperties->DoesPlayerExist())
			{
				//Make sure the player is not on the map from a load map call
				if (!m_map->DoesPlayerExist())
				{
					m_Updated = true;
					Sprite* s = new Sprite();
					string base_path = m_mainGameDirectory->m_required_textures;
					base_path.append("\\");
					s->Initialize(base_path, "player_cover.png", "Render", XMFLOAT3(m_map->GetCameraPosition().x, -m_map->GetCameraPosition().y, 0),
						m_screenWidth, m_screenHeight, m_d3dSystem, 64, 64);
					s->SetSpeed(0.8f);
					s->SetDisplaySprite(false);
					m_map->AddPlayer(s);
					m_mapProperties->SetPlayerExists(true);
					m_mapProperties->SetAddPlayer(false);
				}
			}

			//Why is there a delay with getting the max sprites in map properties
			m_mapProperties->UpdateSpriteStats(m_map->getNumSpritesRendered(), m_map->getMaxSprites());

			//Update the vision and music for the map
			m_map->SetCurrentVision(m_mapProperties->GetVision());
			m_map->SetCurrentMusic(m_mapProperties->GetMusic());
		}
	}
}*/
//================================================================================================================
/*void MapEditorSystem2D::UpdateSpriteProperties()
{
	if (m_MapCreated)
	{
		// Sprite mode and the mouse is in the render window
		if (m_ModeSprite && !mouseNotInRenderWindow)
		{
			if (m_SpriteModeProperties)
			{
				m_map->DeleteDisplaySprite();

				//Need to highlight and click on a sprite
				Sprite* clicked_sprite;
				if (m_inputSystem->LeftMouseDown())
				{
					m_Updated = true;

					if (m_map->SpriteClicked(clicked_sprite, ptX + m_map->GetCameraPosition().x, ptY + -m_map->GetCameraPosition().y))
					{
						m_spritePropertiesSprite = clicked_sprite;
						m_spriteProperties->SetMap(m_map);
						m_spriteProperties->SetD3D(m_d3dSystem);
						m_spriteProperties->SetGD(m_mainGameDirectory);
						OpenSpriteProperties(m_spritePropertiesSprite);
					}
				}
			}
		}
	}
}*/
//================================================================================================================
/*void MapEditorSystem2D::HighlightSprite()
{
	if (m_MapCreated)
	{
		// Sprite mode and the mouse is in the render window
		if (m_ModeSprite && !mouseNotInRenderWindow)
		{
			//Highlight the sprite on mouse over
			if (m_SpriteModeProperties || m_SpriteModeMove || m_SpriteModeDelete || m_SpriteModeEditSize)
			{
				Sprite* p_clicked_sprite;
				if (m_map->SpriteClicked(p_clicked_sprite, ptX + m_map->GetCameraPosition().x, ptY + -m_map->GetCameraPosition().y))
				{
					if (m_sprite_with_box != NULL)
					{
						if (m_sprite_with_box != p_clicked_sprite)
						{
							m_sprite_box = NULL;
							m_display_sprite_box_created = false;
						}
					}

					if (!m_display_sprite_box_created)
					{
						m_sprite_with_box = p_clicked_sprite;

						m_sprite_box = new Sprite();
						string base_path = m_mainGameDirectory->m_required_textures;
						base_path.append("\\");//Textures");
						m_sprite_box->Initialize(base_path, "stamp_normal.png", "Render", XMFLOAT3(p_clicked_sprite->GetPosition().x, p_clicked_sprite->GetPosition().y, 0),
							m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, m_d3dSystem, p_clicked_sprite->GetWidth(), p_clicked_sprite->GetHeight());
						m_display_sprite_box_created = true;
					}
				}
				else
				{
					m_sprite_with_box = NULL;
					m_sprite_box = NULL;
					m_display_sprite_box_created = false;
				}
			}
		}
	}
}*/
//================================================================================================================
/*void MapEditorSystem2D::UpdateSpriteMove()
{
	if (m_MapCreated)
	{
		// Sprite mode and the mouse is in the render window
		if (m_ModeSprite && !mouseNotInRenderWindow)
		{
			if (m_SpriteModeMove)
			{
				m_map->DeleteDisplaySprite();

				if (m_inputSystem->LeftMouseDown())
				{
					m_Updated = true;

					if (m_clicked_sprite == NULL)
					{
						m_map->SpriteClicked(m_clicked_sprite, ptX + m_map->GetCameraPosition().x, ptY + -m_map->GetCameraPosition().y);
						if (m_clicked_sprite != NULL)
							m_spriteBeforeMove = m_clicked_sprite->Clone(m_mainGameDirectory, m_d3dSystem);
					}
				}

				if (m_clicked_sprite != NULL)
				{
					m_clicked_sprite->Move((ptX - m_clicked_sprite->GetWidth()/2) + m_map->GetCameraPosition().x, (ptY - m_clicked_sprite->GetHeight() + 40) + -m_map->GetCameraPosition().y);

					if (m_inputSystem->RightMouseDown())
					{
						string filename = m_clicked_sprite->GetBaseTextureFilename();
						int w = m_clicked_sprite->GetWidth();
						int h = m_clicked_sprite->GetHeight();
						float hbox_x;
						float hbox_y;
						int hbox_w;
						int hbox_h;
						m_clicked_sprite->GetHardbox(hbox_x, hbox_y, hbox_w, hbox_h);
						bool is_player = m_clicked_sprite->IsPlayer();
						int vision = m_clicked_sprite->GetVision();
						int depth = m_clicked_sprite->GetDepth();
						int hard = m_clicked_sprite->IsHard();
						int nohit = m_clicked_sprite->IsNoHit();
						int hp = m_clicked_sprite->GetHP();
						int mana = m_clicked_sprite->GetMana();
						int gold = m_clicked_sprite->GetGold();
						int strength = m_clicked_sprite->GetStrength();
						float speed = m_clicked_sprite->GetSpeed();
						int defense = m_clicked_sprite->GetDefense();
						int exp = m_clicked_sprite->GetExperience();
						int level = m_clicked_sprite->GetLevel();
						int touch = m_clicked_sprite->GetTouch();
						int background = m_clicked_sprite->IsBackgroundSprite();
						int entity = m_clicked_sprite->IsEntitySprite();
						int invisible = m_clicked_sprite->IsInvisibleSprite();
						int teleport = m_clicked_sprite->CanTeleport();
						int teleport_map_x = m_clicked_sprite->GetTeleportX();
						int teleport_map_y = m_clicked_sprite->GetTeleportY();
						int default_seq = m_clicked_sprite->GetDefaultSequence();
						int anim_speed = m_clicked_sprite->GetAnimationSpeed();
						int id = m_clicked_sprite->GetID();
						int max_hp = m_clicked_sprite->GetHPMax();
						int is_static = m_clicked_sprite->IsStatic();
						string name_str = m_clicked_sprite->GetName();
						string animation_str = m_clicked_sprite->GetAnimationProfileName();
						string script_str = m_clicked_sprite->GetScript();
						string teleportMap_str = m_clicked_sprite->GetTeleportMapName();

						if (m_clicked_sprite->IsPlayer())
							m_map->DeletePlayerSprite();
						else
						{
							//Remove the sprite from the sprite list
							m_map->RemoveSprite(m_clicked_sprite);
						}

						//Set m_clicked_sprite to NULL
						//delete m_clicked_sprite;
						m_clicked_sprite = NULL;

						string path;
						if (filename == "player_cover.png")
						{
							path = m_mainGameDirectory->m_required_textures;
						}
						else
						{
							path = m_mainGameDirectory->m_sprites_path;
						}

						//Add the sprite back in
						Sprite* spr = new Sprite();
						spr->Initialize(path, filename, "Render", XMFLOAT3((ptX - w/2) + m_map->GetCameraPosition().x, (ptY - h + 40) + -m_map->GetCameraPosition().y, 0),
							m_screenWidth, m_screenHeight, m_d3dSystem, w, h);
						spr->SetDisplaySprite(false);
						//Need to update all attributes of the sprite
						spr->SetID(id);
						spr->SetVision( vision );
						spr->SetDepth( depth );
						spr->SetHard( hard );
						spr->SetNoHit( nohit );
						spr->SetAnimationProfileName( animation_str );
						spr->SetDefaultSequence( default_seq );
						spr->SetAnimationSpeed( anim_speed );
						spr->SetHP( hp );
						spr->SetMana( mana );
						spr->SetGold( gold );
						spr->SetStrength( strength );
						spr->SetSpeed( speed );
						spr->SetDefense( defense );
						spr->SetExperience( exp );
						spr->SetLevel( level );
						spr->SetScript( script_str );
						spr->SetTouch( touch );
						spr->SetBackgroundSprite( background );
						spr->SetEntitySprite( entity );
						spr->SetInvisibleSprite( invisible );
						spr->SetCanTeleport( teleport );
						spr->SetTeleportMapName( teleportMap_str );
						spr->SetTeleportX( teleport_map_x );
						spr->SetTeleportY( teleport_map_y );
						spr->SetName( name_str );
						spr->SetHPMax( max_hp );
						spr->SetStatic( is_static );
						//Create the new hard box
						string p = m_mainGameDirectory->m_required_textures;
						//p.append("\\Textures");
						spr->SetHardbox((ptX - w/2) + m_map->GetCameraPosition().x, ((ptY - h + 40) + -m_map->GetCameraPosition().y) + hbox_h, hbox_w, hbox_h);
						spr->CreateHardBoxSpr(m_d3dSystem, p);
						if (is_player)
							m_map->AddPlayerNoIndex(spr);
						else
							m_map->AddSpriteNoIndex(spr);
						//Add the animation
						if (animation_str != "NONE")
							m_map->AddSpriteAnimations(animation_str, m_mainGameDirectory->GAME_FOLDER, m_mainGameDirectory->m_sprites_path, m_d3dSystem, spr->GetID());

						SendMessage(hToolbar, TB_ENABLEBUTTON, IDT_TOOLBAR_UNDO, (LPARAM)TRUE);

						vector<Sprite*> sprites;
						sprites.resize(2);
						sprites[0] = m_spriteBeforeMove->Clone(m_mainGameDirectory, m_d3dSystem);
						sprites[1] = spr->Clone(m_mainGameDirectory, m_d3dSystem);
						m_historyList->AddSpritesToUndoList("Move", sprites);
						delete m_spriteBeforeMove;
						m_spriteBeforeMove = NULL;
					}
				}
			}
		}
	}
}*/
//================================================================================================================
/*void MapEditorSystem2D::UpdateSpriteDelete()
{
	if (m_MapCreated)
	{
		// Sprite mode and the mouse is in the render window
		if (m_ModeSprite && !mouseNotInRenderWindow)
		{
			if (m_SpriteModeDelete)
			{
				m_map->DeleteDisplaySprite();

				Sprite* clicked_sprite;
				if (m_inputSystem->LeftMouseDown())
				{
					if (m_map->SpriteClicked(clicked_sprite, ptX + m_map->GetCameraPosition().x, ptY + -m_map->GetCameraPosition().y))
					{
						m_Updated = true;

						vector<Sprite*> sprites;
						sprites.resize(1);
						sprites[0] = clicked_sprite->Clone(m_mainGameDirectory, m_d3dSystem);
						m_historyList->AddSpritesToUndoList("Delete", sprites);

						SendMessage(hToolbar, TB_ENABLEBUTTON, IDT_TOOLBAR_UNDO, (LPARAM)TRUE);

						bool is_player = clicked_sprite->IsPlayer();
						if (is_player)
						{
							m_map->DeletePlayerSprite();
							m_mapProperties->SetPlayerExists(false);
							m_mapProperties->SetAddPlayer(false);
						}
						else
						{
							//Remove the sprite from the sprite list
							m_map->RemoveSprite(clicked_sprite);
						}

						clicked_sprite = NULL;
					}
				}
			}
		}
	}
}*/
//================================================================================================================
/*void MapEditorSystem2D::UpdateSpriteSize()
{
	if (m_MapCreated)
	{
		// Sprite mode and the mouse is in the render window
		if (m_ModeSprite && !mouseNotInRenderWindow)
		{
			if (m_SpriteModeEditSize)
			{
				if (m_PlacementModeGrid)
				{
					ptX = (m_gridSize * floor(pt.x / m_gridSize));
					ptY = (m_gridSize * floor(pt.y / m_gridSize));
				}

				if (m_PlacementModeFree)
				{
					ptX = pt.x;
					ptY = pt.y;
				}

				if (m_inputSystem->LeftMouseDown())
				{
					//If we select a sprite we want to be able to adjust the width and height of the sprite
					Sprite* p_clicked_sprite;
					if (m_map->SpriteClicked(p_clicked_sprite, ptX + m_map->GetCameraPosition().x, ptY + -m_map->GetCameraPosition().y))
					{
						//Get what sprite was clicked
						m_spriteEditSize = p_clicked_sprite;
					}
				}
				
				if (m_spriteEditSize != 0)
				{
					float s_x;
					float s_y;
					int s_width;
					int s_height;

					s_x = m_spriteEditSize->GetPosition().x;
					s_y = m_spriteEditSize->GetPosition().y;
					s_width = m_spriteEditSize->GetWidth();
					s_height = m_spriteEditSize->GetHeight();

					//Need to move the xy position and size of the hardbox

					if (m_inputSystem->KeyPressed(DIK_K))
					{
						m_editSizeUseArrowKeysK = true;
						m_editSizeUseArrowKeysL = false;
					}

					if (m_inputSystem->KeyPressed(DIK_L))
					{
						m_editSizeUseArrowKeysL = true;
						m_editSizeUseArrowKeysK = false;
					}

					if (m_inputSystem->KeyPressed(DIK_O))
					{
						m_editSizeFastToggle = !m_editSizeFastToggle;
					}

					if (m_inputSystem->KeyPressed(DIK_N))
					{
						m_editSizeUseArrowKeysL = false;
						m_editSizeUseArrowKeysK = false;
					}

					//Update position
					if (m_editSizeUseArrowKeysK)
					{
						if (m_editSizeFastToggle)
						{
							if (m_inputSystem->KeyDown(DIK_LEFT))
							{
								s_x -= 1;
							}
							if (m_inputSystem->KeyDown(DIK_RIGHT))
							{
								s_x += 1;
							}
							if (m_inputSystem->KeyDown(DIK_UP))
							{
								s_y -= 1;
							}
							if (m_inputSystem->KeyDown(DIK_DOWN))
							{
								s_y += 1;
							}
						}
						else
						{
							if (m_inputSystem->KeyPressed(DIK_LEFT))
							{
								s_x -= 1;
							}
							if (m_inputSystem->KeyPressed(DIK_RIGHT))
							{
								s_x += 1;
							}
							if (m_inputSystem->KeyPressed(DIK_UP))
							{
								s_y -= 1;
							}
							if (m_inputSystem->KeyPressed(DIK_DOWN))
							{
								s_y += 1;
							}
						}
					}

					//Update size
					if (m_editSizeUseArrowKeysL)
					{
						if (m_editSizeFastToggle)
						{
							if (m_inputSystem->KeyDown(DIK_UP))
							{
								s_height -= 1;
							}
							if (m_inputSystem->KeyDown(DIK_DOWN))
							{
								s_height += 1;
							}
							if (m_inputSystem->KeyDown(DIK_LEFT))
							{
								s_width -= 1;
							}
							if (m_inputSystem->KeyDown(DIK_RIGHT))
							{
								s_width += 1;
							}
						}
						else
						{
							if (m_inputSystem->KeyPressed(DIK_UP))
							{
								s_height -= 1;
							}
							if (m_inputSystem->KeyPressed(DIK_DOWN))
							{
								s_height += 1;
							}
							if (m_inputSystem->KeyPressed(DIK_LEFT))
							{
								s_width -= 1;
							}
							if (m_inputSystem->KeyPressed(DIK_RIGHT))
							{
								s_width += 1;
							}
						}
					}

					m_spriteEditSize->CloneMe(m_mainGameDirectory, m_d3dSystem, s_x, s_y, s_width, s_height);
					
					m_Updated = true;
				}
			}
		}
	}
}*/
//================================================================================================================
/*void MapEditorSystem2D::SpriteModeNoneHotkey()
{
	if (m_MapCreated)
	{
		/*
		// Sprite mode and the mouse is in the render window
		if (m_ModeSprite && !mouseNotInRenderWindow)
		{
			if (m_inputSystem->KeyPressed(DIK_Q))
			{
				m_map->DeleteDisplaySprite();
				
				SwitchToSpriteModeNone();
			}
		}
	}
}*/
//================================================================================================================
