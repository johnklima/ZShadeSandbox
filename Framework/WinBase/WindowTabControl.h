//================================================================================================================
// WindowTabControl.h
//
//================================================================================================================
// History
//
// Created on 10/9/2014 by Dustin Watson
//================================================================================================================
#ifndef __WINDOWTABCONTROL_H
#define __WINDOWTABCONTROL_H
//================================================================================================================
//================================================================================================================
//Includes
#include <memory>
#include <sstream>
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <map>
#include <functional>
#include <CommCtrl.h>
#include "Window.h"
#include "ZShadeMessageCenter.h"
using namespace std;
//================================================================================================================
/*
HWND WINAPI DoCreateDisplayWindow(HWND hwndParent) 
{ 
    HWND hwndStatic = CreateWindow(TEXT("STATIC"), TEXT(""), 
        WS_CHILD | WS_VISIBLE | WS_BORDER, 
        100, 100, 100, 100,      // Position and dimensions; example only.
        hwndParent, NULL, g_hinst, NULL); 
    return hwndStatic; 
}
*/
//================================================================================================================
class WindowTabControl
{
public:

	WindowTabControl(HWND parent);
	~WindowTabControl();

	bool Init(HWND parent);
	
	HWND GetHwnd() { return m_Hwnd; }
	
	void SetVisible(bool bVisable);
	bool IsVisible();

	void Enable(bool bEnable);
	bool IsEnabled();
	
	// Getters and Setters
public:

	bool InsertTab(LPSTR name);
	void SelectTab(int id);
	
	// Returns the currently selected tab id
	int TabID();

	// Class Variables and Privates
private:
	
	int m_TabIndex;
	HWND m_Hwnd;
};
//================================================================================================================
//================================================================================================================
#endif//__WINDOWTABCONTROL_H