//===============================================================================================================================
// HullException
//
//===============================================================================================================================
// History
//
// -Created on 4/6/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __HULLEXCEPTION_H
#define __HULLEXCEPTION_H
//===============================================================================================================================
//===============================================================================================================================
#include "Exceptions.h"
//===============================================================================================================================
namespace ZShadeSandboxMath {
class HullException : public Exception
{

public:

	// Specify an actual error message
	HullException(const std::string& exceptionMessage)
		: Exception(exceptionMessage)
	{
	}

	// Obtains a string for the specified error code
	HullException (DWORD code)
		: errorCode(code)
	{
		CHAR errorString [MAX_PATH];
		::FormatMessage(	FORMAT_MESSAGE_FROM_SYSTEM,
							0,
							errorCode,
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							errorString,
							MAX_PATH,
							NULL	);

		message = "Win32 Error: ";
		message += errorString;
	}

	// Retrieve the error code
	DWORD GetErrorCode() const throw ()
	{
		return errorCode;
	}

protected:
	
	DWORD errorCode;
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__HULLEXCEPTION_H