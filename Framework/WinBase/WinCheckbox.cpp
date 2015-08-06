#include "WinCheckbox.h"
#include "Windowsx.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinCheckbox::WinCheckbox(HWND hWnd, string text, int x, int y, int width, int height, int id)
{
	iID = id;
	hParent = hWnd;
	
	hHandle = CreateWindowEx(NULL, 
		"BUTTON", text.c_str(),
		WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
		x, y, width, height, hWnd, (HMENU)id,
		GetModuleHandle(NULL),
		NULL);
}
//--------------------------------------------------------------------------------------
WinCheckbox::WinCheckbox(const WinCheckbox& o)
{
}
//--------------------------------------------------------------------------------------
WinCheckbox::~WinCheckbox()
{
}
//--------------------------------------------------------------------------------------
void WinCheckbox::Init(HWND hWnd, string text, int x, int y, int width, int height, int id)
{
	iID = id;
	hParent = hWnd;

	hHandle = CreateWindowEx(NULL,
		"BUTTON", text.c_str(),
		WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
		x, y, width, height, hWnd, (HMENU)id,
		GetModuleHandle(NULL),
		NULL);
}
//--------------------------------------------------------------------------------------
void WinCheckbox::Check(bool check)
{
	CheckDlgButton(hParent, iID, (check==true)?BST_CHECKED:BST_UNCHECKED);
}
//--------------------------------------------------------------------------------------
bool WinCheckbox::IsChecked()
{
	return IsDlgButtonChecked(hParent, iID);
}
//--------------------------------------------------------------------------------------
void WinCheckbox::SetVisible(bool bVisable)
{
	ShowWindow(hHandle, bVisable);
}
//--------------------------------------------------------------------------------------
bool WinCheckbox::IsVisible()
{
	return IsWindowVisible(hHandle);
}
//--------------------------------------------------------------------------------------
void WinCheckbox::Enable(bool bEnable)
{
	EnableWindow(hHandle, bEnable);
}
//--------------------------------------------------------------------------------------
bool WinCheckbox::IsEnabled()
{
	return IsWindowEnabled(hHandle);
}
//--------------------------------------------------------------------------------------