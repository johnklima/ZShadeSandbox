//--------------------------------------------------------------------------------------
// WinRadioGroup
//--------------------------------------------------------------------------------------
#ifndef __WINRADIOGROUP_H
#define __WINRADIOGROUP_H
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
#include <vector>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinRadioGroup
{
public:
	WinRadioGroup();
	WinRadioGroup(HWND hWnd, int x, int y, int width, int height, int id);
	WinRadioGroup(const WinRadioGroup& o);
	~WinRadioGroup();

	void Init(HWND hWnd, int x, int y, int width, int height, int id);

	void AddRadioButton(HWND hwnd, bool group, string text, int x, int y, int width, int height, int id);

	HWND GetButtonHandle(int id);
	HWND GetHwnd() { return hGroup; }

	void SetVisible(bool bVisable);
	bool IsVisible();

	void Enable(bool bEnable);
	bool IsEnabled();

	void Check(int id, bool bCheck);

private:
	HWND hGroup;
	HWND hwnd;

	vector< HWND > hButtons;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINRADIOGROUP_H