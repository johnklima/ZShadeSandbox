#include "Environment2DMapHelper.h"
#include "TopdownWorld.h"
#include "Convert.h"
#include "MapEditorSystem2D.h"
#include "MenuEditorSystem.h"
#include "GDI.h"
#include "Scripting.h"
#include "AudioSystem.h"
#include "ZShadeXMLLoader.h"
#include "ZShadeXMLSaver.h"
//================================================================================================================
//================================================================================================================
Environment2DMapHelper::Environment2DMapHelper()
{
}
//================================================================================================================
Environment2DMapHelper::~Environment2DMapHelper()
{
}
//================================================================================================================
TopdownMap* Environment2DMapHelper::ActiveTopdownMap()
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return 0;
	
	if (BetterString(env2D->ActiveWorldName()) == "") return 0;
	if (BetterString(env2D->ActiveMapName()) == "") return 0;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName())) return 0;
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName())) return 0;
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName())) return 0;
		
		return env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName());
	}
	
	return 0;
}
//================================================================================================================
PlatformerMap* Environment2DMapHelper::ActivePlatformerMap()
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return 0;
	
	if (BetterString(env2D->ActiveWorldName()) == "")
		return 0;
	if (BetterString(env2D->ActiveMapName()) == "")
		return 0;

	if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName()))
			return 0;
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName()))
			return 0;
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName()))
			return 0;
		
		return env2D->GetPlatformerWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName());
	}
	
	return 0;
}
//================================================================================================================
void Environment2DMapHelper::GetWorldAndMapName(string& activeMapName, string& activeWorldName)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;

	activeMapName = env2D->ActiveMapName();
	activeWorldName = env2D->ActiveWorldName();
}
//================================================================================================================
void Environment2DMapHelper::SetActiveMapName(string name)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;

	if (env2D->WorldLoaded())
	{
		if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
		{
			TopdownWorld* world = env2D->GetTopdownWorld(env2D->ActiveWorldName());
			world->SetMapToRender(name);
			env2D->ActiveMapName() = world->MapToRender();
		}
		else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
		{
			PlatformerWorld* world = env2D->GetPlatformerWorld(env2D->ActiveWorldName());
			world->SetMapToRender(name);
			env2D->ActiveMapName() = world->MapToRender();
		}

		env2D->MapLoaded() = true;

		//if (Scripting::GetEngineOptions()->m_inEditor)
		//	env2D->GetTabEditWindow2D()->InitWorldLoaded();
	}
}
//================================================================================================================
void Environment2DMapHelper::SetActiveRenderedMap(string world_name, string map_name)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;

	env2D->WorldLoaded() = true;
	env2D->ActiveWorldName() = world_name;
	SetActiveMapName(map_name);
}
//================================================================================================================
XMFLOAT3 Environment2DMapHelper::GetPlayerPosition()
{
	float x = 0;
	float y = 0;

	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return XMFLOAT3(0, 0, 0);

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownWorld* world = env2D->GetTopdownWorld(env2D->ActiveWorldName());
		world->GetPlayerPos(x, y);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerWorld* world = env2D->GetPlatformerWorld(env2D->ActiveWorldName());
		world->GetPlayerPos(x, y);
	}

	return XMFLOAT3(x, y, 0);
}
//================================================================================================================
XMFLOAT2 Environment2DMapHelper::GetSectionScrollOffset(int sectionID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return XMFLOAT2(0, 0);

	if (BetterString(env2D->ActiveWorldName()) == "")
		return XMFLOAT2(0, 0);
	if (BetterString(env2D->ActiveMapName()) == "")
		return XMFLOAT2(0, 0);

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName()))
			return XMFLOAT2(0, 0);
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName()))
			return XMFLOAT2(0, 0);
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName()))
			return XMFLOAT2(0, 0);

		return env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName())->GetSectionScrollOffset(sectionID);
	}

	return XMFLOAT2(0, 0);
}
//================================================================================================================
void Environment2DMapHelper::ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownWorld* world = env2D->GetTopdownWorld(env2D->ActiveWorldName());
		world->ClipSpriteToMap(spr, speed, newX, newY, newHBX, newHBY);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerWorld* world = env2D->GetPlatformerWorld(env2D->ActiveWorldName());
		world->ClipSpriteToMap(spr, speed, newX, newY, newHBX, newHBY);
	}
}
//================================================================================================================
void Environment2DMapHelper::RemoveTileTexture(int x, int y)
{
	PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
	if (map != 0)
	{
		map->RemoveTileTexture(x, y);
	}
}
//================================================================================================================
void Environment2DMapHelper::UpdateTileTexture(string textureName, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			TopdownTile* tile = map->GetTile(x, y);
			if (tile != NULL)
			{
				map->UpdateTileTexture(x, y, env2D->GetGD2D()->m_tiles_path, textureName,
					env2D->GetEngineOptions()->TILE_SIZE, env2D->GetEngineOptions()->TILE_SIZE);
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			PlatformerTile* tile = map->GetTile(x, y);
			if (tile != NULL)
			{
				map->UpdateTileTexture(x, y, env2D->GetGD2D()->m_tiles_path, textureName,
					env2D->GetEngineOptions()->TILE_SIZE, env2D->GetEngineOptions()->TILE_SIZE);
			}
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::NullTileAnimation(int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			TopdownTile* tile = map->GetTile(x, y);
			if (tile == NULL) return;
			tile->AnimationProfileName() = "NONE";
			tile->NullSequence();
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			PlatformerTile* tile = map->GetTile(x, y);
			if (tile == NULL) return;
			tile->AnimationProfileName() = "NONE";
			tile->NullSequence();
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::UpdateTileAnimation(string animProfileName, int animSpeed, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			TopdownTile* tile = map->GetTile(x, y);
			if (tile == NULL) return;
			map->AddTileAnimations(animProfileName, env2D->GetGD2D()->GAME_FOLDER, env2D->GetGD2D()->m_tiles_path, x, y);
			tile->AnimationProfileName() = animProfileName;
			tile->AnimationSpeed() = animSpeed;
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			PlatformerTile* tile = map->GetTile(x, y);
			if (tile == NULL) return;
			map->AddTileAnimations(animProfileName, env2D->GetGD2D()->GAME_FOLDER, env2D->GetGD2D()->m_tiles_path, x, y);
			tile->AnimationProfileName() = animProfileName;
			tile->AnimationSpeed() = animSpeed;
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::UpdateTileHardness(bool bHard, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			map->UpdateTileHardness(x, y, bHard);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			map->UpdateTileHardness(x, y, bHard);
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::MakeTileHard(int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			map->ToggleTileHard(x, y);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			map->ToggleTileHard(x, y);
		}
	}
}
//================================================================================================================
TopdownTile* Environment2DMapHelper::GetSelectedTopdownTile(int x, int y)
{
	TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
	if (map != 0)
	{
		TopdownTile* tile = map->GetTile(x, y);
		if (tile == NULL) return NULL;
		return tile;
	}
	
	return 0;
}
//================================================================================================================
PlatformerTile* Environment2DMapHelper::GetSelectedPlatformerTile(int x, int y)
{
	PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
	if (map != 0)
	{
		PlatformerTile* tile = map->GetTile(x, y);
		if (tile == NULL) return NULL;
		return tile;
	}
	
	return 0;
}
//================================================================================================================
bool Environment2DMapHelper::HasMaps()
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return false;

	if (BetterString(env2D->ActiveWorldName()) == "")
		return false;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName()))
			return false;

		return env2D->GetSTopdownWorld(env2D->ActiveWorldName())->m_mapNames.size() != 0;
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName()))
			return false;

		return env2D->GetSPlatformerWorld(env2D->ActiveWorldName())->m_mapNames.size() != 0;
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapWidthAttribute(int& width)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			width = map->Width();
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			width = map->Width();
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapHeightAttribute(int& height)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			height = map->Height();
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			height = map->Height();
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::ToggleMapHardness(bool bEnable)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			map->ToggleHardness(bEnable);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			map->ToggleHardness(bEnable);
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapVisionAttribute(int& vision)
{
	TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
	if (map != 0)
	{
		vision = map->Vision();
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapMusicAttribute(string& music)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			music = map->MusicName();
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			music = map->MusicName();
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapFOWAttribute(bool& fow)
{
	TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
	if (map != 0)
	{
		fow = map->FOW();
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapFOWRadiusAttribute(int& fowRadius)
{
	TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
	if (map != 0)
	{
		fowRadius = map->FOWRadius();
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapFOWFlashlightAttribute(bool& fowFlashlight)
{
	TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
	if (map != 0)
	{
		fowFlashlight = map->FOWFlashlight();
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapFOWUncoveredAttribute(bool& fowUncovered)
{
	TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
	if (map != 0)
	{
		fowUncovered = map->FOWUncovered();
	}
}
//================================================================================================================
void Environment2DMapHelper::SetMapVisionAttribute(int vision)
{
	TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
	if (map != 0)
	{
		map->Vision() = vision;
	}
}
//================================================================================================================
void Environment2DMapHelper::SetMapMusicAttribute(string music)
{
	TopdownMap* topdownmap = Environment2DMapHelper::ActiveTopdownMap();
	if (topdownmap != 0)
	{
		topdownmap->MusicName() = music;
	}

	PlatformerMap* platformermap = Environment2DMapHelper::ActivePlatformerMap();
	if (platformermap != 0)
	{
		platformermap->MusicName() = music;
	}
}
//================================================================================================================
void Environment2DMapHelper::SetMapFOWAttribute(bool fow)
{
	TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
	if (map != 0)
	{
		map->FOW() = fow;
		map->SetTilesFOW(fow);
		map->FowToggleUpdate();
	}
}
//================================================================================================================
void Environment2DMapHelper::SetMapFOWRadiusAttribute(int fowRadius)
{
	TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
	if (map != 0)
	{
		map->FOWRadius() = fowRadius;
		map->SetTilesFOWRadius(fowRadius);
	}
}
//================================================================================================================
void Environment2DMapHelper::SetMapFOWFlashlightAttribute(bool fowFlashlight)
{
	TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
	if (map != 0)
	{
		map->FOWFlashlight() = fowFlashlight;
		map->SetTilesFOWFlashlight(fowFlashlight);
	}
}
//================================================================================================================
void Environment2DMapHelper::SetMapFOWUncoveredAttribute(bool fowUncovered)
{
	TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
	if (map != 0)
	{
		map->FOWUncovered() = fowUncovered;
		map->SetTilesFOWUncovered(fowUncovered);
	}
}
//================================================================================================================
void Environment2DMapHelper::SetMapNameAttribute(string mapName)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;

	TopdownMap* topdownmap = Environment2DMapHelper::ActiveTopdownMap();
	if (topdownmap != 0)
	{
		topdownmap->MapName() = mapName;
	}

	PlatformerMap* platformermap = Environment2DMapHelper::ActivePlatformerMap();
	if (platformermap != 0)
	{
		platformermap->MapName() = mapName;
	}

	// Update this map name for the editor and make it the new active rendered map since the name has changed
	SetActiveRenderedMap(env2D->ActiveWorldName(), mapName);
}
//================================================================================================================
void Environment2DMapHelper::UpdateNewMapName(string world_name, string old_map_name, string new_map_name)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;

	env2D->UpdateNewMapName(world_name, old_map_name, new_map_name);
}
//================================================================================================================
void Environment2DMapHelper::GetNumSpritesRendered(int& spritesRendered)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			spritesRendered = map->NumSpritesRendered();
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			spritesRendered = map->NumSpritesRendered();
		}
	}
}
//================================================================================================================
int Environment2DMapHelper::GetNumSpritesInMap()
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return 0;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			return map->TotalClippedSprites();
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			return map->TotalClippedSprites();
		}
	}

	return 0;
}
//================================================================================================================
void Environment2DMapHelper::SetToolWindowWidth(int width)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			map->ToolWindowWidth() = width;
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			map->ToolWindowWidth() = width;
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::GetSpriteTotal(int& spriteTotal)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			spriteTotal = map->TotalSpritesInMap();
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			spriteTotal = map->TotalSpritesInMap();
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::VisibleTileCount(int& visibleTileCount)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			visibleTileCount = map->VisibleTileCount();
		}
	}
}
//================================================================================================================
AISprite* Environment2DMapHelper::GetSpriteInMap(int i)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return 0;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			return map->GetSpriteByID(i);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			return map->GetSpriteByID(i);
		}
	}

	return 0;
}
//================================================================================================================
void Environment2DMapHelper::AddSpriteInMap(AISprite* spr, bool noIndex)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			(noIndex) ? map->AddSpriteNoIndex(spr) : map->AddSprite(spr);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			(noIndex) ? map->AddSpriteNoIndex(spr) : map->AddSprite(spr);
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::AddPlayerInMap(AISprite* spr, bool noIndex)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			(noIndex) ? map->AddPlayerNoIndex(spr) : map->AddPlayer(spr);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			(noIndex) ? map->AddPlayerNoIndex(spr) : map->AddPlayer(spr);
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::SelectTopdownSectionID(int& sectionID, int x, int y)
{
	TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
	if (map != 0)
	{
		map->SelectSectionID(sectionID, x, y);
	}
}
//================================================================================================================
void Environment2DMapHelper::RemoveSpriteInMap(AISprite* spr)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			map->RemoveSprite(spr);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			map->RemoveSprite(spr);
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::AddSprite(string spriteBaseTextureName, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;
	
	// Create a new sprite and add it to the map and make it static
	AISprite* spr = new AISprite(env2D->GetD3D());
	
	XMFLOAT3 position(x, y, 0);
	
	// Should the default sprite size be set to 32x32 ?
	spr->Initialize(env2D->GetGD2D()->m_sprites_path, spriteBaseTextureName, position, env2D->GetEngineOptions()->TILE_SIZE, env2D->GetEngineOptions()->TILE_SIZE, STATIC);
	
	Environment2DMapHelper::AddSpriteInMap(spr);
}
//================================================================================================================
bool Environment2DMapHelper::SpriteClicked(AISprite*& spr, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0)
		return false;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			return map->SpriteClicked(spr, x, y);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			return map->SpriteClicked(spr, x, y);
		}
	}
	
	return false;
}
//================================================================================================================
bool Environment2DMapHelper::GetSpriteID(int& spriteID, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return false;

	AISprite* p_clicked_sprite;
	bool clicked = SpriteClicked(p_clicked_sprite, x, y);
	if (clicked)
	{
		spriteID = p_clicked_sprite->ID();
	}

	return clicked;
}
//================================================================================================================
bool Environment2DMapHelper::MoveSpriteStart(AISprite*& spr, int& spriteID, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0)
		return false;
	
	AISprite* p_clicked_sprite;
	bool clicked = SpriteClicked(p_clicked_sprite, x, y);
	if (clicked)
	{
		spriteID = p_clicked_sprite->ID();
		
		// Get the actual sprite rather than clone it since we are moving the sprite
		spr = p_clicked_sprite;
	}
	
	return clicked;
}
//================================================================================================================
void Environment2DMapHelper::MoveSpriteEnd(AISprite*& spr, int spriteID, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;
	
	// Create a new copy of the moved sprite
	AISprite* newCopy = spr->Clone(env2D->GetGD2D(), x, y);
	
	// Add the new sprite created to the map
	if (newCopy->IsPlayer())
		Environment2DMapHelper::AddPlayerInMap(newCopy);
	else
		Environment2DMapHelper::AddSpriteInMap(newCopy);
	
	// Remove the old sprite from the map
	Environment2DMapHelper::DeleteSprite(spriteID);
	
	// The old sprite has been removed so clear the memory
	spr = 0;
}
//================================================================================================================
bool Environment2DMapHelper::CloneSpriteStart(AISprite*& spr, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return false;
	
	AISprite* p_clicked_sprite;
	bool clicked = SpriteClicked(p_clicked_sprite, x, y);
	if (clicked)
	{
		spr = p_clicked_sprite->Clone(env2D->GetGD2D(), x, y);
	}
	
	return clicked;
}
//================================================================================================================
void Environment2DMapHelper::MoveSprite(AISprite*& spr, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			map->MoveSprite(spr, x, y);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			map->MoveSprite(spr, x, y);
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::CloneSpriteEnd(AISprite*& spr, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;
	
	// Create a new copy of the moved sprite
	AISprite* newCopy = spr->Clone(env2D->GetGD2D(), x, y);
	
	// Add the new sprite created to the map
	if (newCopy->IsPlayer())
		Environment2DMapHelper::AddPlayerInMap(newCopy);
	else
		Environment2DMapHelper::AddSpriteInMap(newCopy);
	
	spr = 0;
}
//================================================================================================================
void Environment2DMapHelper::DeleteSprite(AISprite* spr)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			map->RemoveSprite(spr);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			map->RemoveSprite(spr);
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::DeleteSprite(int id)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			map->RemoveSprite(id);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			map->RemoveSprite(id);
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::ResizeSpriteUpdate(AISprite*& spr, bool updatePosition, bool updateSize, bool goFast,
	bool pressedLeftKey, bool pressedRightKey, bool pressedUpKey, bool pressedDownKey)
{
	if (spr != 0)
	{
		Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

		// Did not find an environment
		if (env2D == 0)
			return;
		
		float s_x = spr->TopLeftPosition().x;
		float s_y = spr->TopLeftPosition().y;
		int s_width = spr->Width();
		int s_height = spr->Height();

		//Need to move the xy position and size of the hardbox
		
		if (updatePosition)
		{
			if (goFast)
			{
				if (pressedLeftKey) s_x -= 2;
				if (pressedRightKey) s_x += 2;
				if (pressedUpKey) s_x -= 2;
				if (pressedDownKey) s_x += 2;
			}
			else
			{
				if (pressedLeftKey) s_x -= 1;
				if (pressedRightKey) s_x += 1;
				if (pressedUpKey) s_x -= 1;
				if (pressedDownKey) s_x += 1;
			}
		}
		
		if (updateSize)
		{
			if (goFast)
			{
				if (pressedLeftKey) s_height -= 2;
				if (pressedRightKey) s_height += 2;
				if (pressedUpKey) s_width -= 2;
				if (pressedDownKey) s_width += 2;
			}
			else
			{
				if (pressedLeftKey) s_height -= 1;
				if (pressedRightKey) s_height += 1;
				if (pressedUpKey) s_width -= 1;
				if (pressedDownKey) s_width += 1;
			}
		}
		
		spr->CloneMe(env2D->GetGD2D(), s_x, s_y, s_width, s_height);
	}
}
//================================================================================================================
void Environment2DMapHelper::HighlightSprite(AISprite*& spriteWithBox, Sprite*& highlightBox, bool& boxCreated, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;
	
	AISprite* p_clicked_sprite;
	bool clicked = SpriteClicked(p_clicked_sprite, x, y);
	
	if (clicked)
	{
		if (spriteWithBox != 0)
		{
			if (spriteWithBox != p_clicked_sprite)
			{
				highlightBox = 0;
				boxCreated = false;
			}
		}
		
		if (!boxCreated)
		{
			spriteWithBox = p_clicked_sprite;
			
			string base_path = env2D->GetGD2D()->m_required_textures;
			base_path.append("\\");
			
			XMFLOAT3 position(spriteWithBox->TopLeftPosition().x, spriteWithBox->TopLeftPosition().y, 0);
			
			highlightBox = new Sprite(env2D->GetD3D());
			highlightBox->Initialize(
				base_path,
				"stamp_normal.png",
				position,
				spriteWithBox->Width(),
				spriteWithBox->Height(),
				spriteWithBox->EPhysicsType()
			);
			
			boxCreated = true;
		}
	}
	else
	{
		spriteWithBox = 0;
		highlightBox = 0;
		boxCreated = false;
	}
}
//================================================================================================================
/*AISprite* Environment2DMapHelper::ClickedSprite(int x, int y, bool& clicked)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return 0;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr;
			clicked = map->SpriteClicked(spr, x, y);
			return spr;
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr;
			clicked = map->SpriteClicked(spr, x, y);
			return spr;
		}
	}
	
	return 0;
}*/
//================================================================================================================
void Environment2DMapHelper::RemoveDisplaySprite()
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			map->DeleteDisplaySprite();
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			map->DeleteDisplaySprite();
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::SetCanRenderDisplaySprite(bool canRender)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			map->SetRenderDisplaySprite(canRender);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			map->SetRenderDisplaySprite(canRender);
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::RemoveCloneDisplaySprite()
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			map->DeleteCloneDisplaySprite();
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			map->DeleteCloneDisplaySprite();
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::SetCanRenderCloneDisplaySprite(bool canRender)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			map->SetRenderCloneDisplaySprite(canRender);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			map->SetRenderCloneDisplaySprite(canRender);
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::ToggleSelectedSpriteBackground(bool background, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->IsBackgroundSprite() = background;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->IsBackgroundSprite() = background;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::ToggleSelectedSpriteEntity(bool entity, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->IsEntitySprite() = entity;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->IsEntitySprite() = entity;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::ToggleSelectedSpriteInvisible(bool invisible, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->IsInvisibleSprite() = invisible;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->IsInvisibleSprite() = invisible;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::ToggleSelectedSpriteHard(bool hard, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->IsHard() = hard;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->IsHard() = hard;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::ToggleSelectedSpriteNoHit(bool nohit, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->IsNoHit() = nohit;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->IsNoHit() = nohit;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::ToggleSelectedSpritePhysicsType(bool isstatic, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->EPhysicsType() = (isstatic) ? STATIC : DYNAMIC;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->EPhysicsType() = (isstatic) ? STATIC : DYNAMIC;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::ToggleSelectedSpriteTouch(bool touch, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->IsTouch() = touch;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->IsTouch() = touch;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::ToggleSelectedSpriteTeleport(bool teleport, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->CanTeleport() = teleport;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->CanTeleport() = teleport;
			}
		}
	}
}
//==================================================================================================================================
bool Environment2DMapHelper::SelectedSpriteIsBackground(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->IsBackgroundSprite();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->IsBackgroundSprite();
			}
		}
	}
	
	return false;
}
//==================================================================================================================================
bool Environment2DMapHelper::SelectedSpriteIsEntity(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->IsEntitySprite();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->IsEntitySprite();
			}
		}
	}
	
	return false;
}
//==================================================================================================================================
bool Environment2DMapHelper::SelectedSpriteIsInvisible(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->IsInvisibleSprite();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->IsInvisibleSprite();
			}
		}
	}
	
	return false;
}
//==================================================================================================================================
bool Environment2DMapHelper::SelectedSpriteIsHard(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->IsHard();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->IsHard();
			}
		}
	}
	
	return false;
}
//==================================================================================================================================
bool Environment2DMapHelper::SelectedSpriteIsNoHit(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->IsNoHit();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->IsNoHit();
			}
		}
	}
	
	return false;
}
//==================================================================================================================================
bool Environment2DMapHelper::SelectedSpriteIsStatic(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return (spr->EPhysicsType() == STATIC) ? true : false;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return (spr->EPhysicsType() == STATIC) ? true : false;
			}
		}
	}
	
	return false;
}
//==================================================================================================================================
bool Environment2DMapHelper::SelectedSpriteIsTouch(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->IsTouch();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->IsTouch();
			}
		}
	}
	
	return false;
}
//==================================================================================================================================
bool Environment2DMapHelper::SelectedSpriteIsTeleportSprite(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->CanTeleport();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->CanTeleport();
			}
		}
	}
	
	return false;
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteVision(int vision, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Vision() = vision;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Vision() = vision;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteDepth(int depth, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Depth() = depth;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Depth() = depth;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteTextureName(string textureName, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->BaseTextureFilename() = textureName;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->BaseTextureFilename() = textureName;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteAnimationName(string animationName, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->AnimationProfileName() = animationName;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->AnimationProfileName() = animationName;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteDefaultSequence(int sequenceNum, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->DefaultSequence() = sequenceNum;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->DefaultSequence() = sequenceNum;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteAnimationSpeed(int speed, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->AnimationSpeed() = speed;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->AnimationSpeed() = speed;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteCollideMsg(string collideMsg, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->CollideMsg() = collideMsg;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->CollideMsg() = collideMsg;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteHPMax(int value, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->HPMax() = value;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->HPMax() = value;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteHP(int value, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->HP() = value;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->HP() = value;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteMana(int value, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Mana() = value;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Mana() = value;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteGold(int value, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Gold() = value;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Gold() = value;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteStrength(int value, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Strength() = value;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Strength() = value;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteSpeed(int value, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Speed() = value;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Speed() = value;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteDefense(int value, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Defense() = value;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Defense() = value;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteExperience(int value, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Experience() = value;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Experience() = value;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteLevel(int value, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Level() = value;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->Level() = value;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteScriptName(string scriptName, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->ScriptName() = scriptName;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->ScriptName() = scriptName;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteTeleportMapName(string mapName, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->TeleportMapName() = mapName;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->TeleportMapName() = mapName;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteTeleportMapX(int value, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->TeleportX() = value;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->TeleportX() = value;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteTeleportMapY(int value, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->TeleportY() = value;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->TeleportY() = value;
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::SetSelectedSpriteAlwaysSeenByPlayer(bool value, int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->AlwaysSeenByPlayer() = value;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->AlwaysSeenByPlayer() = value;
			}
		}
	}
}
//==================================================================================================================================
float Environment2DMapHelper::GetSelectedSpriteXPos(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->X();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->X();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
float Environment2DMapHelper::GetSelectedSpriteYPos(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Y();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Y();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteVision(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Vision();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Vision();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteDepth(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Depth();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Depth();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
string Environment2DMapHelper::GetSelectedSpriteTextureName(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return "";
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->BaseTextureFilename();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->BaseTextureFilename();
			}
		}
	}
	
	return "";
}
//==================================================================================================================================
string Environment2DMapHelper::GetSelectedSpriteAnimationName(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return "";
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->AnimationProfileName();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->AnimationProfileName();
			}
		}
	}
	
	return "";
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteDefaultSequence(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->DefaultSequence();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->DefaultSequence();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteAnimationSpeed(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->AnimationSpeed();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->AnimationSpeed();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
string Environment2DMapHelper::GetSelectedSpriteCollideMsg(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return "";
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->CollideMsg();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->CollideMsg();
			}
		}
	}
	
	return "";
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteHPMax(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->HPMax();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->HPMax();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteHP(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->HP();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->HP();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteMana(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Mana();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Mana();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteGold(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Gold();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Gold();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteStrength(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Strength();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Strength();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteSpeed(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Speed();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Speed();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteDefense(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Defense();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Defense();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteExperience(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Experience();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Experience();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteLevel(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Level();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->Level();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
string Environment2DMapHelper::GetSelectedSpriteScriptName(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return "";
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->ScriptName();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->ScriptName();
			}
		}
	}
	
	return "";
}
//==================================================================================================================================
string Environment2DMapHelper::GetSelectedSpriteTeleportMapName(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return "";

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->TeleportMapName();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->TeleportMapName();
			}
		}
	}

	return "";
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteTeleportMapX(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->TeleportX();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->TeleportX();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
int Environment2DMapHelper::GetSelectedSpriteTeleportMapY(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->TeleportY();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->TeleportY();
			}
		}
	}
	
	return -1;
}
//==================================================================================================================================
bool Environment2DMapHelper::GetSelectedSpriteAlwaysSeenByPlayer(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return -1;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->AlwaysSeenByPlayer();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				return spr->AlwaysSeenByPlayer();
			}
		}
	}
	
	return true;
}
//==================================================================================================================================
void Environment2DMapHelper::RevertSelectedSpriteAnimation(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->AnimationProfileName() = "NONE";
				spr->NullSequence();
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->AnimationProfileName() = "NONE";
				spr->NullSequence();
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::UpdateSelectedSpriteAnimation(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				if (spr->AnimationProfileName() != "NONE")
				{
					map->AddSpriteAnimations(spr->AnimationProfileName(), env2D->GetGD2D()->GAME_FOLDER, env2D->GetGD2D()->m_sprites_path, spriteID);
				}
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				if (spr->AnimationProfileName() != "NONE")
				{
					map->AddSpriteAnimations(spr->AnimationProfileName(), env2D->GetGD2D()->GAME_FOLDER, env2D->GetGD2D()->m_sprites_path, spriteID);
				}
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::UpdateSelectedSpriteMeshTexture(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->UpdateMeshTexture(env2D->GetGD2D()->m_sprites_path);
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			AISprite* spr = map->GetSpriteByID(spriteID);
			if (spr != 0)
			{
				spr->UpdateMeshTexture(env2D->GetGD2D()->m_sprites_path);
			}
		}
	}
}
//==================================================================================================================================
void Environment2DMapHelper::OpenSelectedSpriteScript(string scriptName)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;

	string scriptNamePath = env2D->GetGD2D()->m_scripts_path;
	scriptNamePath += "\\";
	scriptNamePath += scriptName;
	scriptNamePath += ".lua";

	//If the file is not created yet create it
	ifstream ifile(scriptNamePath);
	if (!ifile)
	{
		ofstream os(scriptNamePath);
		os << "";
		os.close();
	}

	HINSTANCE temp = ShellExecute(NULL, "open", "notepad++", scriptNamePath.c_str(), NULL, SW_SHOWNORMAL);
	if ((int)temp < 32)
	{
		ShellExecute(NULL, "open", "notepad", scriptNamePath.c_str(), NULL, SW_SHOWNORMAL);
	}
}
//==================================================================================================================================
AISprite* Environment2DMapHelper::GetSpriteByID(int spriteID)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return 0;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		TopdownMap* map = Environment2DMapHelper::ActiveTopdownMap();
		if (map != 0)
		{
			return map->GetSpriteByID(spriteID);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerMap* map = Environment2DMapHelper::ActivePlatformerMap();
		if (map != 0)
		{
			return map->GetSpriteByID(spriteID);
		}
	}
	
	return 0;
}
//==================================================================================================================================
void Environment2DMapHelper::LoadWorld(string& worldName, string filename, bool InEditor)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		STopdownWorld* sw = NULL;

		if (!CGlobal::DoesFileExist(filename))
		{
			string path = env2D->GetGD2D()->m_xml_worlds_path;
			path.append("\\");
			path.append(filename);
			path.append(".xml");
			filename = path;
		}

		ZShadeXMLLoader::LoadWorldXML(filename, sw);
		
		env2D->LoadWorld(sw->sWorldName, sw);

		worldName = sw->sWorldName;
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		SPlatformerWorld* sw = NULL;

		if (!CGlobal::DoesFileExist(filename))
		{
			string path = env2D->GetGD2D()->m_xml_worlds_path;
			path.append("\\");
			path.append(filename);
			path.append(".xml");
			filename = path;
		}

		ZShadeXMLLoader::LoadWorldXML(filename, sw);

		env2D->LoadWorld(sw->sWorldName, sw);

		worldName = sw->sWorldName;
	}

	// Load all the maps in the world if they exist
	Environment2DMapHelper::LoadWorld(worldName, InEditor);
}
//================================================================================================================
void Environment2DMapHelper::SaveWorld(string filename)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
		ZShadeXMLSaver::SaveWorldXML(env2D->GetSTopdownWorld(env2D->ActiveWorldName()), filename);
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
		ZShadeXMLSaver::SaveWorldXML(env2D->GetSPlatformerWorld(env2D->ActiveWorldName()), filename);
}
//================================================================================================================
void Environment2DMapHelper::LoadMap(string& mapName, string filename, bool InEditor)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (env2D->GetTopdownWorld(env2D->ActiveWorldName()) != NULL)
			env2D->GetTopdownWorld(env2D->ActiveWorldName())->LoadMap(mapName, filename,
				env2D->GetGD2D(), InEditor);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (env2D->GetPlatformerWorld(env2D->ActiveWorldName()) != NULL)
			env2D->GetPlatformerWorld(env2D->ActiveWorldName())->LoadMap(mapName, filename,
				env2D->GetGD2D(), InEditor);
	}
}
//================================================================================================================
void Environment2DMapHelper::SaveMap(string filename)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
		ZShadeXMLSaver::SaveMapXML(env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName()), filename, env2D->GetGD2D());
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
		ZShadeXMLSaver::SaveMapXML(env2D->GetPlatformerWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName()), filename, env2D->GetGD2D());
}
//================================================================================================================
void Environment2DMapHelper::CreateMapTopdown(int x, int y, bool fow, int fowRadius, bool fowFlashlight, string world_name, string name, bool bIso, bool InEditor)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->GetEngineOptions()->SECTION_ROW_SIZE = x;
	env2D->GetEngineOptions()->SECTION_COL_SIZE = y;
	env2D->GetEngineOptions()->SetTileMax();

	Map2DType mt;

	if (bIso)
		mt = Map2DType::ISOMETRIC;
	else
		mt = Map2DType::REGULAR;

	Environment2DMapHelper::AddMapName(world_name, name);

	Environment2DMapHelper::NewMapTopdown(world_name, name, mt, InEditor);

	// Set the fog attributes
	env2D->GetTopdownWorld(world_name)->GetMap(name)->FOW() = fow;
	env2D->GetTopdownWorld(world_name)->GetMap(name)->FOWRadius() = fowRadius;
	env2D->GetTopdownWorld(world_name)->GetMap(name)->FOWFlashlight() = fowFlashlight;
}
//================================================================================================================
void Environment2DMapHelper::CreateMapPlatformer(int x, int y, string world_name, string name, bool InEditor)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->GetEngineOptions()->SECTION_ROW_SIZE = x;
	env2D->GetEngineOptions()->SECTION_COL_SIZE = y;
	env2D->GetEngineOptions()->SetTileMax();

	Environment2DMapHelper::AddMapName(world_name, name);

	Environment2DMapHelper::NewMapPlatformer(world_name, name, InEditor);
}
//================================================================================================================
void Environment2DMapHelper::NewMapTopdown(string world_name, string name, Map2DType mt, bool InEditor)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	//Get a copy of the world if it exists
	if (env2D->GetTopdownWorld(world_name) != NULL)
	{
		STopdownWorld* sw = env2D->GetSTopdownWorld(world_name);

		if (sw == NULL) return;

		// If the world does not exist then create it
		TopdownWorld* world = sw->m_World;
		
		if (world == NULL)
		{
			world = new TopdownWorld(env2D->GetD3D());

			sw->m_World = world;
		}

		world->SetD3DSystem(env2D->GetD3D());

		sw->m_World->NewMap(name, env2D->GetGD2D(), mt, InEditor);

		sw->m_World->GetMap(name)->MapName() = name;
	}
}
//================================================================================================================
void Environment2DMapHelper::NewMapPlatformer(string world_name, string name, bool InEditor)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	//Get a copy of the world if it exists
	if (env2D->GetPlatformerWorld(world_name) != NULL)
	{
		SPlatformerWorld* sw = env2D->GetSPlatformerWorld(world_name);

		if (sw == NULL) return;

		// If the world does not exist then create it
		PlatformerWorld* world = sw->m_World;

		if (world == NULL)
		{
			world = new PlatformerWorld(env2D->GetD3D());

			sw->m_World = world;
		}

		world->SetD3DSystem(env2D->GetD3D());

		sw->m_World->NewMap(name, env2D->GetGD2D(), InEditor);

		sw->m_World->GetMap(name)->MapName() = name;
	}
}
//================================================================================================================
void Environment2DMapHelper::AddMapName(string world_name, string name)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		STopdownWorld* sw;

		// Create a new world
		if (env2D->GetSTopdownWorld(world_name) == NULL)
		{
			sw = new STopdownWorld();
		}
		else//Get the world
		{
			sw = env2D->GetSTopdownWorld(world_name);
		}

		if (sw == NULL) return;

		sw->m_mapNames.push_back(name);

		env2D->LoadWorld(world_name, sw);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		SPlatformerWorld* sw;

		// Create a new world
		if (env2D->GetSPlatformerWorld(world_name) == NULL)
		{
			sw = new SPlatformerWorld();
		}
		else//Get the world
		{
			sw = env2D->GetSPlatformerWorld(world_name);
		}

		if (sw == NULL) return;

		sw->m_mapNames.push_back(name);

		env2D->LoadWorld(world_name, sw);
	}
}
//================================================================================================================
void Environment2DMapHelper::LoadWorld(string world_name, bool InEditor)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	string mapName;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		//Get a copy of the world if it exists
		if (env2D->GetSTopdownWorld(world_name) != NULL)
		{
			STopdownWorld* sw = env2D->GetSTopdownWorld(world_name);

			if (sw == NULL) return;

			sw->m_World = new TopdownWorld(env2D->GetD3D());

			for (int i = 0; i < sw->m_mapNames.size(); i++)
			{
				string name = sw->m_mapNames[i];

				sw->m_World->LoadMap(name, name, env2D->GetGD2D(), InEditor);
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		//Get a copy of the world if it exists
		if (env2D->GetSPlatformerWorld(world_name) != NULL)
		{
			SPlatformerWorld* sw = env2D->GetSPlatformerWorld(world_name);

			if (sw == NULL) return;

			sw->m_World = new PlatformerWorld(env2D->GetD3D());

			for (int i = 0; i < sw->m_mapNames.size(); i++)
			{
				string name = sw->m_mapNames[i];

				sw->m_World->LoadMap(name, name, env2D->GetGD2D(), InEditor);
			}
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::LoadMapInWorld(string world_name, string name, bool InEditor)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		//Get a copy of the world if it exists
		if (env2D->GetSTopdownWorld(world_name) != NULL)
		{
			STopdownWorld* sw = env2D->GetSTopdownWorld(world_name);

			if (sw == NULL) return;

			// If the world does not exist then create it
			if (sw->m_World == NULL)
				sw->m_World = new TopdownWorld(env2D->GetD3D());

			for (int i = 0; i < sw->m_mapNames.size(); i++)
			{
				string map_name = sw->m_mapNames[i];

				// Make sure it is a valid map
				if (map_name == name)
				{
					sw->m_World->LoadMap(map_name, name, env2D->GetGD2D(), InEditor);

					break;
				}
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		//Get a copy of the world if it exists
		if (env2D->GetSPlatformerWorld(world_name) != NULL)
		{
			SPlatformerWorld* sw = env2D->GetSPlatformerWorld(world_name);

			if (sw == NULL) return;

			// If the world does not exist then create it
			if (sw->m_World == NULL)
				sw->m_World = new PlatformerWorld(env2D->GetD3D());

			for (int i = 0; i < sw->m_mapNames.size(); i++)
			{
				string map_name = sw->m_mapNames[i];

				// Make sure it is a valid map
				if (map_name == name)
				{
					sw->m_World->LoadMap(map_name, name, env2D->GetGD2D(), InEditor);

					break;
				}
			}
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::CreateWorld(string world_name)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		STopdownWorld* sw = new STopdownWorld();

		sw->sWorldName = world_name;

		TopdownWorld* world = new TopdownWorld(env2D->GetD3D());

		sw->m_World = world;

		env2D->LoadWorld(world_name, sw);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		SPlatformerWorld* sw = new SPlatformerWorld();

		sw->sWorldName = world_name;

		PlatformerWorld* world = new PlatformerWorld(env2D->GetD3D());

		sw->m_World = world;

		env2D->LoadWorld(world_name, sw);
	}
}
//================================================================================================================