//======================================================================================================
// TopdownMap.h
//
//======================================================================================================
// History
//
// Created 7/13/2015 by Dustin Watson
//======================================================================================================
#ifndef __TOPDOWNMAP_H
#define __TOPDOWNMAP_H
//=================================================================================================================
//=================================================================================================================
//Includes
#include <algorithm>
#include "tinyxml2.h"
#include "TopdownTile.h"
#include "SleepTimer.h"
#include "Input.h"
#include "Sprite.h"
#include "Camera.h"
#include "SAT.h"
#include "AISprite.h"
#include "FogData.h"
#include "GameDirectory.h"
#include "EngineOptions.h"
//=================================================================================================================
//=================================================================================================================
//Defines
#ifndef MAXSPRITES
#define MAXSPRITES
#define MAX_SPRITES 5000
#endif//MAXSPRITES
//=================================================================================================================
//=================================================================================================================
struct MapLoadData
{
	int mapVision;
	string mapMusicName;
	vector<string> imageNames;
	vector<string> animProfileNames;
	vector<int> animSpeeds;
	vector<int> hards;
};
//=================================================================================================================
//=================================================================================================================
class TopdownMap
{
public:
	
	TopdownMap(D3D* d3d, Map2DType mt, GameDirectory2D* gd);
	~TopdownMap();
	
	// Size is specified as number of sections in a row of the map
	void Initialize(int size);
	void Initialize(int size, MapLoadData mld);
	void InitializeSectionPickerTiles();
	
	void CreateTile
	(
		int x,
		int y,
		int xPos,
		int yPos,
		int size,
		bool hard,
		bool init,
		string textureFilename,
		string animProfileName,
		int animSpeed,
		bool isSectionPicker
	);
	
	TopdownTile* InsideTile(float xPos, float yPos);
	TopdownTile* GetTile(int xPos, int yPos);
	bool InBounds(int xPos, int yPos);
	bool InScreen(int xPos, int yPos, int width, int height);
	
	void CalculateScrollOffsets();
	XMFLOAT2 GetSectionScrollOffset(int sectionID);
	void SelectSectionID(int& sectionID, int xPos, int yPos);
	
	void UpdateFogBit(unsigned short bit);
	void UpdateFog(AISprite* sprite);
	void FowToggleUpdate();
	void InitPosFOW(XMFLOAT3 realpos, unsigned short mask);
	bool SpriteInFowCircle(AISprite* sprite);
	void SetTileNotInFowCircle();
	void TileOutsideRadiusInShadow();
	void SetTilesFOW(bool fow);
	void SetTilesFOWRadius(int radius);
	void SetTilesFOWFlashlight(bool fowFlashlight);
	void SetTilesFOWUncovered(bool fowUncovered);

	void UpdateTileTexture(int xPos, int yPos, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight);
	void UpdateTileHardness(int xPos, int yPos, bool bHard);
	void ToggleTileHard(int xPos, int yPos);
	void ToggleHardness(bool bEnable);
	
	void AddSprite(AISprite* s);
	void AddSpriteNoIndex(AISprite* s);
	void AddPlayer(AISprite* s);
	void AddPlayerNoIndex(AISprite* s);
	
	bool SpriteClicked(AISprite*& clickedSprite, int mouse_x, int mouse_y);
	void MoveSprite(AISprite*& clickedSprite, int mouse_x, int mouse_y);
	void RemoveSprite(AISprite* s);
	void RemoveSprite(int id);
	void RemoveSprite(XMFLOAT3 position);
	void SetRenderDisplaySprite(bool can_render);
	void SetRenderCloneDisplaySprite(bool can_render);
	void DeleteDisplaySprite();
	void DeleteCloneDisplaySprite();
	void DeletePlayerSprite();
	bool DoesPlayerExist();
	
	void SortAndClipSprites();
	
	int TotalSpritesInMap();

	void AddSpriteAnimations(string profileName, string base_game_folder, string base_sprite_path, int id);
	void AddTileAnimations(string profileName, string base_game_folder, string base_tile_path, float tile_x, float tile_y);
	
	void LoadSpritesMain();

	AISprite* GetSpriteByID(int id);
	AISprite* GetSprite(int i);
	AISprite* GetPlayer();
	
	void Render(Camera* camera);
	void RenderSectionPicker(Camera* camera);
	
	TopdownMap* Clone();

public:
	
