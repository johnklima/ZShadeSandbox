#include "WinProgressBar.h"
#include "Windowsx.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinProgressBar::WinProgressBar(HWND hWnd, int x, int y, int width, int height, int id, int timer_id, int timer_range, int time_val)
{
	hProgressBar = CreateWindowEx(0, PROGRESS_CLASS, NULL,
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
		x, y,
		width, height,
		hWnd,
		(HMENU)id,
		GetModuleHandle(NULL),
		NULL);
	
	timer = new WinTimer(hWnd, timer_id);
	
	iTimerRange = timer_range;
	iTimeVal = time_val;
}
//--------------------------------------------------------------------------------------
WinProgressBar::WinProgressBar(const WinProgressBar& o)
{

}
//--------------------------------------------------------------------------------------
WinProgressBar::~WinProgressBar()
{

}
//--------------------------------------------------------------------------------------
void WinProgressBar::SetRange(int first, int last)
{
	SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(first, last));
}
//--------------------------------------------------------------------------------------
void WinProgressBar::SetStep(int val)
{
	SendMessage(hProgressBar, PBM_SETSTEP, val, 0);
}
//--------------------------------------------------------------------------------------
void WinProgressBar::Step()
{
	SendMessage(hProgressBar, PBM_STEPIT, 0, 0);
}
//--------------------------------------------------------------------------------------
void WinProgressBar::SetPos(WPARAM wParam, LPARAM lParam)
{
	SendMessage(hProgressBar, PBM_SETPOS, wParam, lParam);
}
//--------------------------------------------------------------------------------------
void WinProgressBar::Update(UINT msg)
{
	switch (msg)
	{
		case WM_TIMER:
		{
			Step();
			
			timer->Update( iTimerRange );
		}
		break;
		case WM_COMMAND:// Used with a button
		{
			SetPos( 0, 0 );
			
			timer->Set( iTimeVal );
		}
		break;
		case WM_DESTROY:
		{
			timer->Kill();
		}
		break;
	}
}
//--------------------------------------------------------------------------------------
void WinProgressBar::SetVisible(bool bVisable)
{
	ShowWindow(hProgressBar, bVisable);
}
//--------------------------------------------------------------------------------------
bool WinProgressBar::IsVisible()
{
	return IsWindowVisible(hProgressBar);
}
//--------------------------------------------------------------------------------------
void WinProgressBar::Enable(bool bEnable)
{
	EnableWindow(hProgressBar, bEnable);
}
//--------------------------------------------------------------------------------------
bool WinProgressBar::IsEnabled()
{
	return IsWindowEnabled(hProgressBar);
}
//--------------------------------------------------------------------------------------