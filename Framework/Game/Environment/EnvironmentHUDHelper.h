//================================================================================================================
// EnvironmentHUDHelper.h
//
//================================================================================================================
// History
//
// Created on 3/11/2015 by Dustin Watson
//================================================================================================================
#ifndef __ENVIRONMENTHUDHELPER_H
#define __ENVIRONMENTHUDHELPER_H
//================================================================================================================
//================================================================================================================
//C/C++ Includes
#include <memory>
#include <sstream>
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <map>
#include <vector>
#include "D3D.h"
#include "Input.h"
#include "Camera.h"
#include "TopdownMap.h"
#include "FogData.h"
#include "GameState.h"
#include "GameDirectory.h"
#include "ZShadeINIParser.h"
#include "TextureManager.h"
#include "CreateGameDirectory.h"
#include "ZShadeMessageCenter.h"
#include "MemoryMgr.h"
#include "ShapeContact.h"
#include "PlatformerBackground.h"
#include "Timer.h"
#include "MenuSystem.h"
#include "HUD.h"
#include "Environment2D.h"
using namespace std;
//================================================================================================================
//================================================================================================================
class EnvironmentHUDHelper
{
public:
	EnvironmentHUDHelper();
	~EnvironmentHUDHelper();
	
	// Functions needed by LUA scripting engine and map editor as well as creating a menu
	
	static void LoadHUD(string hud_filename, string& hud_name);
	static void SaveHUD(string hud_filename);
	static void CreateHUD(string hud_name);
	
	//
	// Image Updating
	//
	
	// Attempts to locate a button based on the x,y position and returns if a button was clicked
	static bool ImageClicked(ZShadeSandboxGraphics::Image*& image, int x, int y);
	static bool MoveImageStart(ZShadeSandboxGraphics::Image*& image, int& imageID, int x, int y);
	static void MoveImageEnd(ZShadeSandboxGraphics::Image*& image, int imageID, int x, int y);
	static bool CloneImageStart(ZShadeSandboxGraphics::Image*& image, int x, int y);
	static void MoveImage(ZShadeSandboxGraphics::Image*& image, int x, int y);
	static void CloneImageEnd(ZShadeSandboxGraphics::Image*& image, int x, int y);
	static void AddImage(ZShadeSandboxGraphics::Image* image);
	static void AddImage(string base_path, string textureName, int x, int y, int w, int h);
	static void DeleteImage(ZShadeSandboxGraphics::Image* image);
	static void DeleteImage(int id);
	static void ResizeImageUpdate(
		ZShadeSandboxGraphics::Image*& image,
		bool updatePosition,
		bool updateSize,
		bool goFast,
		bool pressedLeftKey,
		bool pressedRightKey,
		bool pressedUpKey,
		bool pressedDownKey
	);
	static void HighlightImage(ZShadeSandboxGraphics::Image*& imageWithBox, Sprite*& highlightBox, bool& boxCreated, int x, int y);
	static bool GetImageID(int& imageID, int x, int y);
	
	//
	// Text Updating
	//
	
	// Attempts to locate a text based on the x,y position and returns if a text was clicked
	static bool TextClicked(ZShadeSandboxGraphics::Text*& text, int x, int y);
	static bool MoveTextStart(ZShadeSandboxGraphics::Text*& text, int& textID, int x, int y);
	static void MoveTextEnd(ZShadeSandboxGraphics::Text*& text, int textID, int x, int y);
	static bool CloneTextStart(ZShadeSandboxGraphics::Text*& text, int x, int y);
	static void MoveText(ZShadeSandboxGraphics::Text*& text, int x, int y);
	static void CloneTextEnd(ZShadeSandboxGraphics::Text*& text, int x, int y);
	static void AddText(ZShadeSandboxGraphics::Text* text);
	static void AddText(string text, int x, int y);
	static void DeleteText(ZShadeSandboxGraphics::Text* text);
	static void DeleteText(int id);
	static void HighlightText(ZShadeSandboxGraphics::Text*& textWithBox, Sprite*& highlightBox, bool& boxCreated, int x, int y);
	static bool GetTextID(int& textID, int x, int y);
};
//================================================================================================================
//================================================================================================================
#endif//__ENVIRONMENTHUDHELPER_H