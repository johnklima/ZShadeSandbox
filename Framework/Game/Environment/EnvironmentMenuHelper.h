//================================================================================================================
// EnvironmentMenuHelper.h
//
//================================================================================================================
// History
//
// Created on 3/11/2015 by Dustin Watson
//================================================================================================================
#ifndef __ENVIRONMENTMENUHELPER_H
#define __ENVIRONMENTMENUHELPER_H
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
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
using namespace std;

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
class EnvironmentMenuHelper
{
public:
	EnvironmentMenuHelper();
	~EnvironmentMenuHelper();
	
	// Functions needed by LUA scripting engine and map editor as well as creating a menu
	
	static void AddBackground(ZShadeSandboxGraphics::Image* image);
	//static void AddMenuButton(string activeMenuName, ZShadeSandboxGraphics::Button* button);
	//static void AddMenuText(string activeMenuName, ZShadeSandboxGraphics::Text* text);
	static void GrabMenuNames(vector<string>& names);
	static void LoadAMenu(string menu_filename, string& menuName);
	static void SaveMenu(string menu_filename);
	static void CreateMenu(string menu_name);
	static void SetActiveRenderedMenu(string menu_name);
	static void RemoveMenu(string menu_name);
	
	//
	// Button Updating
	//
	
	// Attempts to locate a button based on the x,y position and returns if a button was clicked
	static bool ButtonClicked(ZShadeSandboxGraphics::Button*& button, int x, int y);
	static bool MoveButtonStart(ZShadeSandboxGraphics::Button*& button, int& buttonID, int x, int y);
	static void MoveButtonEnd(ZShadeSandboxGraphics::Button*& button, int buttonID, int x, int y);
	static bool CloneButtonStart(ZShadeSandboxGraphics::Button*& button, int x, int y);
	static void MoveButton(ZShadeSandboxGraphics::Button*& button, int x, int y);
	static void CloneButtonEnd(ZShadeSandboxGraphics::Button*& button, int x, int y);
	static void AddButton(ZShadeSandboxGraphics::Button* button);
	static void AddButton(
		string base_path,
		string normalImageName,
		string pushedImageName,
		string highlightedImageName,
		string disabledImageName,
		int x,
		int y
	);
	static void DeleteButton(ZShadeSandboxGraphics::Button* button);
	static void DeleteButton(int id);
	static void ResizeButtonUpdate(
		ZShadeSandboxGraphics::Button*& button,
		bool updatePosition,
		bool updateSize,
		bool goFast,
		bool pressedLeftKey,
		bool pressedRightKey,
		bool pressedUpKey,
		bool pressedDownKey
	);
	static void HighlightButton(ZShadeSandboxGraphics::Button*& buttonWithBox, Sprite*& highlightBox, bool& boxCreated, int x, int y);
	
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
};
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
#endif//__ENVIRONMENTMENUHELPER_H