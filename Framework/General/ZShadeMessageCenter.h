//=========================================================================
// ZShadeMessageCenter.h file

#ifndef __ZSHADEMESSAGECENTER_H_
#define __ZSHADEMESSAGECENTER_H_

#pragma once

#include <windows.h>
//#include <strsafe.h>

// add log writing for errors

#ifndef __varsvars
#define __varsvars
typedef UINT (IconVariable);
typedef UINT (ButtonType);
typedef struct tagMBICONFLAGS
{
	static const IconVariable NotFound = -1;
	static const IconVariable IconHand = 1;
	static const IconVariable IconStop = 2;
	static const IconVariable IconError = 3;
	static const IconVariable IconQuestion = 4;
	static const IconVariable IconExclamation = 5;
	static const IconVariable IconWarning = 6;
	static const IconVariable IconAsterisk = 7;
	static const IconVariable IconInformation = 8;
} icon_flags;
static icon_flags MB_ICONFLAGS;
typedef struct tagMBBUTTONTYPE
{
	static const ButtonType NoButtonFound = -1;
	static const ButtonType ButtonOK = 1;
	static const ButtonType ButtonAbortRetryIgnore = 2;
	static const ButtonType ButtonCancelRetryContinue = 3;
	static const ButtonType ButtonHelp = 4;
	static const ButtonType ButtonOKCancel = 5;
	static const ButtonType ButtonRetryCancel = 6;
	static const ButtonType ButtonYesNo = 7;
	static const ButtonType ButtonYesNoCancel = 8;
} button_type;
static button_type MB_BUTTON;
/*
MB_ABORTRETRYIGNORE The message box contains three push buttons: Abort, Retry, and Ignore.
MB_OK The message box contains one push button: OK. This is the default.
MB_OKCANCEL The message box contains two push buttons: OK and Cancel.
MB_RETRYCANCEL The message box contains two push buttons: Retry and Cancel.
MB_YESNO The message box contains two push buttons: Yes and No.
MB_YESNOCANCEL The message box contains three push buttons: Yes, No, and Cancel. 
*/
typedef struct tagBUTTONRESULT
{
	static const ButtonType OK = IDOK;
	static const ButtonType CANCEL = IDCANCEL;
	static const ButtonType YES = IDYES;
	static const ButtonType NO = IDNO;
	static const ButtonType ABORT = IDABORT;
	static const ButtonType RETRY = IDRETRY;
	static const ButtonType IGNOREME = IDIGNORE;
	static const ButtonType NONE = -1;
} button_result;
static button_result BUTTON_RESULT;
#endif


class ZShadeMessageCenter
{
public:
	
	static void Assert(bool condition, LPCSTR text);
	
	// Special System Error function
	// that shows a function error
	// Example: ErrorExit(TEXT("LoadLibrary"));
	static void ErrorExit(LPTSTR lpszFunction);
	
	// Message Boxes For Windows
	static void MsgBoxError(HWND hWnd, LPCSTR text, LPCSTR caption = "ZShade ERROR", IconVariable icon_type = MB_ICONFLAGS.NotFound);
	static void MsgBoxAbortRetryIgnore(HWND hWnd, LPCSTR text, LPCSTR caption = "ZShade ERROR", IconVariable icon_type = MB_ICONFLAGS.NotFound);
	static int MsgBoxCancelTryAgainContinue(HWND hWnd, LPCSTR text, LPCSTR caption = "ZShade ERROR", IconVariable icon_type = MB_ICONFLAGS.NotFound);
	static void MsgBoxHelp(HWND hWnd, LPCSTR text, LPCSTR caption = "ZShade HELP", IconVariable icon_type = MB_ICONFLAGS.NotFound);
	static void MsgBoxOK(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type = MB_ICONFLAGS.NotFound);
	static void MsgBoxOKCancel(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type = MB_ICONFLAGS.NotFound);
	static void MsgBoxRetryCancel(HWND hWnd, LPCSTR text, LPCSTR caption = "ZShade ERROR", IconVariable icon_type = MB_ICONFLAGS.NotFound);
	static int MsgBoxYesNo(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type = MB_ICONFLAGS.NotFound);
	static int MsgBoxYesNoCancel(HWND hWnd, LPCSTR text, LPCSTR caption, IconVariable icon_type = MB_ICONFLAGS.NotFound);
	static void MsgBoxUniversal(HWND hWnd, LPCSTR text, LPCSTR caption,
						IconVariable icon_type = MB_ICONFLAGS.NotFound, ButtonType buttons_type = MB_BUTTON.ButtonOK);
	//void MsgWriteLogFile();
	
private:
	// This will find an icon to display
	static IconVariable FindIconName(IconVariable icon_type);
	
	// This will find a button to display
	static ButtonType FindButtonName(ButtonType buttons_type);
};

#endif /* __ZSHADEMESSAGECENTER_H_ */
