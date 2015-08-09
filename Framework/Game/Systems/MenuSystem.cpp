#include "MenuSystem.h"
//================================================================================================================
//================================================================================================================
MenuSystem::MenuSystem(D3D* d3d)
:   m_menu_to_render("")
,   m_d3d(d3d)
{
}
//================================================================================================================
MenuSystem::~MenuSystem()
{
}
//================================================================================================================
void MenuSystem::RemoveMenu(string menu_name)
{
	ZShadeSandboxEnvironment::Menu* menu = 0;
	auto current = m_menus.begin();
	
	while (current != m_menus.end())
	{
		menu = (*current).second;
		if (menu != 0 && menu->MenuName() == menu_name)
		{
			current = m_menus.erase(current);
		}
		else
		{
			++current;
		}
	}
	
	if (m_menu_to_render == menu_name)
	{
		// Just deleted the menu so there is nothing to render
		m_menu_to_render = "";
	}
}
//================================================================================================================
void MenuSystem::GetMenuNames(vector<string>& names)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.begin();
	for (; iter != m_menus.end(); iter++)
	{
		names.push_back(((*iter).first));
	}
}
//================================================================================================================
vector<ZShadeSandboxGraphics::Button*> MenuSystem::GetButtons(string menu_name)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(menu_name);
	if (iter != m_menus.end())
	{
		ZShadeSandboxEnvironment::Menu* menu = (*iter).second;
		return menu->GetButtons();
	}
}
//================================================================================================================
vector<ZShadeSandboxGraphics::Text*> MenuSystem::GetTexts(string menu_name)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(menu_name);
	if (iter != m_menus.end())
	{
		ZShadeSandboxEnvironment::Menu* menu = (*iter).second;
		return menu->GetTexts();
	}
}
//================================================================================================================
void MenuSystem::AddMenuNameToButton(string menu_name, int buttonID)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		ZShadeSandboxEnvironment::Menu* menu = (*iter).second;
		menu->AddMenuNameToButton(menu_name, buttonID);
	}
}
//================================================================================================================
void MenuSystem::RemoveButton(ZShadeSandboxGraphics::Button* b)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		ZShadeSandboxEnvironment::Menu* menu = (*iter).second;
		menu->RemoveButton(b);
	}
}
//================================================================================================================
void MenuSystem::RemoveText(ZShadeSandboxGraphics::Text* t)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		ZShadeSandboxEnvironment::Menu* menu = (*iter).second;
		menu->RemoveText(t);
	}
}
//================================================================================================================
void MenuSystem::RemoveButton(int id)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		ZShadeSandboxEnvironment::Menu* menu = (*iter).second;
		menu->RemoveButton(id);
	}
}
//================================================================================================================
void MenuSystem::RemoveText(int id)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		ZShadeSandboxEnvironment::Menu* menu = (*iter).second;
		menu->RemoveText(id);
	}
}
//================================================================================================================
bool MenuSystem::ButtonClicked(ZShadeSandboxGraphics::Button*& button, int x, int y)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		ZShadeSandboxEnvironment::Menu* menu = (*iter).second;
		return menu->ButtonClicked(button, x, y);
	}
	
	//Not current rendering menu
	return false;
}
//================================================================================================================
bool MenuSystem::TextClicked(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		ZShadeSandboxEnvironment::Menu* menu = (*iter).second;
		return menu->TextClicked(text, x, y);
	}
	
	//Not current rendering menu
	return false;
}
//================================================================================================================
void MenuSystem::AddMenu(ZShadeSandboxEnvironment::Menu* m)
{
	m_menus.insert( make_pair(m->MenuName(), m) );
}
//================================================================================================================
void MenuSystem::SetMenuToRender(string menu)
{
	m_menu_to_render = menu;
}
//================================================================================================================
/*void MenuSystem::AddMenuLinkName(string menu_name, string link_name)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		//The menu does exist
		ZShadeSandboxEnvironment::Menu* m = (*iter).second;
		m->LinkMenuName() = link_name;
		m_menus[menu_name] = m;
	}
}*/
//================================================================================================================
void MenuSystem::AddBackground(string menu_name, ZShadeSandboxGraphics::Image* image)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		//The menu does exist
		ZShadeSandboxEnvironment::Menu* m = (*iter).second;
		m->AddBackground(image);
		m_menus[menu_name] = m;
	}
}
//================================================================================================================
void MenuSystem::AddButton(
	string base_path
,	string normalImageName
,	string pushedImageName
,	string highlightedImageName
,	string disabledImageName
,	int x
,	int y
)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		//The menu does exist
		ZShadeSandboxEnvironment::Menu* m = (*iter).second;
		m->AddButton(base_path, normalImageName, pushedImageName, highlightedImageName, disabledImageName, x, y);
	}
}
//================================================================================================================
void MenuSystem::AddText(string text, int x, int y)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		//The menu does exist
		ZShadeSandboxEnvironment::Menu* m = (*iter).second;
		m->AddText(text, x, y);
	}
}
//================================================================================================================
void MenuSystem::AddButton(string menu_name, ZShadeSandboxGraphics::Button* button)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		//The menu does exist
		ZShadeSandboxEnvironment::Menu* m = (*iter).second;
		m->AddButton(button);
		m_menus[menu_name] = m;
	}
}
//================================================================================================================
void MenuSystem::AddText(string menu_name, ZShadeSandboxGraphics::Text* text)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		//The menu does exist
		ZShadeSandboxEnvironment::Menu* m = (*iter).second;
		m->AddText(text);
		m_menus[menu_name] = m;
	}
}
//================================================================================================================
void MenuSystem::MoveButton(ZShadeSandboxGraphics::Button*& button, int x, int y)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		//The menu does exist
		ZShadeSandboxEnvironment::Menu* m = (*iter).second;
		m->MoveButton(button, x, y);
	}
}
//================================================================================================================
void MenuSystem::MoveText(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		//The menu does exist
		ZShadeSandboxEnvironment::Menu* m = (*iter).second;
		m->MoveText(text, x, y);
	}
}
//================================================================================================================
void MenuSystem::UpdateButtonMouseUp(WPARAM btnState, int x, int y)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		(*iter).second->UpdateButtonMouseUp(btnState, x, y);
	}
}
//================================================================================================================
void MenuSystem::UpdateButtonMouseDown(WPARAM btnState, int x, int y)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		(*iter).second->UpdateButtonMouseDown(btnState, x, y);
	}
}
//================================================================================================================
void MenuSystem::UpdateButtonMouseOn(WPARAM btnState, int x, int y)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		(*iter).second->UpdateButtonMouseOn(btnState, x, y);
	}
}
//================================================================================================================
void MenuSystem::UpdateButtonMouseMove(WPARAM btnState, int x, int y)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		(*iter).second->UpdateButtonMouseMove(btnState, x, y);
	}
}
//================================================================================================================
bool MenuSystem::Render(Camera* camera, int blendAmount)
{
	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = m_menus.find(m_menu_to_render);
	if (iter != m_menus.end())
	{
		ZShadeSandboxEnvironment::Menu* menu = (*iter).second;
		menu->Render(camera, blendAmount);
	}
	
	return true;
}
//================================================================================================================