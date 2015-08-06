#include "WinTrackbar.h"
#include "Windowsx.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinTrackbar::WinTrackbar(HWND hWnd, string leftText, string rightText, int x, int y, int width, int height, int id)
{
	/*
	hTrack = CreateWindowW(L"msctls_trackbar32", L"Trackbar Control",
      WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
      20, 20, 170, 30, hwnd, (HMENU) 3, NULL, NULL);

  SendMessageW(hTrack, TBM_SETRANGE,  TRUE, MAKELONG(0, 100)); 
  SendMessageW(hTrack, TBM_SETPAGESIZE, 0,  10); 
  SendMessageW(hTrack, TBM_SETTICFREQ, 10, 0); 
  SendMessageW(hTrack, TBM_SETPOS, FALSE, 0); 
  SendMessageW(hTrack, TBM_SETBUDDY, TRUE, (LPARAM) hLeftLabel); 
  SendMessageW(hTrack, TBM_SETBUDDY, FALSE, (LPARAM) hRightLabel); 
	*/
	
	hTrackbar = CreateWindow("msctls_trackbar32", "Trackbar Control",
      WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
      x, y, width, height, hWnd, (HMENU)id, NULL, NULL);
	
	leftLabel.Init(hWnd, leftText, x, y, 10, height, 1);
	rightLabel.Init(hWnd, rightText, x, y, 30, height, 2);
	
	SendMessage(hTrackbar, TBM_SETBUDDY, TRUE, (LPARAM)((HWND)leftLabel));
	SendMessage(hTrackbar, TBM_SETBUDDY, FALSE, (LPARAM)((HWND)rightLabel));
}
//--------------------------------------------------------------------------------------
WinTrackbar::WinTrackbar(const WinTrackbar& o)
{
}
//--------------------------------------------------------------------------------------
WinTrackbar::~WinTrackbar()
{
}
//--------------------------------------------------------------------------------------
void WinTrackbar::Init(HWND hWnd, string leftText, string rightText, int x, int y, int width, int height, int id)
{
	hTrackbar = CreateWindow("msctls_trackbar32", "Trackbar Control",
		WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
		x, y, width, height, hWnd, (HMENU)id, NULL, NULL);

	leftLabel.Init(hWnd, leftText, x, y, 10, height, 1);
	rightLabel.Init(hWnd, rightText, x, y, 30, height, 2);

	SendMessage(hTrackbar, TBM_SETBUDDY, TRUE, (LPARAM)((HWND)leftLabel));
	SendMessage(hTrackbar, TBM_SETBUDDY, FALSE, (LPARAM)((HWND)rightLabel));
}
//--------------------------------------------------------------------------------------
void WinTrackbar::SetRange(int first, int last)
{
	SendMessage(hTrackbar, TBM_SETRANGE, TRUE, MAKELONG(first, last));
}
//--------------------------------------------------------------------------------------
void WinTrackbar::SetPageSize(int first, int last)
{
	SendMessage(hTrackbar, TBM_SETPAGESIZE, first, last);
}
//--------------------------------------------------------------------------------------
void WinTrackbar::SetTicFreq(int first, int last)
{
	SendMessage(hTrackbar, TBM_SETTICFREQ, first, last);
}
//--------------------------------------------------------------------------------------
void WinTrackbar::SetPos(WPARAM wParam, LPARAM lParam)
{
	SendMessage(hTrackbar, TBM_SETPOS, wParam, lParam);
}
//--------------------------------------------------------------------------------------
void WinTrackbar::Update(UINT msg)
{
	switch (msg)
	{
		case WM_HSCROLL:// Could also do WM_VSCROLL
		{
			iPos = SendMessageW(hTrackbar, TBM_GETPOS, 0, 0);
			//wchar_t buf[4];
			//wsprintfW(buf, L"%ld", iPos);
		}
		break;
	}
}
//--------------------------------------------------------------------------------------