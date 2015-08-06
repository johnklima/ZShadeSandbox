#include "ZShadeXMLSaver.h"
#include "tinyxml2.h"
#include "CGlobal.h"
#include "FogData.h"
#include <sstream>
#include <CommCtrl.h>
#include "XMLParser.h"
#include "Vertex.h"
using namespace tinyxml2;
//===============================================================================================================================
//===============================================================================================================================
bool ZShadeXMLSaver::SaveTemplatePolyXML(string filename, ZShadeSandboxMath::Polygon* poly)
{
	XMLParser xmlParser;

	xmlParser.CreateRootNode("TemplatePoly");

	xmlParser.InsertElement("Name", poly->Name());
	xmlParser.InsertElement("VertexCount", poly->GetVertexCount());

	ArrayElementXMLSaver vertexSaver(xmlParser.Document(), xmlParser.Element());
	vertexSaver.BeginNewArrayElement("Vertices");
	for (int i = 0; i < poly->GetVertexCount(); i++)
	{
		vertexSaver.CreateElement("Vertex");
		vertexSaver.SetAttribute("id", i);
		vertexSaver.InsertElement("Pos", ZShadeSandboxGlobal::Convert::ConvertFloat3ToString(poly->GetVertexTex(i).position));
		vertexSaver.InsertElement("UV", ZShadeSandboxGlobal::Convert::ConvertFloat2ToString(poly->GetVertexTex(i).texture));
		
		vertexSaver.EndCurrentElement();
	}
	vertexSaver.EndArrayElement();

	xmlParser.InsertElement("IndexCount", poly->GetIndexCount());

	ArrayElementXMLSaver indexSaver(xmlParser.Document(), xmlParser.Element());
	indexSaver.BeginNewArrayElement("Indices");
	for (int i = 0; i < poly->GetIndexCount(); i++)
	{
		indexSaver.CreateElement("Index");
		indexSaver.SetAttribute("id", i);
		indexSaver.InsertElement("Value", ZShadeSandboxGlobal::Convert::ConvertToString<UINT>(poly->GetIndex(i)));
		
		indexSaver.EndCurrentElement();
	}
	indexSaver.EndArrayElement();

	xmlParser.InsertElement("MaterialName", poly->MaterialName());
	xmlParser.InsertElement("Rotation", ZShadeSandboxGlobal::Convert::ConvertFloat3ToString(poly->Rotation()));
	xmlParser.InsertElement("Scale", ZShadeSandboxGlobal::Convert::ConvertFloat3ToString(poly->Scale()));

	xmlParser.Save(filename);

	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveMaterialXML(string filename, ZShadeSandboxLighting::ShaderMaterial* material)
{
	XMLParser xmlParser;

	xmlParser.CreateRootNode("Material");
	
	xmlParser.InsertElement("Name", material->sMaterialName);
	xmlParser.InsertElement("EnableShadowMap", material->bHasShadowMap);
	xmlParser.InsertElement("EnableSSAO", material->bHasSSAOMap);
	xmlParser.InsertElement("EnableTransparency", material->bHasTransparency);
	xmlParser.InsertElement("EnableLighting", material->bEnableLighting);
	xmlParser.InsertElement("DetailBrightness", material->fDetailBrightness);
	xmlParser.InsertElement("AlphaToCoverage", material->vAlphaToCoverageValue);
	xmlParser.InsertElement("AmbientColor", material->vAmbientColor);
	xmlParser.InsertElement("DiffuseColor", material->vDiffuseColor);
	xmlParser.InsertElement("SpecularColor", material->vSpecularColor);
	xmlParser.InsertElement("EmissiveColor", material->vEmissiveColor);
	xmlParser.InsertElement("SpecularPower", material->fSpecularPower);
	xmlParser.InsertElement("SpecularIntensity", material->fSpecularIntensity);
	xmlParser.InsertElement("Emissivity", material->fEmissivity);
	xmlParser.InsertElement("Reflectivity", material->fReflectivity);
	xmlParser.InsertElement("Transmissivity", material->fTransmissivity);
	xmlParser.InsertElement("TransmissionFilter", material->vTransmissionFilter);
	xmlParser.InsertElement("Alpha", material->fAlpha);
	xmlParser.InsertElement("RefractionIndex", material->fRefractionIndex);
	xmlParser.InsertElement("NoDistTessFactor", material->fNoDistTessFactor);
	xmlParser.InsertElement("MinTessDist", material->fMinTessDist);
	xmlParser.InsertElement("MaxTessDist", material->fMaxTessDist);
	xmlParser.InsertElement("MinTessFactor", material->fMinTessFactor);
	xmlParser.InsertElement("MaxTessFactor", material->fMaxTessFactor);
	xmlParser.InsertElement("EnableDistTess", material->bEnableDistTess);
	xmlParser.InsertElement("IlluminationModel", material->iIlluminationModel);
	
	ArrayElementXMLSaver diffuseArraySaver(xmlParser.Document(), xmlParser.Element());
	diffuseArraySaver.BeginNewArrayElement("DiffuseArray");
	for (int i = 0; i < material->GetDiffuseArrayTextureNames().size(); i++)
	{
		string tex_name = material->GetDiffuseArrayTextureNames()[i];
		
		diffuseArraySaver.CreateElement("Texture");
		diffuseArraySaver.SetAttribute("id", i);
		diffuseArraySaver.InsertElement("TexName", tex_name);
		
		diffuseArraySaver.EndCurrentElement();
	}
	diffuseArraySaver.EndArrayElement();
	
	for (int i = 0; i < material->TextureCount(); i++)
	{
		Texture* tex = material->GetTexture(i);
		
		switch (material->GetMaterialTextureType(i))
		{
			case ZShadeSandboxLighting::EMaterialTextureType::eDiffuse:
			{
				xmlParser.InsertElement("DiffuseTexture", tex->GetTextureName());
			}
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eAmbient:
			{
				xmlParser.InsertElement("AmbientTexture", tex->GetTextureName());
			}
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eSpecular:
			{
				xmlParser.InsertElement("SpecularTexture", tex->GetTextureName());
			}
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eEmissive:
			{
				xmlParser.InsertElement("EmissiveTexture", tex->GetTextureName());
			}
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eDetail:
			{
				xmlParser.InsertElement("DetailTexture", tex->GetTextureName());
			}
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eNormal:
			{
				xmlParser.InsertElement("NormalTexture", tex->GetTextureName());
			}
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eBlend:
			{
				xmlParser.InsertElement("BlendTexture", tex->GetTextureName());
			}
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eAlpha:
			{
				xmlParser.InsertElement("AlphaTexture", tex->GetTextureName());
			}
			break;
		}
	}
	
	xmlParser.Save(filename);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveInventoryXML(string filename, PlayerInventory2D* inventory)
{
	XMLParser xmlParser;

	xmlParser.CreateRootNode("Inventory");
	
	xmlParser.InsertElement("BackgroundImageName", inventory->BackgroundImageName());
	xmlParser.InsertElement("ItemSize", inventory->ItemSize());
	xmlParser.InsertElement("ItemRowSizeRegular", inventory->MaxItemsXRegular());
	xmlParser.InsertElement("ItemColSizeRegular", inventory->MaxItemsYRegular());
	xmlParser.InsertElement("ItemRowSizeMagic", inventory->MaxItemsXMagic());
	xmlParser.InsertElement("ItemColSizeMagic", inventory->MaxItemsYMagic());
	xmlParser.InsertElement("ItemRowPaddingRegular", inventory->XPaddingRegular());
	xmlParser.InsertElement("ItemColPaddingRegular", inventory->YPaddingRegular());
	xmlParser.InsertElement("ItemRowPaddingMagic", inventory->XPaddingMagic());
	xmlParser.InsertElement("ItemColPaddingMagic", inventory->YPaddingMagic());
	xmlParser.InsertElement("StartLocationRegular", inventory->GridStartLocationRegular());
	xmlParser.InsertElement("StartLocationMagic", inventory->GridStartLocationMagic());
	
	ArrayElementXMLSaver itemRegularSaver(xmlParser.Document(), xmlParser.Element());
	itemRegularSaver.BeginNewArrayElement("RegularItems");
	for (int i = 0; i < inventory->MaxAllowedItemsRegular(); i++)
	{
		itemRegularSaver.CreateElement("Item");
		itemRegularSaver.SetAttribute("id", i);
		
		itemRegularSaver.SetAttribute("image", inventory->ItemRegularImageName(i).c_str());
		
		itemRegularSaver.InsertElement("Message", inventory->ItemRegularInventoryMessage(i).c_str());
		
		itemRegularSaver.EndCurrentElement();
	}
	itemRegularSaver.EndArrayElement();
	
	ArrayElementXMLSaver itemMagicSaver(xmlParser.Document(), xmlParser.Element());
	itemMagicSaver.BeginNewArrayElement("MagicItems");
	for (int i = 0; i < inventory->MaxAllowedItemsMagic(); i++)
	{
		itemMagicSaver.CreateElement("Item");
		itemMagicSaver.SetAttribute("id", i);
		
		itemMagicSaver.SetAttribute("image", inventory->ItemMagicImageName(i).c_str());
		
		itemMagicSaver.InsertElement("Message", inventory->ItemMagicInventoryMessage(i).c_str());
		
		itemMagicSaver.EndCurrentElement();
	}
	itemMagicSaver.EndArrayElement();
	
	xmlParser.Save(filename);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveHUDXML(string filename, HUD* hs)
{
	XMLParser xmlParser;

	xmlParser.CreateRootNode("HUD");
	
	HUD* hud = hs;
	int hud_x = hud->Width();
	int hud_y = hud->Height();
	stringstream ssx;
	ssx << hud_x;
	stringstream ssy;
	ssy << hud_y;
	string hud_size;
	hud_size.append(ssx.str());
	hud_size.append("x");
	hud_size.append(ssy.str());
	xmlParser.InsertElement("hud_size", hud_size);
	
	ArrayElementXMLSaver imagesSaver(xmlParser.Document(), xmlParser.Element());
	imagesSaver.BeginNewArrayElement("Images");
	for (int i = 0; i < hud->GetImages().size(); i++)
	{
		ZShadeSandboxGraphics::Image* im = hud->GetImages()[i];
		
		imagesSaver.CreateElement("Image");
		imagesSaver.SetAttribute("id", i);

		imagesSaver.InsertElement("image_x", im->XPos());
		imagesSaver.InsertElement("image_y", im->YPos());
		imagesSaver.InsertElement("image_width", im->Width());
		imagesSaver.InsertElement("image_height", im->Height());
		imagesSaver.InsertElement("image_name", im->GetName());
		
		imagesSaver.EndCurrentElement();
	}
	imagesSaver.EndArrayElement();
	
	ArrayElementXMLSaver textsSaver(xmlParser.Document(), xmlParser.Element());
	textsSaver.BeginNewArrayElement("Texts");
	for (int i = 0; i < hud->GetTexts().size(); i++)
	{
		ZShadeSandboxGraphics::Text* t = hud->GetTexts()[i];
		
		textsSaver.CreateElement("Text");
		textsSaver.SetAttribute("id", i);
		
		textsSaver.InsertElement("text_x", t->GetXPos());
		textsSaver.InsertElement("text_y", t->GetYPos());
		textsSaver.InsertElement("text_width", t->Width());
		textsSaver.InsertElement("text_height", t->Height());
		textsSaver.InsertElement("text", t->TextName().toString());
		textsSaver.InsertElement("text_font", t->FontSize());
		textsSaver.InsertElement("text_color", t->TextColor());
		textsSaver.InsertElement("shadow", t->HasShadow());
		textsSaver.InsertElement("optional_background", "NONE");
		
		textsSaver.EndCurrentElement();
	}
	textsSaver.EndArrayElement();
	
	xmlParser.Save(filename);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveWorldXML(STopdownWorld* sworld, string filename)
{
	XMLParser xmlParser;

	xmlParser.CreateRootNode("World");
	
	xmlParser.InsertElement("name", sworld->sWorldName);
	
	ArrayElementXMLSaver mapsSaver(xmlParser.Document(), xmlParser.Element());
	mapsSaver.BeginNewArrayElement("Maps");
	for (int i = 0; i < sworld->m_mapNames.size(); i++)
	{
		string map_name = sworld->m_mapNames[i];
		
		mapsSaver.CreateElement("Map");
		mapsSaver.SetAttribute("id", i);
		
		mapsSaver.InsertElement("map_name", map_name);
		
		mapsSaver.EndCurrentElement();
	}
	mapsSaver.EndArrayElement();
	
	xmlParser.Save(filename);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveWorldXML(SPlatformerWorld* sworld, string filename)
{
	XMLParser xmlParser;

	xmlParser.CreateRootNode("World");
	
	xmlParser.InsertElement("name", sworld->sWorldName);
	
	ArrayElementXMLSaver mapsSaver(xmlParser.Document(), xmlParser.Element());
	mapsSaver.BeginNewArrayElement("Maps");
	for (int i = 0; i < sworld->m_mapNames.size(); i++)
	{
		string map_name = sworld->m_mapNames[i];
		
		mapsSaver.CreateElement("Map");
		mapsSaver.SetAttribute("id", i);
		
		mapsSaver.InsertElement("map_name", map_name);
		
		mapsSaver.EndCurrentElement();
	}
	mapsSaver.EndArrayElement();
	
	xmlParser.Save(filename);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveSpritesXML(TopdownMap* map, string spriteMapPath, string spriteMapfilename)
{
	XMLParser xmlParser;

	xmlParser.CreateRootNode("Sprites");
	
	AISprite* spr;
	ArrayElementXMLSaver spriteSaver(xmlParser.Document(), xmlParser.Element());
	for (int i = 0; i < map->NumSprites(); i++)
	{
		spr = map->GetSprite(i);
		
		if (spr == 0) continue;
		
		spriteSaver.CreateElement("Sprite");
		spriteSaver.SetAttribute("id", i);

		float hb_x = spr->Body().x;
		float hb_y = spr->Body().y;
		int hb_width = spr->Body().width;
		int hb_height = spr->Body().height;
		
		spriteSaver.InsertElement("player", spr->IsPlayer());
		spriteSaver.InsertElement("x", spr->TopLeftPosition().x);
		spriteSaver.InsertElement("y", spr->TopLeftPosition().y);
		spriteSaver.InsertElement("width", spr->Width());
		spriteSaver.InsertElement("height", spr->Height());
		spriteSaver.InsertElement("hbox_x", hb_x);
		spriteSaver.InsertElement("hbox_y", hb_y);
		spriteSaver.InsertElement("hbox_width", hb_width);
		spriteSaver.InsertElement("hbox_height", hb_height);
		spriteSaver.InsertElement("baseimage", spr->BaseTextureFilename());
		spriteSaver.InsertElement("vision", spr->Vision());
		spriteSaver.InsertElement("depth", spr->Depth());
		spriteSaver.InsertElement("hard", spr->IsHard());
		spriteSaver.InsertElement("nohit", spr->IsNoHit());
		spriteSaver.InsertElement("animation", spr->AnimationProfileName());
		spriteSaver.InsertElement("default_seq", spr->DefaultSequence());
		spriteSaver.InsertElement("anim_speed", spr->AnimationSpeed());
		spriteSaver.InsertElement("hp", spr->HP());
		spriteSaver.InsertElement("hp_max", spr->HPMax());
		spriteSaver.InsertElement("mana", spr->Mana());
		spriteSaver.InsertElement("gold", spr->Gold());
		spriteSaver.InsertElement("strength", spr->Strength());
		spriteSaver.InsertElement("speed", spr->Speed());
		spriteSaver.InsertElement("defense", spr->Defense());
		spriteSaver.InsertElement("experience", spr->Experience());
		spriteSaver.InsertElement("level", spr->Level());
		spriteSaver.InsertElement("script", spr->ScriptName());
		spriteSaver.InsertElement("touch", spr->IsTouch());
		spriteSaver.InsertElement("background", spr->IsBackgroundSprite());
		spriteSaver.InsertElement("entity", spr->IsEntitySprite());
		spriteSaver.InsertElement("invisible", spr->IsInvisibleSprite());
		spriteSaver.InsertElement("static", (spr->EPhysicsType() == STATIC) ? true : false);
		spriteSaver.InsertElement("name", spr->Name());
		spriteSaver.InsertElement("collide_msg", spr->CollideMsg());
		spriteSaver.InsertElement("inventory_msg", spr->InventoryMessage());
		spriteSaver.InsertElement("always_seen_by_player", spr->AlwaysSeenByPlayer());
		spriteSaver.InsertElement("teleport", spr->CanTeleport());
		spriteSaver.InsertElement("teleport_map", spr->TeleportMapName());
		spriteSaver.InsertElement("teleport_map_x", spr->TeleportX());
		spriteSaver.InsertElement("teleport_map_y", spr->TeleportY());
		
		spriteSaver.EndCurrentElement();
	}
	spriteSaver.EndArrayElement();
	
	xmlParser.Save(spriteMapfilename);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveSpritesXML(PlatformerMap* map, string spriteMapPath, string spriteMapfilename)
{
	XMLParser xmlParser;

	xmlParser.CreateRootNode("Sprites");
	
	AISprite* spr;
	ArrayElementXMLSaver spriteSaver(xmlParser.Document(), xmlParser.Element());
	for (int i = 0; i < map->NumSprites(); i++)
	{
		spr = map->GetSprite(i);
		
		if (spr == 0) continue;
		
		spriteSaver.CreateElement("Sprite");
		spriteSaver.SetAttribute("id", i);
		
		float hb_x = spr->Body().x;
		float hb_y = spr->Body().y;
		int hb_width = spr->Body().width;
		int hb_height = spr->Body().height;
		
		spriteSaver.InsertElement("player", spr->IsPlayer());
		spriteSaver.InsertElement("x", spr->TopLeftPosition().x);
		spriteSaver.InsertElement("y", spr->TopLeftPosition().y);
		spriteSaver.InsertElement("width", spr->Width());
		spriteSaver.InsertElement("height", spr->Height());
		spriteSaver.InsertElement("hbox_x", hb_x);
		spriteSaver.InsertElement("hbox_y", hb_y);
		spriteSaver.InsertElement("hbox_width", hb_width);
		spriteSaver.InsertElement("hbox_height", hb_height);
		spriteSaver.InsertElement("baseimage", spr->BaseTextureFilename());
		spriteSaver.InsertElement("hard", spr->IsHard());
		spriteSaver.InsertElement("nohit", spr->IsNoHit());
		spriteSaver.InsertElement("animation", spr->AnimationProfileName());
		spriteSaver.InsertElement("default_seq", spr->DefaultSequence());
		spriteSaver.InsertElement("anim_speed", spr->AnimationSpeed());
		spriteSaver.InsertElement("hp", spr->HP());
		spriteSaver.InsertElement("hp_max", spr->HPMax());
		spriteSaver.InsertElement("mana", spr->Mana());
		spriteSaver.InsertElement("gold", spr->Gold());
		spriteSaver.InsertElement("strength", spr->Strength());
		spriteSaver.InsertElement("speed", spr->Speed());
		spriteSaver.InsertElement("defense", spr->Defense());
		spriteSaver.InsertElement("experience", spr->Experience());
		spriteSaver.InsertElement("level", spr->Level());
		spriteSaver.InsertElement("script", spr->ScriptName());
		spriteSaver.InsertElement("touch", spr->IsTouch());
		spriteSaver.InsertElement("background", spr->IsBackgroundSprite());
		spriteSaver.InsertElement("entity", spr->IsEntitySprite());
		spriteSaver.InsertElement("invisible", spr->IsInvisibleSprite());
		spriteSaver.InsertElement("static", (spr->EPhysicsType() == STATIC) ? true : false);
		spriteSaver.InsertElement("name", spr->Name());
		spriteSaver.InsertElement("collide_msg", spr->CollideMsg());
		spriteSaver.InsertElement("inventory_msg", spr->InventoryMessage());
		spriteSaver.InsertElement("always_seen_by_player", spr->AlwaysSeenByPlayer());
		spriteSaver.InsertElement("teleport", spr->CanTeleport());
		spriteSaver.InsertElement("teleport_map", spr->TeleportMapName());
		spriteSaver.InsertElement("teleport_map_x", spr->TeleportX());
		spriteSaver.InsertElement("teleport_map_y", spr->TeleportY());
		
		spriteSaver.EndCurrentElement();
	}
	spriteSaver.EndArrayElement();
	
	BetterString str(spriteMapPath);
	str += "\\";
	str += spriteMapfilename;

	xmlParser.Save(str);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveMapXML(TopdownMap* map, string filename, GameDirectory2D* gd)
{
	XMLParser xmlParser;

	xmlParser.CreateRootNode("Map");

	BetterString str(filename);

	BetterString path = gd->m_xml_sprites_path;

	BetterString map_name = "";
	int indexI = 0;
	for (int i = 0; i < str.size() - 3; i++)
	{
		if (str[i] == 'M' && str[i + 1] == 'a' && str[i + 2] == 'p' && str[i + 3] == 's')
		{
			indexI = i + 5;
			break;
		}
	}

	int j = 0;
	for (int i = indexI; i < str.size(); i++)
	{
		if (str[i] == '.') break;
		map_name.insert_char(str[i], j++);
	}

	string xml_sprite_str = map_name.toString();
	xml_sprite_str += "Sprites.xml";

	xmlParser.InsertElement("map_name", map->MapName());
	xmlParser.InsertElement("xml_sprite", xml_sprite_str);

	string mapType;
	if (map->MapType() == Map2DType::REGULAR)
		mapType = "regular";
	else if (map->MapType() == Map2DType::ISOMETRIC)
		mapType = "isometric";

	xmlParser.InsertElement("map_type", mapType);
	xmlParser.InsertElement("row_size", map->NumSectionsPerRow());
	xmlParser.InsertElement("col_size", map->NumSectionsPerRow());
	xmlParser.InsertElement("vision", map->Vision());
	xmlParser.InsertElement("music", map->MusicName());
	
	xmlParser.SetAttribute("Fow", "enabled", map->FOW());
	xmlParser.SetAttribute("Fow", "radius", map->FOWRadius());
	xmlParser.SetAttribute("Fow", "flashlight", map->FOWFlashlight());
	
	ArrayElementXMLSaver tilesSaver(xmlParser.Document(), xmlParser.Element());
	tilesSaver.BeginNewArrayElement("Tiles");
	SmartArray2D<TopdownTile*> tiles = map->Tiles();
	int tile_id = 0;
	for (int y = 0; y < map->Height(); y++)
	{
		for (int x = 0; x < map->Width(); x++)
		{
			TopdownTile* tile = tiles(x, y);

			if (tile == NULL) continue;
			if (tile->Mesh() == NULL) continue;

			tilesSaver.CreateElement("TopdownTile");
			tilesSaver.SetAttribute("id", tile_id);
			
			tilesSaver.InsertElement("x", tile->TopLeftPosition().x);
			tilesSaver.InsertElement("y", tile->TopLeftPosition().y);
			
			if (tile->FogBit() == fow_n_non)
				tilesSaver.SetAttribute("image", tile->GetTileMeshTextureName().c_str());
			else
				tilesSaver.SetAttribute("image", tile->GetTileBaseTextureName().c_str());
			
			tilesSaver.InsertElement("animation", tile->AnimationProfileName().c_str());
			tilesSaver.InsertElement("anim_speed", tile->AnimationSpeed());
			tilesSaver.InsertElement("hard", tile->Hard());

			tilesSaver.EndCurrentElement();

			tile_id++;
		}
	}
	tilesSaver.EndArrayElement();

	xmlParser.Save(filename);

	//Write the sprites to the xml file
	SaveSpritesXML(map, gd->m_xml_sprites_path, xml_sprite_str);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveMapXML(PlatformerMap* map, string filename, GameDirectory2D* gd)
{
	XMLParser xmlParser;

	xmlParser.CreateRootNode("Map");

	BetterString str(filename);

	BetterString path = gd->m_xml_sprites_path;

	BetterString map_name = "";
	int indexI = 0;
	for (int i = 0; i < str.size() - 3; i++)
	{
		if (str[i] == 'M' && str[i + 1] == 'a' && str[i + 2] == 'p' && str[i + 3] == 's')
		{
			indexI = i + 5;
			break;
		}
	}

	int j = 0;
	for (int i = indexI; i < str.size(); i++)
	{
		if (str[i] == '.') break;
		map_name.insert_char(str[i], j++);
	}

	string xml_sprite_str = map_name.toString();
	xml_sprite_str += "Sprites.xml";

	xmlParser.InsertElement("map_name", map->MapName());
	xmlParser.InsertElement("xml_sprite", xml_sprite_str);
	xmlParser.InsertElement("music", map->MusicName());
	xmlParser.InsertElement("width", map->Width());
	xmlParser.InsertElement("height", map->Height());

	ArrayElementXMLSaver tilesSaver(xmlParser.Document(), xmlParser.Element());
	tilesSaver.BeginNewArrayElement("Tiles");
	SmartArray2D<PlatformerTile*> tiles = map->Tiles();
	int tile_id = 0;
	for (int y = 0; y < map->Height(); y++)
	{
		for (int x = 0; x < map->Width(); x++)
		{
			PlatformerTile* tile = tiles(x, y);

			if (tile == NULL) continue;
			if (tile->GetMesh() == NULL) continue;

			tilesSaver.CreateElement("TopdownTile");
			tilesSaver.SetAttribute("id", tile_id);

			tilesSaver.SetAttribute("image", tile->GetTileTextureName().c_str());

			tilesSaver.InsertElement("x", tile->TopLeftPosition().x);
			tilesSaver.InsertElement("y", tile->TopLeftPosition().y);
			tilesSaver.InsertElement("animation", tile->AnimationProfileName().c_str());
			tilesSaver.InsertElement("anim_speed", tile->AnimationSpeed());
			tilesSaver.InsertElement("hard", tile->Hard());
			tilesSaver.InsertElement("invisible", tile->TileInvisible());

			string physics_type = "";
			string surface_type = "";

			switch (tile->EPhysicsType())
			{
				case PhysicsType::STATIC:
				{
					physics_type = "static";
				}
				break;
				case PhysicsType::DYNAMIC:
				{
					physics_type = "dynamic";
				}
				break;
			}

			switch (tile->SurfaceType())
			{
				case ESurfaceType::Type::eNormal:
				{
					surface_type = "normal";
				}
				break;
				case ESurfaceType::Type::eFrictionless:
				{
					surface_type = "frictionless";
				}
				break;
				case ESurfaceType::Type::eSticky:
				{
					surface_type = "sticky";
				}
				break;
				case ESurfaceType::Type::eLadder:
				{
					surface_type = "ladder";
				}
				break;
				case ESurfaceType::Type::eJumpThrough:
				{
					surface_type = "jumpthrough";
				}
				break;
			}

			tilesSaver.InsertElement("physics_type", physics_type);
			tilesSaver.InsertElement("surface_type", surface_type);

			tilesSaver.EndCurrentElement();

			tile_id++;
		}
	}
	tilesSaver.EndArrayElement();

	xmlParser.Save(filename);

	//Write the sprites to the xml file
	SaveSpritesXML(map, gd->m_xml_sprites_path, xml_sprite_str);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveMenuXML(string menufilename, MenuSystem* ms)
{
	XMLParser xmlParser;

	xmlParser.CreateRootNode("Menu");

	ZShadeSandboxEnvironment::Menu* m;
	//Loop through all the menus in the menu system
	map<string, ZShadeSandboxEnvironment::Menu*> menus = ms->GetMenus();

	string menu_render_name = ms->GetMenuRenderName();

	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = menus.find(menu_render_name);

	if (iter != menus.end())
	{
		m = (*iter).second;

		int menu_x = m->Width();
		int menu_y = m->Height();
		stringstream ssx;
		ssx << menu_x;
		stringstream ssy;
		ssy << menu_y;
		string menu_size;
		menu_size.append(ssx.str());
		menu_size.append("x");
		menu_size.append(ssy.str());

		xmlParser.InsertElement("menu_name", m->MenuName());
		xmlParser.InsertElement("menu_size", menu_size);

		//Save the menu type
		string menu_type = "";
		if (m->GetMenuType() == ZShadeSandboxEnvironment::EMenuType::eMainMenu)
			menu_type = "MainMenu";
		else if (m->GetMenuType() == ZShadeSandboxEnvironment::EMenuType::eInGameMenu)
			menu_type = "InGameMenu";

		xmlParser.InsertElement("menu_type", menu_type);
		xmlParser.InsertElement("menu_background", m->GetBackgroundName());

		ArrayElementXMLSaver buttonsSaver(xmlParser.Document(), xmlParser.Element());
		buttonsSaver.BeginNewArrayElement("Buttons");
		for (int i = 0; i < m->GetButtons().size(); i++)
		{
			ZShadeSandboxGraphics::Button* b = m->GetButtons()[i];

			buttonsSaver.CreateElement("Button");
			buttonsSaver.SetAttribute("id", i);

			buttonsSaver.InsertElement("button_x", b->XPos());
			buttonsSaver.InsertElement("button_y", b->YPos());
			buttonsSaver.InsertElement("button_width", b->Width());
			buttonsSaver.InsertElement("button_height", b->Height());
			buttonsSaver.InsertElement("button_text", b->GetText());
			buttonsSaver.InsertElement("normal_image", b->GetNormalImageName());
			buttonsSaver.InsertElement("pushed_image", b->GetPushedImageName());
			buttonsSaver.InsertElement("highlighted_image", b->GetHighlightedImageName());
			buttonsSaver.InsertElement("disabled_image", b->GetDisabledImageName());

			//Save the button script type
			int st = b->ScriptType();
			string st_name = "none";
			switch (st)
			{
				case ZShadeSandboxGraphics::EScriptType::eStart: st_name = "start"; break;
				case ZShadeSandboxGraphics::EScriptType::eResume: st_name = "resume"; break;
				case ZShadeSandboxGraphics::EScriptType::eContinue: st_name = "continue"; break;
				case ZShadeSandboxGraphics::EScriptType::eOptions: st_name = "options"; break;
				case ZShadeSandboxGraphics::EScriptType::eExit: st_name = "exit"; break;
				case ZShadeSandboxGraphics::EScriptType::eNone: st_name = "none"; break;
			}

			buttonsSaver.InsertElement("script_type", st_name);

			buttonsSaver.EndCurrentElement();
		}
		buttonsSaver.EndArrayElement();

		ArrayElementXMLSaver textsSaver(xmlParser.Document(), xmlParser.Element());
		textsSaver.BeginNewArrayElement("Texts");
		for (int i = 0; i < m->GetTexts().size(); i++)
		{
			ZShadeSandboxGraphics::Text* t = m->GetTexts()[i];

			textsSaver.CreateElement("Text");
			textsSaver.SetAttribute("id", i);

			textsSaver.InsertElement("text_x", t->GetXPos());
			textsSaver.InsertElement("text_y", t->GetYPos());
			textsSaver.InsertElement("text", t->TextName().toString());
			textsSaver.InsertElement("text_font", t->FontSize());
			textsSaver.InsertElement("text_color", t->TextColor());
			textsSaver.InsertElement("shadow", t->HasShadow());
			textsSaver.InsertElement("optional_background", "NONE");

			textsSaver.EndCurrentElement();
		}
		textsSaver.EndArrayElement();

		xmlParser.Save(menufilename);
	}

	return true;
}
//===============================================================================================================================