#include "Environment.h"
#include "ZShadeResource.h"
#include "ZShadeMessageCenter.h"
#include <functional>
#include "TerrainEnvironment.h"
#include "OutdoorEnvironment.h"
#include "OceanEnvironment.h"
#include "VolumeEnvironment.h"
#include "TessellationEnvironment.h"
#include "ModelEnvironment.h"
#include "BSPEnvironment.h"
//#include "MapEditorSystem2D.h"
//#include "MenuEditorSystem.h"
//#include "HUDEditorSystem.h"
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
using ZShadeSandboxEnvironment::Environment;
//================================================================================================================
//================================================================================================================
Environment2D* Environment::env2D = 0;
Environment3D* Environment::env3D = 0;
//================================================================================================================
Environment::Environment(EngineOptions* engineOptions)
:	mEngineOptions(engineOptions)
{
	Init();
}
//================================================================================================================
bool Environment::Init()
{
	switch (mEngineOptions->m_DimType)
	{
		case ZSHADE_2D:
		{
			env2D = new Environment2D(mEngineOptions);
		}
		break;
		case ZSHADE_3D:
		{
			if (mEngineOptions->m_Env3DType == "Outdoor")
				env3D = new OutdoorEnvironment(mEngineOptions);
			else if (mEngineOptions->m_Env3DType == "Ocean")
				env3D = new OceanEnvironment(mEngineOptions);
			else if (mEngineOptions->m_Env3DType == "Volume")
				env3D = new VolumeEnvironment(mEngineOptions);
			else if (mEngineOptions->m_Env3DType == "Tessellation")
				env3D = new TessellationEnvironment(mEngineOptions);
			else if (mEngineOptions->m_Env3DType == "Model")
				env3D = new ModelEnvironment(mEngineOptions);
			else if (mEngineOptions->m_Env3DType == "Terrain")
				env3D = new TerrainEnvironment(mEngineOptions);
			else if (mEngineOptions->m_Env3DType == "BSP")
				env3D = new BSPEnvironment(mEngineOptions);
			//else if (env3DType == "Editor")
			//	env3D = new Editor3DEnvironment(base_window_name, render_window_name, m_EngineOptions, init_lua);
		}
		break;
	}
	
	return true;
}
//================================================================================================================
Environment::~Environment()
{
}
//===============================================================================================================================
void Environment::OnMouseDown(WPARAM btnState, int x, int y)
{
	if (mEngineOptions == 0) return;

	switch (mEngineOptions->m_DimType)
	{
		case ZSHADE_2D:
		{
			env2D->OnMouseDown(btnState, x, y);
		}
		break;
		case ZSHADE_3D:
		{
			env3D->OnMouseDown(btnState, x, y);
		}
		break;
	}
}
//===============================================================================================================================
void Environment::OnMouseUp(WPARAM btnState, int x, int y)
{
	if (mEngineOptions == 0) return;

	switch (mEngineOptions->m_DimType)
	{
		case ZSHADE_2D:
		{
			env2D->OnMouseUp(btnState, x, y);
		}
		break;
		case ZSHADE_3D:
		{
			env3D->OnMouseUp(btnState, x, y);
		}
		break;
	}
}
//===============================================================================================================================
void Environment::OnMouseMove(WPARAM btnState, int x, int y)
{
	if (mEngineOptions == 0) return;

	switch (mEngineOptions->m_DimType)
	{
		case ZSHADE_2D:
		{
			env2D->OnMouseMove(btnState, x, y);
		}
		break;
		case ZSHADE_3D:
		{
			env3D->OnMouseMove(btnState, x, y);
		}
		break;
	}
}
//================================================================================================================
void Environment::Run()
{
	if (mEngineOptions == 0) return;

	switch (mEngineOptions->m_DimType)
	{
		case ZSHADE_2D:
		{
			env2D->Run();
		}
		break;
		case ZSHADE_3D:
		{
			env3D->Run();
		}
		break;
	}
}
//================================================================================================================