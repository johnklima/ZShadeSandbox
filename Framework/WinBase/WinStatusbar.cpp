#include "WinStatusbar.h"
#include "Windowsx.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinStatusbar::WinStatusbar(HWND hWnd, int idStatus, int cParts)
{
	RECT rcClient;
    HLOCAL hloc;
    PINT paParts;
    int i, nWidth;
	
	// Create the status bar.
    hStatusbar = CreateWindowEx(
        0,                       // no extended styles
        STATUSCLASSNAME,         // name of status bar class
        (PCTSTR) NULL,           // no text when first created
        SBARS_SIZEGRIP |         // includes a sizing grip
        WS_CHILD | WS_VISIBLE,   // creates a visible child window
        0, 0, 0, 0,              // ignores size and position
        hWnd,                    // handle to parent window
        (HMENU) idStatus,        // child window identifier
		GetModuleHandle(NULL),   // handle to application instance
        NULL);                   // no window creation data
	
	// Get the coordinates of the parent window's client area.
    GetClientRect(hWnd, &rcClient);
	
    // Allocate an array for holding the right edge coordinates.
    hloc = LocalAlloc(LHND, sizeof(int) * cParts);
    paParts = (PINT) LocalLock(hloc);
	
    // Calculate the right edge coordinate for each part, and
    // copy the coordinates to the array.
    nWidth = rcClient.right / cParts;
    int rightEdge = nWidth;
    for (i = 0; i < cParts; i++) { 
       paParts[i] = rightEdge;
       rightEdge += nWidth;
    }
	
    // Tell the status bar to create the window parts.
    SendMessage(hStatusbar, SB_SETPARTS, (WPARAM) cParts, (LPARAM)paParts);
	
    // Free the array, and return.
    LocalUnlock(hloc);
    LocalFree(hloc);
	
	SendMessage(hStatusbar, WM_SIZE, 0, 0);
}
//--------------------------------------------------------------------------------------
WinStatusbar::WinStatusbar(const WinStatusbar& o)
{
}
//--------------------------------------------------------------------------------------
WinStatusbar::~WinStatusbar()
{
}
//--------------------------------------------------------------------------------------
void WinStatusbar::SetText(string text)
{
	SendMessage(hStatusbar, SB_SETTEXT, 2, (LPARAM)text.c_str());
}
//--------------------------------------------------------------------------------------