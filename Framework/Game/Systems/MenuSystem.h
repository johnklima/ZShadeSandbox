//*************************************************************************************************************************
// MenuSystem.h
//*************************************************************************************************************************
#ifndef __MENUSYSTEM_H
#define __MENUSYSTEM_H
//================================================================================================================
//================================================================================================================
//Includes
#include <string>
#include <map>
#include "Menu.h"
#include "Button.h"
#include "Text.h"
using namespace std;
//================================================================================================================
//================================================================================================================
// http://kotaku.com/5955855/the-ten-commandments-of-video-game-menus
class MenuSystem
{
public:
	
	MenuSystem(D3D* d3d);
	~MenuSystem();
	
	void GetMenuNames(vector<string>& names);
	void SetMenuToRender(string menu);
	void AddMenu(ZShadeSandboxEnvironment::Menu* m);
	void RemoveMenu(string menu_name);
	
	void AddButton(
		string base_path,
		string normalImageName,
		string pushedImageName,
		string highlightedImageName,
		string disabledImageName,
		int x,
		int y
	);
	
	void AddText(string text, int x, int y);
	
	//void AddMenuLinkName(string menu_name, string link_name);
	void AddMenuNameToButton(string menu_name, int buttonID);
	void AddBackground(string menu_name, ZShadeSandboxGraphics::Image* image);
	void AddButton(string menu_name, ZShadeSandboxGraphics::Button* button);
	void AddText(string menu_name, ZShadeSandboxGraphics::Text* text);
	void RemoveButton(ZShadeSandboxGraphics::Button* b);
	void RemoveText(ZShadeSandboxGraphics::Text* t);
	void RemoveButton(int id);
	void RemoveText(int id);
	bool ButtonClicked(ZShadeSandboxGraphics::Button*& button, int x, int y);
	bool TextClicked(ZShadeSandboxGraphics::Text*& text, int x, int y);
	void MoveButton(ZShadeSandboxGraphics::Button*& button, int x, int y);
	void MoveText(ZShadeSandboxGraphics::Text*& text, int x, int y);
	
	void UpdateButtonMouseUp(WPARAM btnState, int x, int y);
	void UpdateButtonMouseDown(WPARAM btnState, int x, int y);
	void UpdateButtonMouseOn(WPARAM btnState, int x, int y);
	void UpdateButtonMouseMove(WPARAM btnState, int x, int y);
	
	bool Render(Camera* camera, int blendAmount);
	
public:
	
	string GetMenuRenderName() { return m_menu_to_render; }
	map<string, ZShadeSandboxEnvironment::Menu*> GetMenus() { return m_menus; }
	vector<ZShadeSandboxGraphics::Button*> GetButtons(string menu_name);
	vector<ZShadeSandboxGraphics::Text*> GetTexts(string menu_name);
	ZShadeSandboxEnvironment::Menu* GetMenu(string name) { return m_menus[name]; }
	
	//ZShadeSandboxEnvironment::Menu*& MainMenu() { return mMainMenu; }
	//ZShadeSandboxEnvironment::Menu* MainMenu() const { return mMainMenu; }
	//ZShadeSandboxEnvironment::Menu*& InGameMenu() { return mInGameMenu; }
	//ZShadeSandboxEnvironment::Menu* InGameMenu() const { return mInGameMenu; }
	//bool& RenderMainMenu() { return bRenderMainMenu; }
	//bool RenderMainMenu() const { return bRenderMainMenu; }
	
private:
	
	D3D* m_d3d;
	
	//bool bRenderMainMenu;
	//ZShadeSandboxEnvironment::Menu* mMainMenu;
	//ZShadeSandboxEnvironment::Menu* mInGameMenu;
	
	string m_menu_to_render;
	map<string, ZShadeSandboxEnvironment::Menu*> m_menus;
};
//================================================================================================================
//================================================================================================================
#endif//__MENUSYSTEM_H