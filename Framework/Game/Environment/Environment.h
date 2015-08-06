//================================================================================================================
// Environment.h
//
// This will be the entry point for the game (Non-Editor)
//================================================================================================================
// History
//
// Created on 2/24/2015 by Dustin Watson
//================================================================================================================
#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H
//================================================================================================================
//================================================================================================================
#include <memory>
#include <Windows.h>
#include <string>
#include <windowsx.h>
#include <CommCtrl.h>
#include "EngineOptions.h"
#include "DXGameWindow.h"
#include "Environment2D.h"
#include "Environment3D.h"
//#include "OutdoorEnvironment.h"
//#include "BlurredEnvironment.h"
//#include "CubeReflectionOutdoorEnvironment.h"
//#include "GrassFieldEnvironment.h"
//#include "OceanEnvironment.h"
//#include "PlaneEnvironment.h"
//#include "VolumeEnvironment.h"
//#include "TessellationEnvironment.h"
//#include "ModelEnvironment.h"
//#include "TerrainEnvironment.h"
//#include "Window.h"
//class MapEditorSystem2D;
//class MenuEditorSystem;
//================================================================================================================
//================================================================================================================
namespace ZShadeSandboxEnvironment {
class Environment
{
public:
	
	Environment(EngineOptions* engineOptions);
	~Environment();
	
public:
	
	bool Init();
	
	void Run();
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	static Environment2D* Env2D() { return env2D; }
	static Environment3D* Env3D() { return env3D; }

private:
	
	EngineOptions* mEngineOptions;

	// These are used only by the editor
	//MapEditorSystem2D* m_MapEditorSystem2D;
	//MenuEditorSystem* m_MenuEditorSystem;
	//HUDEditorSystem* m_HUDEditorSystem;
	
	static Environment2D* env2D;
	static Environment3D* env3D;
};
}
//================================================================================================================
#endif//__ENVIRONMENT_H