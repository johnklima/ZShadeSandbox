//==================================================================================================================================
// PlatformerMap.h
//
//==================================================================================================================================
// History
//
// -Created on 1/15/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __PLATFORMERMAP_H
#define __PLATFORMERMAP_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "ZMath.h"
#include "MemoryMgr.h"
#include "PlatformerTile.h"
#include "AISprite.h"
#include "Input.h"
#include "ZShadeMap.h"
//==================================================================================================================================
#ifndef MAXSPRITES
#define MAXSPRITES
#define MAX_SPRITES 5000
#endif//MAXSPRITES
//==================================================================================================================================
//==================================================================================================================================
struct PlatformerMapLoadData
{
	vector<string> imageNames;
	vector<string> animProfileNames;
	vector<int> animSpeeds;
	vector<int> hards;
	vector<int> invisibles;
	vector<float> xs;
	vector<float> ys;
	vector<string> physicsTypes;
	vector<string> surfaceTypes;
};
//==================================================================================================================================
//==================================================================================================================================
class PlatformerMap
{
public:
	
	PlatformerMap(D3D* d3d, GameDirectory2D* gd);
	
	void Initialize(int width, int height);
	void Initialize(int width, int height, PlatformerMapLoadData pmld);
	
	void CreateTile
	(
		int x,
		int y,
		int xPos,
		int yPos,
		int size,
		bool hard,
		bool invisible,
		bool init,
		PhysicsType pt,
		ESurfaceType::Type surfaceType,
		string textureFilename,
		string animProfileName,
		int animSpeed
	);
	
	PlatformerTile* GetTile(int xPos, int yPos);
	bool InBounds(int xPos, int yPos);
	bool InScreen(int xPos, int yPos, int width, int height);
	
	void RemoveTileTexture(int xPos, int yPos);
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
	
	int VisibleTileCount();
	
	void ClipSprites();

	int TotalSpritesInMap();
	
	void AddSpriteAnimations(string profileName, string base_game_folder, string base_sprite_path, int id);
	void AddTileAnimations(string profileName, string base_game_folder, string base_tile_path, float tile_x, float tile_y);
	
	//void InitAISprite(GameDirectory2D* gd);
	void LoadSpritesMain();

	AISprite* GetSpriteByID(int id);
	AISprite* GetSprite(int i);
	AISprite* GetPlayer();
	
	void Render(Camera* camera);
	
public:
	
	vector<AISprite*>& Sprites()						{ return m_Sprites; }
	vector<AISprite*> Sprites() const					{ return m_Sprites; }
	SmartArray2D<PlatformerTile*> Tiles() const			{ return mTiles; }
	int Width()											{ return m_MapWidth; }
	int Height()										{ return m_MapHeight; }
	int& ToolWindowWidth()								{ return m_ToolWindowWidth; }
	int TotalClippedSprites()							{ return m_ClippedSprites.size(); }
	AISprite* ClippedSprite(int i)						{ return m_ClippedSprites[i]; }
	void AddSubMapName(string name) 					{ m_SubMapNames.push_back(name); }
	string SubMapName(int i) const						{ return m_SubMapNames[i]; }
	XMFLOAT2& Offset()									{ return m_Offset; }
	XMFLOAT2 Offset() const								{ return m_Offset; }
	string& MusicName()									{ return m_MusicName; }
	string MusicName() const							{ return m_MusicName; }
	string& MapName()									{ return m_MapName; }
	string MapName() const								{ return m_MapName; }
	int& NumSpritesRendered()							{ return m_numSpritesRendered; }
	int& NumSprites()									{ return spriteNum; }
	int NumSpritesRendered() const						{ return m_numSpritesRendered; }
	int NumSprites() const								{ return spriteNum; }
	bool& InEditor()									{ return bInEditor; }
	bool InEditor() const								{ return bInEditor; }
	int& BackgroundScreenWidth()						{ return m_BackgroundScreenWidth; }
	int BackgroundScreenWidth() const					{ return m_BackgroundScreenWidth; }
	
private:
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	Camera* m_camera;
	GameDirectory2D* m_GD2D;
	SleepTimer timer;
	
	SmartArray2D<PlatformerTile*> mTiles;
	
	AISprite* mPlayer;
	
	// Full list of sprites in the map
	vector< AISprite* > m_Sprites;
	
	// Sprites that are seen on the screen
	vector< AISprite* > m_ClippedSprites;
	
	vector<string> m_SubMapNames;
	string m_MapName;
	string m_MusicName;
	int m_MapWidth;
	int m_MapHeight;
	int m_ToolWindowWidth;
	int m_numSpritesRendered;
	int spriteNum;
	int m_BackgroundScreenWidth;
	bool m_canRenderDisplaySprite;
	bool m_canRenderCloneDisplaySprite;
	bool bInEditor;
	
	XMFLOAT2 m_Offset;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PLATFORMERMAP_H