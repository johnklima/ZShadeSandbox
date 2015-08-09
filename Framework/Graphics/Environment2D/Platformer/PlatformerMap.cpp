#include "PlatformerMap.h"
//==================================================================================================================================
//==================================================================================================================================
PlatformerMap::PlatformerMap(D3D* d3d, GameDirectory2D* gd)
:   m_D3DSystem(d3d)
,	m_GD2D(gd)
,	bInEditor(false)
,	m_BackgroundScreenWidth(1600)
,	mPlayer(0)
,	spriteNum(0)
,	m_canRenderCloneDisplaySprite(false)
{
	m_Offset = XMFLOAT2(0, 0);
	m_EngineOptions = d3d->GetEngineOptions();
	m_Sprites.resize(MAX_SPRITES);
	m_ClippedSprites.resize(MAX_SPRITES);
}
//==================================================================================================================================
void PlatformerMap::Initialize(int width, int height, PlatformerMapLoadData pmld)
{
	m_MapWidth = width;
	m_MapHeight = height;
	
	mTiles.resize(m_MapWidth, m_MapHeight);
	
	int tile_id = 0;
	bool init = false; // We are loading a map and not creating a brand new one
	int size = m_EngineOptions->TILE_SIZE;
	
	// Need to make sure now that all load data is the same size as the map
	// and that there is a value for each of them
	
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			if (tile_id >= pmld.imageNames.size()) break;
			
			string textureFilename = pmld.imageNames[tile_id];
			string animProfileName = pmld.animProfileNames[tile_id];
			int animSpeed = pmld.animSpeeds[tile_id];
			bool hard = pmld.hards[tile_id];
			bool invisible = pmld.invisibles[tile_id];
			string physicsType = pmld.physicsTypes[tile_id];
			string surfaceType = pmld.surfaceTypes[tile_id];
			
			PhysicsType pt;
			if (physicsType == "static")
			{
				pt = STATIC;
			}
			else if (physicsType == "dynamic")
			{
				pt = DYNAMIC;
			}

			ESurfaceType::Type st;
			if (surfaceType == "normal")
			{
				st = ESurfaceType::Type::eNormal;
			}
			else if (surfaceType == "frictionless")
			{
				st = ESurfaceType::Type::eFrictionless;
			}
			else if (surfaceType == "sticky")
			{
				st = ESurfaceType::Type::eSticky;
			}
			else if (surfaceType == "ladder")
			{
				st = ESurfaceType::Type::eLadder;
			}
			else if (surfaceType == "jumpthrough")
			{
				st = ESurfaceType::Type::eJumpThrough;
			}

			int xs = x * size;
			int ys = y * size;
			
			CreateTile(
				x, y,
				xs, ys,
				size,
				hard,
				invisible,
				init,
				pt, st,
				textureFilename,
				animProfileName,
				animSpeed
			);
			
			tile_id++;
		}
	}
}
//==================================================================================================================================
void PlatformerMap::Initialize(int width, int height)
{
	m_MapWidth = width;
	m_MapHeight = height;
	
	mTiles.resize(m_MapWidth, m_MapHeight);
	
	bool hard = false;
	bool invisible = true; // If this is false then texture will be loaded, tile can be seen and a sprite can walk through it
	bool init = true; // We are creating a brand new map
	int size = m_EngineOptions->TILE_SIZE;
	
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			int xs = x * size;
			int ys = y * size;
			
			CreateTile(
				x, y,
				xs, ys,
				size,
				hard,
				invisible,
				init,
				STATIC,
				ESurfaceType::Type::eNormal,
				"PlatformTile.png",
				"NONE",
				0
			);
		}
	}
	
	// Create the player and add it to the map
	AISprite* player = new AISprite(m_D3DSystem);
	player->Initialize(m_GD2D->m_required_textures, "player_cover.png", XMFLOAT3(0, 0, 0), 64, 64, PhysicsType::DYNAMIC);
	AddPlayer(player);
}
//==================================================================================================================================
void PlatformerMap::CreateTile
(	int x
,	int y
,	int xPos
,	int yPos
,	int size
,	bool hard
,	bool invisible
,	bool init
,	PhysicsType pt
,	ESurfaceType::Type surfaceType
,	string textureFilename
,	string animProfileName
,	int animSpeed
)
{
	mTiles.insert(x, y, new PlatformerTile(m_D3DSystem));
	mTiles(x, y)->InitializeBody(size, pt, surfaceType, XMFLOAT3(xPos, yPos, 0));
	mTiles(x, y)->AnimationProfileName() = animProfileName;
	mTiles(x, y)->AnimationSpeed() = animSpeed;
	mTiles(x, y)->Hard() = hard;
	mTiles(x, y)->TileInvisible() = invisible;
	
	string base_path = "";

	if (init)
	{
		base_path = m_GD2D->m_required_textures;
	}
	else
	{
		base_path = m_GD2D->m_tiles_path;
	}
	
	mTiles(x, y)->InitializeMesh(base_path, textureFilename, size, size);

	// If an animation exists then load it
	if (animProfileName != "NONE")
	{
		//Load a animation profile
		AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

		bool something_wrong = false;

		if (profile->LoadAnimationProfileNames(animProfileName, "TILE", m_GD2D->GAME_FOLDER))
		{
			if (profile->ReadProfile(animProfileName, "TILE", m_GD2D->m_tiles_path, m_GD2D->GAME_FOLDER))
			{
				vector<Animation2D*> anims = profile->GetAnimations();

				PlatformerTile* tile = mTiles(x, y);

				tile->ClearAnimationList();

				for (int i = 0; i < anims.size(); i++)
				{
					tile->AddAnimationSequence(anims[i]);
				}

				tile->SetSequence(0);
			}
			else something_wrong = true;
		}
		else something_wrong = true;

		if (something_wrong)
		{
			MessageBox(NULL, "The Animation2D does not exist", "Add Platform Tile Animations", MB_OK);
		}
	}
}
//==================================================================================================================================
PlatformerTile* PlatformerMap::GetTile(int xPos, int yPos)
{
	if (InBounds(xPos, yPos))
	{
		return mTiles(xPos, yPos);
	}
	else
	{
		int x = xPos / m_EngineOptions->TILE_SIZE;
		int y = yPos / m_EngineOptions->TILE_SIZE;

		if (InBounds(x, y))
		{
			return mTiles(x, y);
		}
	}
	
	return 0;
}
//==================================================================================================================================
bool PlatformerMap::InBounds(int xPos, int yPos)
{
	return (xPos >= 0 && xPos < m_MapWidth) && (yPos >= 0 && yPos < m_MapHeight);
}
//==================================================================================================================================
bool PlatformerMap::InScreen(int xPos, int yPos, int width, int height)
{
	if (bInEditor)
		return (xPos >= -width && xPos < (m_EngineOptions->m_screenWidth - m_ToolWindowWidth + width)) && (yPos >= -height && yPos < m_EngineOptions->m_screenHeight + height);
	else
		return (xPos >= -width && xPos < m_EngineOptions->m_screenWidth + width) && (yPos >= -height && yPos < m_EngineOptions->m_screenHeight + height);
}
//==================================================================================================================================
int PlatformerMap::VisibleTileCount()
{
	int visibleTileCount = 0;
	
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			PlatformerTile* tile = mTiles(x, y);
			if (tile == NULL) continue;
			
			if (!tile->TileInvisible())
			{
				visibleTileCount++;
			}
		}
	}
	
	return visibleTileCount;
}
//==================================================================================================================================
void PlatformerMap::RemoveTileTexture(int xPos, int yPos)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			PlatformerTile* tile = mTiles(x, y);
			if (tile == NULL) continue;
			
			if (tile->Body().Inside(xPos, yPos))
			{
				// Update texture image of the tile
				//mTiles(x, y)->DisableTexture();
				// Just make it invisible so the texture cannot be seen
				mTiles(x, y)->TileInvisible() = true;

				return;
			}
		}
	}
}
//==================================================================================================================================
void PlatformerMap::UpdateTileTexture(int xPos, int yPos, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			PlatformerTile* tile = mTiles(x, y);
			if (tile == NULL) continue;
			
			if (tile->Body().Inside(xPos, yPos))
			{
				// Update texture image of the tile (really just need to change the material diffuse texture)
				mTiles(x, y)->UpdateMeshTexture(base_tile_path, textureFilename);
				mTiles(x, y)->TileInvisible() = false;

				return;
			}
		}
	}
}
//==================================================================================================================================
void PlatformerMap::UpdateTileHardness(int xPos, int yPos, bool bHard)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			PlatformerTile* tile = mTiles(x, y);
			if (tile == NULL) continue;
			
			if (tile->Body().Inside(xPos, yPos))
			{
				mTiles(x, y)->Hard() = bHard;
				
				return;
			}
		}
	}
}
//==================================================================================================================================
void PlatformerMap::ToggleTileHard(int xPos, int yPos)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			PlatformerTile* tile = mTiles(x, y);
			if (tile == NULL) continue;
			
			if (tile->Body().Inside(xPos, yPos))
			{
				if (mTiles(x, y)->Hard())
					mTiles(x, y)->Hard() = false;
				else
					mTiles(x, y)->Hard() = true;
				
				return;
			}
		}
	}
}
//==================================================================================================================================
void PlatformerMap::ToggleHardness(bool bEnable)
{
	// Set hardness for the tiles
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			PlatformerTile* tile = mTiles(x, y);
			if (tile == NULL) continue;

			tile->DisplayHardbox() = bEnable;
		}
	}

	// Set hardness for the sprites
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		AISprite* s = m_Sprites[i];
		if (s == 0) continue;
		s->CanDisplayHardbox() = bEnable;
	}
}
//==================================================================================================================================
int PlatformerMap::TotalSpritesInMap()
{
	int total = 0;
	AISprite* s;

	for (int j = 0; j < m_Sprites.size(); j++)
	{
		s = m_Sprites[j];
		if (s == 0) continue;

		total++;

		if (s->IsDisplaySprite())
		{
			total--;
		}
	}

	return total;
}
//==================================================================================================================================
bool PlatformerMap::SpriteClicked(AISprite*& clickedSprite, int mouse_x, int mouse_y)
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_Sprites.size(); sprite_num++)
	{
		spr = m_Sprites[sprite_num];

		if (spr == 0) continue;

		if (!spr->IsDisplaySprite())
		{
			if (spr->Body().Inside(mouse_x, mouse_y))
			{
				clickedSprite = spr;
				return true;
			}
		}
	}
	
	clickedSprite = 0;
	return false;
}
//==================================================================================================================================
void PlatformerMap::MoveSprite(AISprite*& clickedSprite, int mouse_x, int mouse_y)
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_Sprites.size(); sprite_num++)
	{
		spr = m_Sprites[sprite_num];

		if (spr == 0) continue;

		if (spr == clickedSprite)
		{
			spr->Move(mouse_x, mouse_y, mouse_x, mouse_y);
			return;
		}
	}
}
//==================================================================================================================================
void PlatformerMap::SetRenderDisplaySprite(bool can_render)
{
	AISprite* spr;
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		spr = m_Sprites[i];

		if (spr == 0) continue;

		if (spr->IsDisplaySprite())
		{
			m_canRenderDisplaySprite = can_render;
			break;
		}
	}
}
//==================================================================================================================================
void PlatformerMap::SetRenderCloneDisplaySprite(bool can_render)
{
	AISprite* spr;
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		spr = m_Sprites[i];

		if (spr == 0) continue;

		if (spr->IsCloneDisplaySprite())
		{
			m_canRenderCloneDisplaySprite = can_render;
			break;
		}
	}
}
//==================================================================================================================================
void PlatformerMap::DeleteDisplaySprite()
{
	AISprite* spr = 0;
	auto current = m_Sprites.begin();
	while (current != m_Sprites.end())
	{
		spr = (*current);
		if (spr != 0 && spr->IsDisplaySprite())
		{
			current = m_Sprites.erase(current);
			spriteNum--;
		}
		else
		{
			++current;
		}
	}

	// Refractor the sprite ids
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		spr = m_Sprites[i];

		if (spr == 0) continue;

		spr->ID() = i;
	}
}
//==================================================================================================================================
void PlatformerMap::DeleteCloneDisplaySprite()
{
	AISprite* spr = 0;
	auto current = m_Sprites.begin();
	while (current != m_Sprites.end())
	{
		spr = (*current);
		if (spr != 0 && spr->IsCloneDisplaySprite())
		{
			current = m_Sprites.erase(current);
			spriteNum--;
		}
		else
		{
			++current;
		}
	}

	// Refractor the sprite ids
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		spr = m_Sprites[i];

		if (spr == 0) continue;

		spr->ID() = i;
	}
}
//==================================================================================================================================
void PlatformerMap::DeletePlayerSprite()
{
	AISprite* spr = 0;
	auto current = m_Sprites.begin();
	while (current != m_Sprites.end())
	{
		spr = (*current);
		if (spr != 0 && spr->IsPlayer())
		{
			current = m_Sprites.erase(current);
			spriteNum--;
		}
		else
		{
			++current;
		}
	}

	// Refractor the sprite ids
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		spr = m_Sprites[i];

		if (spr == 0) continue;

		spr->ID() = i;
	}
}
//==================================================================================================================================
void PlatformerMap::RemoveSprite(AISprite* s)
{
	AISprite* spr = 0;
	auto current = m_Sprites.begin();
	while (current != m_Sprites.end())
	{
		spr = (*current);
		if (spr != 0 && spr->ID() == s->ID())
		{
			current = m_Sprites.erase(current);
			spriteNum--;
		}
		else
		{
			++current;
		}
	}

	// Refractor the sprite ids
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		spr = m_Sprites[i];

		if (spr == 0) continue;

		spr->ID() = i;
	}
}
//==================================================================================================================================
void PlatformerMap::RemoveSprite(int id)
{
	AISprite* spr = 0;
	auto current = m_Sprites.begin();
	while (current != m_Sprites.end())
	{
		spr = (*current);
		if (spr != 0 && spr->ID() == id)
		{
			current = m_Sprites.erase(current);
			spriteNum--;
		}
		else
		{
			++current;
		}
	}

	// Refractor the sprite ids
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		spr = m_Sprites[i];

		if (spr == 0) continue;

		spr->ID() = i;
	}
}
//==================================================================================================================================
void PlatformerMap::RemoveSprite(XMFLOAT3 position)
{
	AISprite* spr = 0;
	auto current = m_Sprites.begin();
	while (current != m_Sprites.end())
	{
		spr = (*current);
		if (spr != 0
		&& spr->TopLeftPosition().x == position.x
		&& spr->TopLeftPosition().y == position.y
		&& spr->TopLeftPosition().z == position.z)
		{
			current = m_Sprites.erase(current);
			spriteNum--;
		}
		else
		{
			++current;
		}
	}

	// Refractor the sprite ids
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		spr = m_Sprites[i];

		if (spr == 0) continue;

		spr->ID() = i;
	}
}
//==================================================================================================================================
void PlatformerMap::AddSprite(AISprite* s)
{
	s->IsPlayer() = false;
	s->ID() = spriteNum;
	m_Sprites[spriteNum] = s;
	spriteNum++;
}
//==================================================================================================================================
void PlatformerMap::AddSpriteNoIndex(AISprite* s)
{
	s->IsPlayer() = false;
	s->ID() = spriteNum;
	m_Sprites[spriteNum] = s;
}
//==================================================================================================================================
void PlatformerMap::AddPlayer(AISprite* s)
{
	s->IsPlayer() = true;
	s->ID() = spriteNum;
	m_Sprites[spriteNum] = s;
	spriteNum++;
}
//==================================================================================================================================
void PlatformerMap::AddPlayerNoIndex(AISprite* s)
{
	s->IsPlayer() = true;
	s->ID() = spriteNum;
	m_Sprites[spriteNum] = s;
}
//==================================================================================================================================
void PlatformerMap::ClipSprites()
{
	vector<AISprite*> renderList(MAX_SPRITES);
	
	for (int j = 0; j < m_Sprites.size(); j++)
	{
		if (m_Sprites[j] == NULL) continue;

		AISprite* spr = m_Sprites[j];
		
		float x = spr->X();
		float y = spr->Y();

		if (!spr->IsPlayer())
		{
			x += m_Offset.x;
			y += m_Offset.y;
		}

		// Clip the sprite if it is not in the screen
		if (!InScreen(x, y, spr->Width(), spr->Height()) && !spr->IsDisplaySprite()) continue;

		renderList[j] = spr;
	}
	
	//Fix the render list based on the clipping of each sprite
	if (m_Sprites.size() > 0)
	{
		if (m_ClippedSprites.size() > 0)
		{
			m_ClippedSprites.clear();
			m_ClippedSprites.resize(MAX_SPRITES);
		}

		m_numSpritesRendered = 0;

		for (int j = 0; j < renderList.size(); j++)
		{
			if (renderList[j] != NULL)
			{
				m_numSpritesRendered++;
			}

			m_ClippedSprites[j] = renderList[j];
		}
	}
}
//==================================================================================================================================
void PlatformerMap::AddSpriteAnimations(string profileName, string base_game_folder, string base_sprite_path, int id)
{
	//Load a animation profile
	AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

	bool something_wrong = false;
	
	if (profile->ReadProfile(profileName, "SPRITE", base_sprite_path, base_game_folder))
	{
		vector<Animation2D*> anims = profile->GetAnimations();

		AISprite* spr;

		for (int sprite_num = 0; sprite_num < m_Sprites.size(); sprite_num++)
		{
			if (sprite_num == id)
			{
				spr = m_Sprites[sprite_num];

				if (spr == 0) continue;

				spr->ClearAnimationList();

				for (int i = 0; i < anims.size(); i++)
				{
					spr->AddAnimationSequence(anims[i]);
				}

				spr->SetSequence(spr->DefaultSequence());

				break;
			}
		}
	}
	else something_wrong = true;

	if (something_wrong)
	{
		MessageBox(NULL, "The Animation2D does not exist", "Add Animations", MB_OK);
	}
}
//==================================================================================================================================
void PlatformerMap::AddTileAnimations(string profileName, string base_game_folder, string base_tile_path, float tile_x, float tile_y)
{
	//Load a animation profile
	AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

	bool something_wrong = false;
	
	if (profile->ReadProfile(profileName, "TILE", base_tile_path, base_game_folder))
	{
		vector<Animation2D*> anims = profile->GetAnimations();

		PlatformerTile* tile = mTiles(tile_x, tile_y);
		
		if (tile == 0) return;
		
		tile->ClearAnimationList();

		for (int i = 0; i < anims.size(); i++)
		{
			tile->AddAnimationSequence(anims[i]);
		}

		tile->SetSequence(0);
	}
	else something_wrong = true;

	if (something_wrong)
	{
		MessageBox(NULL, "The Animation2D does not exist", "Add Animations", MB_OK);
	}
}
//==================================================================================================================================
void PlatformerMap::LoadSpritesMain()
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_Sprites.size(); sprite_num++)
	{
		spr = m_Sprites[sprite_num];
		
		if (spr == 0) continue;
		
		spr->AddScript(spr->ScriptName());
		
		if (spr->GetScript() != NULL)
		{
			spr->Main();
		}
	}
}
//==================================================================================================================================
AISprite* PlatformerMap::GetSpriteByID(int id)
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_Sprites.size(); sprite_num++)
	{
		spr = m_Sprites[sprite_num];

		if (spr == 0) continue;

		if (spr->ID() == id)
		{
			return spr;
		}
	}

	return NULL;
}
//==================================================================================================================================
AISprite* PlatformerMap::GetSprite(int i)
{
	return m_Sprites[i];
}
//==================================================================================================================================
AISprite* PlatformerMap::GetPlayer()
{
	if (mPlayer != 0)
	{
		// This should help speed up rendering since the player was found
		return mPlayer;
	}
	else
	{
		AISprite* spr;
		for (int sprite_num = 0; sprite_num < m_Sprites.size(); sprite_num++)
		{
			spr = m_Sprites[sprite_num];

			if (spr == 0) continue;

			if (spr->IsPlayer())
			{
				mPlayer = spr;
				return spr;
			}
		}
	}
	
	return NULL;
}
//==================================================================================================================================
void PlatformerMap::Render(Camera* camera)
{
	if (!bInEditor)
	{
		AISprite* player = GetPlayer();
		
		if (player != NULL)
		{
			// Keep the player in the center of the screen
			m_Offset.x = m_EngineOptions->m_screenWidth / 2 - player->Width() / 2 - player->TopLeftPosition().x;
			m_Offset.y = m_EngineOptions->m_screenHeight / 2 - player->Height() / 2 - player->TopLeftPosition().y;
			
			float levelExtentX = static_cast<float>(m_EngineOptions->m_screenWidth * m_BackgroundScreenWidth);
			float levelExtentY = static_cast<float>(m_EngineOptions->m_screenHeight) * 2.0f;
			
			// Limit horizontal scrolling
			if (player->TopLeftPosition().x < m_EngineOptions->m_screenWidth / 2 - player->Width() / 2)
				m_Offset.x = 0.0f;
			
			if (player->TopLeftPosition().x > levelExtentX - m_EngineOptions->m_screenWidth / 2 - player->Width() / 2)
				m_Offset.x = -(levelExtentX - m_EngineOptions->m_screenWidth);
			
			// Limit vertical scrolling
			if (player->TopLeftPosition().y >= m_EngineOptions->m_screenHeight / 2 - player->Height() / 2)
				m_Offset.y = 0.0f;
			
			if (player->TopLeftPosition().y < -levelExtentY + m_EngineOptions->m_screenHeight / 2 - player->Height() / 2)
				m_Offset.y = levelExtentY - m_EngineOptions->m_screenHeight;
		}
	}
	
	int firstTileX = (int)ZShadeSandboxMath::ZMath::DoRound(-m_Offset.x) / m_EngineOptions->TILE_SIZE;
	int lastTileX = firstTileX + ((int)ZShadeSandboxMath::ZMath::DoRound(m_EngineOptions->m_screenWidth) / m_EngineOptions->TILE_SIZE) + 1;

	int firstTileY = (int)ZShadeSandboxMath::ZMath::DoRound(-m_Offset.y) / m_EngineOptions->TILE_SIZE;
	int lastTileY = firstTileY + ((int)ZShadeSandboxMath::ZMath::DoRound(m_EngineOptions->m_screenHeight) / m_EngineOptions->TILE_SIZE) + 1;

	for (int y = firstTileY; y <= lastTileY; y++)
	{
		for (int x = firstTileX; x <= lastTileX; x++)
		{
			if (InBounds(x, y) && !mTiles(x, y)->TileInvisible())
			{
				m_D3DSystem->TurnOnAlphaBlending();
				{
					mTiles(x, y)->Render(camera, 0, m_Offset);
				}
				m_D3DSystem->TurnOffAlphaBlending();
			}
		}
	}
	
	//
	// Render the Sprites
	//
	
	ClipSprites();

	for (int i = 0; i < m_ClippedSprites.size(); i++)
	{
		if (m_ClippedSprites[i] == NULL) continue;

		AISprite* s = m_ClippedSprites[i];
		
		if (s->IsDisplaySprite())
		{
			if (!m_canRenderDisplaySprite)
			{
				continue;
			}
		}

		if (s->IsCloneDisplaySprite())
		{
			if (!m_canRenderCloneDisplaySprite)
			{
				continue;
			}
		}
		
		//Update the sprites behaviour if their are any
		s->updateBehavior();

		//Update the animation if it exists
		s->UpdateAnimation();
		
		m_D3DSystem->TurnOnAlphaBlending();
		if (!s->IsPlayer())
		{
			if (s->IsDisplaySprite())
			{
				// Render the display sprite normally so it follows the mouse
				s->Render(camera, 0);
			}
			else
			{
				// Render a regular sprite with an offset
				s->Render(camera, 0, m_Offset);
			}
		}
		else
		{
			if (bInEditor)
			{
				// Render the player with an offset since the player is also affected by it in the editor
				s->Render(camera, 0, m_Offset);
			}
			else
			{
				// Render the player normally since everything moves around the player
				s->Render(camera, 0);
			}
		}
		m_D3DSystem->TurnOffAlphaBlending();
	}
}
//==================================================================================================================================