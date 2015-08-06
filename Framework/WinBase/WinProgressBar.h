//--------------------------------------------------------------------------------------
// WinProgressBar
//--------------------------------------------------------------------------------------
#ifndef __WINPROGRESSBAR_H
#define __WINPROGRESSBAR_H
//--------------------------------------------------------------------------------------
#include "WinTimer.h"
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinProgressBar
{
public:
	WinProgressBar(HWND hWnd, int x, int y, int width, int height, int id, int timer_id, int timer_range=150, int time_val=5);
	WinProgressBar(const WinProgressBar& o);
	~WinProgressBar();
	
	void SetRange(int first=0, int last=150);
	void SetStep(int val=1);
	
	void Step();
	void SetPos(WPARAM wParam, LPARAM lParam);
	
	// This goes into the window message loop
	void Update(UINT msg);
	
	void SetVisible(bool bVisable);
	bool IsVisible();

	void Enable(bool bEnable);
	bool IsEnabled();

	HWND GetHwnd() { return hProgressBar; }

private:
	HWND hProgressBar;
	WinTimer* timer;
	int iTimerRange;
	int iTimeVal;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINPROGRESSBAR_H