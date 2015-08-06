#include "WinToolbar.h"
#include "Windowsx.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinToolbar::WinToolbar(HWND hWnd, int buttonSize, int numButtons, int id)
{
	hParent = hWnd;
	iButtonSize = buttonSize;
	iNumButtons = numButtons;
	iID = id;
	
	tbrButtons = new TBBUTTON[iNumButtons];
	
	iCount = 0;
}
//--------------------------------------------------------------------------------------
WinToolbar::WinToolbar(const WinToolbar& o)
{
}
//--------------------------------------------------------------------------------------
WinToolbar::~WinToolbar()
{
}
//--------------------------------------------------------------------------------------
void WinToolbar::Init(HWND hWnd, int buttonSize, int numButtons, int id)
{
	hParent = hWnd;
	iButtonSize = buttonSize;
	iNumButtons = numButtons;
	iID = id;
	
	tbrButtons = new TBBUTTON[iNumButtons];
	
	iCount = 0;
}
//--------------------------------------------------------------------------------------
void WinToolbar::AddButton(UINT id, UINT state, UINT style)
{
	tbrButtons[iCount].iBitmap   = iCount;
	tbrButtons[iCount].idCommand = id;
	tbrButtons[iCount].fsState   = state;
	tbrButtons[iCount].fsStyle   = style;
	tbrButtons[iCount].dwData    = 0L;
	tbrButtons[iCount].iString   = 0;
	
	iCount++;
}
//--------------------------------------------------------------------------------------
void WinToolbar::EnableButton(UINT id, bool enable)
{
	SendMessage(hToolbar, TB_ENABLEBUTTON, id, (enable==true)?(LPARAM)FALSE:(LPARAM)TRUE);
}
//--------------------------------------------------------------------------------------
void WinToolbar::Create()
{
	hToolbar = CreateToolbarEx(hParent,
		WS_VISIBLE | WS_CHILD | WS_BORDER | TBSTYLE_TOOLTIPS,
		iID, iNumButtons,
		GetModuleHandle(NULL), iID,
		tbrButtons, iNumButtons,
		iButtonSize, iButtonSize, iButtonSize, iButtonSize,
		sizeof(TBBUTTON));
}
//--------------------------------------------------------------------------------------