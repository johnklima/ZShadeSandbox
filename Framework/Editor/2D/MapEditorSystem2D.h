//================================================================================================================
// MapEditorSystem2D.h
//
// Includes everything needed for the map editor
//================================================================================================================
// History
//
// Created on 10/9/2014 by Dustin Watson
//================================================================================================================
#ifndef __MAPEDITORSYSTEM2D_H
#define __MAPEDITORSYSTEM2D_H
//================================================================================================================
//================================================================================================================
#include "Sprite.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
#include "Environment2D.h"
//================================================================================================================
class MapEditorSystem2D
{
	typedef enum
	{
		EM_Tiles = 0,
		EM_Sprites = 1,
		EM_Hardness = 2
	} EEditMode;
	
	typedef enum
	{
		TA_Select = 0,
		TA_Regular = 1,
		TA_MultiStamp = 2,
		TA_RemoveTexture = 3
	} ETileActions;
	
	typedef enum
	{
		SA_Select = 0,
		SA_Move = 1,
		SA_Delete = 2,
		SA_Place = 3,
		SA_Clone = 4,
		SA_EditSize = 5
	} ESpriteActions;
	
	typedef enum
	{
		HA_Tiles = 0,
		HA_Sprites = 1
	} EHardnessActions;
	
	typedef enum
	{
		GM_Free = 0,
		GM_Snap = 1
	} EGridMode;
	
	typedef enum
	{
		MSS_1X1 = 0,
		MSS_2X2 = 1,
		MSS_3X3 = 2,
		MSS_4X4 = 3,
		MSS_5X5 = 4,
		MSS_6X6 = 5,
		MSS_7X7 = 6,
		MSS_8X8 = 7,
		MSS_9X9 = 8,
		MSS_10X10 = 9,
	} EMultiStampSize;
	
public:
	
	MapEditorSystem2D(Environment2D* env2D);
	~MapEditorSystem2D();
	
	void Init();
	
public:
	
	XMFLOAT2 SnapToGrid(int x, int y);
	
	void CreateWorld(string world_name);
	void LoadWorld(string& worldName, string filename, bool InEditor);
	void SaveWorld(string filename);
	void RemoveMap(string mapName);
	void NewMapTopdown(string world_name, string name, int x, int y, bool fow, int fowRadius, bool fowFlashlight, bool bIso, bool InEditor);
	void NewMapPlatformer(string world_name, string name, int x, int y, bool InEditor);
	void LoadMap(string& mapName, string filename, bool InEditor);
	void SaveMap(string filename);
	void SetActiveRenderedMap(string world_name, string map_name);
	void GrabWorldNames(vector<string>& names);
	void GrabMapNames(string world_name, vector<string>& names);
	bool HasMaps(string worldName);

	void LoadRequiredSprites();
	
	void RevertTileAnim();
	void UpdateTileAnimation(string animProfileName, int animSpeed);
	void UpdateTileHardness(bool bHard);
	TopdownTile* GetSelectedTopdownTile();
	PlatformerTile* GetSelectedPlatformerTile();
	void MakeTileHard();
	void RemoveTexture();
	void UpdateTileStamp();
	void ToggleMapHardness(bool bEnable);
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	
	void UpdateStampSprites();
	void RenderStampSprites(Camera* camera);
	void RenderMultiStampSprites(Camera* camera);
	
	void SelectTopdownSection();
	void ReturnToTopdownSectionPicker();
	XMFLOAT2 MultiSampleSpritePos();
	void CalculateNewTilePosition(int x, int y);
	
