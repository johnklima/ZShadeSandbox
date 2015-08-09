#include "TopdownMap.h"
#include "FogData.h"
//=================================================================================================================
//=================================================================================================================
TopdownMap::TopdownMap(D3D* d3d, Map2DType mt, GameDirectory2D* gd)
:   m_D3DSystem(d3d)
,	m_GD2D(gd)
,	bInEditor(false)
,	mPlayer(0)
,	m_bFOW(false)
,	m_iFOWRadius(0)
,	m_iFOWLastRadius(0)
,	m_bFOWFlashlight(false)
,	m_bFOWUncovered(false)
,	m_currentVision(0)
,	spriteNum(0)
,	m_numSpritesRendered(0)
{
	m_Offset = XMFLOAT2(0, 0);
	m_EngineOptions = d3d->GetEngineOptions();
	m_Sprites.resize(MAX_SPRITES);
	mSpriteList.resize(MAX_SPRITES);
}
//=================================================================================================================
TopdownMap::~TopdownMap()
{
}
//=================================================================================================================
void TopdownMap::Initialize(int size)
{
	m_NumSectionsPerRow = size;
	
	m_NumSections = m_NumSectionsPerRow * m_NumSectionsPerRow;
	
	bool hard = false;
	bool init = true; // We are creating a brand new map
	bool isSectionPicker = false;
	int animSpeed = 0;
	
	int tileSize = m_EngineOptions->TILE_SIZE;
	
	m_MapWidth = m_EngineOptions->ROW_SIZE * m_NumSectionsPerRow;
	m_MapHeight = m_EngineOptions->COL_SIZE * m_NumSectionsPerRow;
	
	m_Tiles.resize(m_MapWidth, m_MapHeight);

	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			int xs = x * tileSize;
			int ys = y * tileSize;
			
			CreateTile(
				x, y,
				xs, ys,
				tileSize,
				hard,
				init,
				"PatchyGrass04.png",
				"NONE",
				animSpeed,
				isSectionPicker
			);
		}
	}
	
	// Create the player and add it to the map
	AISprite* player = new AISprite(m_D3DSystem);
	player->Initialize(m_GD2D->m_required_textures, "player_cover.png", XMFLOAT3(0, 0, 0), 64, 64, PhysicsType::DYNAMIC);
	AddPlayer(player);
	
	InitializeSectionPickerTiles();

	CalculateScrollOffsets();
}
//=================================================================================================================
void TopdownMap::Initialize(int size, MapLoadData mld)
{
	m_NumSectionsPerRow = size;
	
	m_NumSections = m_NumSectionsPerRow * m_NumSectionsPerRow;
	
	int tile_id = 0;
	bool init = false; // We are loading a map
	bool isSectionPicker = false;
	
	int tileSize = m_EngineOptions->TILE_SIZE;
	
	m_MapWidth = m_EngineOptions->ROW_SIZE * m_NumSectionsPerRow;
	m_MapHeight = m_EngineOptions->COL_SIZE * m_NumSectionsPerRow;
	
	m_Tiles.resize(m_MapWidth, m_MapHeight);
	
	m_currentVision = mld.mapVision;
	m_MusicName = mld.mapMusicName;
	
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			int xs = x * tileSize;
			int ys = y * tileSize;
			
			string textureFilename = mld.imageNames[tile_id];
			string animProfileName = mld.animProfileNames[tile_id];
			int animSpeed = mld.animSpeeds[tile_id];
			int hard = mld.hards[tile_id];
			
			CreateTile(
				x, y,
				xs, ys,
				tileSize,
				hard,
				init,
				textureFilename,
				animProfileName,
				animSpeed,
				isSectionPicker
			);
			
			tile_id++;
		}
	}
	
	InitializeSectionPickerTiles();

	CalculateScrollOffsets();
}
//=================================================================================================================
void TopdownMap::InitializeSectionPickerTiles()
{
	bool hard = false;
	bool init = true;
	bool isSectionPicker = true;
	int animSpeed = 0;
	
	int tileSize = m_EngineOptions->TILE_SIZE / 2;
	
	m_SectionPickerTiles.resize(m_NumSectionsPerRow, m_NumSectionsPerRow);
	
	for (int y = 0; y < m_NumSectionsPerRow; y++)
	{
		for (int x = 0; x < m_NumSectionsPerRow; x++)
		{
			int xs = x * tileSize;
			int ys = y * tileSize;
			
			CreateTile(
				x, y,
				xs, ys,
				tileSize,
				hard,
				init,
				"overtile.png",
				"NONE",
				animSpeed,
				isSectionPicker
			);
		}
	}
}
//=================================================================================================================
void TopdownMap::CreateTile
(	int x
,	int y
,	int xPos
,	int yPos
,	int size
,	bool hard
,	bool init
,	string textureFilename
,	string animProfileName
,	int animSpeed
,	bool isSectionPicker
)
{
	string base_path = "";

	if (init)
	{
		base_path = m_GD2D->m_required_textures;
	}
	else
	{
		base_path = m_GD2D->m_tiles_path;
	}
	
	if (isSectionPicker)
	{
		m_SectionPickerTiles.insert(x, y, new TopdownTile(m_D3DSystem));
		m_SectionPickerTiles(x, y)->InitializeBody(size, XMFLOAT3(xPos, yPos, 0));
		m_SectionPickerTiles(x, y)->AnimationProfileName() = animProfileName;
		m_SectionPickerTiles(x, y)->AnimationSpeed() = animSpeed;
		m_SectionPickerTiles(x, y)->Hard() = hard;
		m_SectionPickerTiles(x, y)->InitializeMesh(base_path, textureFilename, size, size);
		
		return;
	}
	
	m_Tiles.insert(x, y, new TopdownTile(m_D3DSystem));
	m_Tiles(x, y)->InitializeBody(size, XMFLOAT3(xPos, yPos, 0));
	m_Tiles(x, y)->AnimationProfileName() = animProfileName;
	m_Tiles(x, y)->AnimationSpeed() = animSpeed;
	m_Tiles(x, y)->Hard() = hard;
	m_Tiles(x, y)->InitializeMesh(base_path, textureFilename, size, size);
	
	
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

				TopdownTile* tile = m_Tiles(x, y);

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
			MessageBox(NULL, "The Animation2D does not exist", "Add Platform TopdownTile Animations", MB_OK);
		}
	}
}
//=================================================================================================================
void TopdownMap::CalculateScrollOffsets()
{
	int secid = 0;
	int sectionXOffsetSize = 0;
	int sectionYOffsetSize = 0;
	
	for (int secCol = 0; secCol < m_NumSectionsPerRow; secCol++)
	{
		for (int secRow = 0; secRow < m_NumSectionsPerRow; secRow++)
		{
			SectionOffset so;

			so.sectionID = secid++;

			so.offset = XMFLOAT2((float)sectionXOffsetSize, (float)sectionYOffsetSize);

			sectionXOffsetSize += -(m_EngineOptions->ROW_SIZE * m_EngineOptions->TILE_SIZE);

			mSectionOffsets.push_back(so);
		}

		sectionXOffsetSize = 0;
		sectionYOffsetSize += -(m_EngineOptions->COL_SIZE * m_EngineOptions->TILE_SIZE);
	}
}
//=================================================================================================================
XMFLOAT2 TopdownMap::GetSectionScrollOffset(int sectionID)
{
	for (int i = 0; i < mSectionOffsets.size(); i++)
	{
		if (mSectionOffsets[i].sectionID == sectionID)
		{
			return mSectionOffsets[i].offset;
		}
	}
}
//=================================================================================================================
void TopdownMap::SelectSectionID(int& sectionID, int xPos, int yPos)
{
	int secID = 0;

	for (int y = 0; y < m_NumSectionsPerRow; y++)
	{
		for (int x = 0; x < m_NumSectionsPerRow; x++)
		{
			if (m_SectionPickerTiles(x, y)->Body().Inside(xPos, yPos))
			{
				sectionID = secID;

				return;
			}

			secID++;
		}
	}

	sectionID = -1;
}
//=================================================================================================================
TopdownTile* TopdownMap::InsideTile(float xPos, float yPos)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			TopdownTile* tile = GetTile(x, y);
			if (tile == NULL) continue;
			if (tile->Body().Inside(xPos, yPos))
			{
				return tile;
			}
		}
	}
	
	return 0;
}
//==================================================================================================================================
TopdownTile* TopdownMap::GetTile(int xPos, int yPos)
{
	if (InBounds(xPos, yPos))
	{
		return m_Tiles(xPos, yPos);
	}
	else
	{
		int x = xPos / m_EngineOptions->TILE_SIZE;
		int y = yPos / m_EngineOptions->TILE_SIZE;

		if (InBounds(x, y))
		{
			return m_Tiles(x, y);
		}
	}
	
	return 0;
}
//==================================================================================================================================
bool TopdownMap::InBounds(int xPos, int yPos)
{
	return (xPos >= 0 && xPos < m_MapWidth) && (yPos >= 0 && yPos < m_MapHeight);
}
//==================================================================================================================================
bool TopdownMap::InScreen(int xPos, int yPos, int width, int height)
{
	if (bInEditor)
		return (xPos >= -width && xPos < (m_EngineOptions->m_screenWidth - m_ToolWindowWidth + width)) && (yPos >= -height && yPos < m_EngineOptions->m_screenHeight + height);
	else
		return (xPos >= -width && xPos < m_EngineOptions->m_screenWidth + width) && (yPos >= -height && yPos < m_EngineOptions->m_screenHeight + height);
}
//==================================================================================================================================
void TopdownMap::SetTilesFOW(bool fow)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			m_Tiles(x, y)->FOW() = fow;
		}
	}
}
//==================================================================================================================================
void TopdownMap::SetTilesFOWRadius(int radius)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			m_Tiles(x, y)->FOWRadius() = radius;
		}
	}
}
//==================================================================================================================================
void TopdownMap::SetTilesFOWFlashlight(bool fowFlashlight)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			m_Tiles(x, y)->FOWFlashlight() = fowFlashlight;
		}
	}
}
//==================================================================================================================================
void TopdownMap::SetTilesFOWUncovered(bool fowUncovered)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			m_Tiles(x, y)->FOWUncovered() = fowUncovered;

			if (fowUncovered)
				m_Tiles(x, y)->FowUncovered() = false;
		}
	}
}
//==================================================================================================================================
void TopdownMap::UpdateFogBit(unsigned short bit)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			m_Tiles(x, y)->FogBit() = bit;
		}
	}
}
//==================================================================================================================================
void TopdownMap::UpdateFog(AISprite* sprite)
{
	//Create a window area based on the fow circle so the fow circle says around the sprite
	// and the map is never uncovered
	if (m_bFOWFlashlight)
	{
		FowToggleUpdate();
	}
	
	if (!m_bFOW) return;
	
	// Has the FOW radius changed
	if (m_iFOWLastRadius != m_iFOWRadius)
	{
		// If the radius has changed reset the map to be covered in fog again
		for (int y = 0; y < m_MapHeight; y++)
		{
			for (int x = 0; x < m_MapWidth; x++)
			{
				TopdownTile* tile = m_Tiles(x, y);
				if (tile == NULL) continue;
				tile->FogBit() = fow_n_all;
				tile->FogBitShadow() = fow_n_non;
				tile->FowUncovered() = false;
				tile->FowAllUncovered() = false;
				tile->UpdateTexture();
			}
		}
	}
	
	m_iFOWLastRadius = m_iFOWRadius;
	
	float spriteX = sprite->TopLeftPosition().x;
	float spriteY = sprite->TopLeftPosition().y;
	
	// Figure out what tile the sprite is in
	//TopdownTile* tileIn = InsideTile(spriteX - m_EngineOptions->TILE_SIZE, spriteY + m_EngineOptions->TILE_SIZE);
	// Figure out what tile the sprite is in
	bool breakOut = false;
	TopdownTile* tileIn = NULL;
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			TopdownTile* tile = GetTile(x, y);
			if (tile == NULL) continue;
			// Found the tile the sprite is in, Fan the circle out from this tile
			if (tile->Body().Inside(spriteX, spriteY))
			{
				tileIn = tile;
				breakOut = true;
				break;
			}
		}

		if (breakOut) break;
	}

	if (tileIn != 0)
	{
		sprite->TopLeftPosition() = tileIn->TopLeftPosition();

		//
		// Locate the normal and shadow circle mask bits
		//
		
		// The fog circle will stay around the sprites position
		//const int length = (m_iFOWRadius * 2) + 1;
		const unsigned short* mask = &circle_mask[m_iFOWRadius - FOW_RADIUS_MIN][0];
		const unsigned short* shadow_mask = &circle_mask_shadow[m_iFOWRadius - FOW_RADIUS_MIN][0];
		
		float xTo = (tileIn->TopLeftPosition().x + (m_iFOWRadius * m_EngineOptions->TILE_SIZE));
		float xBack = (tileIn->TopLeftPosition().x - (m_iFOWRadius * m_EngineOptions->TILE_SIZE));

		float yTo = (tileIn->TopLeftPosition().y + (m_iFOWRadius * m_EngineOptions->TILE_SIZE));
		float yBack = (tileIn->TopLeftPosition().y - (m_iFOWRadius * m_EngineOptions->TILE_SIZE));

		for (float yy = yBack; yy <= yTo; yy += m_EngineOptions->TILE_SIZE)
		{
			for (float xx = xBack; xx <= xTo; xx += m_EngineOptions->TILE_SIZE)
			{
				if ((xx >= 0 && xx < m_MapWidth * m_EngineOptions->TILE_SIZE)
				&&  (yy >= 0 && yy < m_MapHeight * m_EngineOptions->TILE_SIZE))
				{
					TopdownTile* tile = GetTile(xx, yy);

					tile->IsInFowCircle() = true;

					if (sprite->IsPlayer())
					{
						tile->InPlayerFowCircle() = true;
					}
					else
					{
						tile->InPlayerFowCircle() = false;
					}
					
					if (!m_bFOWFlashlight)
					{
						// The fog bits will be bitwise ANDed with the previous data
						//if (tile->FogBit() != fow_n_non)
						{
							tile->FogBit() &= *mask;
							tile->FogBitShadow() &= *shadow_mask;
						}
					}
					else
					{
						//Update every bit since we are using flash light based fog
						tile->FogBit() = *mask;
						tile->FogBitShadow() = *shadow_mask;
					}
					
					tile->UpdateTexture();
				}
				
				mask++;
				shadow_mask++;
			}
		}
	}
}
//==================================================================================================================================
void TopdownMap::SetTileNotInFowCircle()
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			TopdownTile* tile = m_Tiles(x, y);
			if (tile == NULL) continue;
			tile->IsInFowCircle() = false;
			//tile->FowUncovered() = false;
		}
	}
}
//==================================================================================================================================
void TopdownMap::TileOutsideRadiusInShadow()
{
	if (m_bFOWUncovered) return;

	// The checks for this should be done outside the UpdateFog function so that every sprite that has fow
	//   does not set shadow bit for what it does not see and what is not seen only gets set once.
	// If a tile has been uncovered and is not in the radius of the circle then
	// it will have a shadow because the sprite cannot see any sprites on the tile
	// I think that this will just show squares and the connector pieces will not be there
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			TopdownTile* tile = m_Tiles(x, y);

			if (tile == NULL) continue;

			//if (tile->FowUncovered() && !tile->IsInFowCircle())
			//{
			//	// Assign the all uncovered for pieces that connect to a uncovered shadow piece
			//	if (tile->FogBit() != fow_n_non && tile->FogBit() != fow_n_all)
			//	{
			//		tile->FowAllUncovered() = true;
			//		tile->UpdateTexture();
			//	}
			//}

			unsigned short SavedBit = tile->FogBit();

			if (tile->FogBit() == fow_n_non && !tile->IsInFowCircle() || tile->FowUncovered() && !tile->IsInFowCircle() && tile->FogBit() != fow_n_all)
			{
				tile->FogBit() = fow_n_all;
				tile->FowUncovered() = true;
				tile->UpdateTexture();
			}

			if (tile->IsInFowCircle())
			{
				tile->HasFowShadow() = false;
				tile->FogBit() = SavedBit;
				tile->UpdateTexture();
			}
			else
			{
				tile->HasFowShadow() = true;
				tile->UpdateTexture();
			}
			
			// If the tile is a SSS tile and the tile below it is a CSE or CSW then it is the wrong tile
			// If the tile is a EEE tile and the tile to the east(->) is a CNE or CSE then it is the wrong tile
			// If the tile is a WWW tile and the tile to the west(<-) is a CNW or CSW then it is the wrong tile
			// If the tile is a NNN tile and the tile above it is a CNE or CNW then it is the wrong tile


			//// Make sure the tile has a valid fow bit
			//
			//TopdownTile* northTile = GetTile(x, y - 1);
			//TopdownTile* southTile = GetTile(x, y + 1);
			//TopdownTile* eastTile  = GetTile(x - 1, y);
			//TopdownTile* westTile  = GetTile(x + 1, y);
			//
			//if (northTile && southTile && eastTile && westTile)
			//{
			//	unsigned short northBit = northTile->FogBit();
			//	unsigned short southBit = southTile->FogBit();
			//	unsigned short eastBit = eastTile->FogBit();
			//	unsigned short westBit = westTile->FogBit();

			//	northTile->CalculateFowName();
			//	southTile->CalculateFowName();
			//	eastTile->CalculateFowName();
			//	westTile->CalculateFowName();

			//	string northFowStringName = northTile->FowName();
			//	string southFowStringName = southTile->FowName();
			//	string easthFowStringName = eastTile->FowName();
			//	string westFowStringName = westTile->FowName();

			//	unsigned short bit = FogData::Instance()->FindValidConnectorFromNeighbors(
			//		northBit,
			//		southBit,
			//		eastBit,
			//		westBit
			//	);
			//	
			//	if (tile->FogBit() != bit && bit != -1)
			//	{
			//		// The tile has an invalid bit
			//		tile->FogBit() = bit;
			//		tile->UpdateTexture();
			//	}
			//}
		}
	}
}
//==================================================================================================================================
bool TopdownMap::SpriteInFowCircle(AISprite* sprite)
{
	if (m_bFOW && !m_bFOWUncovered)
	{
		if (sprite->AlwaysSeenByPlayer())
		{
			return true;
		}

		float spriteX = sprite->TopLeftPosition().x;
		float spriteY = sprite->TopLeftPosition().y;

		// Figure out what tile the sprite is in
		TopdownTile* tileIn = InsideTile(spriteX, spriteY);

		sprite->SeenByPlayer() = false;

		bool breakOut = false;
		float xTo = (tileIn->TopLeftPosition().x + (m_iFOWRadius * m_EngineOptions->TILE_SIZE));
		float xBack = (tileIn->TopLeftPosition().x - (m_iFOWRadius * m_EngineOptions->TILE_SIZE));
		float yTo = (tileIn->TopLeftPosition().y + (m_iFOWRadius * m_EngineOptions->TILE_SIZE));
		float yBack = (tileIn->TopLeftPosition().y - (m_iFOWRadius * m_EngineOptions->TILE_SIZE));
		for (float yy = yBack; yy <= yTo; yy += m_EngineOptions->TILE_SIZE)
		{
			for (float xx = xBack; xx <= xTo; xx += m_EngineOptions->TILE_SIZE)
			{
				if ((xx >= 0 && xx < m_MapWidth * m_EngineOptions->TILE_SIZE)
					&& (yy >= 0 && yy < m_MapHeight * m_EngineOptions->TILE_SIZE))
				{
					TopdownTile* tile = GetTile(xx, yy);

					if (tile->InPlayerFowCircle())
					{
						sprite->SeenByPlayer() = true;
						breakOut = true;
						break;
					}
				}
			}

			if (breakOut) break;
		}

		if (!sprite->SeenByPlayer() && !sprite->IsPlayer())
		{
			sprite->InFowCircle() = false;

			if (tileIn != 0 && tileIn->IsInFowCircle())
			{
				// The sprite is in the Fow Circle
				sprite->InFowCircle() = true;
			}

			return sprite->InFowCircle();
		}

		// Using FOW but the sprite is the player so it is seen
		return true;
	}

	// Not using FOW so the sprite will always be rendered
	return true;
}
//==================================================================================================================================
void TopdownMap::FowToggleUpdate()
{
	//Each time fow is toggled, need to look at the map to set the tile bits to either fow_non or fow_all
	
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			TopdownTile* tile = m_Tiles(x, y);
			if (tile == NULL) continue;

			if (m_bFOW)
			{
				tile->FogBit() = fow_n_all;
				tile->FogBitShadow() = fow_ignor;
				tile->FowUncovered() = false;
				tile->HasFowShadow() = true;
			}
			else
			{
				tile->FogBit() = fow_n_non;
				tile->FogBitShadow() = fow_ignor;
			}
			
			tile->UpdateTexture();
		}
	}
}
//==================================================================================================================================
void TopdownMap::InitPosFOW(XMFLOAT3 realpos, unsigned short mask)
{
	if (InBounds(realpos.x, realpos.y))
	{
		if (!m_bFOWFlashlight)
		{
			GetTile(realpos.x, realpos.y)->FogBit() &= mask;
		}
		else//Update bit since we are using flashlight based fog
		{
			GetTile(realpos.x, realpos.y)->FogBit() = mask;
		}
	}
}
//==================================================================================================================================
void TopdownMap::UpdateTileTexture(int xPos, int yPos, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			TopdownTile* tile = m_Tiles(x, y);
			if (tile == NULL) continue;
			
			if (tile->Body().Inside(xPos, yPos))
			{
				// Update texture image of the tile (really just need to change the material diffuse texture)
				tile->UpdateMeshTexture(base_tile_path, textureFilename);

				return;
			}
		}
	}
}
//==================================================================================================================================
void TopdownMap::UpdateTileHardness(int xPos, int yPos, bool bHard)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			TopdownTile* tile = m_Tiles(x, y);
			if (tile == NULL) continue;
			
			if (tile->Body().Inside(xPos, yPos))
			{
				tile->Hard() = bHard;
				
				return;
			}
		}
	}
}
//==================================================================================================================================
void TopdownMap::ToggleTileHard(int xPos, int yPos)
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			TopdownTile* tile = m_Tiles(x, y);
			if (tile == NULL) continue;
			
			if (tile->Body().Inside(xPos, yPos))
			{
				if (tile->Hard())
					tile->Hard() = false;
				else
					tile->Hard() = true;
				
				return;
			}
		}
	}
}
//==================================================================================================================================
void TopdownMap::ToggleHardness(bool bEnable)
{
	// Set hardness for the tiles
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			TopdownTile* tile = m_Tiles(x, y);
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
//=================================================================================================================
bool TopdownMap::DoesPlayerExist()
{
	if (mPlayer)
	{
		return true;
	}
	else
	{
		Sprite* spr;
		for (int sprite_num = 0; sprite_num < m_Sprites.size(); sprite_num++)
		{
			spr = m_Sprites[sprite_num];
			
			if (spr == 0) continue;

			if (spr->IsPlayer())
			{
				return true;
			}
		}
	}
	
	return false;
}
//==================================================================================================================================
bool TopdownMap::SpriteClicked(AISprite*& clickedSprite, int mouse_x, int mouse_y)
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
void TopdownMap::MoveSprite(AISprite*& clickedSprite, int mouse_x, int mouse_y)
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
void TopdownMap::SetRenderDisplaySprite(bool can_render)
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
void TopdownMap::SetRenderCloneDisplaySprite(bool can_render)
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
//=================================================================================================================
void TopdownMap::DeleteDisplaySprite()
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
//=================================================================================================================
void TopdownMap::DeleteCloneDisplaySprite()
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
//=================================================================================================================
void TopdownMap::DeletePlayerSprite()
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
void TopdownMap::RemoveSprite(AISprite* s)
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
void TopdownMap::RemoveSprite(int id)
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
void TopdownMap::RemoveSprite(XMFLOAT3 position)
{
	AISprite* spr = 0;
	auto current = m_Sprites.begin();
	while (current != m_Sprites.end())
	{
		spr = (*current);
		if (spr != 0
		&&	spr->TopLeftPosition().x == position.x
		&&	spr->TopLeftPosition().y == position.y
		&&	spr->TopLeftPosition().z == position.z)
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
void TopdownMap::AddSprite(AISprite* s)
{
	s->IsPlayer() = false;
	s->ID() = spriteNum;
	m_Sprites[spriteNum] = s;
	spriteNum++;
}
//==================================================================================================================================
void TopdownMap::AddSpriteNoIndex(AISprite* s)
{
	s->IsPlayer() = false;
	m_Sprites[s->ID()] = s;
}
//==================================================================================================================================
void TopdownMap::AddPlayer(AISprite* s)
{
	s->IsPlayer() = true;
	s->ID() = spriteNum;
	m_Sprites[spriteNum] = s;
	spriteNum++;
}
//==================================================================================================================================
void TopdownMap::AddPlayerNoIndex(AISprite* s)
{
	s->IsPlayer() = true;
	m_Sprites[s->ID()] = s;
}
//=================================================================================================================
int TopdownMap::TotalSpritesInMap()
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
//=================================================================================================================
void TopdownMap::SortAndClipSprites()
{
	//Depth que is based on sorting the y position of the sprites on the map if it has a default depth of 0
	//otherwise use assigned sprite depth (range 1 - 2000)(default 0)

	//Get the depth of each sprite and put each sprite in a list

	if (mSpriteList.size() > 0)
	{
		mSpriteList.clear();
		mSpriteList.resize(MAX_SPRITES);
	}

	m_numSpritesRendered = 0;

	SPRQUE sque;
	AISprite* s;
	int temp_depth;
	int num = 0;
	
	for (int j = 0; j < m_Sprites.size(); j++)
	{
		s = m_Sprites[j];

		if (s == 0) continue;
		
		float x = s->X();
		float y = s->Y();

		if (!s->IsPlayer())
		{
			x += m_Offset.x;
			y += m_Offset.y;
		}

		// Clip the sprite if it is not in the screen
		if (!InScreen(x, y, s->Width(), s->Height()) && !s->IsDisplaySprite()) continue;

		//Make sure the sprites vision match the current vision of the map
		if (s->Vision() != m_currentVision) continue;

		//If depth que of the sprite is 0 then use its y position
		//otherwise use its assigned depth
		if (s->Depth() == 0)
		{
			// Using the hardbox position here so that depth is done from the collision point of the sprite
			// and the image will be properly placed in front or behind the image it is in contact with.
			// So that if s small sprite walks into a tree then it will go behind the tree if it enters the tree
			// from the top of the tree if the hardbox is set to the base of the tree.
			temp_depth = s->HardboxPosition().y + s->Height();
		}
		else
		{
			temp_depth = s->Depth();
		}
		
		sque.sprite = s;
		sque.depth = temp_depth;
		sque.sprite_num = num;

		mSpriteList[j] = sque;

		m_numSpritesRendered++;

		if (s->IsDisplaySprite())
		{
			m_numSpritesRendered--;
		}
		
		num++;
	}
	
	//Use insertion sort to sort the sprite list
	SPRQUE X;
	int j;
	for (int i = 1; i < mSpriteList.size(); i++)
	{
		if (mSpriteList[i].sprite == NULL) continue;

		X = mSpriteList[i];
		
		for (j = i; j > 0 && mSpriteList[j - 1].depth > X.depth; j--)
		{
			mSpriteList[j] = mSpriteList[j - 1];
		}

		mSpriteList[j] = X;
	}
}
//==================================================================================================================================
/*void TopdownMap::ClipSprites()
{
	vector<SPRQUE> renderList(MAX_SPRITES);
	
	for (int j = 0; j < mSpriteList.size(); j++)
	{
		if (mSpriteList[j].sprite == NULL) continue;

		AISprite* spr = mSpriteList[j].sprite;
		int depth = mSpriteList[j].depth;
		int sn = mSpriteList[j].sprite_num;
		
		if (InScreen(spr->X(), spr->Y()))
		{
			//The sprite is in the screen
			SPRQUE sque;
			sque.sprite = spr;
			sque.depth = depth;
			sque.sprite_num = sn;
			renderList[j] = sque;
		}
	}
	
	//Fix the render list based on the clipping of each sprite
	if (mSpriteList.size() > 0)
	{
		if (mSpriteList.size() > 0)
		{
			mSpriteList.clear();
			mSpriteList.resize(MAX_SPRITES);
		}

		m_numSpritesRendered = 0;

		for (int j = 0; j < renderList.size(); j++)
		{
			if (renderList[j].sprite != NULL)
				m_numSpritesRendered++;

			//sprite_list.push_back( renderList[j] );
			mSpriteList[j] = renderList[j];
		}
	}
}*/
//==================================================================================================================================
void TopdownMap::AddSpriteAnimations(string profileName, string base_game_folder, string base_sprite_path, int id)
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
void TopdownMap::AddTileAnimations(string profileName, string base_game_folder, string base_tile_path, float tile_x, float tile_y)
{
	//Load a animation profile
	AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

	bool something_wrong = false;
	
	if (profile->ReadProfile(profileName, "TILE", base_tile_path, base_game_folder))
	{
		vector<Animation2D*> anims = profile->GetAnimations();

		TopdownTile* tile = m_Tiles(tile_x, tile_y);
		
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
void TopdownMap::LoadSpritesMain()
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
AISprite* TopdownMap::GetSpriteByID(int id)
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
AISprite* TopdownMap::GetSprite(int i)
{
	return m_Sprites[i];
}
//==================================================================================================================================
AISprite* TopdownMap::GetPlayer()
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
TopdownMap* TopdownMap::Clone()
{
	// Copy the attributes of the map
	int vision = m_currentVision;
	string music = m_MusicName;
	int sectionsPerRow = m_NumSectionsPerRow;
	int numSections = m_NumSections;
	int mapWidth = m_MapWidth;
	int mapHeight = m_MapHeight;
	int toolWindowWidth = m_ToolWindowWidth;
	int spritesRendered = m_numSpritesRendered;
	int numSprites = spriteNum;
	bool renderDisplaySprite = m_canRenderDisplaySprite;
	bool renderCloneDisplaySprite = m_canRenderCloneDisplaySprite;
	bool inEditor = bInEditor;
	bool fow = m_bFOW;
	int fowRadius = m_iFOWRadius;
	bool fowFlashlight = m_bFOWFlashlight;

	// Copy the sprites in the map
	vector<AISprite*> sprites(MAX_SPRITES);
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		if (m_Sprites[i] == NULL) continue;
		sprites[i] = m_Sprites[i]->Clone(m_GD2D);
	}

	// Copy the tiles in the map
	SmartArray2D<TopdownTile*> tiles;
	tiles.resize(mapWidth, mapHeight);
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			tiles.insert(x, y, m_Tiles(x, y)->Clone(m_GD2D));
		}
	}

	// Create a new map and add attributes, sprites and tiles into it
	TopdownMap* map = new TopdownMap(m_D3DSystem, m_MapType, m_GD2D);

	map->Vision() = vision;
	map->MusicName() = music;
	map->NumSectionsPerRow() = sectionsPerRow;
	map->NumSections() = numSections;
	map->Width() = mapWidth;
	map->Height() = mapHeight;
	map->ToolWindowWidth() = toolWindowWidth;
	map->NumSpritesRendered() = spritesRendered;
	map->NumSprites() = numSprites;
	map->CanRenderDisplaySprite() = renderDisplaySprite;
	map->CanRenderCloneDisplaySprite() = renderCloneDisplaySprite;
	map->InEditor() = inEditor;
	map->FOW() = fow;
	map->FOWRadius() = fowRadius;
	map->FOWFlashlight() = fowFlashlight;
	map->Sprites() = sprites;
	map->Tiles() = tiles;
	map->InitializeSectionPickerTiles();
	map->CalculateScrollOffsets();

	return map;
}
//==================================================================================================================================
void TopdownMap::RenderSectionPicker(Camera* camera)
{
	for (int y = 0; y < m_NumSectionsPerRow; y++)
	{
		for (int x = 0; x < m_NumSectionsPerRow; x++)
		{
			if (m_SectionPickerTiles(x, y) == 0) continue;

			// A 30x30 section might need the offset if the window is small
			m_SectionPickerTiles(x, y)->Render(camera, m_Offset);
		}
	}
}
//==================================================================================================================================
void TopdownMap::Render(Camera* camera)
{
	if (!bInEditor)
	{
		AISprite* player = GetPlayer();
		
		if (player != NULL)
		{
			// Keep the player in the center of the screen
			m_Offset.x = m_EngineOptions->m_screenWidth / 2 - player->Width() / 2 - player->TopLeftPosition().x;
			m_Offset.y = m_EngineOptions->m_screenHeight / 2 - player->Height() / 2 - player->TopLeftPosition().y;
			
			// Limit horizontal scrolling
			if (player->TopLeftPosition().x < m_EngineOptions->m_screenWidth / 2 - player->Width() / 2)
				m_Offset.x = 0.0f;
			
			if (player->TopLeftPosition().x > m_EngineOptions->m_screenWidth / 2 - player->Width() / 2)
				m_Offset.x = -(m_EngineOptions->m_screenWidth);
			
			// Limit vertical scrolling
			if (player->TopLeftPosition().y < m_EngineOptions->m_screenHeight / 2 - player->Height() / 2)
				m_Offset.y = 0.0f;
			
			if (player->TopLeftPosition().y > m_EngineOptions->m_screenHeight / 2 - player->Height() / 2)
				m_Offset.y = m_EngineOptions->m_screenHeight;
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
			if (InBounds(x, y))
			{
				if (m_Tiles(x, y) == 0) continue;

				//Update tile animations if they exist
				m_Tiles(x, y)->UpdateAnimation();
				
				m_D3DSystem->TurnOnAlphaBlending();
				{
					// If FOW is enabled then only render what is seen
					if (m_bFOW)
					{
						// Render the tiles under the fow that is not a completely black tile
						// or is in the circle mask radius and has no fow
						if (m_Tiles(x, y)->FogBit() != fow_n_all || m_Tiles(x, y)->FowUncovered())
						{
							m_Tiles(x, y)->Render(camera, 0, m_Offset);
						}
					}
					else
					{
						m_Tiles(x, y)->Render(camera, 0, m_Offset);
					}
				}
				m_D3DSystem->TurnOffAlphaBlending();
			}
		}
	}
	
	// Set all the tiles to be not in the fow circle since a sprite
	// can move every frame. When a sprite has fow then the tiles will
	// be updated to be in the fow circle.
	if (m_bFOW) SetTileNotInFowCircle();
	
	//
	// Render the Sprites
	//
	
	SortAndClipSprites();

	for (int i = 0; i < mSpriteList.size(); i++)
	{
		if (mSpriteList[i].sprite == NULL) continue;

		AISprite* s = mSpriteList[i].sprite;
		
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
		
		// Figure out if the sprite can be seen if fow is enabled
		// The sprite is not seen if it is outside the fow circle
		if (!SpriteInFowCircle(s)) continue;
		
		// If FOW is enabled update the fog for each sprite
		UpdateFog(s);
		
		//Update the sprites behaviour if their are any
		//s->updateBehavior();

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
	
	//If Fog of War is turned on check the map and render the fog tiles in front of everything else
	if (m_bFOW)
	{
		// If a tile has been uncovered and is no longer in the radius of any sprites fow then it will be in shadow
		// meaning that sprites are not rendered because it is not in the fow of a sprite.
		TileOutsideRadiusInShadow();
		
		// Any tile that is a fog tile will be rendered here instead of with the tiles
		//  so that it can be rendered on top of sprites.
		
		for (int y = firstTileY; y <= lastTileY; y++)
		{
			for (int x = firstTileX; x <= lastTileX; x++)
			{
				if (InBounds(x, y))
				{
					if (!m_Tiles(x, y)->IsInFowCircle() && !m_bFOWUncovered)
					{
						m_D3DSystem->TurnOnAlphaBlending();
						{
							m_Tiles(x, y)->RenderFowShadow(camera, 0, m_Offset);
						}
						m_D3DSystem->TurnOffAlphaBlending();
					}

					if (m_Tiles(x, y)->FogBit() != fow_n_non)
					{
						m_D3DSystem->TurnOnAlphaBlending();
						{
							m_Tiles(x, y)->RenderFow(camera, 0, m_Offset);
						}
						m_D3DSystem->TurnOffAlphaBlending();
					}
				}
			}
		}
	}
}
//==================================================================================================================================