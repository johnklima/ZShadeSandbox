//================================================================================================================
// WinSplitterContainer.h
//
// http://www.codeproject.com/Articles/19766/Win-splitter-window-project
//================================================================================================================
// History
//
// Created on 5/3/2015 by Dustin Watson
//================================================================================================================
#ifndef __WINSPLITTERCONTAINER_H
#define __WINSPLITTERCONTAINER_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include <Windows.h>
#include <CommCtrl.h>
#include <string>
using namespace std;

#include "Window.h"
class Window;
//================================================================================================================
//================================================================================================================
namespace GUIComponents {
class WinSplitterContainer
{
public:

	static const int SPLITTER_WIDTH = 2;

	WinSplitterContainer();
	~WinSplitterContainer();

	void SetWindow1(Window* win);
	void SetWindow2(Window* win);
	void SetBaseWindow(Window* win);

	void UpdateSize(WPARAM wParam, LPARAM lParam);
	void UpdateMouseMove(WPARAM wParam, LPARAM lParam);
	void UpdateLeftButtonDown(WPARAM wParam, LPARAM lParam);
	void UpdateLeftButtonUp(WPARAM wParam, LPARAM lParam);
	
	bool HasLeftChild() { return bHasLeftChild; }
	bool HasRightChild() { return bHasRightChild; }

private:

	bool bSplitterMoving;
	DWORD dwSplitterPosition;

	Window* mWindow1;
	Window* mWindow2;
	Window* mBaseWindow;

	HCURSOR hCurrentCursor;
	HCURSOR hCursor;

	bool bHasLeftChild;
	bool bHasRightChild;

	bool xSizing;
	int leftWindowWidth;
};
}
//================================================================================================================
//================================================================================================================
#endif//__WINSPLITTERCONTAINER_H