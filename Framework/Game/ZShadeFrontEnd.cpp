#include "ZShadeFrontEnd.h"
#include "CGlobal.h"
#include "MemoryMgr.h"
#include "ZShadeResource.h"
#include "ZShadeMessageCenter.h"
#include "ZShadeINIParser.h"
//===============================================================================================================================
//===============================================================================================================================
ZShadeFrontEnd::ZShadeFrontEnd()
{
}
//===============================================================================================================================
ZShadeFrontEnd::~ZShadeFrontEnd()
{
}
//===============================================================================================================================
void ZShadeFrontEnd::Init(HINSTANCE hInstance)
{
	ZShadeINIParser iniParser("ZShadeSandbox.ini", true);

	string dx_version = iniParser.GetString("Editor", "DirectX");
	bool fullscreen = iniParser.GetBoolean("Editor", "Fullscreen");
	bool vsync = iniParser.GetBoolean("Editor", "vsync");
	bool fit_screen = iniParser.GetBoolean("Editor", "FitScreen");
	int screenWidth = iniParser.GetInt("Editor", "Screen Width");
	int screenHeight = iniParser.GetInt("Editor", "Screen Height");
	string dimension = iniParser.GetString("Editor", "Dim");
	string env3DType = iniParser.GetString("Editor", "Env3D");
	//int renderDistance2D = iniParser.GetInt("Editor", "RenderDistance2D");
	string transparency_key = iniParser.GetString("Editor", "TransparencyKey");
	string game_folder = iniParser.GetString("Editor", "GameFolder");
	//bool remember = iniParser.GetBoolean("Editor", "RememberMode");
	//string mode = iniParser.GetString("Editor", "Mode");
	float nearPlane = iniParser.GetFloat("Editor", "NearPlane");
	float farPlane = iniParser.GetFloat("Editor", "FarPlane");
	string type2d = iniParser.GetString("Editor", "Type2D");
	
	if (fit_screen)
	{
		//Find the maximum resolution support by the monitor
		CGlobal::GetDesktopResolution(screenWidth, screenHeight);
	}

	DimType dimType;
	if (dimension == "2D")
		dimType = ZSHADE_2D;
	else if (dimension == "3D")
		dimType = ZSHADE_3D;

	mEngineOptions = new EngineOptions();

	mEngineOptions->InitializeDefaults();

	if (dx_version == "11")
		mEngineOptions->m_d3dVersion = DIRECTX11;
	else if (dx_version == "10")
		mEngineOptions->m_d3dVersion = DIRECTX10;

	if (type2d == "Platformer")
		mEngineOptions->m_GameType2D = PLATFORMER;
	else if (type2d == "Topdown")
		mEngineOptions->m_GameType2D = TOPDOWN;

	//if (type2d == "Editor")
	//	mEngineOptions->m_inEditor = true;
	//else if (type2d == "Game")
	//	mEngineOptions->m_inEditor = false;
	
	XMFLOAT3 transparencyKey;
	BetterString tkey(transparency_key);
	vector<string> tkey_values = tkey.split(' ');
	transparencyKey.x = atoi(tkey_values[0].c_str());
	transparencyKey.y = atoi(tkey_values[1].c_str());
	transparencyKey.z = atoi(tkey_values[2].c_str());

	mEngineOptions->m_vsync = vsync;
	mEngineOptions->fullscreen = fullscreen;
	mEngineOptions->m_screenWidth = screenWidth;
	mEngineOptions->m_screenHeight = screenHeight;
	mEngineOptions->m_DimType = dimType;
	//mEngineOptions->m_renderDistance2D = renderDistance2D;
	mEngineOptions->m_TransparencyKey = transparencyKey;
	mEngineOptions->hinstance = hInstance;
	mEngineOptions->m_Env3DType = env3DType;
	mEngineOptions->m_GameFolderName = game_folder;
	mEngineOptions->fFarPlane = farPlane;
	mEngineOptions->fNearPlane = nearPlane;
}
//===============================================================================================================================
EngineOptions* ZShadeFrontEnd::GetEngineOptions()
{
	return mEngineOptions;
}
//===============================================================================================================================
void ZShadeFrontEnd::SetInEditor(bool inEditor)
{
	mEngineOptions->m_inEditor = inEditor;
}
//===============================================================================================================================