	AISprite* GetSelectedSprite();
	void AddSprite();
	void MoveSprite(float x, float y);
	bool MoveSpriteStart();
	void MoveSpriteEnd();
	bool CloneSpriteStart();
	void CloneSpriteEnd();
	void DeleteSprite();
	bool ResizeSpriteStart();
	void ResizeSpriteUpdate(
		bool updatePosition,
		bool updateSize,
		bool goFast,
		bool pressedLeftKey,
		bool pressedRightKey,
		bool pressedUpKey,
		bool pressedDownKey
	);
	void HighlightSprite(float x, float y);
	void UpdateDisplaySprite(float x, float y);
	void SelectModeSpriteClicked();
	void ToggleSelectedSpriteBackground(bool background);
	void ToggleSelectedSpriteEntity(bool entity);
	void ToggleSelectedSpriteInvisible(bool invisible);
	void ToggleSelectedSpriteHard(bool hard);
	void ToggleSelectedSpriteNoHit(bool nohit);
	void ToggleSelectedSpritePhysicsType(bool isstatic);
	void ToggleSelectedSpriteTouch(bool touch);
	void ToggleSelectedSpriteAlwaysSeenByPlayer(bool value);
	bool SelectedSpriteIsBackground();
	bool SelectedSpriteIsEntity();
	bool SelectedSpriteIsInvisible();
	bool SelectedSpriteIsHard();
	bool SelectedSpriteIsNoHit();
	bool SelectedSpriteIsStatic();
	bool SelectedSpriteIsTouch();
	bool SelectedSpriteIsTeleportSprite();
	void SetSelectedSpriteVision(int vision);
	void SetSelectedSpriteDepth(int depth);
	void SetSelectedSpriteTextureName(string textureName);
	void SetSelectedSpriteAnimationName(string animationName);
	void SetSelectedSpriteDefaultSequence(int sequenceNum);
	void SetSelectedSpriteAnimationSpeed(int speed);
	void SetSelectedSpriteCollideMsg(string collideMsg);
	void SetSelectedSpriteHPMax(int value);
	void SetSelectedSpriteHP(int value);
	void SetSelectedSpriteMana(int value);
	void SetSelectedSpriteGold(int value);
	void SetSelectedSpriteStrength(int value);
	void SetSelectedSpriteSpeedX(int value);
	void SetSelectedSpriteSpeedY(int value);
	void SetSelectedSpriteMaxSpeedX(int value);
	void SetSelectedSpriteMaxSpeedY(int value);
	void SetSelectedSpriteDefense(int value);
	void SetSelectedSpriteExperience(int value);
	void SetSelectedSpriteLevel(int value);
	void SetSelectedSpriteScriptName(string scriptName);
	void SetSelectedSpriteTeleportMapName(string mapName);
	void SetSelectedSpriteTeleportMapX(int value);
	void SetSelectedSpriteTeleportMapY(int value);
	float GetSelectedSpriteXPos();
	float GetSelectedSpriteYPos();
	int GetSelectedSpriteVision();
	int GetSelectedSpriteDepth();
	string GetSelectedSpriteTextureName();
	string GetSelectedSpriteAnimationName();
	int GetSelectedSpriteDefaultSequence();
	int GetSelectedSpriteAnimationSpeed();
	string GetSelectedSpriteCollideMsg();
	int GetSelectedSpriteHPMax();
	int GetSelectedSpriteHP();
	int GetSelectedSpriteMana();
	int GetSelectedSpriteGold();
	int GetSelectedSpriteStrength();
	int GetSelectedSpriteSpeedX();
	int GetSelectedSpriteSpeedY();
	int GetSelectedSpriteMaxSpeedX();
	int GetSelectedSpriteMaxSpeedY();
	int GetSelectedSpriteDefense();
	int GetSelectedSpriteExperience();
	int GetSelectedSpriteLevel();
	string GetSelectedSpriteScriptName();
	string GetSelectedSpriteTeleportMapName();
	int GetSelectedSpriteTeleportMapX();
	int GetSelectedSpriteTeleportMapY();
	bool GetSelectedSpriteAlwaysSeenByPlayer();
	void RevertSelectedSpriteAnimation();
	void UpdateSelectedSpriteAnimation();
	void OpenSelectedSpriteScript(string scriptName);
	void UpdateSelectedSpriteMeshTexture();

	// Update the text in the title bar
	string UpdateTitleBarText();
	
	bool& WasTileOrSpriteClicked() { return bWasClicked; }
	bool WasTileOrSpriteClicked() const { return bWasClicked; }
	
