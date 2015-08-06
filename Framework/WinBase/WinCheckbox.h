//--------------------------------------------------------------------------------------
// WinCheckbox
//--------------------------------------------------------------------------------------
#ifndef __WINCHECKBOX_H
#define __WINCHECKBOX_H
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinCheckbox
{
public:
	WinCheckbox() {}
	WinCheckbox(HWND hWnd, string text, int x, int y, int width, int height, int id);
	WinCheckbox(const WinCheckbox& o);
	~WinCheckbox();
	
	void Init(HWND hWnd, string text, int x, int y, int width, int height, int id);

	void Check(bool check=true);
	bool IsChecked();
	
	HWND GetHwnd() { return hHandle; }

	void SetVisible(bool bVisable);
	bool IsVisible();

	void Enable(bool bEnable);
	bool IsEnabled();

private:
	
	HWND hParent;
	HWND hHandle;
	
	int iID;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINCHECKBOX_H