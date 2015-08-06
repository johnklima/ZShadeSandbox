#include "WinRadioGroup.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinRadioGroup::WinRadioGroup()
{

}
//--------------------------------------------------------------------------------------
WinRadioGroup::WinRadioGroup(HWND hWnd, int x, int y, int width, int height, int id)
{
	//WS_EX_CLIENTEDGE
    hGroup = CreateWindowEx( WS_EX_WINDOWEDGE, "Button", "",
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		x, y, width, height, hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);

	/*HWND hRButton1 = CreateWindowEx( WS_EX_WINDOWEDGE, "Button", "Yes",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
		x, y, width, height, hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);

	HWND hRButton2 = CreateWindowEx( WS_EX_WINDOWEDGE, "Button", "No",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		x, y, width, height, hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);*/
}
//--------------------------------------------------------------------------------------
WinRadioGroup::WinRadioGroup(const WinRadioGroup& o)
{

}
//--------------------------------------------------------------------------------------
WinRadioGroup::~WinRadioGroup()
{

}
//--------------------------------------------------------------------------------------
void WinRadioGroup::Init(HWND hWnd, int x, int y, int width, int height, int id)
{
	//WS_EX_CLIENTEDGE
	//hGroup = CreateWindowEx(WS_EX_WINDOWEDGE, "Button", "",
	//	WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
	//	x, y, width, height, hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);

	hGroup = CreateWindow("Button", "", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		x, y, width, height, hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);
}
//--------------------------------------------------------------------------------------
HWND WinRadioGroup::GetButtonHandle(int id)
{
	return hButtons[id];
}
//--------------------------------------------------------------------------------------
void WinRadioGroup::AddRadioButton(HWND hwnd, bool group, string text, int x, int y, int width, int height, int id)
{
	DWORD styles;

	if (group)
		styles = WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP;
	else
		styles = WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON;

	HWND hRButton = CreateWindowEx( WS_EX_WINDOWEDGE, "Button", text.c_str(),
		styles, x, y, width, height, hwnd, (HMENU)id, GetModuleHandle(NULL), NULL);

	hButtons.push_back( hRButton );
}
//--------------------------------------------------------------------------------------
void WinRadioGroup::Check(int id, bool bCheck)
{
	if (bCheck)
		SendMessage(hButtons[id], BM_SETCHECK, BST_CHECKED, 0);
	else
		SendMessage(hButtons[id], BM_SETCHECK, BST_UNCHECKED, 0);
}
//--------------------------------------------------------------------------------------
void WinRadioGroup::SetVisible(bool bVisable)
{
	ShowWindow(hGroup, bVisable);

	for (int i = 0; i < hButtons.size(); i++)
	{
		ShowWindow(hButtons[i], bVisable);
	}
}
//--------------------------------------------------------------------------------------
bool WinRadioGroup::IsVisible()
{
	if (!IsWindowVisible(hGroup))
		return false;

	bool visible = false;
	int tally = 0;

	for (int i = 0; i < hButtons.size(); i++)
	{
		if (IsWindowVisible(hButtons[i]))
		{
			tally++;
		}
	}

	if (tally == hButtons.size() || tally == 0)
		visible = true;

	return visible;
}
//--------------------------------------------------------------------------------------
void WinRadioGroup::Enable(bool bEnable)
{
	EnableWindow(hGroup, bEnable);

	for (int i = 0; i < hButtons.size(); i++)
	{
		EnableWindow(hButtons[i], bEnable);
	}
}
//--------------------------------------------------------------------------------------
bool WinRadioGroup::IsEnabled()
{
	if (!IsWindowEnabled(hGroup))
		return false;

	bool visible = false;
	int tally = 0;

	for (int i = 0; i < hButtons.size(); i++)
	{
		if (IsWindowEnabled(hButtons[i]))
		{
			tally++;
		}
	}

	if (tally == hButtons.size() || tally == 0)
		visible = true;

	return visible;
}
//--------------------------------------------------------------------------------------