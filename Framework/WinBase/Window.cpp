#include "Window.h"
#include "../../../ZShadeResource.h"
//================================================================================================================
bool Window::m_CreateFuncDefined = false;
LRESULT (*Window::create_func)(HWND,UINT,WPARAM,LPARAM);
std::vector< Window* > Window::windows;
//================================================================================================================
Window::Window()
{
}
//================================================================================================================
Window::~Window()
{
}
//================================================================================================================
void Window::SetHwnd(HWND hwnd)
{
	m_hwnd = hwnd;
}
//================================================================================================================
void Window::Invalidate()
{
	InvalidateRect(m_hwnd, NULL, TRUE);
}
//================================================================================================================
BOOL Window::IsAlive() const
{
	return ::IsWindow(m_hwnd);
}
//================================================================================================================
BOOL Window::WinIsMinimized() const
{
	return ::IsIconic(m_hwnd);
}
//================================================================================================================
LONG_PTR Window::WinGetWindowStyle() const
{
	return GetWindowLongPtr(m_hwnd, GWL_STYLE);
}
//================================================================================================================
void Window::Run()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}
//================================================================================================================
BOOL Window::HasFocus() const
{
	if (GetForegroundWindow() != m_hwnd)
		return false;
	else
		return true;
}
//================================================================================================================
void Window::Show(bool bShow)
{
	ShowWindow(m_hwnd, bShow);
	UpdateWindow(m_hwnd);
}
//================================================================================================================
void Window::CreateMenuResource(int id)
{
	HMENU menu = LoadMenu(m_hInstance, MAKEINTRESOURCE(id));
	SetMenu(m_hwnd, menu);
}
//================================================================================================================
void Window::SetClientArea(INT clientX, INT clientY)
{
	RECT windowRect;
	::SetRect(&windowRect, 0, 0, clientX, clientY);

	BOOL bIsMenu = (::GetMenu(m_hwnd) != NULL);
	if (!::AdjustWindowRectEx(&windowRect, style, bIsMenu, exStyle))
		;// throw Win32Exception(::GetLastError());
	//HWND_NOTOPMOST
	if (!::SetWindowPos(m_hwnd, HWND_TOP, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_NOMOVE))
		;// throw Win32Exception(::GetLastError());

	if (!::MoveWindow(m_hwnd, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, TRUE))
		;// throw Win32Exception(::GetLastError());
}
//================================================================================================================
void Window::GetClientArea(INT& clientX, INT& clientY) const
{
	RECT clientRect;
	if (!::GetClientRect(m_hwnd, &clientRect))
		;// throw Win32Exception(::GetLastError());

	clientX = clientRect.right;
	clientY = clientRect.bottom;
}
//================================================================================================================
void Window::SetScrollRanges(INT scrollRangeX,
	INT scrollRangeY,
	INT posX,
	INT posY)
{
	INT clientX, clientY;
	GetClientArea(clientX, clientY);

	// Horizontal first
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	scrollInfo.nMin = 0;
	scrollInfo.nMax = scrollRangeX;
	scrollInfo.nPos = posX;
	scrollInfo.nTrackPos = 0;
	scrollInfo.nPage = static_cast<INT>(((FLOAT)clientX / scrollRangeX) * clientX);
	::SetScrollInfo(m_hwnd, SB_HORZ, &scrollInfo, TRUE);

	// Then vertical
	scrollInfo.nMax = scrollRangeX;
	scrollInfo.nPos = posY;
	scrollInfo.nPage = static_cast<INT>(((FLOAT)clientY / scrollRangeX) * clientY);
	::SetScrollInfo(m_hwnd, SB_VERT, &scrollInfo, TRUE);
}
//================================================================================================================
void Window::SendWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_hwnd, msg, wParam, lParam);
}
//================================================================================================================
void Window::SetWindowStyle(DWORD newStyle)
{
	if (!::SetWindowLongPtr(m_hwnd, GWL_STYLE, newStyle))
		;// throw Win32Exception(::GetLastError());
	
	style = newStyle;
}
//================================================================================================================
void Window::SetExtendedStyle(DWORD newExStyle)
{
	if (!::SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, newExStyle))
		;// throw Win32Exception(::GetLastError());

	exStyle = newExStyle;
}
//================================================================================================================
void Window::Maximize()
{
	::ShowWindow(m_hwnd, SW_MAXIMIZE);
}
//================================================================================================================
void Window::Destroy()
{
	::DestroyWindow(m_hwnd);
	::UnregisterClass(appName.c_str(), m_hInstance);
}
//================================================================================================================
void Window::SetWindowTitle(LPCSTR title)
{
	if (!::SetWindowText(m_hwnd, title))
		;// throw Win32Exception(::GetLastError());
}
//================================================================================================================
void Window::SetWindowPos(INT posX, INT posY)
{
	RECT clientRect;
	if (!::GetClientRect(m_hwnd, &clientRect))
		;// throw Win32Exception(::GetLastError());

	::MoveWindow(m_hwnd, posX, posY, clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top, TRUE);

	if (!::SetWindowPos(m_hwnd, HWND_NOTOPMOST, posX, posY, 0, 0, SWP_NOSIZE))
		;// throw Win32Exception(::GetLastError());
}
//================================================================================================================
void Window::GetWindowPos(INT& posX, INT& posY) const
{
	RECT windowRect;
	if (!::GetWindowRect(m_hwnd, &windowRect))
		;// throw Win32Exception(::GetLastError());
	posX = windowRect.left;
	posY = windowRect.top;
}
//================================================================================================================
bool Window::Init
(
	HINSTANCE hInstance,
	bool fullscreen,
	int size_x,
	int size_y,
	LPCSTR title,
	LPCSTR className,
	HWND parent,
	int iIconID,
	int iCursorID,
	bool loadDefaultCursor,
	bool loadDefaultIcon
)
{
	m_isFullscreen = fullscreen;
	m_screenWidth = size_x;
	m_screenHeight = size_y;
	m_hInstance = hInstance;
	appName = className;

	ZeroMemory(&m_winclass_ex, sizeof(WNDCLASSEX));

	m_winclass_ex.cbSize = sizeof(WNDCLASSEX);
	m_winclass_ex.style = CS_HREDRAW | CS_VREDRAW;
	m_winclass_ex.lpfnWndProc = (WNDPROC)WndProc;
	m_winclass_ex.cbClsExtra = 0;
	m_winclass_ex.cbWndExtra = 0;
	m_winclass_ex.hInstance = hInstance;
	
	if (loadDefaultCursor)
		m_winclass_ex.hCursor = LoadCursor(NULL, IDC_ARROW);
	else
	{
		m_winclass_ex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(iCursorID));
	}
	
	m_winclass_ex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	m_winclass_ex.lpszMenuName = NULL;
	m_winclass_ex.lpszClassName = className;
	
	if (loadDefaultIcon)
	{
		m_winclass_ex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		m_winclass_ex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	}
	else
	{
		HICON icon = reinterpret_cast<HICON>(::LoadImageW(hInstance, MAKEINTRESOURCEW(iIconID), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR));
		m_winclass_ex.hIcon = icon;
		m_winclass_ex.hIconSm = icon;
	}

	if (!RegisterClassEx(&m_winclass_ex))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "Window Registration Failed");
		return false;
	}

	DWORD flags;

	int x = 0;
	int y = 0;

	if (parent == NULL)
	{
		flags = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;

		x = y = CW_USEDEFAULT;
	}
	else
	{
		flags = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		x = y = CW_USEDEFAULT;
	}

	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,//WS_EX_CLIENTEDGE,// | WS_EX_TOPMOST,
		className,
		title,
		flags,
		x, y,
		size_x, size_y,
		parent, NULL,
		hInstance,
		(void*)this);

	if (m_hwnd == NULL)
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "Window Creation Failed");
		return false;
	}

	windows.push_back(this);

	return true;
}
//================================================================================================================
LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//switch (msg)
	//{
	//	case WM_CREATE:
	//	{
	//		// Only gets created once
	//		if (m_CreateFuncDefined)
	//		{
	//			create_func(hwnd, msg, wParam, lParam);
	//			m_CreateFuncDefined = false;
	//		}
	//	}
	//	break;
	//}

	for (int i = 0; i < windows.size(); i++)
	{
		Window* win = windows[i];

		if (win->GetHwnd() == hwnd)
		{
			return win->WinProc(hwnd, msg, wParam, lParam);
		}
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);

	/*Window* pObj = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	
	if (pObj)
		return pObj->WinProc(hwnd, msg, wParam, lParam);
	else
		return ::DefWindowProc(hwnd, msg, wParam, lParam);*/
}
//================================================================================================================
LRESULT Window::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (userMessages.find(msg) != userMessages.end())
	{
		MsgFunction msgFunction = userMessages[msg];
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
void Window::SetUserMessageFunction(UINT uMessage, MsgFunction msgFunction)
{
	//userMessages[uMessage] = msgFunction;
	userMessages.insert( make_pair(uMessage, msgFunction) );
}
//================================================================================================================
HWND Window::CreateAWindow(HINSTANCE hInstance, int size_x, int size_y, LPCSTR title, LPCSTR className, HWND parent, LRESULT (*win_func)(HWND,UINT,WPARAM,LPARAM))
{
	HWND hwnd;
	WNDCLASSEX wndClassEx;

	ZeroMemory(&wndClassEx, sizeof(WNDCLASSEX));

	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	wndClassEx.lpfnWndProc = (WNDPROC)win_func;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = hInstance;
	wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.lpszClassName = className;
	wndClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wndClassEx))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "Window Registration Failed");
		return false;
	}

	DWORD flags;

	if (parent == NULL)
		flags = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
	else
		flags = WS_VISIBLE | WS_CHILD;

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE | WS_EX_TOPMOST,
		className,
		title,
		flags,
		CW_USEDEFAULT, CW_USEDEFAULT,
		size_x, size_y,
		parent, NULL,
		hInstance,
		NULL);

	if (hwnd == NULL)
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "Window Creation Failed");
		return false;
	}

	ShowWindow(hwnd, TRUE);
	UpdateWindow(hwnd);

	return hwnd;
}
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
CustomWindow::CustomWindow()
{
}
//================================================================================================================
CustomWindow::CustomWindow(HINSTANCE hInstance, int size_x, int size_y, LPCSTR title, LPCSTR className, HWND parent, LRESULT(*win_func)(HWND, UINT, WPARAM, LPARAM))
{
	m_hwnd = Window::CreateAWindow(hInstance, size_x, size_y, title, className, parent, win_func);
}
//================================================================================================================
void CustomWindow::Init(HINSTANCE hInstance, int size_x, int size_y, LPCSTR title, LPCSTR className, HWND parent, LRESULT(*win_func)(HWND, UINT, WPARAM, LPARAM))
{
	m_hwnd = Window::CreateAWindow(hInstance, size_x, size_y, title, className, parent, win_func);
}
//================================================================================================================
BOOL CustomWindow::HasFocus() const
{
	if (GetForegroundWindow() != m_hwnd)
		return false;
	else
		return true;
}
//================================================================================================================
BOOL CustomWindow::IsAlive() const
{
	return ::IsWindow(m_hwnd);
}
//================================================================================================================
BOOL CustomWindow::WinIsMinimized() const
{
	return ::IsIconic(m_hwnd);
}
//================================================================================================================
LONG_PTR CustomWindow::WinGetWindowStyle() const
{
	return GetWindowLongPtr(m_hwnd, GWL_STYLE);
}
//================================================================================================================
void CustomWindow::Show(bool bShow)
{
	ShowWindow(m_hwnd, bShow);
	UpdateWindow(m_hwnd);
}
//================================================================================================================
//LONG_PTR CustomWindow::GetExtendedStyle() const
//{
//
//}
//================================================================================================================
void CustomWindow::SetWindowStyle(DWORD newStyle)
{
	if (!::SetWindowLongPtr(m_hwnd, GWL_STYLE, newStyle))
		;// throw Win32Exception(::GetLastError());

	style = newStyle;
}
//================================================================================================================
void CustomWindow::SetExtendedStyle(DWORD newExStyle)
{
	if (!::SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, newExStyle))
		;// throw Win32Exception(::GetLastError());

	exStyle = newExStyle;
}
//================================================================================================================
void CustomWindow::Maximize()
{
	::ShowWindow(m_hwnd, SW_MAXIMIZE);
}
//================================================================================================================
void CustomWindow::SetWindowPos(INT posX, INT posY)
{
	RECT clientRect;
	if (!::GetClientRect(m_hwnd, &clientRect))
		;// throw Win32Exception(::GetLastError());

	::MoveWindow(m_hwnd, posX, posY, clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top, TRUE);

	if (!::SetWindowPos(m_hwnd, HWND_NOTOPMOST, posX, posY, 0, 0, SWP_NOSIZE))
		;// throw Win32Exception(::GetLastError());
}
//================================================================================================================
void CustomWindow::GetWindowPos(INT& posX, INT& posY) const
{
	RECT windowRect;
	if (!::GetWindowRect(m_hwnd, &windowRect))
		;// throw Win32Exception(::GetLastError());
	posX = windowRect.left;
	posY = windowRect.top;
}
//================================================================================================================
void CustomWindow::SetClientArea(INT clientX, INT clientY)
{
	RECT windowRect;
	::SetRect(&windowRect, 0, 0, clientX, clientY);

	BOOL bIsMenu = (::GetMenu(m_hwnd) != NULL);
	if (!::AdjustWindowRectEx(&windowRect, style, bIsMenu, exStyle))
		;// throw Win32Exception(::GetLastError());
	//HWND_NOTOPMOST
	if (!::SetWindowPos(m_hwnd, HWND_TOP, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_NOMOVE))
		;// throw Win32Exception(::GetLastError());

	if (!::MoveWindow(m_hwnd, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, TRUE))
		;// throw Win32Exception(::GetLastError());
}
//================================================================================================================
void CustomWindow::GetClientArea(INT& clientX, INT& clientY) const
{
	RECT clientRect;
	if (!::GetClientRect(m_hwnd, &clientRect))
		;// throw Win32Exception(::GetLastError());

	clientX = clientRect.right;
	clientY = clientRect.bottom;
}
//================================================================================================================
void CustomWindow::SetWindowTitle(LPCSTR title)
{
	if (!::SetWindowText(m_hwnd, title))
		;// throw Win32Exception(::GetLastError());
}
//================================================================================================================
void CustomWindow::Invalidate()
{
	InvalidateRect(m_hwnd, NULL, TRUE);
}
//================================================================================================================