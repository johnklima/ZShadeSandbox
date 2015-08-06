#include "ZShadeXMLLoader.h"
#include "Convert.h"
#include "BetterString.h"
#include "ShaderMaterial.h"
#include "MaterialManager.h"
#include "Scripting.h"
#include "XMLParser.h"
#include "Vertex.h"
//===============================================================================================================================
//===============================================================================================================================
bool ZShadeXMLLoader::LoadTemplatePolyXML(string filename, ZShadeSandboxMath::Polygon*& poly)
{
	XMLParser xmlParser(filename);

	const char* name_str = xmlParser.ReadElement("Name");

	const char* vertex_count_str = xmlParser.ReadElement("VertexCount");
	int vertex_count = ZShadeSandboxGlobal::Convert::ConvertStringToT<int>(vertex_count_str);
	vector<ZShadeSandboxMesh::VertexTex> vertices(vertex_count);

	ArrayElementXMLParser vertexParser(xmlParser.Element());
	vertexParser.ReadArrayElement("Vertices", "Vertex");
	while (vertexParser.ArrayElementNotNull("Vertex"))
	{
		string eName = vertexParser.ReadArrayElementValue();
		string eIDAtt = vertexParser.ReadArrayElementAttribute("id");
		int id = ZShadeSandboxGlobal::Convert::ConvertStringToT<int>(eIDAtt);

		const char* vertexPosition = vertexParser.ReadArrayChildElement("Pos");
		const char* vertexUV = vertexParser.ReadArrayChildElement("UV");

		XMFLOAT3 position = ZShadeSandboxGlobal::Convert::ConvertToFloat3(vertexPosition);
		XMFLOAT2 uv = ZShadeSandboxGlobal::Convert::ConvertToFloat2(vertexUV);

		vertices[id].position = position;
		vertices[id].texture = uv;
	}

	const char* index_count_str = xmlParser.ReadElement("IndexCount");
	int index_count = ZShadeSandboxGlobal::Convert::ConvertStringToT<int>(index_count_str);
	vector<UINT> indices(index_count);

	ArrayElementXMLParser indexParser(xmlParser.Element());
	indexParser.ReadArrayElement("Indices", "Index");
	while (indexParser.ArrayElementNotNull("Index"))
	{
		string eName = indexParser.ReadArrayElementValue();
		string eIDAtt = indexParser.ReadArrayElementAttribute("id");
		int id = ZShadeSandboxGlobal::Convert::ConvertStringToT<int>(eIDAtt);

		const char* value = indexParser.ReadArrayChildElement("Value");
		UINT index_value = ZShadeSandboxGlobal::Convert::ConvertStringToT<UINT>(value);
		
		indices[id] = index_value;
	}

	const char* material_name_str = xmlParser.ReadElement("MaterialName");

	const char* polygon_rotation_str = xmlParser.ReadElement("Rotation");
	XMFLOAT3 polygon_rotation = ZShadeSandboxGlobal::Convert::ConvertToFloat3(polygon_rotation_str);

	const char* polygon_scale_str = xmlParser.ReadElement("Scale");
	XMFLOAT3 polygon_scale = ZShadeSandboxGlobal::Convert::ConvertToFloat3(polygon_scale_str);

	if (poly == 0)
	{
		poly = new ZShadeSandboxMath::Polygon();
	}
	
	poly->Name() = name_str;
	
	poly->Create(vertices);
	poly->CreateIndices(indices);
	
	// Since it is a template it will start out at the origin
	// or the position that was clicked by the mouse
	poly->Position() = XMFLOAT3(0, 0, 0);
	poly->Rotation() = polygon_rotation;
	poly->Scale() = polygon_scale;
	
	poly->MaterialName() = material_name_str;

	return true;
}
//===============================================================================================================================
bool ZShadeXMLLoader::LoadMaterialXML(string basePath, string filename, D3D* d3d)
{
	XMLParser xmlParser(filename);
	
	const char* material_name_str = xmlParser.ReadElement("Name");
	const char* enable_shadowmap_str = xmlParser.ReadElement("EnableShadowMap");
	const char* enable_ssao_str = xmlParser.ReadElement("EnableSSAO");
	const char* enable_transparency_str = xmlParser.ReadElement("EnableTransparency");
	const char* enable_lighting_str = xmlParser.ReadElement("EnableLighting");
	const char* detail_brightness_str = xmlParser.ReadElement("DetailBrightness");
	const char* alpha_to_coverage_str = xmlParser.ReadElement("AlphaToCoverage");
	const char* ambient_color_str = xmlParser.ReadElement("AmbientColor");
	const char* diffuse_color_str = xmlParser.ReadElement("DiffuseColor");
	const char* specular_color_str = xmlParser.ReadElement("SpecularColor");
	const char* emissive_color_str = xmlParser.ReadElement("EmissiveColor");
	const char* specular_power_str = xmlParser.ReadElement("SpecularPower");
	const char* specular_intensity_str = xmlParser.ReadElement("SpecularIntensity");
	const char* emissive_str = xmlParser.ReadElement("Emissivity");
	const char* reflectivity_str = xmlParser.ReadElement("Reflectivity");
	const char* transmissivity_str = xmlParser.ReadElement("Transmissivity");
	const char* transmission_filter_str = xmlParser.ReadElement("TransmissionFilter");
	const char* alpha_str = xmlParser.ReadElement("Alpha");
	const char* refraction_index_str = xmlParser.ReadElement("RefractionIndex");
	const char* no_dist_tess_factor_str = xmlParser.ReadElement("NoDistTessFactor");
	const char* min_tess_dist_str = xmlParser.ReadElement("MinTessDist");
	const char* max_tess_dist_str = xmlParser.ReadElement("MaxTessDist");
	const char* min_tess_factor_str = xmlParser.ReadElement("MinTessFactor");
	const char* max_tess_factor_str = xmlParser.ReadElement("MaxTessFactor");
	const char* enable_dist_tess_str = xmlParser.ReadElement("EnableDistTess");
	const char* illumination_model_str = xmlParser.ReadElement("IlluminationModel");
	
	vector<string> diffuseArrayTextureNames;
	ArrayElementXMLParser diffuseArrayParser(xmlParser.Element());
	diffuseArrayParser.ReadArrayElement("DiffuseArray", "Texture");
	while (diffuseArrayParser.ArrayElementNotNull("Texture"))
	{
		string eName = diffuseArrayParser.ReadArrayElementValue();
		string eIDAtt = diffuseArrayParser.ReadArrayElementAttribute("id");
		
		const char* tex_name_str = diffuseArrayParser.ReadArrayChildElement("TexName");
		
		diffuseArrayTextureNames.push_back(string(tex_name_str));
	}
	
	const char* diffuse_texture_str = xmlParser.ReadElement("DiffuseTexture");
	const char* ambient_texture_str = xmlParser.ReadElement("AmbientTexture");
	const char* specular_texture_str = xmlParser.ReadElement("SpecularTexture");
	const char* emissive_texture_str = xmlParser.ReadElement("EmissiveTexture");
	const char* detail_texture_str = xmlParser.ReadElement("DetailTexture");
	const char* normal_texture_str = xmlParser.ReadElement("NormalTexture");
	const char* blend_texture_str = xmlParser.ReadElement("BlendTexture");
	const char* alpha_texture_str = xmlParser.ReadElement("AlphaTexture");
	
	string name(material_name_str);
	bool enableShadowMap = (BetterString(enable_shadowmap_str) == "true") ? true : false;
	bool enableSSAO = (BetterString(enable_ssao_str) == "true") ? true : false;
	bool enableTransparency = (BetterString(enable_transparency_str) == "true") ? true : false;
	bool enableLighting = (BetterString(enable_lighting_str) == "true") ? true : false;
	float detailBrightness = atof(detail_brightness_str);
	XMFLOAT3 alphaToCoverage = ZShadeSandboxGlobal::Convert::ConvertToFloat3(alpha_to_coverage_str);
	XMFLOAT4 ambientColor = ZShadeSandboxGlobal::Convert::ConvertToFloat4(ambient_color_str);
	XMFLOAT4 diffuseColor = ZShadeSandboxGlobal::Convert::ConvertToFloat4(diffuse_color_str);
	XMFLOAT4 specularColor = ZShadeSandboxGlobal::Convert::ConvertToFloat4(specular_color_str);
	XMFLOAT4 emissiveColor = ZShadeSandboxGlobal::Convert::ConvertToFloat4(emissive_color_str);
	float specularPower = atof(specular_power_str);
	float specularIntensity = atof(specular_intensity_str);
	float emissive = atof(emissive_str);
	float reflectivity = atof(reflectivity_str);
	float transmissivity = atof(transmissivity_str);
	float alpha = atof(alpha_str);
	XMFLOAT3 transmissionFilter = ZShadeSandboxGlobal::Convert::ConvertToFloat3(transmission_filter_str);
	float refractionIndex = atof(refraction_index_str);
	float noDistTessFactor = atof(no_dist_tess_factor_str);
	float minTessDist = atof(min_tess_dist_str);
	float maxTessDist = atof(max_tess_dist_str);
	float minTessFactor = atof(min_tess_factor_str);
	float maxTessFactor = atof(max_tess_factor_str);
	bool enableDistTess = (BetterString(enable_dist_tess_str) == "true") ? true : false;
	int illuminationModel = atoi(illumination_model_str);
	string diffuseTextureName(diffuse_texture_str);
	string ambientTextureName(ambient_texture_str);
	string specularTextureName(specular_texture_str);
	string emissiveTextureName(emissive_texture_str);
	string detailTextureName(detail_texture_str);
	string normalTextureName(normal_texture_str);
	string blendTextureName(blend_texture_str);
	string alphaTextureName(alpha_texture_str);

	//
	// Load the material into the material manager
	//
	
	ZShadeSandboxLighting::ShaderMaterial* material = new ZShadeSandboxLighting::ShaderMaterial(d3d, name);
	
	material->bHasShadowMap = enableShadowMap;
	material->bHasSSAOMap = enableSSAO;
	material->bHasTransparency = enableTransparency;
	material->bEnableLighting = enableLighting;
	material->fDetailBrightness = detailBrightness;
	material->vAlphaToCoverageValue = alphaToCoverage;
	material->vAmbientColor = ambientColor;
	material->vDiffuseColor = diffuseColor;
	material->vSpecularColor = specularColor;
	material->vEmissiveColor = emissiveColor;

	bool hasDiffuseArrayTexture;
	(BetterString(diffuseArrayTextureNames[0]) == "NONE") ? hasDiffuseArrayTexture = false : hasDiffuseArrayTexture = true;
	if (hasDiffuseArrayTexture)
		material->CreateTexture2DArray(diffuseArrayTextureNames);
	
	material->fSpecularPower = specularPower;
	material->fSpecularIntensity = specularIntensity;
	material->fEmissivity = emissive;
	material->fReflectivity = reflectivity;
	material->fTransmissivity = transmissivity;
	material->fAlpha = alpha;
	material->vTransmissionFilter = transmissionFilter;
	material->fRefractionIndex = refractionIndex;
	material->fNoDistTessFactor = noDistTessFactor;
	material->fMinTessDist = minTessDist;
	material->fMaxTessDist = maxTessDist;
	material->fMinTessFactor = minTessFactor;
	material->fMaxTessFactor = maxTessFactor;
	material->bEnableDistTess = enableDistTess;
	material->iIlluminationModel = illuminationModel;
	
	bool hasDiffuseTexture;
	(BetterString(diffuseTextureName) == "NONE") ? hasDiffuseTexture = false : hasDiffuseTexture = true;
	if (hasDiffuseTexture)
		material->AddDiffuseTexture(basePath, diffuseTextureName);
	
	bool hasAmbientTexture;
	(BetterString(ambientTextureName) == "NONE") ? hasAmbientTexture = false : hasAmbientTexture = true;
	if (hasAmbientTexture)
		material->AddAmbientTexture(basePath, ambientTextureName);

	bool hasSpecularTexture;
	(BetterString(specularTextureName) == "NONE") ? hasSpecularTexture = false : hasSpecularTexture = true;
	if (hasSpecularTexture)
		material->AddSpecularTexture(basePath, specularTextureName);

	bool hasEmissiveTexture;
	(BetterString(emissiveTextureName) == "NONE") ? hasEmissiveTexture = false : hasEmissiveTexture = true;
	if (hasEmissiveTexture)
		material->AddEmissiveTexture(basePath, emissiveTextureName);

	bool hasDetailTexture;
	(BetterString(detailTextureName) == "NONE") ? hasDetailTexture = false : hasDetailTexture = true;
	if (hasDetailTexture)
		material->AddDetailMapTexture(basePath, detailTextureName);
	
	bool hasNormalMapTexture;
	(BetterString(normalTextureName) == "NONE") ? hasNormalMapTexture = false : hasNormalMapTexture = true;
	if (hasNormalMapTexture)
		material->AddNormalMapTexture(basePath, normalTextureName);
	
	bool hasBlendMapTexture;
	(BetterString(blendTextureName) == "NONE") ? hasBlendMapTexture = false : hasBlendMapTexture = true;
	if (hasBlendMapTexture)
		material->AddBlendMapTexture(basePath, blendTextureName);
	
	bool hasAlphaMapTexture;
	(BetterString(alphaTextureName) == "NONE") ? hasAlphaMapTexture = false : hasAlphaMapTexture = true;
	if (hasAlphaMapTexture)
		material->AddAlphaMapTexture(basePath, alphaTextureName);
	
	material->bHasDiffuseArrayTexture = hasDiffuseArrayTexture;
	material->bHasDiffuseTexture = hasDiffuseTexture;
	material->bHasAmbientTexture = hasAmbientTexture;
	material->bHasSpecularTexture = hasSpecularTexture;
	material->bHasEmissiveTexture = hasEmissiveTexture;
	material->bHasDetailMapTexture = hasDetailTexture;
	material->bHasNormalMapTexture = hasNormalMapTexture;
	material->bHasBlendMapTexture = hasBlendMapTexture;
	material->bHasAlphaMapTexture = hasAlphaMapTexture;

	MaterialManager::Instance()->Add(material);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLLoader::LoadInventoryXML(string filename, GameDirectory2D* gd, EngineOptions* eo, PlayerInventory2D*& inventory)
{
	XMLParser xmlParser(filename);
	
	const char* background_image_name_str = xmlParser.ReadElement("BackgroundImageName");
	const char* item_size_str = xmlParser.ReadElement("ItemSize");
	const char* item_row_size_regular_str = xmlParser.ReadElement("ItemRowSizeRegular");
	const char* item_col_size_regular_str = xmlParser.ReadElement("ItemColSizeRegular");
	const char* item_row_size_magic_str = xmlParser.ReadElement("ItemRowSizeMagic");
	const char* item_col_size_magic_str = xmlParser.ReadElement("ItemColSizeMagic");
	const char* item_row_padding_regular_str = xmlParser.ReadElement("ItemRowPaddingRegular");
	const char* item_col_padding_regular_str = xmlParser.ReadElement("ItemColPaddingRegular");
	const char* item_row_padding_magic_str = xmlParser.ReadElement("ItemRowPaddingMagic");
	const char* item_col_padding_magic_str = xmlParser.ReadElement("ItemColPaddingMagic");
	const char* start_location_regular_str = xmlParser.ReadElement("StartLocationRegular");
	const char* start_location_magic_str = xmlParser.ReadElement("StartLocationMagic");
	
	int item_size = atoi(item_size_str);
	int item_row_size_regular = atoi(item_row_size_regular_str);
	int item_col_size_regular = atoi(item_col_size_regular_str);
	int item_row_size_magic = atoi(item_row_size_magic_str);
	int item_col_size_magic = atoi(item_col_size_magic_str);
	int item_row_padding_regular = atoi(item_row_padding_regular_str);
	int item_col_padding_regular = atoi(item_col_padding_regular_str);
	int item_row_padding_magic = atoi(item_row_padding_magic_str);
	int item_col_padding_magic = atoi(item_col_padding_magic_str);
	XMFLOAT2 start_location_regular = ZShadeSandboxGlobal::Convert::ConvertToFloat2(start_location_regular_str);
	XMFLOAT2 start_location_magic = ZShadeSandboxGlobal::Convert::ConvertToFloat2(start_location_magic_str);
	
	inventory->Init(
		gd->m_required_textures,
		gd->m_sprites_path,
		string(background_image_name_str),
		eo->m_screenWidth,
		eo->m_screenHeight,
		item_size,
		item_size,
		item_size,
		item_row_size_regular, item_col_size_regular,
		item_row_size_magic, item_col_size_magic,
		item_row_padding_regular, item_col_padding_regular,
		item_row_padding_magic, item_col_padding_magic,
		start_location_regular,
		start_location_magic
	);
	
	ArrayElementXMLParser regularItemsParser(xmlParser.Element());
	regularItemsParser.ReadArrayElement("RegularItems", "Item");
	while (regularItemsParser.ArrayElementNotNull("Item"))
	{
		string eName = regularItemsParser.ReadArrayElementValue();
		string eIDAtt = regularItemsParser.ReadArrayElementAttribute("id");
		string eImageNameAtt = regularItemsParser.ReadArrayElementAttribute("image");
		
		const char* inventory_message_str = regularItemsParser.ReadArrayChildElement("Message");
		
		inventory->AddItemRegular(gd->m_sprites_path, eImageNameAtt, string(inventory_message_str));
	}
	
	ArrayElementXMLParser magicItemsParser(xmlParser.Element());
	magicItemsParser.ReadArrayElement("MagicItems", "Item");
	while (magicItemsParser.ArrayElementNotNull("Item"))
	{
		string eName = magicItemsParser.ReadArrayElementValue();
		string eIDAtt = magicItemsParser.ReadArrayElementAttribute("id");
		string eImageNameAtt = magicItemsParser.ReadArrayElementAttribute("image");
		
		const char* inventory_message_str = magicItemsParser.ReadArrayChildElement("Message");
		
		inventory->AddItemMagic(gd->m_sprites_path, eImageNameAtt, string(inventory_message_str));
	}
	
	inventory->PositionItemsOnGrid();
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLLoader::LoadMenuXML(string menufilename, GameDirectory2D* gd, EngineOptions* eo, D3D* d3d, MenuSystem*& ms)
{
	XMLParser xmlParser(menufilename);
	
	const char* menu_name_str = xmlParser.ReadElement("menu_name");
	const char* menu_size_str = xmlParser.ReadElement("menu_size");
	const char* menu_type_str = xmlParser.ReadElement("menu_type");
	const char* background_name_str = xmlParser.ReadElement("menu_background");
	string background_name(background_name_str);
	
	BetterString split_b_str(menu_size_str);
	vector<string> split_str = split_b_str.split('x');
	int menu_width = atoi(split_str[0].c_str());
	int menu_height = atoi(split_str[1].c_str());
	
	ZShadeSandboxEnvironment::EMenuType::Type menuType;
	if (strcmp(menu_type_str, "MainMenu") == 0)
		menuType = ZShadeSandboxEnvironment::EMenuType::eMainMenu;
	else if (strcmp(menu_type_str, "InGameMenu") == 0)
		menuType = ZShadeSandboxEnvironment::EMenuType::eInGameMenu;
	
	ZShadeSandboxEnvironment::Menu* m = new ZShadeSandboxEnvironment::Menu(d3d);
	m->MenuName() = string(menu_name_str);
	m->Width() = menu_width;
	m->Height() = menu_height;
	m->GetMenuType() = menuType;
	
	if (background_name != "")
	{
		ZShadeSandboxGraphics::Image* im = new ZShadeSandboxGraphics::Image(d3d, background_name, gd->m_sprites_path, 0, 0, menu_width, menu_height);
		m->AddBackground( im );
	}
	
	ms->AddMenu(m);
	
	int st = ZShadeSandboxGraphics::EScriptType::eNone;
	
	ArrayElementXMLParser buttonsParser(xmlParser.Element());
	buttonsParser.ReadArrayElement("Buttons", "Button");
	while (buttonsParser.ArrayElementNotNull("Button"))
	{
		string eName = buttonsParser.ReadArrayElementValue();
		string eIDAtt = buttonsParser.ReadArrayElementAttribute("id");
		
		const char* x_str = buttonsParser.ReadArrayChildElement("button_x");
		const char* y_str = buttonsParser.ReadArrayChildElement("button_y");
		const char* width_str = buttonsParser.ReadArrayChildElement("button_width");
		const char* height_str = buttonsParser.ReadArrayChildElement("button_height");
		const char* text_str = buttonsParser.ReadArrayChildElement("button_text");
		const char* normal_image_str = buttonsParser.ReadArrayChildElement("normal_image");
		const char* pushed_image_str = buttonsParser.ReadArrayChildElement("pushed_image");
		const char* highlighted_image_str = buttonsParser.ReadArrayChildElement("highlighted_image");
		const char* disabled_image_str = buttonsParser.ReadArrayChildElement("disabled_image");
		const char* script_type_str = buttonsParser.ReadArrayChildElement("script_type");
		
		if (strcmp(script_type_str, "start") == 0) st = ZShadeSandboxGraphics::EScriptType::eStart;
		if (strcmp(script_type_str, "resume") == 0) st = ZShadeSandboxGraphics::EScriptType::eResume;
		if (strcmp(script_type_str, "continue") == 0) st = ZShadeSandboxGraphics::EScriptType::eContinue;
		if (strcmp(script_type_str, "options") == 0) st = ZShadeSandboxGraphics::EScriptType::eOptions;
		if (strcmp(script_type_str, "exit") == 0) st = ZShadeSandboxGraphics::EScriptType::eExit;
		if (strcmp(script_type_str, "none") == 0) st = ZShadeSandboxGraphics::EScriptType::eNone;
		
		ZShadeSandboxGraphics::Button* b = new ZShadeSandboxGraphics::Button(
			d3d,
			ZShadeSandboxGlobal::Convert::ConvertStringToCharPointer(string(text_str)),
			false,
			atof(x_str), atof(y_str),
			atoi(width_str), atoi(height_str),
			gd->m_menu_sprites_path,
			string(normal_image_str),
			string(pushed_image_str),
			string(highlighted_image_str),
			string(disabled_image_str),
			"white.png",
			"blue.png",
			"yellow.png",
			"black.png"
		);
		
		b->ScriptType() = st;

		switch (st)
		{
			case ZShadeSandboxGraphics::EScriptType::eStart: b->GetScript() = Scripting::GetScript("start"); break;
			case ZShadeSandboxGraphics::EScriptType::eResume: b->GetScript() = Scripting::GetScript("resume"); break;
			case ZShadeSandboxGraphics::EScriptType::eContinue: b->GetScript() = Scripting::GetScript("continue"); break;
			case ZShadeSandboxGraphics::EScriptType::eOptions: b->GetScript() = Scripting::GetScript("options"); break;
			case ZShadeSandboxGraphics::EScriptType::eExit: b->GetScript() = Scripting::GetScript("exit"); break;
		}
		
		m->AddButton(b);
	}
	
	ArrayElementXMLParser textsParser(xmlParser.Element());
	textsParser.ReadArrayElement("Texts", "Text");
	while (textsParser.ArrayElementNotNull("Text"))
	{
		string eName = textsParser.ReadArrayElementValue();
		string eIDAtt = textsParser.ReadArrayElementAttribute("id");
		
		const char* x_str = textsParser.ReadArrayChildElement("text_x");
		const char* y_str = textsParser.ReadArrayChildElement("text_y");
		const char* text_str = textsParser.ReadArrayChildElement("text");
		const char* text_font_str = textsParser.ReadArrayChildElement("text_font");
		const char* text_color_str = textsParser.ReadArrayChildElement("text_color");
		const char* shadow_str = textsParser.ReadArrayChildElement("shadow");
		const char* optional_background_str = textsParser.ReadArrayChildElement("optional_background");
		
		XMFLOAT3 color = ZShadeSandboxGlobal::Convert::ConvertToFloat3(string(text_color_str));
		bool shadow = (string(shadow_str) == "1") ? true : false;
		
		ZShadeSandboxGraphics::Text* t = new ZShadeSandboxGraphics::Text();
		t->SetD3D(d3d);
		t->Initialize();
		t->TextName() = BetterString(text_str);
		if (shadow)
		{
			t->CreateShadow();
		}
		t->ChangeFont(L"Times New Roman", atoi(text_font_str), FontStyle::BoldItalic, true);
		t->TextTransform() = XMMatrixTranslation(atof(x_str), atof(y_str), 0);
		t->TextColor() = XMFLOAT4(color.x, color.y, color.z, 1);
		
		m->AddText(t);
	}
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLLoader::LoadSpritesXML(string spriteMapfilename, GameDirectory2D* gd, D3D* d3d, TopdownMap*& map)
{
	XMLParser xmlParser(spriteMapfilename);
	
	AISprite *spr;
	ArrayElementXMLParser spritesParser(xmlParser.Element());
	spritesParser.ReadArrayElement("Sprite");
	do
	{
		string eName = spritesParser.ReadArrayElementValue();
		string eIDAtt = spritesParser.ReadArrayElementAttribute("id");
		
		const char* player_str = spritesParser.ReadArrayChildElement("player");
		const char* x_str = spritesParser.ReadArrayChildElement("x");
		const char* y_str = spritesParser.ReadArrayChildElement("y");
		const char* width_str = spritesParser.ReadArrayChildElement("width");
		const char* height_str = spritesParser.ReadArrayChildElement("height");
		const char* hbox_x_str = spritesParser.ReadArrayChildElement("hbox_x");
		const char* hbox_y_str = spritesParser.ReadArrayChildElement("hbox_y");
		const char* hbox_width_str = spritesParser.ReadArrayChildElement("hbox_width");
		const char* hbox_height_str = spritesParser.ReadArrayChildElement("hbox_height");
		const char* baseimage_str = spritesParser.ReadArrayChildElement("baseimage");
		const char* vision_str = spritesParser.ReadArrayChildElement("vision");
		const char* depth_str = spritesParser.ReadArrayChildElement("depth");
		const char* hard_str = spritesParser.ReadArrayChildElement("hard");
		const char* nohit_str = spritesParser.ReadArrayChildElement("nohit");
		const char* animation_name_str = spritesParser.ReadArrayChildElement("animation");
		const char* default_seq_str = spritesParser.ReadArrayChildElement("default_seq");
		const char* anim_speed_str = spritesParser.ReadArrayChildElement("anim_speed");
		const char* hp_str = spritesParser.ReadArrayChildElement("hp");
		const char* hp_max_str = spritesParser.ReadArrayChildElement("hp_max");
		const char* mana_str = spritesParser.ReadArrayChildElement("mana");
		const char* gold_str = spritesParser.ReadArrayChildElement("gold");
		const char* strength_str = spritesParser.ReadArrayChildElement("strength");
		const char* speed_str = spritesParser.ReadArrayChildElement("speed");
		const char* defense_str = spritesParser.ReadArrayChildElement("defense");
		const char* experience_str = spritesParser.ReadArrayChildElement("experience");
		const char* level_str = spritesParser.ReadArrayChildElement("level");
		const char* script_str = spritesParser.ReadArrayChildElement("script");
		const char* touch_str = spritesParser.ReadArrayChildElement("touch");
		const char* background_str = spritesParser.ReadArrayChildElement("background");
		const char* entity_str = spritesParser.ReadArrayChildElement("entity");
		const char* invisible_str = spritesParser.ReadArrayChildElement("invisible");
		const char* static_str = spritesParser.ReadArrayChildElement("static");
		const char* name_str = spritesParser.ReadArrayChildElement("name");
		const char* collide_msg_str = spritesParser.ReadArrayChildElement("collide_msg");
		const char* inventory_msg_str = spritesParser.ReadArrayChildElement("inventory_msg");
		const char* seen_by_player_str = spritesParser.ReadArrayChildElement("always_seen_by_player");
		const char* teleport_str = spritesParser.ReadArrayChildElement("teleport");
		const char* teleport_map_str = spritesParser.ReadArrayChildElement("teleport_map");
		const char* teleport_map_x_str = spritesParser.ReadArrayChildElement("teleport_map_x");
		const char* teleport_map_y_str = spritesParser.ReadArrayChildElement("teleport_map_y");
		
		bool is_player = (string(player_str) == "true") ? true : false;
		float x = atof(x_str);
		float y = atof(y_str);
		int width = atoi(width_str);
		int height = atoi(height_str);
		float hbox_x = atof(hbox_x_str);
		float hbox_y = atof(hbox_y_str);
		int hbox_width = atoi(hbox_width_str);
		int hbox_height = atoi(hbox_height_str);
		int vision = atoi(vision_str);
		int depth = atoi(depth_str);
		int hard = atoi(hard_str);
		int nohit = atoi(nohit_str);
		int default_seq = atoi(default_seq_str);
		int anim_speed = atoi(anim_speed_str);
		int hp = atoi(hp_str);
		int hp_max = atoi(hp_max_str);
		int mana = atoi(mana_str);
		int gold = atoi(gold_str);
		int strength = atoi(strength_str);
		int speed = atoi(speed_str);
		int defense = atoi(defense_str);
		int experience = atoi(experience_str);
		int level = atoi(level_str);
		bool touch = (string(touch_str) == "true") ? true : false;
		bool background = (string(background_str) == "true") ? true : false;
		bool entity = (string(entity_str) == "true") ? true : false;
		bool invisible = (string(invisible_str) == "true") ? true : false;
		bool is_static = (string(static_str) == "true") ? true : false;
		bool teleport = (string(teleport_str) == "true") ? true : false;
		int teleport_map_x = atoi(teleport_map_x_str);
		int teleport_map_y = atoi(teleport_map_y_str);
		bool seen_by_player = (string(seen_by_player_str) == "true") ? true : false;
		
		string path;
		if (strcmp(baseimage_str, "player_cover.png") == 0)
		{
			path = gd->m_required_textures;
		}
		else
		{
			path = gd->m_sprites_path;
		}

		PhysicsType pt;
		if (is_static)
			pt = STATIC;
		else
			pt = DYNAMIC;

		spr = new AISprite(d3d);
		spr->Initialize(path, string(baseimage_str), XMFLOAT3(x, y, 0), width, height, pt);
		
		spr->ScriptName() = string(script_str);
		
		spr->PlatformerSprite() = false; // This is a sprite on a TopDown map
		spr->ID() = atoi(eIDAtt.c_str());
		spr->Vision() = vision;
		spr->Depth() = depth;
		spr->IsHard() = hard;
		spr->IsNoHit() = nohit;
		spr->AnimationProfileName() = string(animation_name_str);
		spr->DefaultSequence() = default_seq;
		spr->AnimationSpeed() = anim_speed;
		spr->HP() = hp;
		spr->Mana() = mana;
		spr->Gold() = gold;
		spr->Strength() = strength;
		spr->Speed() = speed;
		spr->Defense() = defense;
		spr->Experience() = experience;
		spr->Level() = level;
		spr->IsTouch() = touch;
		spr->IsBackgroundSprite() = background;
		spr->IsEntitySprite() = entity;
		spr->IsInvisibleSprite() = invisible;
		spr->CanTeleport() = teleport;
		spr->TeleportMapName() = string(teleport_map_str);
		spr->TeleportX() = teleport_map_x;
		spr->TeleportY() = teleport_map_y;
		spr->Name() = string(name_str);
		spr->HPMax() = hp_max;
		//spr->IsStatic() = is_static;
		spr->EPhysicsType() = (is_static) ? STATIC : DYNAMIC;
		spr->CollideMsg() = string(collide_msg_str);
		spr->InventoryMessage() = string(inventory_msg_str);
		spr->AlwaysSeenByPlayer() = seen_by_player;
		//Set the sprites hardbox
		//spr->SetHardbox(
		//	hbox_x, hbox_y, hbox_width, hbox_height
		//);
		//path = gd->m_required_textures;		
		//Create the new hard box
		//spr->CreateHardBoxSpr(path);

		if (is_player)
		{
			//Add the sprite to the map
			map->AddPlayer(spr);
		}
		else
		{
			//Add the sprite to the map
			map->AddSprite(spr);
		}

		//Add the animation
		if (strcmp(animation_name_str, "NONE") != 0)
			map->AddSpriteAnimations(string(animation_name_str), gd->GAME_FOLDER, gd->m_sprites_path, spr->ID());
	} while (spritesParser.ArrayElementNotNull("Sprite"));
	
	return true;
}
//================================================================================================================
bool ZShadeXMLLoader::LoadSpritesXML(string spriteMapfilename, GameDirectory2D* gd, D3D* d3d, PlatformerMap*& map)
{
	XMLParser xmlParser(spriteMapfilename);
	
	AISprite *spr;
	ArrayElementXMLParser spritesParser(xmlParser.Element());
	spritesParser.ReadArrayElement("Sprite");
	do
	{
		string eName = spritesParser.ReadArrayElementValue();
		string eIDAtt = spritesParser.ReadArrayElementAttribute("id");
		
		const char* player_str = spritesParser.ReadArrayChildElement("player");
		const char* x_str = spritesParser.ReadArrayChildElement("x");
		const char* y_str = spritesParser.ReadArrayChildElement("y");
		const char* width_str = spritesParser.ReadArrayChildElement("width");
		const char* height_str = spritesParser.ReadArrayChildElement("height");
		const char* hbox_x_str = spritesParser.ReadArrayChildElement("hbox_x");
		const char* hbox_y_str = spritesParser.ReadArrayChildElement("hbox_y");
		const char* hbox_width_str = spritesParser.ReadArrayChildElement("hbox_width");
		const char* hbox_height_str = spritesParser.ReadArrayChildElement("hbox_height");
		const char* baseimage_str = spritesParser.ReadArrayChildElement("baseimage");
		const char* hard_str = spritesParser.ReadArrayChildElement("hard");
		const char* nohit_str = spritesParser.ReadArrayChildElement("nohit");
		const char* animation_name_str = spritesParser.ReadArrayChildElement("animation");
		const char* default_seq_str = spritesParser.ReadArrayChildElement("default_seq");
		const char* anim_speed_str = spritesParser.ReadArrayChildElement("anim_speed");
		const char* hp_str = spritesParser.ReadArrayChildElement("hp");
		const char* hp_max_str = spritesParser.ReadArrayChildElement("hp_max");
		const char* mana_str = spritesParser.ReadArrayChildElement("mana");
		const char* gold_str = spritesParser.ReadArrayChildElement("gold");
		const char* strength_str = spritesParser.ReadArrayChildElement("strength");
		const char* speed_str = spritesParser.ReadArrayChildElement("speed");
		const char* defense_str = spritesParser.ReadArrayChildElement("defense");
		const char* experience_str = spritesParser.ReadArrayChildElement("experience");
		const char* level_str = spritesParser.ReadArrayChildElement("level");
		const char* script_str = spritesParser.ReadArrayChildElement("script");
		const char* touch_str = spritesParser.ReadArrayChildElement("touch");
		const char* background_str = spritesParser.ReadArrayChildElement("background");
		const char* entity_str = spritesParser.ReadArrayChildElement("entity");
		const char* invisible_str = spritesParser.ReadArrayChildElement("invisible");
		const char* static_str = spritesParser.ReadArrayChildElement("static");
		const char* name_str = spritesParser.ReadArrayChildElement("name");
		const char* collide_msg_str = spritesParser.ReadArrayChildElement("collide_msg");
		const char* inventory_msg_str = spritesParser.ReadArrayChildElement("inventory_msg");
		const char* seen_by_player_str = spritesParser.ReadArrayChildElement("always_seen_by_player");
		const char* teleport_str = spritesParser.ReadArrayChildElement("teleport");
		const char* teleport_map_str = spritesParser.ReadArrayChildElement("teleport_map");
		const char* teleport_map_x_str = spritesParser.ReadArrayChildElement("teleport_map_x");
		const char* teleport_map_y_str = spritesParser.ReadArrayChildElement("teleport_map_y");
		
		bool is_player = (string(player_str) == "true") ? true : false;
		float x = atof(x_str);
		float y = atof(y_str);
		int width = atoi(width_str);
		int height = atoi(height_str);
		float hbox_x = atof(hbox_x_str);
		float hbox_y = atof(hbox_y_str);
		int hbox_width = atoi(hbox_width_str);
		int hbox_height = atoi(hbox_height_str);
		int hard = atoi(hard_str);
		int nohit = atoi(nohit_str);
		int default_seq = atoi(default_seq_str);
		int anim_speed = atoi(anim_speed_str);
		int hp = atoi(hp_str);
		int hp_max = atoi(hp_max_str);
		int mana = atoi(mana_str);
		int gold = atoi(gold_str);
		int strength = atoi(strength_str);
		int speed = atoi(speed_str);
		int defense = atoi(defense_str);
		int experience = atoi(experience_str);
		int level = atoi(level_str);
		bool touch = (string(touch_str) == "true") ? true : false;
		bool background = (string(background_str) == "true") ? true : false;
		bool entity = (string(entity_str) == "true") ? true : false;
		bool invisible = (string(invisible_str) == "true") ? true : false;
		bool is_static = (string(static_str) == "true") ? true : false;
		bool teleport = (string(teleport_str) == "true") ? true : false;
		int teleport_map_x = atoi(teleport_map_x_str);
		int teleport_map_y = atoi(teleport_map_y_str);
		bool seen_by_player = (string(seen_by_player_str) == "true") ? true : false;
		
		string path;
		if (strcmp(baseimage_str, "player_cover.png") == 0)
		{
			path = gd->m_required_textures;
		}
		else
		{
			path = gd->m_sprites_path;
		}

		PhysicsType pt;
		if (is_static)
			pt = STATIC;
		else
			pt = DYNAMIC;

		spr = new AISprite(d3d);
		spr->Initialize(path, string(baseimage_str), XMFLOAT3(x, y, 0), width, height, pt);
		
		spr->ScriptName() = string(script_str);
		
		spr->PlatformerSprite() = true; // This is a sprite on a Platformer map
		spr->ID() = atoi(eIDAtt.c_str());
		spr->IsHard() = hard;
		spr->IsNoHit() = nohit;
		spr->AnimationProfileName() = string(animation_name_str);
		spr->DefaultSequence() = default_seq;
		spr->AnimationSpeed() = anim_speed;
		spr->HP() = hp;
		spr->Mana() = mana;
		spr->Gold() = gold;
		spr->Strength() = strength;
		spr->Speed() = speed;
		spr->Defense() = defense;
		spr->Experience() = experience;
		spr->Level() = level;
		spr->IsTouch() = touch;
		spr->IsBackgroundSprite() = background;
		spr->IsEntitySprite() = entity;
		spr->IsInvisibleSprite() = invisible;
		spr->CanTeleport() = teleport;
		spr->TeleportMapName() = string(teleport_map_str);
		spr->TeleportX() = teleport_map_x;
		spr->TeleportY() = teleport_map_y;
		spr->Name() = string(name_str);
		spr->HPMax() = hp_max;
		//spr->IsStatic() = is_static;
		spr->EPhysicsType() = (is_static) ? STATIC : DYNAMIC;
		spr->CollideMsg() = string(collide_msg_str);
		spr->InventoryMessage() = string(inventory_msg_str);
		spr->AlwaysSeenByPlayer() = seen_by_player;
		//Set the sprites hardbox
		//spr->SetHardbox(
		//	hbox_x, hbox_y, hbox_width, hbox_height
		//);
		//path = gd->m_required_textures;
		//Create the new hard box
		//spr->CreateHardBoxSpr(path);
		
		//if (is_player)
		//{
		//	//Add the sprite to the map
		//	map->AddPlayer(spr);
		//}
		//else
		//{
		//	//Add the sprite to the map
		//	map->AddSprite(spr);
		//}

		////Add the animation
		//if (strcmp(animation_name_str, "NONE") != 0)
		//	map->AddSpriteAnimations(string(animation_name_str), gd->GAME_FOLDER, gd->m_sprites_path, spr->ID());
	} while (spritesParser.ArrayElementNotNull("Sprite"));
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLLoader::LoadWorldXML(string worldname, STopdownWorld*& sworld)
{
	if (sworld == NULL)
		sworld = new STopdownWorld();
	
	XMLParser xmlParser(worldname);

	const char* name_str = xmlParser.ReadElement("name");
	sworld->sWorldName = string(name_str);
	
	ArrayElementXMLParser worldMapParser(xmlParser.Element());
	worldMapParser.ReadArrayElement("Maps", "Map");
	while (worldMapParser.ArrayElementNotNull("Map"))
	{
		string eName = worldMapParser.ReadArrayElementValue();
		string eIDAtt = worldMapParser.ReadArrayElementAttribute("id");
		
		const char* map_name_str = worldMapParser.ReadArrayChildElement("map_name");
		
		sworld->m_mapNames.push_back(map_name_str);
	}
	
	return true;
}
//==================================================================================================================================
bool ZShadeXMLLoader::LoadWorldXML(string worldname, SPlatformerWorld*& sworld)
{
	if (sworld == NULL)
		sworld = new SPlatformerWorld();
	
	XMLParser xmlParser(worldname);

	const char* name_str = xmlParser.ReadElement("name");
	sworld->sWorldName = string(name_str);
	
	ArrayElementXMLParser worldMapParser(xmlParser.Element());
	worldMapParser.ReadArrayElement("Maps", "Map");
	while (worldMapParser.ArrayElementNotNull("Map"))
	{
		string eName = worldMapParser.ReadArrayElementValue();
		string eIDAtt = worldMapParser.ReadArrayElementAttribute("id");
		
		const char* map_name_str = worldMapParser.ReadArrayChildElement("map_name");
		
		sworld->m_mapNames.push_back(map_name_str);
	}

	return true;
}
//===============================================================================================================================
bool ZShadeXMLLoader::LoadHUDXML(string filename, GameDirectory2D* gd, EngineOptions* eo, D3D* d3d, HUD*& hs)
{
	XMLParser xmlParser(filename);
	
	const char* hud_size_str = xmlParser.ReadElement("hud_size");
	BetterString hud_splite_str(hud_size_str);
	vector<string> split_str = hud_splite_str.split('x');
	int width = atoi(split_str[0].c_str());
	int height = atoi(split_str[1].c_str());
	
	if (hs == 0)
	{
		hs = new HUD(d3d);
	}
	
	hs->Width() = width;
	hs->Height() = height;
	
	ArrayElementXMLParser imagesParser(xmlParser.Element());
	imagesParser.ReadArrayElement("Images", "Image");
	while (imagesParser.ArrayElementNotNull("Image"))
	{
		string eName = imagesParser.ReadArrayElementValue();
		string eIDAtt = imagesParser.ReadArrayElementAttribute("id");
		
		const char* x_str = imagesParser.ReadArrayChildElement("image_x");
		const char* y_str = imagesParser.ReadArrayChildElement("image_y");
		const char* image_width_str = imagesParser.ReadArrayChildElement("image_width");
		const char* image_height_str = imagesParser.ReadArrayChildElement("image_height");
		const char* image_name_str = imagesParser.ReadArrayChildElement("image_name");
		
		ZShadeSandboxGraphics::Image* im = new ZShadeSandboxGraphics::Image(
			d3d,
			string(image_name_str),
			gd->m_hud_path,
			atof(x_str),
			atof(y_str),
			atoi(image_width_str),
			atoi(image_height_str)
		);

		hs->AddImage(im);
	}
	
	ArrayElementXMLParser textsParser(xmlParser.Element());
	textsParser.ReadArrayElement("Texts", "Text");
	while (textsParser.ArrayElementNotNull("Text"))
	{
		string eName = textsParser.ReadArrayElementValue();
		string eIDAtt = textsParser.ReadArrayElementAttribute("id");
		
		const char* x_str = textsParser.ReadArrayChildElement("text_x");
		const char* y_str = textsParser.ReadArrayChildElement("text_y");
		const char* text_str = textsParser.ReadArrayChildElement("text");
		const char* text_font_str = textsParser.ReadArrayChildElement("text_font");
		const char* text_color_str = textsParser.ReadArrayChildElement("text_color");
		const char* shadow_str = textsParser.ReadArrayChildElement("shadow");
		const char* optional_background_str = textsParser.ReadArrayChildElement("optional_background");
		
		XMFLOAT3 color = ZShadeSandboxGlobal::Convert::ConvertToFloat3(string(text_color_str));
		bool shadow = (string(shadow_str) == "1") ? true : false;
		
		ZShadeSandboxGraphics::Text* t = new ZShadeSandboxGraphics::Text();
		t->SetD3D(d3d);
		t->Initialize();
		t->TextName() = BetterString(text_str);
		if (shadow)
		{
			t->CreateShadow();
		}
		t->ChangeFont(L"Times New Roman", atoi(text_font_str), FontStyle::BoldItalic, true);
		t->TextTransform() = XMMatrixTranslation(atof(x_str), atof(y_str), 0);
		t->TextColor() = XMFLOAT4(color.x, color.y, color.z, 1);
		
		hs->AddText(t);
	}
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLLoader::LoadMapXML(string& mapname, string filename, GameDirectory2D* gd, D3D* d3d, TopdownMap*& map, bool inEditor)
{
	XMLParser xmlParser(filename);
	
	const char* map_name_str = xmlParser.ReadElement("map_name");
	const char* xml_sprite_str = xmlParser.ReadElement("xml_sprite");
	const char* map_type_str = xmlParser.ReadElement("map_type");
	const char* row_size_str = xmlParser.ReadElement("row_size");
	const char* col_size_str = xmlParser.ReadElement("col_size");
	const char* vision_str = xmlParser.ReadElement("vision");
	const char* music_str = xmlParser.ReadElement("music");
	
	int row_size = atoi(row_size_str);
	int col_size = atoi(col_size_str);

	d3d->GetEngineOptions()->SECTION_ROW_SIZE = row_size;
	d3d->GetEngineOptions()->SECTION_COL_SIZE = col_size;
	d3d->GetEngineOptions()->SetTileMax();
	
	bool fow_enabled;
	int fow_radius;
	bool fow_flashlight;
	
	xmlParser.ReadAttribute("Fow", "enabled", fow_enabled);
	xmlParser.ReadAttribute("Fow", "radius", fow_radius);
	xmlParser.ReadAttribute("Fow", "flashlight", fow_flashlight);
	
	MapLoadData mld;
	ArrayElementXMLParser tilesParser(xmlParser.Element());
	tilesParser.ReadArrayElement("Tiles", "Tile");
	while (tilesParser.ArrayElementNotNull("Tile"))
	{
		string eName = tilesParser.ReadArrayElementValue();
		string eIDAtt = tilesParser.ReadArrayElementAttribute("id");
		string eImageNameAtt = tilesParser.ReadArrayElementAttribute("image");
		
		const char* x_str = tilesParser.ReadArrayChildElement("x");
		const char* y_str = tilesParser.ReadArrayChildElement("y");
		
		const char* animation_str = tilesParser.ReadArrayChildElement("animation");
		const char* anim_speed_str = tilesParser.ReadArrayChildElement("anim_speed");
		const char* hard_str = tilesParser.ReadArrayChildElement("hard");
		
		int x = atoi(x_str);
		int y = atoi(y_str);
		int anim_speed = atoi(anim_speed_str);
		int hard = atoi(hard_str);
		
		mld.imageNames.push_back(eImageNameAtt);
		mld.animProfileNames.push_back(string(animation_str));
		mld.animSpeeds.push_back(anim_speed);
		mld.hards.push_back(hard);
	}
	
	Map2DType mt;
	if (BetterString(map_type_str) == "Regular")
		mt = Map2DType::REGULAR;
	else if (BetterString(map_type_str) == "Isometric")
		mt = Map2DType::ISOMETRIC;

	map = new TopdownMap(d3d, mt, gd);

	if (!map) return false;

	map->InEditor() = inEditor;

	map->Initialize(row_size);
	
	// Set the FOW data
	map->FOW() = fow_enabled;
	map->FOWRadius() = fow_radius;
	map->FOWFlashlight() = fow_flashlight;
	
	BetterString str(gd->m_xml_sprites_path);
	str += "\\";
	str += xml_sprite_str;

	//Load the sprites onto the map
	LoadSpritesXML(str, gd, d3d, map);
	
	return true;
}
//================================================================================================================
bool ZShadeXMLLoader::LoadMapXML(string& mapname, string filename, GameDirectory2D* gd, D3D* d3d, PlatformerMap*& map, bool inEditor)
{
	XMLParser xmlParser(filename);
	
	const char* map_name_str = xmlParser.ReadElement("map_name");
	const char* xml_sprite_str = xmlParser.ReadElement("xml_sprite");
	const char* xml_music_str = xmlParser.ReadElement("music");
	const char* width_str = xmlParser.ReadElement("width");
	const char* height_str = xmlParser.ReadElement("height");
	
	int map_width = atoi(width_str);
	int map_height = atoi(height_str);

	d3d->GetEngineOptions()->SECTION_ROW_SIZE = map_width;
	d3d->GetEngineOptions()->SECTION_COL_SIZE = map_height;
	d3d->GetEngineOptions()->SetTileMax();
	
	PlatformerMapLoadData pmld;
	ArrayElementXMLParser tilesParser(xmlParser.Element());
	tilesParser.ReadArrayElement("Tiles", "Tile");
	while (tilesParser.ArrayElementNotNull("Tile"))
	{
		string eName = tilesParser.ReadArrayElementValue();
		string eIDAtt = tilesParser.ReadArrayElementAttribute("id");
		string eImageNameAtt = tilesParser.ReadArrayElementAttribute("image");
		
		const char* x_str = tilesParser.ReadArrayChildElement("x");
		const char* y_str = tilesParser.ReadArrayChildElement("y");
		const char* animation_str = tilesParser.ReadArrayChildElement("animation");
		const char* anim_speed_str = tilesParser.ReadArrayChildElement("anim_speed");
		const char* hard_str = tilesParser.ReadArrayChildElement("hard");
		const char* invisible_str = tilesParser.ReadArrayChildElement("invisible");
		const char* physics_type_str = tilesParser.ReadArrayChildElement("physics_type");
		const char* surface_type_str = tilesParser.ReadArrayChildElement("surface_type");

		int x = atoi(x_str);
		int y = atoi(y_str);
		int anim_speed = atoi(anim_speed_str);
		int hard = atoi(hard_str);
		int invisible = atoi(invisible_str);
		
		pmld.imageNames.push_back(eImageNameAtt);
		pmld.animProfileNames.push_back(string(animation_str));
		pmld.animSpeeds.push_back(anim_speed);
		pmld.hards.push_back(hard);
		pmld.invisibles.push_back(invisible);
		pmld.physicsTypes.push_back(string(physics_type_str));
		pmld.surfaceTypes.push_back(string(surface_type_str));
	}
	
	map = new PlatformerMap(d3d, gd);
	if (!map) return false;
	
	map->MapName() = mapname;
	map->MusicName() = string(xml_music_str);
	map->Initialize(map_width, map_height, pmld);

	BetterString str(gd->m_xml_sprites_path);
	str += "\\";
	str += xml_sprite_str;

	//Load the sprites onto the map
	LoadSpritesXML(str.toString(), gd, d3d, map);
	
	return true;
}
//===============================================================================================================================