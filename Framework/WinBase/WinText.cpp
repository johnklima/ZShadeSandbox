#include "WinText.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinText::WinText(HWND hWnd, string text, int x, int y, int width, int height, int id)
{
	//WS_EX_CLIENTEDGE
    hStatic = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "",
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		x, y, width, height, hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);

	SetFont();
	SetText(text);

	//WS_EX_LAYERED
	//SetLayeredWindowAttributes( hStatic, RGB(255,255,255), 0, LWA_COLORKEY );
}
//--------------------------------------------------------------------------------------
WinText::WinText(const WinText& o)
{

}
//--------------------------------------------------------------------------------------
WinText::~WinText()
{

}
//--------------------------------------------------------------------------------------
void WinText::Init(HWND hWnd, string text, int x, int y, int width, int height, int id)
{
	//WS_EX_CLIENTEDGE
	hStatic = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "",
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		x, y, width, height, hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);

	SetFont();
	SetText(text);

	//WS_EX_LAYERED
	//SetLayeredWindowAttributes( hStatic, RGB(255,255,255), 0, LWA_COLORKEY );
}
//--------------------------------------------------------------------------------------
void WinText::SetFont(int height, int weight, bool italic, bool underline)
{
    hfFont = CreateFont(height, 0, 0, 0, weight, italic, underline, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        DEFAULT_PITCH, "Verdana");
}
//--------------------------------------------------------------------------------------
void WinText::SetText(string text)
{
	SendMessage(hStatic, WM_SETTEXT, NULL, (LPARAM)text.c_str());

    SendMessage(hStatic, WM_SETFONT, (WPARAM)hfFont, NULL);
}
//--------------------------------------------------------------------------------------
void WinText::SetVisible(bool bVisable)
{
	ShowWindow(hStatic, bVisable);
}
//--------------------------------------------------------------------------------------
bool WinText::IsVisible()
{
	return IsWindowVisible(hStatic);
}
//--------------------------------------------------------------------------------------
void WinText::Enable(bool bEnable)
{
	EnableWindow(hStatic, bEnable);
}
//--------------------------------------------------------------------------------------
bool WinText::IsEnabled()
{
	return IsWindowEnabled(hStatic);
}
//--------------------------------------------------------------------------------------