	vector<AISprite*>& Sprites()						{ return m_Sprites; }
	vector<AISprite*> Sprites() const					{ return m_Sprites; }
	SmartArray2D<TopdownTile*>& Tiles()					{ return m_Tiles; }
	SmartArray2D<TopdownTile*> Tiles() const			{ return m_Tiles; }
	int& Width()										{ return m_MapWidth; }
	int Width() const									{ return m_MapWidth; }
	int& Height()										{ return m_MapHeight; }
	int Height() const									{ return m_MapHeight; }
	int& ToolWindowWidth()								{ return m_ToolWindowWidth; }
	int& NumSectionsPerRow()							{ return m_NumSectionsPerRow; }
	int NumSectionsPerRow() const						{ return m_NumSectionsPerRow; }
	int& NumSections()									{ return m_NumSections; }
	int NumSections() const								{ return m_NumSections; }
	int TotalClippedSprites()							{ return mSpriteList.size(); }
	AISprite* ClippedSprite(int i)						{ return mSpriteList[i].sprite; }
	XMFLOAT2& Offset()									{ return m_Offset; }
	XMFLOAT2 Offset() const								{ return m_Offset; }
	string& MusicName()									{ return m_MusicName; }
	string MusicName() const							{ return m_MusicName; }
	string& MapName()									{ return m_MapName; }
	string MapName() const								{ return m_MapName; }
	int& Vision()										{ return m_currentVision; }
	int Vision() const									{ return m_currentVision; }
	int& NumSpritesRendered()							{ return m_numSpritesRendered; }
	int NumSpritesRendered() const						{ return m_numSpritesRendered; }
	int& NumSprites()									{ return spriteNum; }
	int NumSprites() const								{ return spriteNum; }
	bool& InEditor()									{ return bInEditor; }
	bool InEditor() const								{ return bInEditor; }
	bool& FOW()											{ return m_bFOW; }
	bool FOW() const									{ return m_bFOW; }
	int& FOWRadius()									{ return m_iFOWRadius; }
	int FOWRadius() const								{ return m_iFOWRadius; }
	bool& FOWFlashlight()								{ return m_bFOWFlashlight; }
	bool FOWFlashlight() const							{ return m_bFOWFlashlight; }
	bool& FOWUncovered()								{ return m_bFOWUncovered; }
	bool FOWUncovered() const							{ return m_bFOWUncovered; }
	Map2DType& MapType()								{ return m_MapType; }
	Map2DType MapType() const							{ return m_MapType; }
	bool& CanRenderDisplaySprite()						{ return m_canRenderDisplaySprite; }
	bool CanRenderDisplaySprite() const					{ return m_canRenderDisplaySprite; }
	bool& CanRenderCloneDisplaySprite()					{ return m_canRenderCloneDisplaySprite; }
	bool CanRenderCloneDisplaySprite() const			{ return m_canRenderCloneDisplaySprite; }

private:
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	Camera* m_camera;
	GameDirectory2D* m_GD2D;
	SleepTimer timer;
	
	SmartArray2D<TopdownTile*> m_Tiles;
	SmartArray2D<TopdownTile*> m_SectionPickerTiles;
	
	// After the player is found store a copy
	AISprite* mPlayer;
	
	//What is the current vision for the map
	static const int MAX_VISION = 25;
	int m_currentVision;
	
	// Full list of sprites in the map
	vector<AISprite*> m_Sprites;
	
	struct SectionOffset
	{
		int sectionID;
		XMFLOAT2 offset;
	};

	vector<SectionOffset> mSectionOffsets;
	
	struct SPRQUE
	{
		AISprite* sprite;
		int depth;
		int sprite_num;
	};
	
	vector<SPRQUE> mSpriteList;

	bool sorted;
	Map2DType m_MapType;
	string m_MapName;
	string m_MusicName;
	int m_NumSectionsPerRow;
	int m_NumSections;
	int m_MapWidth;
	int m_MapHeight;
	int m_ToolWindowWidth;
	int m_numSpritesRendered;
	int spriteNum;
	bool m_canRenderDisplaySprite;
	bool m_canRenderCloneDisplaySprite;
	bool bInEditor;
	bool m_bFOW;
	int m_iFOWRadius;
	int m_iFOWLastRadius;
	bool m_bFOWFlashlight;
	bool m_bFOWUncovered;

	// How about amount of tiles seen from a sprite after the fow has been uncovered
	// for example what about a sprite only being able to see so far so that when beyond that point the only thing seen is the map
	// and other sprite are not seen
	// that would mean 2 layers of fow, a main layer that hides everything and another layer that hides sprites and shows the map
	// the fow radius is the distance seen from the sprite for it to see other sprites
	// Need two sets of fow circle textures one for covered and one for uncovered
	// the tile class has a new variable for being uncovered from fow
	
	XMFLOAT2 m_Offset;
};
//=================================================================================================================
//=================================================================================================================
#endif//__TOPDOWNMAP_H