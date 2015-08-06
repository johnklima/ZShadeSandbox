//--------------------------------------------------------------------------------------
// WinText
//--------------------------------------------------------------------------------------
#ifndef __WINTEXT_H
#define __WINTEXT_H
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinText
{
public:
	WinText() {}
	WinText(HWND hWnd, string text, int x, int y, int width, int height, int id);
	WinText(const WinText& o);
	~WinText();

	void Init(HWND hWnd, string text, int x, int y, int width, int height, int id);

	void SetFont(int height = 12, int weight = FW_DONTCARE, bool italic = 0, bool underline = 0);
	void SetText(string text);

	void SetVisible(bool bVisable);
	bool IsVisible();

	void Enable(bool bEnable);
	bool IsEnabled();

	HWND GetHwnd() { return hStatic; }

private:
	HWND hStatic;
	HFONT hfFont;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINTEXT_H