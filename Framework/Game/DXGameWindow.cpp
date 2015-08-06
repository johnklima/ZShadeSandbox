#include "DXGameWindow.h"
#include "CGlobal.h"
#include "MemoryMgr.h"
#include "ZShadeResource.h"
#include "ZShadeMessageCenter.h"
#include "ZShadeINIParser.h"
#include "Exceptions.h"
#include "Environment.h"
#include <functional>
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
#pragma managed
//===============================================================================================================================
//===============================================================================================================================
ZShadeSandboxEnvironment::Environment* DXGameWindow::mEnv = 0;
Window* DXGameWindow::mWindow = 0;
bool DXGameWindow::bDestroyed = false;
//===============================================================================================================================
DXGameWindow::DXGameWindow()
{
}
//===============================================================================================================================
DXGameWindow::~DXGameWindow()
{
}
//===============================================================================================================================
void DXGameWindow::Init(EngineOptions* eo, HWND hwnd)
{
	mEngineOptions = eo;

	mWindow = new Window();

	if (!mWindow->Init(mEngineOptions->hinstance, mEngineOptions->fullscreen, mEngineOptions->m_screenWidth, mEngineOptions->m_screenHeight,
		"ZShadeSandbox", "RenderWindow", hwnd, IDI_ICON, IDC_TARGET, false, false))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "Render Window Creation Failed");
		return;
	}

	mEngineOptions->hwnd = mWindow->GetHwnd();

	mWindow->SetUserMessageFunction(WM_LBUTTONDOWN, bind(mem_fn(&DXGameWindow::OnMouseDown), this, _1, _2, _3, _4));
	mWindow->SetUserMessageFunction(WM_MBUTTONDOWN, bind(mem_fn(&DXGameWindow::OnMouseDown), this, _1, _2, _3, _4));
	mWindow->SetUserMessageFunction(WM_RBUTTONDOWN, bind(mem_fn(&DXGameWindow::OnMouseDown), this, _1, _2, _3, _4));
	mWindow->SetUserMessageFunction(WM_LBUTTONUP, bind(mem_fn(&DXGameWindow::OnMouseUp), this, _1, _2, _3, _4));
	mWindow->SetUserMessageFunction(WM_MBUTTONUP, bind(mem_fn(&DXGameWindow::OnMouseUp), this, _1, _2, _3, _4));
	mWindow->SetUserMessageFunction(WM_RBUTTONUP, bind(mem_fn(&DXGameWindow::OnMouseUp), this, _1, _2, _3, _4));
	mWindow->SetUserMessageFunction(WM_MOUSEMOVE, bind(mem_fn(&DXGameWindow::OnMouseMove), this, _1, _2, _3, _4));

	mWindow->Show();

	mEnv = new ZShadeSandboxEnvironment::Environment(mEngineOptions);
}
//===============================================================================================================================
EngineOptions* DXGameWindow::GetEngineOptions()
{
	return mEngineOptions;
}
//===============================================================================================================================
LRESULT DXGameWindow::OnMouseDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	mEnv->OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	return 1;
}
//===============================================================================================================================
LRESULT DXGameWindow::OnMouseUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	mEnv->OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	return 1;
}
//===============================================================================================================================
LRESULT DXGameWindow::OnMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	mEnv->OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	return 1;
}
//===============================================================================================================================
int DXGameWindow::Run()
{
	try
	{
		while (mWindow->IsAlive())
		{
			if (!mWindow->WinIsMinimized())
			{
				mEnv->Run();

				//Quick esc for the game
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
				{
					bDestroyed = true;
					mWindow->Destroy();
					System::Windows::Forms::Application::Exit();
					break;
				}
			}

			mWindow->Run();
		}
	}
	catch (Exception exception)
	{
		exception.ShowErrorMessage();
	}

	return 1;
}
//===============================================================================================================================
#pragma unmanaged