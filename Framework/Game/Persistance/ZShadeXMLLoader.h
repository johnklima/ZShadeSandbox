//===============================================================================================================================
// ZShadeXMLLoader.h
//
//===============================================================================================================================
// History
//
// -Created 5/20/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __ZSHADEXMLLOADER_H
#define __ZSHADEXMLLOADER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "HUD.h"
#include "Sprite.h"
#include "MenuSystem.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
#include "TopdownWorld.h"
#include "PlatformerWorld.h"
#include "PlayerInventory2D.h"
#include "tinyxml2.h"
#include "Polygon.h"
#include <string>
using namespace std;
using namespace tinyxml2;

//===============================================================================================================================
//===============================================================================================================================

class ZShadeXMLLoader
{
public:
	
	static bool LoadTemplatePolyXML(string filename, ZShadeSandboxMath::Polygon*& poly);
	static bool LoadMaterialXML(string basePath, string filename, D3D* d3d);
	static bool LoadInventoryXML(string filename, GameDirectory2D* gd, EngineOptions* eo, PlayerInventory2D*& inventory);
	static bool LoadWorldXML(string worldname, STopdownWorld*& sworld);
	static bool LoadWorldXML(string worldname, SPlatformerWorld*& sworld);
	static bool LoadMapXML(string& mapname, string filename, GameDirectory2D* gd, D3D* d3d, TopdownMap*& map, bool inEditor);
	static bool LoadMapXML(string& mapname, string filename, GameDirectory2D* gd, D3D* d3d, PlatformerMap*& map, bool inEditor);
	static bool LoadMap3DXML() { return true; } // Not ready for 3D map yet
	static bool LoadMenuXML(string menufilename, GameDirectory2D* gd, EngineOptions* eo, D3D* d3d, MenuSystem*& ms);
	static bool LoadSpritesXML(string spriteMapfilename, GameDirectory2D* gd, D3D* d3d, TopdownMap*& map);
	static bool LoadSpritesXML(string spriteMapfilename, GameDirectory2D* gd, D3D* d3d, PlatformerMap*& map);
	static bool LoadHUDXML(string filename, GameDirectory2D* gd, EngineOptions* eo, D3D* d3d, HUD*& hs);
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__ZSHADEXMLLOADER_H