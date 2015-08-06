//===============================================================================================
//------------------------ ZShadeMessageCenter implementation -----------------------------------
//===============================================================================================

#include "ZShadeMessageCenter.h"
#include <strsafe.h>

void ZShadeMessageCenter::ErrorExit(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process
	
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf);
	MsgBoxError(NULL, (LPCTSTR)lpDisplayBuf, "ZShade ERROR", MB_ICONFLAGS.IconError);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}

void ZShadeMessageCenter::Assert(bool condition, LPCSTR text)
{
	if (!condition)
	{
		MsgBoxError(NULL, text);
		
		PostQuitMessage(0);
	}
}

void ZShadeMessageCenter::MsgBoxError(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type)
{
	icon_type = FindIconName(icon_type);
	if( icon_type == (MB_ICONFLAGS.NotFound) )
		MessageBox(hWnd, text, caption, MB_OK);
	else
		MessageBox(hWnd, text, caption, MB_OK | icon_type);
}

void ZShadeMessageCenter::MsgBoxAbortRetryIgnore(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type)
{
	icon_type = FindIconName(icon_type);
	if( icon_type == (MB_ICONFLAGS.NotFound) )
	{
		if( MessageBox(hWnd, text, caption, MB_ABORTRETRYIGNORE) == BUTTON_RESULT.RETRY )
		{
			
		}
	}
	else
		MessageBox(hWnd, text, caption, MB_ABORTRETRYIGNORE | icon_type);
}

int ZShadeMessageCenter::MsgBoxCancelTryAgainContinue(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type)
{
	icon_type = FindIconName(icon_type);
	if( icon_type == (MB_ICONFLAGS.NotFound) )
		return MessageBox(hWnd, text, caption, MB_CANCELTRYCONTINUE);
	else
		return MessageBox(hWnd, text, caption, MB_CANCELTRYCONTINUE | icon_type);
}

void ZShadeMessageCenter::MsgBoxHelp(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type)
{
	icon_type = FindIconName(icon_type);
	if( icon_type == (MB_ICONFLAGS.NotFound) )
		MessageBox(hWnd, text, caption, MB_HELP);
	else
		MessageBox(hWnd, text, caption, MB_HELP | icon_type);
}

void ZShadeMessageCenter::MsgBoxOK(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type)
{
	icon_type = FindIconName(icon_type);
	if( icon_type == (MB_ICONFLAGS.NotFound) )
		MessageBox(hWnd, text, caption, MB_OK);
	else
		MessageBox(hWnd, text, caption, MB_OK | icon_type);
}

void ZShadeMessageCenter::MsgBoxOKCancel(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type)
{
	icon_type = FindIconName(icon_type);
	if( icon_type == (MB_ICONFLAGS.NotFound) )
		MessageBox(hWnd, text, caption, MB_OKCANCEL);
	else
		MessageBox(hWnd, text, caption, MB_OKCANCEL | icon_type);
}

void ZShadeMessageCenter::MsgBoxRetryCancel(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type)
{
	icon_type = FindIconName(icon_type);
	if( icon_type == (MB_ICONFLAGS.NotFound) )
		MessageBox(hWnd, text, caption, MB_RETRYCANCEL);
	else
		MessageBox(hWnd, text, caption, MB_RETRYCANCEL | icon_type);
}

int ZShadeMessageCenter::MsgBoxYesNo(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type)
{
	icon_type = FindIconName(icon_type);
	if( icon_type == (MB_ICONFLAGS.NotFound) )
		return MessageBox(hWnd, text, caption, MB_YESNO);
	else
		return MessageBox(hWnd, text, caption, MB_YESNO | icon_type);
}

int ZShadeMessageCenter::MsgBoxYesNoCancel(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type)
{
	icon_type = FindIconName(icon_type);
	if( icon_type == (MB_ICONFLAGS.NotFound) )
		return MessageBox(hWnd, text, caption, MB_YESNOCANCEL);
	else
		return MessageBox(hWnd, text, caption, MB_YESNOCANCEL | icon_type);
}

void ZShadeMessageCenter::MsgBoxUniversal(HWND hWnd, LPCSTR text, LPCSTR caption,
										  IconVariable icon_type, ButtonType buttons_type)
{
	
	icon_type = FindIconName(icon_type);
	buttons_type = FindButtonName(buttons_type);
	if( icon_type == (MB_ICONFLAGS.NotFound) )
		MessageBox(hWnd, text, caption, buttons_type);
	else
		MessageBox(hWnd, text, caption, buttons_type | icon_type);
}



IconVariable ZShadeMessageCenter::FindIconName(IconVariable icon_type)
{
	if( icon_type == (MB_ICONFLAGS.IconHand) )
		return MB_ICONHAND;
	if( icon_type == (MB_ICONFLAGS.IconStop) )
		return MB_ICONSTOP;
	if( icon_type == (MB_ICONFLAGS.IconError) )
		return MB_ICONERROR;
	if( icon_type == (MB_ICONFLAGS.IconQuestion) )
		return MB_ICONQUESTION;
	if( icon_type == (MB_ICONFLAGS.IconExclamation) )
		return MB_ICONEXCLAMATION;
	if( icon_type == (MB_ICONFLAGS.IconWarning) )
		return MB_ICONWARNING;
	if( icon_type == (MB_ICONFLAGS.IconAsterisk) )
		return MB_ICONASTERISK;
	if( icon_type == (MB_ICONFLAGS.IconInformation) )
		return MB_ICONINFORMATION;
	
	// Did not find anything
	return (MB_ICONFLAGS.NotFound);
}

ButtonType ZShadeMessageCenter::FindButtonName(ButtonType buttons_type)
{
	if( buttons_type == (MB_BUTTON.ButtonOK) )
		return MB_OK;
	if( buttons_type == (MB_BUTTON.ButtonAbortRetryIgnore) )
		return MB_ABORTRETRYIGNORE;
	if( buttons_type == (MB_BUTTON.ButtonCancelRetryContinue) )
		return MB_CANCELTRYCONTINUE;
	if( buttons_type == (MB_BUTTON.ButtonHelp) )
		return MB_HELP;
	if( buttons_type == (MB_BUTTON.ButtonOKCancel) )
		return MB_OKCANCEL;
	if( buttons_type == (MB_BUTTON.ButtonRetryCancel) )
		return MB_RETRYCANCEL;
	if( buttons_type == (MB_BUTTON.ButtonYesNo) )
		return MB_YESNO;
	if( buttons_type == (MB_BUTTON.ButtonYesNoCancel) )
		return MB_YESNOCANCEL;
	
	// Did not find anything
	return (MB_BUTTON.NoButtonFound);
}
