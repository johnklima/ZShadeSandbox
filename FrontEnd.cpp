//===============================================================================================================================
// FrontEnd Source File
//
// //http://daniel-albuschat.blogspot.com/2011/05/using-net-and-wpf-in-win32-legacy.html
//===============================================================================================================================
// History
//
// -Created 6/21/2015 by Dustin Watson
//===============================================================================================================================

#include "FrontEnd.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread] // This is IMPORTANT, but it's for in C++/CLR  only
int CALLBACK WinMain
(	HINSTANCE hInstance
,	HINSTANCE hPrevInstance
,	LPSTR lpCmdLine
,	int nCmdShow
)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	
	ZShadeSandbox::FrontEnd form(hInstance);
	//int screenX = form.GetEngineOptions()->m_screenWidth;
	//int screenY = form.GetEngineOptions()->m_screenHeight;
	//form.Location = new System::Drawing::Point(screenX / 2, screenY / 2);
	Application::Run(%form);
}