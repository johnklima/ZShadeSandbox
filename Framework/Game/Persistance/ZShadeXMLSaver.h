//===============================================================================================================================
// ZShadeXMLSaver.h
//
//===============================================================================================================================
// History
//
// -Created 5/20/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __ZSHADEXMLSAVER_H
#define __ZSHADEXMLSAVER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "ShaderMaterial.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
#include "TopdownWorld.h"
#include "PlatformerWorld.h"
#include "GameDirectory.h"
#include "MenuSystem.h"
#include "PlayerInventory2D.h"
#include "HUD.h"
#include "tinyxml2.h"
#include "Polygon.h"
#include <string>
using namespace std;
using namespace tinyxml2;

//===============================================================================================================================
//===============================================================================================================================
class ZShadeXMLSaver
{
public:
	
	static bool SaveTemplatePolyXML(string filename, ZShadeSandboxMath::Polygon* poly);
	static bool SaveMaterialXML(string filename, ZShadeSandboxLighting::ShaderMaterial* material);
	static bool SaveInventoryXML(string filename, PlayerInventory2D* inventory);
	static bool SaveHUDXML(string filename, HUD* hs);
	static bool SaveWorldXML(STopdownWorld* sworld, string filename);
	static bool SaveWorldXML(SPlatformerWorld* sworld, string filename);
	static bool SaveSpritesXML(TopdownMap* map, string spriteMapPath, string spriteMapfilename);
	static bool SaveSpritesXML(PlatformerMap* map, string spriteMapPath, string spriteMapfilename);
	static bool SaveMapXML(TopdownMap* map, string filename, GameDirectory2D* gd);
	static bool SaveMapXML(PlatformerMap* map, string filename, GameDirectory2D* gd);
	static bool SaveMenuXML(string menufilename, MenuSystem* ms);
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__ZSHADEXMLSAVER_H