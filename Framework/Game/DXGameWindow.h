//===============================================================================================================================
// DXGameWindow
//
//===============================================================================================================================
// History
//
// -Created 6/21/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __DXGAMEWINDOW_H
#define __DXGAMEWINDOW_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include <Windows.h>
#include "Window.h"
#include "EngineOptions.h"

namespace ZShadeSandboxEnvironment {
	class Environment;
}

//===============================================================================================================================
//===============================================================================================================================

class DXGameWindow
{
public:
	
	DXGameWindow();
	~DXGameWindow();

	void Init(EngineOptions* eo, HWND hwnd);

	static int Run();

	LRESULT OnMouseDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	EngineOptions* GetEngineOptions();
	
	static bool IsDestroyed() { return bDestroyed; }
	
	static Window* GetWindow();

	static ZShadeSandboxEnvironment::Environment* Env() { return mEnv; }

private:
	
	static bool bDestroyed;
	static ZShadeSandboxEnvironment::Environment* mEnv;
	static Window* mWindow;
	EngineOptions* mEngineOptions;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__DXGAMEWINDOW_H