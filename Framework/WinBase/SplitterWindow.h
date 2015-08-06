//================================================================================================================
// SplitterWindow.h
//
//================================================================================================================
// History
//
// Created on 5/22/2015 by Dustin Watson
//================================================================================================================
#ifndef __SPLITTERWINDOW_H
#define __SPLITTERWINDOW_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include <memory>
#include <sstream>
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <CommCtrl.h>
#include "ZShadeMessageCenter.h"
using namespace std;

//================================================================================================================
//================================================================================================================

#define MAX_LOADSTRING			100

#define RESET					256
#define WIDTH_ADJUST			2
#define SET						1
#define CLEAR					0

static INT		main_Wnd_clr[3] =	{ 236, 233, 216 };
static INT		left_Wnd_clr[3] =	{ 255, 255, 255 };
static INT		right_Wnd_clr[3] =	{ 255, 255, 255 };

//================================================================================================================
//================================================================================================================
namespace GUIComponents {
class SplitterWindow
{
public:
	
	typedef std::tr1::function<LRESULT (HWND, UINT, WPARAM, LPARAM)> MsgFunction;
	
public:
	
	SplitterWindow();
	~SplitterWindow();
	
	// Returns the this as the main window
	operator HWND() { return m_hwndMain; }
	
	void Run();
	void Show(bool bShow=TRUE);
	bool Init(HINSTANCE hinst, bool fullscreen, int size_x, int size_y, LPCSTR title, LPCSTR className, int iIconID=-1, int iCursorID=-1, bool loadDefaultCursor=true, bool loadDefaultIcon=true);
	void SetUserMessageFunctionMain(UINT uMessage, MsgFunction msgFunction);
	void SetUserMessageFunctionLeft(UINT uMessage, MsgFunction msgFunction);
	void SetUserMessageFunctionRight(UINT uMessage, MsgFunction msgFunction);
	void CreateMenuResource(int id);
	
	static bool m_CreateFuncDefined;
	// Create a function and assign it to this for the WM_CREATE message
	static LRESULT (*create_func)(HWND,UINT,WPARAM,LPARAM);
	
	void SendMainWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	void SendLeftWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	void SendRightWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	
// Getters and Setters
public:

	BOOL        HasFocus() const;
	BOOL		IsAlive() const;
	BOOL		WinIsMinimized() const;
	LONG_PTR	WinGetWindowStyle() const;
	
	void		Maximize();
	void		SetWindowTitle(LPCSTR title);
	void 		Destroy();
	void		SetFullscreen(bool bEnable) { m_isFullscreen = bEnable; }
	
	HWND		GetHwndMain() const { return m_hwndMain; }
	HWND		GetHwndLeft() const { return m_hwndLeft; }
	HWND		GetHwndRight() const { return m_hwndRight; }
	
	bool		IsFullscreen() const { return m_isFullscreen; }
	int			GetWidth() const { return m_ScreenWidth; }
	int			GetHeight() const { return m_ScreenHeight; }

	int			GetWidthLeft() const;
	int			GetHeightLeft()  const;

	int			GetWidthRight();
	int			GetHeightRight()  const;

	void		Invalidate();
	
	// Class Variables and Privates
private:
	
	LRESULT CALLBACK		WinProcMain(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI	WndProcMain(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	LRESULT CALLBACK		WinProcLeft(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI	WndProcLeft(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	LRESULT CALLBACK		WinProcRight(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI	WndProcRight(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	HWND  m_hwndMain;
	HWND  m_hwndLeft;
	HWND  m_hwndRight;
	
	WNDCLASSEX				m_main_winclass_ex;
	WNDCLASSEX				m_left_winclass_ex;
	WNDCLASSEX				m_right_winclass_ex;
	
	HINSTANCE m_hInstance;
	
	int 					m_MinResizableWidth;
	int 					m_MaxResizableWidth;
	int						m_SplitterBarWidth;
	int						m_LeftWindowWidth;
	int						m_LeftMinimumSpace;
	int						m_RightMinimumSpace;
	int						m_TopPos;
	int						m_BottomPos;
	int						m_ScreenWidth;
	int						m_ScreenHeight;
	int						m_LeftWindowHeight;
	int						m_RightWindowWidth;
	int						m_RightWindowHeight;

	int						m_UpdatedLeftWndWidth;
	
	bool					m_isFullscreen;
	RECT					m_MainWndRect;
	std::string				m_appName;		// The name of the application
	DWORD					m_style;			// The current window style
	DWORD					m_exStyle;		// The extended window style
	HACCEL					m_accelTable;		// Accelerator table handle
	
	static std::map<UINT, MsgFunction>	userMessagesMain;
	static std::map<UINT, MsgFunction>	userMessagesLeft;
	static std::map<UINT, MsgFunction>	userMessagesRight;
	
	static std::vector< SplitterWindow* > windows;
};
}
//================================================================================================================
//================================================================================================================
#endif//__SPLITTERWINDOW_H