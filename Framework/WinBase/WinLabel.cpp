#include "WinLabel.h"
#include "Windowsx.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinLabel::WinLabel(HWND hWnd, string text, int x, int y, int width, int height, int id)
{
	//hLabel = CreateWindow("STATIC", text.c_str(), 
	//	WS_CHILD | WS_VISIBLE, x, y, width, height, hWnd, (HMENU)id, NULL, NULL);

	hLabel = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", text.c_str(),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		x, y, width, height, hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);
}
//--------------------------------------------------------------------------------------
WinLabel::WinLabel(const WinLabel& o)
{

}
//--------------------------------------------------------------------------------------
WinLabel::~WinLabel()
{

}
//--------------------------------------------------------------------------------------
void WinLabel::Init(HWND hWnd, string text, int x, int y, int width, int height, int id)
{
	hLabel = CreateWindow("STATIC", text.c_str(),
		WS_CHILD | WS_VISIBLE, x, y, width, height, hWnd, (HMENU)id, NULL, NULL);

	/*hLabel = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", text.c_str(),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		x, y, width, height, hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);*/
}
//--------------------------------------------------------------------------------------
void WinLabel::SetVisible(bool bVisable, HWND wnd, int id)
{
	HWND hwnd = GetDlgItem(wnd, id);

	//ShowWindow(hLabel, bVisable);
	if (bVisable)
		ShowWindow(hwnd, SW_SHOW);
	else
		ShowWindow(hwnd, SW_HIDE);
}
//--------------------------------------------------------------------------------------
void WinLabel::SetVisible(bool bVisable)
{
	ShowWindow(hLabel, bVisable);
}
//--------------------------------------------------------------------------------------
void WinLabel::Invalidate()
{
	InvalidateRect(hLabel, NULL, TRUE);
}
//--------------------------------------------------------------------------------------
bool WinLabel::IsVisible()
{
	return IsWindowVisible(hLabel);
}
//--------------------------------------------------------------------------------------
void WinLabel::Enable(bool bEnable)
{
	//EnableWindow(hLabel, bEnable);
}
//--------------------------------------------------------------------------------------
bool WinLabel::IsEnabled()
{
	return IsWindowEnabled(hLabel);
}
//--------------------------------------------------------------------------------------
void WinLabel::SetText(string text)
{
	SendMessage(hLabel, WM_SETTEXT, NULL, (LPARAM)text.c_str());
}
//--------------------------------------------------------------------------------------