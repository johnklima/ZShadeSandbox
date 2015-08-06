//--------------------------------------------------------------------------------------
// WinTimer
//--------------------------------------------------------------------------------------
#ifndef __WINTIMER_H
#define __WINTIMER_H
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
using namespace std;
//--------------------------------------------------------------------------------------
namespace GUIComponents {
//--------------------------------------------------------------------------------------
class WinTimer
{
public:
	WinTimer(HWND hWnd, int id);
	WinTimer(const WinTimer& o);
	~WinTimer();
	
	void Reset() { iTime = 0; }
	// Use WM_TIMER for Update
	void Update(int max_range=150);
	// Use WM_COMMAND for Set with a button click
	void Set(int val=5);
	void Kill();
	
	HWND GetHwnd() { return hHandle; }

private:
	int iTime;
	
	// ID of the timer
	int m_id;
	
	// Window handle for the timer
	HWND hHandle;
};
//--------------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
#endif __WINTIMER_H