	void SwitchToSpriteModeMove();
	void SwitchToSpriteModeDelete();
	void SwitchToSpriteModePlace();
	void SwitchToSpriteModeClone();
	void SwitchToSpriteModeSize();
	void SwitchToSpriteModeSelect();
	void SwitchToTileModeSelect();
	void SwitchToTileModeRegular();
	void SwitchToTileModeMultiStamp();
	void SwitchToTileModeRemoveTexture();
	void SwitchToHardnessModeTiles();
	void SwitchToHardnessModeSprites();
	void SwitchToGridModeFree();
	void SwitchToGridModeSnap();
	void SwitchToStampSize1x1();
	void SwitchToStampSize2x2();
	void SwitchToStampSize3x3();
	void SwitchToStampSize4x4();
	void SwitchToStampSize5x5();
	void SwitchToStampSize6x6();
	void SwitchToStampSize7x7();
	void SwitchToStampSize8x8();
	void SwitchToStampSize9x9();
	void SwitchToStampSize10x10();
	void SwitchToEditModeTiles();
	void SwitchToEditModeSprites();
	void SwitchToEditModeHardness();
	
	void SetMapCreated(bool created) { m_MapCreated = created; }
	void SetGameDirectory(GameDirectory2D* gd);
	void GoFullscreen(bool bEnable);

	void GetWorldAndMapName(string& activeMapName, string& activeWorldName);
	void GetMapWidthAttribute(int& width);
	void GetMapHeightAttribute(int& height);
	void GetMapSizeAttribute(int& size);
	void GetMapVisionAttribute(int& vision);
	void GetMapMusicAttribute(string& music);
	void GetMapFOWAttribute(bool& fow);
	void GetMapFOWRadiusAttribute(int& fowRadius);
	void GetMapFOWFlashlightAttribute(bool& fowFlashlight);
	void GetMapFOWUncoveredAttribute(bool& fowUncovered);

	void VisibleTileCount(int& visibleTileCount);
	void GetNumSpritesRendered(int& spritesRendered);
	void GetSpriteTotal(int& spriteTotal);

	void SetMapVisionAttribute(int vision);
	void SetMapMusicAttribute(string music);
	void SetMapFOWAttribute(bool fow);
	void SetMapFOWRadiusAttribute(int fowRadius);
	void SetMapFOWFlashlightAttribute(bool fowFlashlight);
	void SetMapFOWUncoveredAttribute(bool fowUncovered);
	void SetMapNameAttribute(string mapName);
	void UpdateNewMapName(string world_name, string old_map_name, string new_map_name);

	void ScrollMapUp();
	void ScrollMapDown();
	void ScrollMapLeft();
	void ScrollMapRight();
	void ScrollOffsetToBounds();
	void BoundSectionPicker();
	
	void UpdateInput(Keyboard* keyboard);
	
	GameDirectory2D* GetGD() { return m_mainGameDirectory; }
	D3D* D3DSystem() { return m_D3DSystem; }
	XMFLOAT2 GetScrollOffset() const { return ScrollOffset; }
	string& ActiveWorldName() { return m_ActiveWorldName; }
	string ActiveWorldName() const { return m_ActiveWorldName; }
	string& ActiveMapName() { return m_ActiveMapName; }
	string ActiveMapName() const { return m_ActiveMapName; }
	string& SelectedTileName() { return m_SelectedTileName; }
	string SelectedTileName() const { return m_SelectedTileName; }
	string& SelectedSpriteName() { return m_SelectedSpriteName; }
	string SelectedSpriteName() const { return m_SelectedSpriteName; }
	bool& SpriteCacheSelectionChanged() { return m_sprite_cache_selection_changed; }
	bool SpriteCacheSelectionChanged() const { return m_sprite_cache_selection_changed; }
	int& ToolWindowWidth() { return iToolWindowWidth; }
	int ToolWindowWidth() const { return iToolWindowWidth; }
	bool& IsInSectionPicker() { return m_InSectionPicker; }
	bool IsInSectionPicker() const { return m_InSectionPicker; }
	bool IsMapCreated() { return m_MapCreated; }
	bool IsWorldLoaded();
	
private:
	
	Environment2D* m_Env2D;
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	GameDirectory2D* m_mainGameDirectory;

	string m_ActiveWorldName;
	string m_ActiveMapName;
	string m_SelectedSpriteName;
	string m_SelectedTileName;
	
