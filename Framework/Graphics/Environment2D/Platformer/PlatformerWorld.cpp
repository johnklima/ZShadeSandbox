#include "PlatformerWorld.h"
#include "ZShadeXMLLoader.h"
#include "ZShadeXMLSaver.h"
//==================================================================================================================================
//==================================================================================================================================
PlatformerWorld::PlatformerWorld(D3D* d3d)
:	m_D3DSystem(d3d)
{
	m_physicsWorld.reset(NULL);
	m_sprites_main_init = false;
}
//==================================================================================================================================
PlatformerWorld::~PlatformerWorld()
{
}
//==================================================================================================================================
void PlatformerWorld::UpdateScrollOffset(XMFLOAT2 offset)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->Offset() = offset;
	}
}
//==================================================================================================================================
void PlatformerWorld::UpdateScrollOffset(float x, float y)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->Offset() = XMFLOAT2(x, y);
	}
}
//==================================================================================================================================
void PlatformerWorld::SetBackgroundScreenWidth(int backgroundScreenWidth)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->BackgroundScreenWidth() = backgroundScreenWidth;
	}
}
//==================================================================================================================================
void PlatformerWorld::PlayerMovingDir(bool& moved, int& dirUD, int& dirLR)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//(*iter).second->PlayerMovingDir(moved, dirUD, dirLR);
	}
}
//==================================================================================================================================
XMFLOAT3 PlatformerWorld::MatchingTilePos(float x, float y)
{
	/*map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->MatchingTilePos(x, y);
	}*/
	
	return XMFLOAT3(0, 0, 0);
}
//==================================================================================================================================
void PlatformerWorld::ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//(*iter).second->ClipSpriteToMap(spr, speed, newX, newY, newHBX, newHBY);
	}
}
//==================================================================================================================================
int PlatformerWorld::NumSpritesInMap()
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->NumSprites();
	}

	return 0;
}
//==================================================================================================================================
void PlatformerWorld::RemoveSprite(AISprite* spr)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->RemoveSprite(spr);
	}
}
//==================================================================================================================================
void PlatformerWorld::AddSprite(AISprite* s)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->AddSprite(s);
	}
}
//==================================================================================================================================
void PlatformerWorld::AddSpriteAnimation(string profile_name, GameDirectory2D* gd, AISprite* s)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->AddSpriteAnimations(profile_name, gd->GAME_FOLDER, gd->m_sprites_path, s->ID());
	}
}
//==================================================================================================================================
AISprite* PlatformerWorld::GetSpriteByID(int id)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->GetSpriteByID(id);
	}

	return NULL;
}
//==================================================================================================================================
void PlatformerWorld::GetPlayerPos(float& x, float &y)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		Sprite* player = (*iter).second->GetPlayer();
		x = player->TopLeftPosition().x;
		y = player->TopLeftPosition().y;
	}
}
//==================================================================================================================================
void PlatformerWorld::CreateSpritesText(GameDirectory2D* gd, HWND hwnd)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//(*iter).second->CreateSpritesText(gd, hwnd);
	}
}
//==================================================================================================================================
void PlatformerWorld::LoadSpritesMain()
{
	//Using this once
	if (!m_sprites_main_init)
	{
		m_sprites_main_init = true;
		map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
		if (iter != m_world_maps.end())
		{
			(*iter).second->LoadSpritesMain();
		}
	}
}
//==================================================================================================================================
void PlatformerWorld::InitAISprites(GameDirectory2D* gd)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//(*iter).second->InitAISprite(gd);
	}
}
//==================================================================================================================================
void PlatformerWorld::RunSpritesMain()
{
	//map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	//if (iter != m_world_maps.end())
	//{
	//	//Load the main function for each sprite
	//	(*iter).second->LoadSpritesMain();
	//}
}
//==================================================================================================================================
void PlatformerWorld::InitPhysicsWorld()
{
	// Initialize the physics world for the map
	m_physicsWorld.reset(new PhysicsWorld2D(m_D3DSystem, GetMap(m_map_to_render)));
	m_physicsWorld->BuildTileBodies();
}
//==================================================================================================================================
void PlatformerWorld::SetMapToRender(string p_map_to_render)
{
	m_map_to_render = p_map_to_render;
	
	InitPhysicsWorld();
}
//==================================================================================================================================
void PlatformerWorld::NewMap(string map_name, GameDirectory2D* gd, bool InEditor)
{
	PlatformerMap* map = new PlatformerMap(m_D3DSystem, gd);

	map->InEditor() = InEditor;

	string path = gd->m_required_textures;
	
	// Just for testing
	int width = m_D3DSystem->GetEngineOptions()->SECTION_ROW_SIZE;
	int height = m_D3DSystem->GetEngineOptions()->SECTION_COL_SIZE;
	
	map->Initialize(width, height);

	m_world_maps.insert(make_pair(map_name, map));
}
//==================================================================================================================================
void PlatformerWorld::LoadMap(string& map_name, string filename, GameDirectory2D* gd, bool InEditor)
{
	PlatformerMap* map;

	string path = gd->m_xml_maps_path;
	path.append("\\").append(filename).append(".xml");

	ZShadeXMLLoader::LoadMapXML(map_name, path, gd, m_D3DSystem, map, InEditor);

	map->InEditor() = InEditor;

	m_world_maps.insert( make_pair(map_name, map) );
}
//==================================================================================================================================
void PlatformerWorld::RemoveMap(string mapName)
{
	// Get the iterator to remove
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(mapName);
	
	// Clear the rendered map
	m_map_to_render = "";

	// Remove the map
	m_world_maps.erase(mapName);
}
//==================================================================================================================================
PlatformerMap* PlatformerWorld::GetMap(string mapName)
{
	return m_world_maps[mapName];
}
//==================================================================================================================================
void PlatformerWorld::Update(float dt, Keyboard* keyboard, HWND hwnd)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		if (m_physicsWorld.get() != NULL)
		{
			if ((*iter).second->InEditor())
				m_physicsWorld->IsGravityOn() = false;
			
			m_physicsWorld->Update(dt, keyboard);
		}
	}
}
//==================================================================================================================================
bool PlatformerWorld::Render(Camera* camera)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->Render(camera);
		
		if (m_physicsWorld.get() != NULL)
		{
			//m_physicsWorld->RenderContactStats();
		}
	}

	return true;
}
//==================================================================================================================================