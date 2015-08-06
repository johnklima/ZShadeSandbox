//================================================================================================================
// Window.h
//
//================================================================================================================
// History
//
// Created on 10/6/2014 by Dustin Watson
//================================================================================================================
#ifndef __WINDOW_H
#define __WINDOW_H
//================================================================================================================
//================================================================================================================

//
//Includes
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

//================================================================================================================
class Window
{
public:
	
	typedef std::tr1::function<LRESULT (HWND, UINT, WPARAM, LPARAM)> MsgFunction;

	Window();
	~Window();
	
	operator HWND() { return m_hwnd; }
	void		SetHwnd(HWND hwnd);
	void		Run();
	void		Show(bool bShow=TRUE);
	bool		Init(HINSTANCE hInstance, bool fullscreen, int size_x, int size_y, LPCSTR title, LPCSTR className, HWND parent, int iIconID=-1, int iCursorID=-1, bool loadDefaultCursor=true, bool loadDefaultIcon=true);
	void		SetUserMessageFunction(UINT uMessage, MsgFunction msgFunction);
	void		CreateMenuResource(int id);
	
	static bool m_CreateFuncDefined;
	// Create a function and assign it to this for the WM_CREATE message
	static LRESULT (*create_func)(HWND,UINT,WPARAM,LPARAM);

	//Create a random window
	static HWND CreateAWindow(HINSTANCE hInstance, int size_x, int size_y, LPCSTR title, LPCSTR className, HWND parent, LRESULT (*win_func)(HWND,UINT,WPARAM,LPARAM));

	void SendWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	// Getters and Setters
public:

	BOOL        HasFocus() const;
	BOOL		IsAlive() const;
	BOOL		WinIsMinimized() const;
	LONG_PTR	WinGetWindowStyle() const;
	//LONG_PTR	GetExtendedStyle() const;
	void		SetWindowStyle(DWORD newStyle);
	void		SetExtendedStyle(DWORD newExStyle);
	void		Maximize();
	void		SetWindowPos(INT posX, INT posY);
	void		GetWindowPos(INT& posX, INT& posY) const;
	void		SetClientArea(INT clientX, INT clientY);
	void		GetClientArea(INT& clientX, INT& clientY) const;
	void		SetWindowTitle(LPCSTR title);
	void		SetScrollRanges(INT scrollRangeX,
								INT scrollRangeY,
								INT posX,
								INT posY);
	void Destroy();
	void		SetFullscreen(bool bEnable) { m_isFullscreen = bEnable; }

	HWND		GetHwnd() const { return m_hwnd; }
	bool		IsFullscreen() const { return m_isFullscreen; }
	int			GetWidth() const { return m_screenWidth; }
	int			GetHeight() const { return m_screenHeight; }

	void		Invalidate();

	// Class Variables and Privates
private:

	LRESULT CALLBACK		WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static LRESULT WINAPI	WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND					m_hwnd;
	HINSTANCE				m_hInstance;
	WNDCLASSEX				m_winclass_ex;
	bool					m_isFullscreen;
	int						m_screenWidth;
	int						m_screenHeight;
	std::string		appName;		// The name of the application
	DWORD			style;			// The current window style
	DWORD			exStyle;		// The extended window style
	HACCEL			accelTable;		// Accelerator table handle

	std::map<UINT, MsgFunction>	userMessages;

	static std::vector< Window* > windows;
};
//================================================================================================================
class CustomWindow
{
	HWND  m_hwnd;
	int	  m_screenWidth;
	int	  m_screenHeight;
	DWORD style;			// The current window style
	DWORD exStyle;		// The extended window style

public:

	CustomWindow();
	CustomWindow(HINSTANCE hInstance, int size_x, int size_y, LPCSTR title, LPCSTR className, HWND parent, LRESULT(*win_func)(HWND, UINT, WPARAM, LPARAM));

	void Init(HINSTANCE hInstance, int size_x, int size_y, LPCSTR title, LPCSTR className, HWND parent, LRESULT(*win_func)(HWND, UINT, WPARAM, LPARAM));

	operator HWND() { return m_hwnd; }
	void		Show(bool bShow = TRUE);
	BOOL        HasFocus() const;
	BOOL		IsAlive() const;
	BOOL		WinIsMinimized() const;
	LONG_PTR	WinGetWindowStyle() const;
	//LONG_PTR	GetExtendedStyle() const;
	void		SetWindowStyle(DWORD newStyle);
	void		SetExtendedStyle(DWORD newExStyle);
	void		Maximize();
	void		SetWindowPos(INT posX, INT posY);
	void		GetWindowPos(INT& posX, INT& posY) const;
	void		SetClientArea(INT clientX, INT clientY);
	void		GetClientArea(INT& clientX, INT& clientY) const;
	void		SetWindowTitle(LPCSTR title);
	void		Invalidate();
	HWND		GetHwnd() const { return m_hwnd; }
	int			GetWidth() const { return m_screenWidth; }
	int			GetHeight() const { return m_screenHeight; }
};
//================================================================================================================
//Create a random window
//HWND CreateAWindow(HINSTANCE hInstance, int size_x, int size_y, LPCSTR title, LPCSTR className, HWND parent, LRESULT (*win_func)(HWND,UINT,WPARAM,LPARAM));
//================================================================================================================
//================================================================================================================
#endif//__WINDOW_H