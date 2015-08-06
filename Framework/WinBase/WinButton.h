//--------------------------------------------------------------------------------------
// WinButton
//--------------------------------------------------------------------------------------
#ifndef __WINBUTTON_H
#define __WINBUTTON_H
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinButton
{
public:
	WinButton();
	WinButton(HWND hWnd, string text, int x, int y, int width, int height, int id);
	WinButton(const WinButton& o);
	~WinButton();

	void Init(HWND hWnd, string text, int x, int y, int width, int height, int id);

	HWND GetHwnd() { return hButton; }

	void SetVisible(bool bVisable);
	bool IsVisible();

	void Enable(bool bEnable);
	bool IsEnabled();

private:
	HWND hButton;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINBUTTON_H