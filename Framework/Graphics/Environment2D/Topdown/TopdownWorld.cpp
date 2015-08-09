#include "TopdownWorld.h"
#include "ZShadeXMLLoader.h"
#include "ZShadeXMLSaver.h"
#include "Environment2D.h"
#include "Environment.h"
//=================================================================================================================
TopdownWorld::TopdownWorld(D3D* d3d)
:	m_D3DSystem(d3d)
{
	m_physicsWorld.reset(NULL);
	m_sprites_main_init = false;
}
//=================================================================================================================
TopdownWorld::~TopdownWorld()
{
}
//=================================================================================================================
void TopdownWorld::UpdateScrollOffset(XMFLOAT2 offset)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->Offset() = offset;
	}
}
//=================================================================================================================
void TopdownWorld::ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//(*iter).second->ClipSpriteToMap(spr, speed, newX, newY, newHBX, newHBY);
	}
}
//=================================================================================================================
int TopdownWorld::NumSpritesInMap()
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->NumSprites();
	}

	return 0;
}
//=================================================================================================================
void TopdownWorld::RemoveSprite(AISprite* spr)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->RemoveSprite(spr);
	}
}
//=================================================================================================================
void TopdownWorld::AddSprite(AISprite* s)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->AddSprite(s);
	}
}
//=================================================================================================================
void TopdownWorld::AddSpriteAnimation(string profile_name, GameDirectory2D* gd, AISprite* s)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->AddSpriteAnimations(profile_name, gd->GAME_FOLDER, gd->m_sprites_path, s->ID());
	}
}
//=================================================================================================================
AISprite* TopdownWorld::GetSpriteByID(int id)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->GetSpriteByID(id);
	}

	return NULL;
}
//=================================================================================================================
void TopdownWorld::GetPlayerPos(float& x, float &y)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		AISprite* player = (*iter).second->GetPlayer();
		x = player->TopLeftPosition().x;
		y = player->TopLeftPosition().y;
	}
}
//=================================================================================================================
/*void TopdownWorld::CreateSpritesText(GameDirectory2D* gd, HWND hwnd)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->CreateSpritesText(gd, hwnd);
	}
}*/
//=================================================================================================================
void TopdownWorld::InitAISprites(GameDirectory2D* gd)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//(*iter).second->InitAISprite(gd);
	}
}
//=================================================================================================================
void TopdownWorld::LoadSpritesMain()
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//Load the main function for each sprite
		(*iter).second->LoadSpritesMain();
	}
}
//=================================================================================================================
void TopdownWorld::SetMapToRender(string p_map_to_render)
{
	m_map_to_render = p_map_to_render;

	InitPhysicsWorld();
}
//=================================================================================================================
void TopdownWorld::NewMap(string map_name, GameDirectory2D* gd, Map2DType mt, bool InEditor)
{
	TopdownMap* map = new TopdownMap(m_D3DSystem, mt, gd);

	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	map->Initialize(env2D->GetEngineOptions()->SECTION_ROW_SIZE);

	map->InEditor() = InEditor;
	
	m_world_maps.insert(make_pair(map_name, map));
}
//=================================================================================================================
void TopdownWorld::LoadMap(string& map_name, string filename, GameDirectory2D* gd, bool InEditor)
{
	TopdownMap* map;

	string path = gd->m_xml_maps_path;
	path.append("\\").append(filename).append(".xml");

	ZShadeXMLLoader::LoadMapXML(map_name, path, gd, m_D3DSystem, map, InEditor);
	
	if (map == 0) return;

	map->InEditor() = InEditor;

	m_world_maps.insert( make_pair(map_name, map) );
}
//=================================================================================================================
void TopdownWorld::RemoveMap(string activeMapName)
{
	// Get the iterator to remove
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(activeMapName);
	
	// Clear the rendered map
	m_map_to_render = "";

	// Remove the map
	m_world_maps.erase(activeMapName);
}
//=================================================================================================================
void TopdownWorld::UpdateMapName(string mapName)
{
	// Clone the current rendered map
	TopdownMap* newMap = 0;
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		newMap = (*iter).second->Clone();
	}

	// remove the current rendered map
	RemoveMap(m_map_to_render);

	// add the clone with the new map name
	m_world_maps.insert(make_pair(mapName, newMap));
}
//=================================================================================================================
TopdownMap* TopdownWorld::GetMap(string mapName)
{
	return m_world_maps[mapName];
}
//=================================================================================================================
void TopdownWorld::InitPhysicsWorld()
{
	// Initialize the physics world for the map
	m_physicsWorld.reset(new PhysicsWorld2D(m_D3DSystem, GetMap(m_map_to_render)));
	m_physicsWorld->BuildTileBodies();
}
//=================================================================================================================
void TopdownWorld::Update(float dt, Keyboard* keyboard)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		if (m_physicsWorld.get() != NULL)
		{
			m_physicsWorld->Update(dt, keyboard);
		}
		else
		{
			InitPhysicsWorld();
			m_physicsWorld->Update(dt, keyboard);
		}
	}
}
//=================================================================================================================
bool TopdownWorld::RenderSectionPicker(Camera* camera)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->RenderSectionPicker(camera);
	}

	return true;
}
//=================================================================================================================
bool TopdownWorld::Render(Camera* camera)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
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
//=================================================================================================================
