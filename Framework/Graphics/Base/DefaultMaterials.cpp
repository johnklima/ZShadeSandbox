#include "DefaultMaterials.h"
#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
DefaultMaterials::DefaultMaterials()
{
}
//===============================================================================================================================
void DefaultMaterials::Build(D3D* d3d, string base_path)
{
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Target", "target.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "White", "white.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Black", "black.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Blue", "blue.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Red", "red.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Brick", "bricks.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Floor", "floor.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Grass", "grass.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Metal Panel", "metalpanel.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Soil", "soil.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Stone", "stone.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Snow", "snow.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Dirt", "dirt01.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Ice", "ice.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Metal", "metal001.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Wall", "wall01.dds");
	
	XMFLOAT3 transparencyKey = d3d->GetEngineOptions()->m_TransparencyKey;
	
	// FOW Shadow cover for uncovered world, explored area but not seen because sprite is not there
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW", "fow\\fow_shadow.png", "fow\\fow_shadow.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW CNE", "fow\\fow_u_CNE.png", "fow\\fow_u_CNE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW CNW", "fow\\fow_u_CNW.png", "fow\\fow_u_CNW.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW CSE", "fow\\fow_u_CSE.png", "fow\\fow_u_CSE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW CSW", "fow\\fow_u_CSW.png", "fow\\fow_u_CSW.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW EEE", "fow\\fow_u_EEE.png", "fow\\fow_u_EEE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW JNE", "fow\\fow_u_JNE.png", "fow\\fow_u_JNE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW JNW", "fow\\fow_u_JNW.png", "fow\\fow_u_JNW.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW JSE", "fow\\fow_u_JSE.png", "fow\\fow_u_JSE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW JSW", "fow\\fow_u_JSW.png", "fow\\fow_u_JSW.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW NNN", "fow\\fow_u_NNN.png", "fow\\fow_u_NNN.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW SSS", "fow\\fow_u_SSS.png", "fow\\fow_u_SSS.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SHADOW WWW", "fow\\fow_u_WWW.png", "fow\\fow_u_WWW.png", transparencyKey);

	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL SHADOW CNE", "fow\\fow_au_CNE.png", "fow\\fow_au_CNE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL SHADOW CNW", "fow\\fow_au_CNW.png", "fow\\fow_au_CNW.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL SHADOW CSE", "fow\\fow_au_CSE.png", "fow\\fow_au_CSE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL SHADOW CSW", "fow\\fow_au_CSW.png", "fow\\fow_au_CSW.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL SHADOW EEE", "fow\\fow_au_EEE.png", "fow\\fow_au_EEE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL SHADOW JNE", "fow\\fow_au_JNE.png", "fow\\fow_au_JNE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL SHADOW JNW", "fow\\fow_au_JNW.png", "fow\\fow_au_JNW.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL SHADOW JSE", "fow\\fow_au_JSE.png", "fow\\fow_au_JSE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL SHADOW JSW", "fow\\fow_au_JSW.png", "fow\\fow_au_JSW.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL SHADOW NNN", "fow\\fow_au_NNN.png", "fow\\fow_au_NNN.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL SHADOW SSS", "fow\\fow_au_SSS.png", "fow\\fow_au_SSS.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL SHADOW WWW", "fow\\fow_au_WWW.png", "fow\\fow_au_WWW.png", transparencyKey);
	
	// FOW Dark cover for coverted world, not seen at all because sprite has not entered tiles yet
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW ALL", "fow\\fow_all.png", "fow\\fow_all.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW CNE", "fow\\fow_CNE.png", "fow\\fow_CNE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW CNW", "fow\\fow_CNW.png", "fow\\fow_CNW.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW CSE", "fow\\fow_CSE.png", "fow\\fow_CSE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW CSW", "fow\\fow_CSW.png", "fow\\fow_CSW.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW EEE", "fow\\fow_EEE.png", "fow\\fow_EEE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW JNE", "fow\\fow_JNE.png", "fow\\fow_JNE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW JNW", "fow\\fow_JNW.png", "fow\\fow_JNW.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW JSE", "fow\\fow_JSE.png", "fow\\fow_JSE.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW JSW", "fow\\fow_JSW.png", "fow\\fow_JSW.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW NNN", "fow\\fow_NNN.png", "fow\\fow_NNN.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW SSS", "fow\\fow_SSS.png", "fow\\fow_SSS.png", transparencyKey);
	MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path, "FOW WWW", "fow\\fow_WWW.png", "fow\\fow_WWW.png", transparencyKey);
}
//===============================================================================================================================