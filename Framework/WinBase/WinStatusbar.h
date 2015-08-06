//--------------------------------------------------------------------------------------
// WinStatusbar
//--------------------------------------------------------------------------------------
#ifndef __WINSTATUSBAR_H
#define __WINSTATUSBAR_H
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinStatusbar
{
public:
	WinStatusbar(HWND hWnd, int idStatus, int cParts);
	WinStatusbar(const WinStatusbar& o);
	~WinStatusbar();
	
	void SetText(string text);
	
	HWND GetHwnd() { return hStatusbar; }

private:
	
	HWND hStatusbar;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINSTATUSBAR_H