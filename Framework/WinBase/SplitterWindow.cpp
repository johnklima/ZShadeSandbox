#include "SplitterWindow.h"
using GUIComponents::SplitterWindow;
//================================================================================================================
//================================================================================================================
bool SplitterWindow::m_CreateFuncDefined = false;
LRESULT (*SplitterWindow::create_func)(HWND,UINT,WPARAM,LPARAM);
std::vector< SplitterWindow* > SplitterWindow::windows;
std::map<UINT, SplitterWindow::MsgFunction>	SplitterWindow::userMessagesMain;
std::map<UINT, SplitterWindow::MsgFunction>	SplitterWindow::userMessagesLeft;
std::map<UINT, SplitterWindow::MsgFunction>	SplitterWindow::userMessagesRight;
//================================================================================================================
SplitterWindow::SplitterWindow()
{
}
//================================================================================================================
SplitterWindow::~SplitterWindow()
{
}
//================================================================================================================
void SplitterWindow::Invalidate()
{
	InvalidateRect(m_hwndMain, NULL, TRUE);
}
//================================================================================================================
BOOL SplitterWindow::IsAlive() const
{
	return ::IsWindow(m_hwndMain);
}
//================================================================================================================
BOOL SplitterWindow::WinIsMinimized() const
{
	return ::IsIconic(m_hwndMain);
}
//================================================================================================================
BOOL SplitterWindow::HasFocus() const
{
	if (GetForegroundWindow() != m_hwndMain)
		return false;
	else
		return true;
}
//================================================================================================================
void SplitterWindow::Show(bool bShow)
{
	ShowWindow(m_hwndMain, bShow);
	UpdateWindow(m_hwndMain);
	
	ShowWindow(m_hwndLeft, bShow);
	UpdateWindow(m_hwndLeft);
	
	ShowWindow(m_hwndRight, bShow);
	UpdateWindow(m_hwndRight);
}
//================================================================================================================
void SplitterWindow::CreateMenuResource(int id)
{
	HMENU menu = LoadMenu(m_hInstance, MAKEINTRESOURCE(id));
	SetMenu(m_hwndMain, menu);
}
//================================================================================================================
void SplitterWindow::SendMainWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_hwndMain, msg, wParam, lParam);
}
//================================================================================================================
void SplitterWindow::SendLeftWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_hwndLeft, msg, wParam, lParam);
}
//================================================================================================================
void SplitterWindow::SendRightWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_hwndRight, msg, wParam, lParam);
}
//================================================================================================================
void SplitterWindow::Maximize()
{
	::ShowWindow(m_hwndMain, SW_MAXIMIZE);
}
//================================================================================================================
int	SplitterWindow::GetWidthLeft() const
{
	return m_LeftWindowWidth;
}
//================================================================================================================
int	SplitterWindow::GetHeightLeft() const
{
	return m_LeftWindowHeight;
}
//================================================================================================================
int	SplitterWindow::GetWidthRight()
{
	m_RightWindowWidth = m_MainWndRect.right - (m_MainWndRect.left + m_LeftWindowWidth + m_SplitterBarWidth);
	return m_RightWindowWidth;
}
//================================================================================================================
int	SplitterWindow::GetHeightRight() const
{
	return m_RightWindowHeight;
}
//================================================================================================================
void SplitterWindow::Destroy()
{
	::DestroyWindow(m_hwndLeft);
	::UnregisterClass("Left Window", m_hInstance);
	
	::DestroyWindow(m_hwndRight);
	::UnregisterClass("Right Window", m_hInstance);
	
	::DestroyWindow(m_hwndMain);
	::UnregisterClass(m_appName.c_str(), m_hInstance);
}
//================================================================================================================
void SplitterWindow::SetWindowTitle(LPCSTR title)
{
	::SetWindowText(m_hwndMain, title);
}
//================================================================================================================
void SplitterWindow::Run()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}
//================================================================================================================
bool SplitterWindow::Init(HINSTANCE hinst, bool fullscreen, int size_x, int size_y, LPCSTR title, LPCSTR className,
	int iIconID, int iCursorID, bool loadDefaultCursor, bool loadDefaultIcon)
{
	m_isFullscreen = fullscreen;
	m_hInstance = hinst;
	m_appName = className;
	
	m_ScreenWidth = size_x;
	m_ScreenHeight = size_y;
	m_MinResizableWidth = 400;
	m_MaxResizableWidth = 350;
	m_SplitterBarWidth = 2;
	m_LeftWindowWidth = (m_ScreenWidth / 2) + 300;
	m_LeftMinimumSpace = 150;
	m_RightMinimumSpace = m_ScreenWidth - 100;
	m_TopPos = 0;
	m_BottomPos = 0;
	m_UpdatedLeftWndWidth = m_LeftWindowWidth;
	
	ZeroMemory(&m_main_winclass_ex, sizeof(WNDCLASSEX));

	m_main_winclass_ex.cbSize = sizeof(WNDCLASSEX);
	m_main_winclass_ex.style = CS_HREDRAW | CS_VREDRAW;
	m_main_winclass_ex.lpfnWndProc = (WNDPROC)WndProcMain;
	m_main_winclass_ex.cbClsExtra = 0;
	m_main_winclass_ex.cbWndExtra = 0;
	m_main_winclass_ex.hInstance = m_hInstance;
	
	if (loadDefaultCursor)
	{
		m_main_winclass_ex.hCursor = LoadCursor(NULL, IDC_ARROW);
	}
	else
	{
		m_main_winclass_ex.hCursor = LoadCursor(m_hInstance, MAKEINTRESOURCE(iCursorID));
	}
	
	HBRUSH hBrush = CreateSolidBrush(RGB(main_Wnd_clr[0], main_Wnd_clr[1], main_Wnd_clr[2]));
	
	m_main_winclass_ex.hbrBackground = (HBRUSH)hBrush;
	m_main_winclass_ex.lpszMenuName = NULL;
	m_main_winclass_ex.lpszClassName = className;
	
	if (loadDefaultIcon)
	{
		m_main_winclass_ex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		m_main_winclass_ex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	}
	else
	{
		HICON icon = reinterpret_cast<HICON>(::LoadImageW(m_hInstance, MAKEINTRESOURCEW(iIconID), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR));
		m_main_winclass_ex.hIcon = icon;
		m_main_winclass_ex.hIconSm = icon;
	}

	if (!RegisterClassEx(&m_main_winclass_ex))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "SplitterWindow Main Window Registration Failed");
		return false;
	}
	
	// Initializing the class for the left hand side window
	ZeroMemory(&m_left_winclass_ex, sizeof(WNDCLASSEX));
	
	m_left_winclass_ex.cbSize = sizeof(WNDCLASSEX);
	m_left_winclass_ex.style = CS_HREDRAW | CS_VREDRAW;
	m_left_winclass_ex.lpfnWndProc = (WNDPROC)WndProcLeft;
	m_left_winclass_ex.cbClsExtra = 0;
	m_left_winclass_ex.cbWndExtra = 0;
	m_left_winclass_ex.hInstance = m_hInstance;
	
	if (loadDefaultCursor)
	{
		m_left_winclass_ex.hCursor = LoadCursor(NULL, IDC_ARROW);
	}
	else
	{
		m_left_winclass_ex.hCursor = LoadCursor(m_hInstance, MAKEINTRESOURCE(iCursorID));
	}
	
	hBrush = CreateSolidBrush(RGB(left_Wnd_clr[0], left_Wnd_clr[1], left_Wnd_clr[2]));
	m_left_winclass_ex.hbrBackground = (HBRUSH)hBrush;
	m_left_winclass_ex.lpszMenuName = NULL;
    m_left_winclass_ex.lpszClassName = "Left Window";
    
	if (loadDefaultIcon)
	{
		m_left_winclass_ex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		m_left_winclass_ex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	}
	else
	{
		HICON icon = reinterpret_cast<HICON>(::LoadImageW(m_hInstance, MAKEINTRESOURCEW(iIconID), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR));
		m_left_winclass_ex.hIcon = icon;
		m_left_winclass_ex.hIconSm = icon;
	}
	
    // Register the Window class with the OS
    if (!RegisterClassEx(&m_left_winclass_ex))
    {
        return false;
    }
	
    // Initializing the class for the right hand side window
	ZeroMemory(&m_right_winclass_ex, sizeof(WNDCLASSEX));
	
	m_right_winclass_ex.cbSize = sizeof(WNDCLASSEX);
	m_right_winclass_ex.style = CS_HREDRAW | CS_VREDRAW;
	m_right_winclass_ex.lpfnWndProc = (WNDPROC)WndProcRight;
	m_right_winclass_ex.cbClsExtra = 0;
	m_right_winclass_ex.cbWndExtra = 0;
	m_right_winclass_ex.hInstance = m_hInstance;
	
	if (loadDefaultCursor)
	{
		m_right_winclass_ex.hCursor = LoadCursor(NULL, IDC_ARROW);
	}
	else
	{
		m_right_winclass_ex.hCursor = LoadCursor(m_hInstance, MAKEINTRESOURCE(iCursorID));
	}
	
	hBrush = CreateSolidBrush(RGB(right_Wnd_clr[0], right_Wnd_clr[1], right_Wnd_clr[2]));
	m_right_winclass_ex.hbrBackground = (HBRUSH)hBrush;
	m_right_winclass_ex.lpszMenuName = NULL;
    m_right_winclass_ex.lpszClassName = "Right Window";
    
	if (loadDefaultIcon)
	{
		m_right_winclass_ex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		m_right_winclass_ex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	}
	else
	{
		HICON icon = reinterpret_cast<HICON>(::LoadImageW(m_hInstance, MAKEINTRESOURCEW(iIconID), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR));
		m_right_winclass_ex.hIcon = icon;
		m_right_winclass_ex.hIconSm = icon;
	}
	
    // Register the Window class with the OS
    if (!RegisterClassEx(&m_right_winclass_ex))
    {
        return false;
    }
	
	DeleteObject(hBrush);
	
	DWORD flags = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
	
	// Center the window
	UINT nx_size = GetSystemMetrics(SM_CXSCREEN);
    UINT ny_size = GetSystemMetrics(SM_CYSCREEN);
	
    UINT nx_pos = (nx_size - size_x) / 2;
    UINT ny_pos = (ny_size - size_y) / 2;
	
	//WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
	m_hwndMain = CreateWindow(
		className,
		title,
		flags,
		nx_pos,
		ny_pos,
		size_x, size_y,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);
	
	if (m_hwndMain == NULL)
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "SplitterWindow Main Window Creation Failed");
		return false;
	}
	
	GetClientRect(m_hwndMain, &m_MainWndRect);
	
	m_LeftWindowWidth = m_LeftWindowWidth;
	m_LeftWindowHeight = (m_MainWndRect.bottom - m_MainWndRect.top) - (m_TopPos + m_BottomPos);

	m_hwndLeft = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"Left Window",
		"",
		WS_CHILD | WS_VISIBLE | SS_SUNKEN,
		m_MainWndRect.left, 
		m_MainWndRect.top + m_TopPos,
		m_LeftWindowWidth, 
		(m_MainWndRect.bottom - m_MainWndRect.top) - (m_TopPos + m_BottomPos),
		m_hwndMain,
		NULL,
		m_hInstance,
		NULL
	);
	
	if (m_hwndLeft == NULL)
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "SplitterWindow Left Window Creation Failed");
		return false;
	}
	
	m_RightWindowWidth = m_MainWndRect.right - (m_MainWndRect.left + m_LeftWindowWidth + m_SplitterBarWidth);
	m_RightWindowHeight = (m_MainWndRect.bottom - m_MainWndRect.top) - (m_TopPos + m_BottomPos);

	m_hwndRight = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"Right Window",
		"", 
		WS_CHILD | WS_VISIBLE | SS_SUNKEN,
		m_MainWndRect.left + m_LeftWindowWidth + m_SplitterBarWidth,
		m_MainWndRect.top + m_TopPos,
		m_MainWndRect.right - (m_MainWndRect.left + m_LeftWindowWidth + m_SplitterBarWidth),
		(m_MainWndRect.bottom - m_MainWndRect.top) - (m_TopPos + m_BottomPos),
		m_hwndMain,
		NULL,
		m_hInstance,
		NULL
	);
	
	if (m_hwndRight == NULL)
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "SplitterWindow Right Window Creation Failed");
		return false;
	}
	
	windows.push_back(this);
	
	return true;
}
//================================================================================================================
LRESULT CALLBACK SplitterWindow::WndProcMain(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < windows.size(); i++)
	{
		SplitterWindow* win = windows[i];

		if (win->GetHwndMain() == hwnd)
		{
			return win->WinProcMain(hwnd, msg, wParam, lParam);
		}
	}
	
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
//================================================================================================================
LRESULT SplitterWindow::WinProcMain(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	
	static bool 		xSizing = false;
	static HCURSOR 		hcSizeEW = NULL;
	static HBRUSH		hmain_Wnd_brush	= NULL;
	// Variable which informs whether the splitter is dragged
    // or not
    
	// Update the splitter window
	switch (msg)
	{
		case WM_CREATE:
		{
			hcSizeEW = LoadCursor(NULL, IDC_SIZEWE);
		}
		break;
		case WM_SIZE:
		{
			GetClientRect(hwnd, &m_MainWndRect);
			
			// Adjust the left window position
			MoveWindow(
				m_hwndLeft,
				m_MainWndRect.left,
				m_MainWndRect.top + m_TopPos,
				m_MainWndRect.left + (m_UpdatedLeftWndWidth - WIDTH_ADJUST),
				(m_MainWndRect.bottom - (m_TopPos + m_BottomPos)),
				FALSE
			);
			
			// Adjust the right window position
			MoveWindow(
				m_hwndRight,
				m_MainWndRect.left + m_UpdatedLeftWndWidth + WIDTH_ADJUST,
				m_MainWndRect.top + m_TopPos,
				m_MainWndRect.right - (m_UpdatedLeftWndWidth + WIDTH_ADJUST),
				m_MainWndRect.bottom - (m_TopPos + m_BottomPos),
				FALSE
			);
			
			InvalidateRect(hwnd, &m_MainWndRect, TRUE);
		}
		break;
		case WM_LBUTTONDOWN:
		{
			int xPos = (int)LOWORD(lParam);
			int yPos = (int)HIWORD(lParam);
			
			// Check if the mouse is hovering over the splitter window
			xSizing = (xPos > m_UpdatedLeftWndWidth - m_SplitterBarWidth && xPos < m_UpdatedLeftWndWidth + m_SplitterBarWidth);
			
			// If the mouse is over the splitter then set mouse cursor 
            // image to IDC_SIZEWE which helps the user to drag the window
			if (xSizing)
			{
				SetCapture(hwnd);
				
				if (xSizing)
				{
					SetCursor(hcSizeEW);
				}
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			if (xSizing)
			{
				RECT    focusrect;
				HDC     hdc;
				
				// Releases the captured mouse input
				ReleaseCapture();
				
				// Get the main window dc to draw a focus rectangle
				hdc = GetDC(hwnd);
				GetClientRect(hwnd, &rect);
				
				if (xSizing)
				{
					SetRect(
						&focusrect,
						m_UpdatedLeftWndWidth - (WIDTH_ADJUST * 2),
						rect.top + m_TopPos, 
						m_UpdatedLeftWndWidth + WIDTH_ADJUST, 
						rect.bottom - 80
					);
					
					// Call api to vanish the dragging rectangle 
					DrawFocusRect(hdc, &focusrect);
					
					xSizing = FALSE;
				}
				
				// Release the dc once done 
				ReleaseDC(hwnd, hdc);
			}
			
			// Post a WM_SIZE message to redraw the windows
			PostMessage(hwnd, WM_SIZE, 0, 0);
		}
		break;
		case WM_MOUSEMOVE:
		{
			int xPos = (int)LOWORD(lParam);
			int yPos = (int)HIWORD(lParam);
			
			if (xPos < m_LeftMinimumSpace || xPos > m_RightMinimumSpace)
			{
				return 0;
			}
			
			// Checks if the left button is pressed during dragging the splitter
			if (wParam == MK_LBUTTON)   
			{
				// If the window is dragged using the splitter, get the
				// cursors current position and draws a focus rectangle 
				if (xSizing)
				{
					RECT    focusrect;
					HDC     hdc;
					
					hdc = GetDC(hwnd);
					GetClientRect(hwnd, &rect);
					
					if (xSizing)
					{
						SetRect(
							&focusrect,
							m_UpdatedLeftWndWidth - (WIDTH_ADJUST * 2),
							rect.top + m_TopPos,
							m_UpdatedLeftWndWidth +  WIDTH_ADJUST, 
							rect.bottom - m_BottomPos
						);
						
						DrawFocusRect(hdc, &focusrect);
						
						// Get the size of the left window to increase
						m_UpdatedLeftWndWidth = xPos;
						
						// Draws a focus rectangle
						SetRect(
							&focusrect,
							m_UpdatedLeftWndWidth - (m_SplitterBarWidth * 2),
							rect.top + 80,
							m_UpdatedLeftWndWidth + m_SplitterBarWidth, 
							rect.bottom - m_BottomPos
						);
						
						DrawFocusRect(hdc, &focusrect);
					}
					
					ReleaseDC(hwnd, hdc);
				}
			}
			
			// Set the cursor image to east west direction when the mouse is over 
			// the splitter window
			if( (xPos > m_UpdatedLeftWndWidth - m_SplitterBarWidth && xPos < m_UpdatedLeftWndWidth + m_SplitterBarWidth))
			{
				 SetCursor(hcSizeEW); 
			}
		}
		break;
		case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			// Painting 
			GetClientRect(hwnd, &rect);
			hmain_Wnd_brush = CreateSolidBrush(RGB(main_Wnd_clr[0], main_Wnd_clr[1], main_Wnd_clr[2]));
			FillRect(hdc, &rect, hmain_Wnd_brush);
			DeleteObject(hmain_Wnd_brush);
			EndPaint(hwnd, &ps);
		}
		break;
	}

	if (userMessagesMain.find(msg) != userMessagesMain.end())
	{
		MsgFunction msgFunction = userMessagesMain[msg];
		return msgFunction(hwnd, msg, wParam, lParam);
	}
	else
	{
		switch (msg)
		{
			// Window is being destroyed
			case WM_DESTROY:
				::PostQuitMessage(0);
				return 0;

			// Window is being closed
			case WM_CLOSE:
			{
				DestroyWindow(hwnd);
				return 0;
			}
		}
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
//================================================================================================================
LRESULT CALLBACK SplitterWindow::WndProcLeft(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < windows.size(); i++)
	{
		SplitterWindow* win = windows[i];

		if (win->GetHwndLeft() == hwnd)
		{
			return win->WinProcLeft(hwnd, msg, wParam, lParam);
		}
	}
	
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
//================================================================================================================
LRESULT SplitterWindow::WinProcLeft(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	static HBRUSH	hleft_Wnd_brush = NULL;
	
	switch (msg)
	{
		case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			// Painting 
			GetClientRect(hwnd, &rect);
			hleft_Wnd_brush = CreateSolidBrush(RGB(left_Wnd_clr[0], left_Wnd_clr[1], left_Wnd_clr[2]));
			FillRect(hdc, &rect, hleft_Wnd_brush);
			DeleteObject(hleft_Wnd_brush);
			EndPaint(hwnd, &ps);
		}
		break;
	}
	
	if (userMessagesLeft.find(msg) != userMessagesLeft.end())
	{
		MsgFunction msgFunction = userMessagesLeft[msg];
		return msgFunction(hwnd, msg, wParam, lParam);
	}
	else
	{
		switch (msg)
		{
			// Window is being destroyed
			case WM_DESTROY:
				::PostQuitMessage(0);
				return 0;

			// Window is being closed
			case WM_CLOSE:
			{
				DestroyWindow(hwnd);
				return 0;
			}
		}
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
//================================================================================================================
LRESULT CALLBACK SplitterWindow::WndProcRight(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < windows.size(); i++)
	{
		SplitterWindow* win = windows[i];

		if (win->GetHwndRight() == hwnd)
		{
			return win->WinProcRight(hwnd, msg, wParam, lParam);
		}
	}
	
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
//================================================================================================================
LRESULT SplitterWindow::WinProcRight(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	static HBRUSH	hright_Wnd_brush = NULL;
	
	switch (msg)
	{
		case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			// Painting 
			GetClientRect(hwnd, &rect);
			hright_Wnd_brush = CreateSolidBrush(RGB(right_Wnd_clr[0], right_Wnd_clr[1], right_Wnd_clr[2]));
			FillRect(hdc, &rect, hright_Wnd_brush);
			DeleteObject(hright_Wnd_brush);
			EndPaint(hwnd, &ps);
		}
		break;
		case WM_COMMAND:
		{
			int sd = 34;
			sd = 2;
		}
		break;
	}
	
	if (userMessagesRight.find(msg) != userMessagesRight.end())
	{
		MsgFunction msgFunction = userMessagesRight[msg];
		return msgFunction(hwnd, msg, wParam, lParam);
	}
	else
	{
		switch (msg)
		{
			// Window is being destroyed
			case WM_DESTROY:
				::PostQuitMessage(0);
				return 0;

			// Window is being closed
			case WM_CLOSE:
			{
				DestroyWindow(hwnd);
				return 0;
			}
		}
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
//================================================================================================================
void SplitterWindow::SetUserMessageFunctionMain(UINT uMessage, MsgFunction msgFunction)
{
	userMessagesMain.insert( make_pair(uMessage, msgFunction) );
}
//================================================================================================================
void SplitterWindow::SetUserMessageFunctionLeft(UINT uMessage, MsgFunction msgFunction)
{
	userMessagesLeft.insert( make_pair(uMessage, msgFunction) );
}
//================================================================================================================
void SplitterWindow::SetUserMessageFunctionRight(UINT uMessage, MsgFunction msgFunction)
{
	userMessagesRight.insert( make_pair(uMessage, msgFunction) );
}
//================================================================================================================