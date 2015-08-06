//--------------------------------------------------------------------------------------
// WinToolbar
//--------------------------------------------------------------------------------------
#ifndef __WINTOOLBAR_H
#define __WINTOOLBAR_H
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinToolbar
{
public:
	WinToolbar() {}
	WinToolbar(HWND hWnd, int buttonSize, int numButtons, int id);
	WinToolbar(const WinToolbar& o);
	~WinToolbar();
	
	void Init(HWND hWnd, int buttonSize, int numButtons, int id);
	
	void AddButton(UINT id, UINT state, UINT style);
	void EnableButton(UINT id, bool enable=false);
	void Create();
	
	HWND GetHwnd() { return hToolbar; }

private:
	
	int iID;
	int iButtonSize;
	int iNumButtons;
	
	TBBUTTON* tbrButtons;
	
	// This is the count for the iBitmap flag
	int iCount;
	
	HWND hParent;
	HWND hToolbar;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINTOOLBAR_H