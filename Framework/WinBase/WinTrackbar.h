//--------------------------------------------------------------------------------------
// WinTrackbar
//--------------------------------------------------------------------------------------
#ifndef __WINTRACKBAR_H
#define __WINTRACKBAR_H
//--------------------------------------------------------------------------------------
#include "WinLabel.h"
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinTrackbar
{
public:

	WinTrackbar() {}
	WinTrackbar(HWND hWnd, string leftText, string rightText, int x, int y, int width, int height, int id);
	WinTrackbar(const WinTrackbar& o);
	~WinTrackbar();
	
	void Init(HWND hWnd, string leftText, string rightText, int x, int y, int width, int height, int id);

	LRESULT GetPos() { return iPos; }
	
	void SetRange(int first=0, int last=100);
	void SetPageSize(int first=0, int last=10);
	void SetTicFreq(int first=10, int last=0);
	void SetPos(WPARAM wParam, LPARAM lParam);
	
	// This goes into the window message loop
	void Update(UINT msg);
	
	operator HWND () { return hTrackbar; }
	
private:
	HWND hTrackbar;
	
	WinLabel leftLabel;
	WinLabel rightLabel;
	
	LRESULT iPos;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINTRACKBAR_H