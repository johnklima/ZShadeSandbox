//--------------------------------------------------------------------------------------
// WinPopupMenu
//--------------------------------------------------------------------------------------
#ifndef __WINPOPUPMENU_H
#define __WINPOPUPMENU_H
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinPopupMenu
{
public:
	// x = LOWORD(lParam)
	// y = HIWORD(lParam)
	WinPopupMenu(HWND hWnd, int x, int y);
	WinPopupMenu(const WinPopupMenu& o);
	~WinPopupMenu();
	
	void AddMenuItem(UINT flag, UINT_PTR uIDNewItem, string text);
	void TrackRightButton();
	void Destroy();
	
	HWND GetHwnd() { return hParent; }

private:
	POINT p;
	
	HWND hParent;
	HMENU hPopup;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINPOPUPMENU_H