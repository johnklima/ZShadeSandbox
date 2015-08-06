#include "WinTextbox.h"
#include "Windowsx.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinTextbox::WinTextbox(HWND hWnd, int x, int y, int width, int height, int id)
{
	hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		x, y,
		width, height,
		hWnd,
		(HMENU)id,
		GetModuleHandle(NULL),
		NULL);
}
//--------------------------------------------------------------------------------------
WinTextbox::WinTextbox(const WinTextbox& o)
{

}
//--------------------------------------------------------------------------------------
WinTextbox::~WinTextbox()
{

}
//--------------------------------------------------------------------------------------
void WinTextbox::Init(HWND hWnd, int x, int y, int width, int height, int id)
{
	hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		x, y,
		width, height,
		hWnd,
		(HMENU)id,
		GetModuleHandle(NULL),
		NULL);
}
//--------------------------------------------------------------------------------------
void WinTextbox::SetText(string text)
{
	SendMessage(hEdit, WM_SETTEXT, NULL, (LPARAM)text.c_str());
}
//--------------------------------------------------------------------------------------
string WinTextbox::GetText()
{
	int nLength = Edit_GetTextLength(hEdit);
	if (nLength > 0)
	{
	   TCHAR *sText = new TCHAR[nLength + 1];
	   Edit_GetText(hEdit, sText, nLength + 1);
	   return sText;
	}
	return "";
}
//--------------------------------------------------------------------------------------
void WinTextbox::SetVisible(bool bVisable)
{
	ShowWindow(hEdit, bVisable);
}
//--------------------------------------------------------------------------------------
bool WinTextbox::IsVisible()
{
	return IsWindowVisible(hEdit);
}
//--------------------------------------------------------------------------------------
void WinTextbox::Enable(bool bEnable)
{
	EnableWindow(hEdit, bEnable);
}
//--------------------------------------------------------------------------------------
bool WinTextbox::IsEnabled()
{
	return IsWindowEnabled(hEdit);
}
//--------------------------------------------------------------------------------------