//--------------------------------------------------------------------------------------
// WinLabel
//--------------------------------------------------------------------------------------
#ifndef __WINLABEL_H
#define __WINLABEL_H
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinLabel
{
public:
	WinLabel() {}
	WinLabel(HWND hWnd, string text, int x, int y, int width, int height, int id);
	WinLabel(const WinLabel& o);
	~WinLabel();
	
	void Init(HWND hWnd, string text, int x, int y, int width, int height, int id);

	HWND GetHwnd() { return hLabel; }
	operator HWND () { return hLabel; }
	
	void SetVisible(bool bVisable, HWND wnd, int id);
	void SetVisible(bool bVisable);
	bool IsVisible();

	void Enable(bool bEnable);
	bool IsEnabled();

	void SetText(string text);

	void Invalidate();

private:
	HWND hLabel;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINLABEL_H