	//Controls how fast the camera will move when transforming (Not needed)
	float m_camera_speed;
	
	//When the user makes a change on the map then this flag becomes true
	// Need to make sure that when this flag is set to true the user saves the map and then it becomes false
	// or when the user exists he/she is notified that there are updates and the map needs to be saved or exited and lost.
	bool m_Updated;
	
	XMFLOAT2 m_SelectedTilePos;

	// This is the main stamp tool to select a tile
	Sprite* m_StampNormal;
	// The stamp tool that shows when the left mouse button is clicked
	Sprite* m_StampHighlight;

	// This is the main stamp tool to select a tile
	Sprite* m_StampNormal32x32;
	// The stamp tool that shows when the left mouse button is clicked
	Sprite* m_StampHighlight32x32;
	
	int m_selectModeSpriteID;
	int m_originalSpriteID;
	bool m_sprite_cache_selection_changed;
	bool m_resizing_sprite;
	bool m_moving_sprite;
	bool m_cloning_sprite;
	bool m_selected_sprite_cover;
	bool m_display_sprite_box_created;
	bool m_display_selected_sprite_box;
	AISprite* m_sprite_with_box;
	Sprite* m_sprite_box;
	Sprite* m_select_sprite_box;
	//AISprite* m_spriteBeforeMove;
	AISprite* m_MovingSprite;
	AISprite* m_SelectedSprite;
	AISprite* m_EditSizeSprite;
	AISprite* m_displaySprite;
	AISprite* m_cloneDisplaySprite;
	
	//Stamp Tool sizes of how many tiles it can select
	Sprite* m_TileEdit1x1Sprite;
	Sprite* m_TileEdit2x2Sprite;
	Sprite* m_TileEdit3x3Sprite;
	Sprite* m_TileEdit4x4Sprite;
	Sprite* m_TileEdit5x5Sprite;
	Sprite* m_TileEdit6x6Sprite;
	Sprite* m_TileEdit7x7Sprite;
	Sprite* m_TileEdit8x8Sprite;
	Sprite* m_TileEdit9x9Sprite;
	Sprite* m_TileEdit10x10Sprite;
	
	Sprite* m_TileEdit1x1Sprite2;
	Sprite* m_TileEdit2x2Sprite2;
	Sprite* m_TileEdit3x3Sprite2;
	Sprite* m_TileEdit4x4Sprite2;
	Sprite* m_TileEdit5x5Sprite2;
	Sprite* m_TileEdit6x6Sprite2;
	Sprite* m_TileEdit7x7Sprite2;
	Sprite* m_TileEdit8x8Sprite2;
	Sprite* m_TileEdit9x9Sprite2;
	Sprite* m_TileEdit10x10Sprite2;

	int m_currentSection;
	
	// Mouse position at start of drag operation
	int dragStartX, dragStartY;

	// Mouse position in previous frame of drag operation
	int dragPrevX, dragPrevY;

	// Currently panning with RMB
	bool panning;
	
	// X and Y grid square size
	int gridSizeX, gridSizeY;
	
	// If in the editor do not want to get the offset from the player in the map
	
	// Needs to be set in an update loop with the map
	XMFLOAT2 ScrollOffset;
	
	EEditMode editMode;
	ETileActions tileAction;
	ESpriteActions spriteAction;
	EHardnessActions hardnessAction;
	EGridMode gridMode;
	EMultiStampSize multiStampSize;

	int iToolWindowWidth;
	
	//Are we rendering the overview tileset of the map or in a section of the map
	bool m_InSectionPicker;
	
	//Is the map created
	bool m_MapCreated;
	
	// Do we have a world
	bool m_WorldLoaded;
	
	// This flag is to update the tile/sprite tab interface when a tile/sprite was clicked on
	// in select mode
	bool bWasClicked;
	
	bool stampPressed;
	bool usingGrid;
	bool m_startDelayTimer;
	bool m_platformer;
	
	SleepTimer* m_ClickDelayTimer;
	SleepTimer* m_SelectionTimer;
};
//================================================================================================================
#endif//__MAPEDITORSYSTEM2D_H