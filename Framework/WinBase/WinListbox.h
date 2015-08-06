//--------------------------------------------------------------------------------------
// WinListbox
//--------------------------------------------------------------------------------------
#ifndef __WINLISTBOX_H
#define __WINLISTBOX_H
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <vector>
#include <CommCtrl.h>
#include <string>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinListbox
{
public:

	WinListbox() {}
	WinListbox(HWND hWnd, int x, int y, int width, int height, int id);
	WinListbox(const WinListbox& o);
	~WinListbox();

	void Init(HWND hWnd, int x, int y, int width, int height, int id);

	void AddItem(string text);
	string GetItem(WPARAM wParam);
	void DeleteItem(WPARAM wParam);
	void DeleteList();

	void SetVisible(bool bVisable);
	bool IsVisible();

	void Enable(bool bEnable);
	bool IsEnabled();

	HWND GetHwnd() { return hList; }

private:

	HWND hList;
	int m_id;
	int m_childID;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINLISTBOX_H