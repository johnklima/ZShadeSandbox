//--------------------------------------------------------------------------------------
// WinTextbox
//--------------------------------------------------------------------------------------
#ifndef __WINTEXTBOX_H
#define __WINTEXTBOX_H
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinTextbox
{
public:
	WinTextbox() {}
	WinTextbox(HWND hWnd, int x, int y, int width, int height, int id);
	WinTextbox(const WinTextbox& o);
	~WinTextbox();

	void Init(HWND hWnd, int x, int y, int width, int height, int id);

	string GetText();
	void SetText(string text);

	HWND GetHwnd() { return hEdit; }

	void SetVisible(bool bVisable);
	bool IsVisible();

	void Enable(bool bEnable);
	bool IsEnabled();

private:
	HWND hEdit;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINTEXTBOX_H