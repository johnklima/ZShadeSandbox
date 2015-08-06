#include "WinTooltip.h"
#include "Windowsx.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinTooltip::WinTooltip(HWND hWnd, string text)
{
	TOOLINFO ti;
	const char* c = text.c_str();
	char tooltip[30];
    RECT rect;
	
	strcpy_s(tooltip, c);

	hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,		
        0, 0, 0, 0, hWnd, NULL, NULL, NULL );
	
	SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	
	GetClientRect (hWnd, &rect);
	
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = hWnd;
    ti.hinst = NULL;
    ti.uId = 0;
    ti.lpszText = tooltip;
    ti.rect.left = rect.left;    
    ti.rect.top = rect.top;
    ti.rect.right = rect.right;
    ti.rect.bottom = rect.bottom;
	
    SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
}
//--------------------------------------------------------------------------------------
WinTooltip::WinTooltip(const WinTooltip& o)
{
}
//--------------------------------------------------------------------------------------
WinTooltip::~WinTooltip()
{
}
//--------------------------------------------------------------------------------------
