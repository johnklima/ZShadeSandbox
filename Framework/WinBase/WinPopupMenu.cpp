#include "WinPopupMenu.h"
#include "Windowsx.h"
#include "BetterString.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinPopupMenu::WinPopupMenu(HWND hWnd, int x, int y)
{
	hParent = hWnd;
	
	hPopup = CreatePopupMenu();
	
	p.x = x;
	p.y = y;
	
	ClientToScreen(hWnd, &p);
}
//--------------------------------------------------------------------------------------
WinPopupMenu::WinPopupMenu(const WinPopupMenu& o)
{
}
//--------------------------------------------------------------------------------------
WinPopupMenu::~WinPopupMenu()
{
}
//--------------------------------------------------------------------------------------
void WinPopupMenu::AddMenuItem(UINT flag, UINT_PTR uIDNewItem, string text)
{
	unique_ptr<wchar_t> name = BetterString(text).ToWideStr();
	AppendMenuW(hPopup, flag, uIDNewItem, name.get());
}
//--------------------------------------------------------------------------------------
void WinPopupMenu::TrackRightButton()
{
	TrackPopupMenu(hPopup, TPM_RIGHTBUTTON, p.x, p.y, 0, hParent, NULL);
}
//--------------------------------------------------------------------------------------
void WinPopupMenu::Destroy()
{
	DestroyMenu(hPopup);
}
//--------------------------------------------------------------------------------------
