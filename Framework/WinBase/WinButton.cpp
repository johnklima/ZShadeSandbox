#include "WinButton.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinButton::WinButton()
{

}
//--------------------------------------------------------------------------------------
WinButton::WinButton(HWND hWnd, string text, int x, int y, int width, int height, int id)
{
	hButton = CreateWindowEx(NULL, 
		"BUTTON", text.c_str(),
		WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		x, y, width, height, hWnd, (HMENU)id,
		GetModuleHandle(NULL),
		NULL);
}
//--------------------------------------------------------------------------------------
WinButton::WinButton(const WinButton& o)
{

}
//--------------------------------------------------------------------------------------
WinButton::~WinButton()
{

}
//--------------------------------------------------------------------------------------
void WinButton::Init(HWND hWnd, string text, int x, int y, int width, int height, int id)
{
	hButton = CreateWindowEx(NULL,
		"BUTTON", text.c_str(),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		x, y, width, height, hWnd, (HMENU)id,
		GetModuleHandle(NULL),
		NULL);
}
//--------------------------------------------------------------------------------------
void WinButton::SetVisible(bool bVisable)
{
	ShowWindow(hButton, bVisable);
}
//--------------------------------------------------------------------------------------
bool WinButton::IsVisible()
{
	return IsWindowVisible(hButton);
}
//--------------------------------------------------------------------------------------
void WinButton::Enable(bool bEnable)
{
	EnableWindow(hButton, bEnable);
}
//--------------------------------------------------------------------------------------
bool WinButton::IsEnabled()
{
	return IsWindowEnabled(hButton);
}
//--------------------------------------------------------------------------------------