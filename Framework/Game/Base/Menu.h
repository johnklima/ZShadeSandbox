//*************************************************************************************************************************
// Menu.h
//*************************************************************************************************************************
#ifndef __MENU_H
#define __MENU_H
//================================================================================================================
//================================================================================================================

//
//Includes
//

#include <string>
#include <vector>
#include "Image.h"
#include "Button.h"
#include "Text.h"
using namespace std;
class Menu;

//================================================================================================================
//================================================================================================================
namespace ZShadeSandboxEnvironment {
namespace EMenuType
{
	enum Type
	{
		eMainMenu,
		eInGameMenu
	};
}
//================================================================================================================
//================================================================================================================
class Menu
{
public:
	
	Menu(D3D* d3d);
	~Menu();
	
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
	void AddBackground(ZShadeSandboxGraphics::Image* image);
	void AddButton(ZShadeSandboxGraphics::Button* button);
	void AddText(ZShadeSandboxGraphics::Text* text);
	void RemoveButton(ZShadeSandboxGraphics::Button* b);
	void RemoveText(ZShadeSandboxGraphics::Text* t);
	void RemoveButton(int id);
	void RemoveText(int id);
	bool ButtonClicked(ZShadeSandboxGraphics::Button*& button, int x, int y);
	bool TextClicked(ZShadeSandboxGraphics::Text*& text, int x, int y);
	void MoveButton(ZShadeSandboxGraphics::Button*& button, int x, int y);
	void MoveText(ZShadeSandboxGraphics::Text*& text, int x, int y);
	void AddMenuNameToButton(string menu_name, int buttonID);
	
	void UpdateButtonMouseUp(WPARAM btnState, int x, int y);
	void UpdateButtonMouseDown(WPARAM btnState, int x, int y);
	void UpdateButtonMouseOn(WPARAM btnState, int x, int y);
	void UpdateButtonMouseMove(WPARAM btnState, int x, int y);
	
	bool Render(Camera* camera, int blendAmount);
	
public:
	
	string GetBackgroundName() { return m_background->GetName(); }
	vector<ZShadeSandboxGraphics::Button*> GetButtons() { return m_menu_buttons; }
	vector<ZShadeSandboxGraphics::Text*> GetTexts() { return m_menu_text; }
	int& Width() { return m_menu_width; }
	int Width() const { return m_menu_width; }
	int& Height() { return m_menu_height; }
	int Height() const { return m_menu_height; }
	//string& LinkMenuName() { return m_link_menu_name; }
	//string LinkMenuName() const { return m_link_menu_name; }
	string& MenuName() { return m_menu_name; }
	string MenuName() const { return m_menu_name; }
	EMenuType::Type& GetMenuType() { return m_menu_type; }
	EMenuType::Type GetMenuType() const { return m_menu_type; }
	
private:
	
	D3D* mD3D;
	
	int m_menu_width;
	int m_menu_height;
	
	EMenuType::Type m_menu_type;
	string m_menu_name;
	//string m_link_menu_name;
	
	int mButtonID;
	int mTextID;
	
	ZShadeSandboxGraphics::Image* m_background;
	vector<ZShadeSandboxGraphics::Button*> m_menu_buttons;
	vector<ZShadeSandboxGraphics::Text*> m_menu_text;
};
}
//================================================================================================================
//================================================================================================================
#endif//__MENU_H