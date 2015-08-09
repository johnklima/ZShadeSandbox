//==================================================================================================================================
// afxHeader.h
// 
// Header file that defines some stuff that everything needs
// This header was created so that the engine will be able to run in windows 7, 8 or 10
//==================================================================================================================================
// History
//
// -Created on 12/18/2014 by Dustin Watson
//==================================================================================================================================

#pragma once

#include <windows.h>


#pragma pack(push, 1) // No padding


#define WIN32_LEAN_AND_MEAN

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


//
// Defines the version of windows
//

// #define WINVER 0x0601

#if WINVER == 0x0601 // Windows 7
#define _WIN32_WINNT 0x0601
#elif WINVER == 0x0602 // Windows 8
#define _WIN32_WINNT 0x0602
#elif WINVER == 0x0603 // Windows 8.1
#define _WIN32_WINNT 0x0603
#elif WINVER == 0x0A00 // Windows 10
#define _WIN32_WINNT 0x0A00
#endif

#pragma pack(pop) // Back to previous packing mode