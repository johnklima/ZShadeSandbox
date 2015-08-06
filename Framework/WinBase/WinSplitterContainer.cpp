#include "WinSplitterContainer.h"
using GUIComponents::WinSplitterContainer;
//================================================================================================================
//================================================================================================================
WinSplitterContainer::WinSplitterContainer()
:   bSplitterMoving(false)
,   dwSplitterPosition(130)
,   bHasLeftChild(false)
,   bHasRightChild(false)
{
	hCurrentCursor = GetCursor();
	hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE));
}
//================================================================================================================
WinSplitterContainer::~WinSplitterContainer()
{
}
//================================================================================================================
void WinSplitterContainer::SetWindow1(Window* win)
{
	mWindow1 = win;
	leftWindowWidth = mWindow1->GetWidth();
}
//================================================================================================================
void WinSplitterContainer::SetWindow2(Window* win)
{
	mWindow2 = win;
	bHasLeftChild = true;
}
//================================================================================================================
void WinSplitterContainer::SetBaseWindow(Window* win)
{
	mBaseWindow = win;
	bHasRightChild = true;
}
//================================================================================================================
void WinSplitterContainer::UpdateSize(WPARAM wParam, LPARAM lParam)
{
	if ((wParam != SIZE_MINIMIZED) && LOWORD(lParam) < dwSplitterPosition)
	{
		dwSplitterPosition = LOWORD(lParam) - 10;
	}

	MoveWindow(mWindow1->GetHwnd(), 0, 0, dwSplitterPosition - 1, HIWORD(lParam), true);
	MoveWindow(mWindow2->GetHwnd(), dwSplitterPosition + 2, 0, HIWORD(lParam) - dwSplitterPosition - 2, HIWORD(lParam), true);

	mWindow1->Invalidate();
	mWindow2->Invalidate();

	//MoveWindow(mWindow1->GetHwnd(), 0, 0, LOWORD(lParam), dwSplitterPosition - 1, true);
	//MoveWindow(mWindow2->GetHwnd(), 0, dwSplitterPosition + 2, LOWORD(lParam), HIWORD(lParam) - dwSplitterPosition - 2, true);

	/*int TOP_POS = 1920;
	int WIDTH_ADJUST = 10;
	int BOTTOM_POS = 1080;

	RECT rect;
	GetClientRect(mBaseWindow->GetHwnd(), &rect);
	
	MoveWindow(mWindow1->GetHwnd(), rect.left, rect.top + TOP_POS, rect.left + (leftWindowWidth - WIDTH_ADJUST), (rect.bottom - (TOP_POS + BOTTOM_POS)), false);
	MoveWindow(mWindow2->GetHwnd(), rect.left + leftWindowWidth + WIDTH_ADJUST, rect.top + TOP_POS, rect.right - (leftWindowWidth + WIDTH_ADJUST), rect.bottom - (TOP_POS + BOTTOM_POS), false);

	InvalidateRect(mBaseWindow->GetHwnd(), &rect, TRUE);*/
}
//================================================================================================================
void WinSplitterContainer::UpdateMouseMove(WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	
	if (LOWORD(lParam) > 10)
	{
		if ((wParam == MK_LBUTTON) && bSplitterMoving)
		{
			SetCursor(hCursor);
			
			GetClientRect(mBaseWindow->GetHwnd(), &rect);

			if (LOWORD(lParam) > rect.right)
			{
				return;
			}

			dwSplitterPosition = LOWORD(lParam);

			mBaseWindow->SendWindowMessage(WM_SIZE, 0, MAKELPARAM(rect.right, rect.bottom));

			//SendMessage(mBaseWindow->GetHwnd(), WM_SIZE, 0, MAKELPARAM(rect.right, rect.bottom));
			//SendMessage(mBaseWindow->GetHwnd(), WM_SIZE, 0, MAKELPARAM(rect.left, rect.top));

			//UpdateSize(0, MAKELPARAM(rect.right, rect.bottom));

			//UpdateSize(0, MAKELPARAM(rect.left, rect.top));
		}
	}
	
	//int TOP_POS = 80;
	//int WIDTH_ADJUST = 2;
	//int BOTTOM_POS = 80;

	//int xPos = (int)LOWORD(lParam);
	//int yPos = (int)HIWORD(lParam);

	//if (wParam == MK_LBUTTON)
	//{
	//	if (xSizing)
	//	{
	//		RECT rect;
	//		HDC hdc;

	//		hdc = GetDC(mBaseWindow->GetHwnd());
	//		GetClientRect(mBaseWindow->GetHwnd(), &rect);

	//		if (xSizing)
	//		{
	//			SetRect(&rect, leftWindowWidth - (WIDTH_ADJUST * 2), rect.top + TOP_POS, leftWindowWidth + WIDTH_ADJUST, rect.bottom - BOTTOM_POS);

	//			DrawFocusRect(hdc, &rect);

	//			leftWindowWidth = xPos;

	//			SetRect(&rect, leftWindowWidth - (SPLITTER_WIDTH * 2), rect.top + TOP_POS, leftWindowWidth + SPLITTER_WIDTH, rect.bottom - BOTTOM_POS);

	//			DrawFocusRect(hdc, &rect);
	//		}

	//		ReleaseDC(mBaseWindow->GetHwnd(), hdc);

	//		//UpdateSize(0, MAKELPARAM(rect.right, rect.bottom));
	//	}
	//}
}
//================================================================================================================
void WinSplitterContainer::UpdateLeftButtonDown(WPARAM wParam, LPARAM lParam)
{
	SetCursor(hCursor);
	bSplitterMoving = true;
	SetCapture(mBaseWindow->GetHwnd());

	/*int xPos = (int)LOWORD(lParam);
	int yPos = (int)HIWORD(lParam);

	xSizing = (xPos > leftWindowWidth - SPLITTER_WIDTH && xPos < leftWindowWidth + SPLITTER_WIDTH);

	if (xSizing)
	{
		SetCapture(mBaseWindow->GetHwnd());
		if (xSizing)
		{
			SetCursor(hCursor);
		}
	}*/
}
//================================================================================================================
void WinSplitterContainer::UpdateLeftButtonUp(WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	SetCursor(hCurrentCursor);
	bSplitterMoving = false;

	/*RECT rect;
	HDC hdc;

	ReleaseCapture();

	hdc = GetDC(mBaseWindow->GetHwnd());
	GetClientRect(mBaseWindow->GetHwnd(), &rect);

	if (xSizing)
	{
		int TOP_POS = 80;
		int WIDTH_ADJUST = 2;
		int BOTTOM_POS = 80;

		SetRect(&rect, leftWindowWidth - (WIDTH_ADJUST * 2), rect.top + TOP_POS, leftWindowWidth + WIDTH_ADJUST, rect.bottom - 80);

		DrawFocusRect(hdc, &rect);

		xSizing = false;
	}

	ReleaseDC(mBaseWindow->GetHwnd(), hdc);

	PostMessage(mBaseWindow->GetHwnd(), WM_SIZE, 0, 0);*/
}
//================================================================================================================