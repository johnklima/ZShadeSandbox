//******************************************************************************************************
// Convert.h
//******************************************************************************************************
#ifndef __CONVERT_H
#define __CONVERT_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Includes
//#define UNICODE
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "ZMath.h"
using namespace std;

#pragma managed

//#ifdef UNICODE
//#define wchar_t TCHAR
//#else
//#define char TCHAR
//#endif
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
namespace ZShadeSandboxGlobal {
	class Convert
	{
	public:

		/*************************************************************************/
		/* --- Engine conversion methods --- */
		/*************************************************************************/

		static XMFLOAT4 ConvertToFloat4(string p_pNameToConvert)
		{
			BetterString str(p_pNameToConvert);
			vector<string> str_values = str.split(' ');

			XMFLOAT4 value;
			value.x = atof(str_values[0].c_str());
			value.y = atof(str_values[1].c_str());
			value.z = atof(str_values[2].c_str());
			value.w = atof(str_values[3].c_str());

			return value;
		}

		static XMFLOAT3 ConvertToFloat3(string p_pNameToConvert)
		{
			BetterString str(p_pNameToConvert);
			vector<string> str_values = str.split(' ');

			XMFLOAT3 value;
			value.x = atof(str_values[0].c_str());
			value.y = atof(str_values[1].c_str());
			value.z = atof(str_values[2].c_str());

			return value;
		}

		static XMFLOAT2 ConvertToFloat2(string p_pNameToConvert)
		{
			BetterString str(p_pNameToConvert);
			vector<string> str_values = str.split(' ');

			XMFLOAT2 value;
			value.x = atof(str_values[0].c_str());
			value.y = atof(str_values[1].c_str());

			return value;
		}
		
		static string ConvertFloat4ToString(XMFLOAT4 vecToConvert)
		{
			stringstream x, y, z, w;
			x << vecToConvert.x;
			y << vecToConvert.y;
			z << vecToConvert.z;
			w << vecToConvert.w;

			string str;
			str.append(x.str());
			str.append(" ");
			str.append(y.str());
			str.append(" ");
			str.append(z.str());
			str.append(" ");
			str.append(w.str());
			
			return str;
		}

		static string ConvertFloat3ToString(XMFLOAT3 vecToConvert)
		{
			stringstream x, y, z;
			x << vecToConvert.x;
			y << vecToConvert.y;
			z << vecToConvert.z;

			string str;
			str.append(x.str());
			str.append(" ");
			str.append(y.str());
			str.append(" ");
			str.append(z.str());

			return str;
		}

		static string ConvertFloat2ToString(XMFLOAT2 vecToConvert)
		{
			stringstream x, y;
			x << vecToConvert.x;
			y << vecToConvert.y;

			string str;
			str.append(x.str());
			str.append(" ");
			str.append(y.str());

			return str;
		}

		/*************************************************************************/
		/* --- Generic conversion methods --- */
		/*************************************************************************/
		
		template <class T>
		static std::string ConvertToString(const T& t)
		{
			std::stringstream ss;
			ss << t;
			return ss.str();
		}
		
		template <class T>
		static T ConvertStringToT(const std::string& t)
		{
			std::stringstream ss(t);
			T retval;
			ss >> retval;
			return retval;
		}
		
		/*************************************************************************/
		/* --- Char conversion methods --- */
		/*************************************************************************/

		// convert from char to Binary
		static int ConvertCharToBinary(char p_pNameToConvert)
		{
			vector<int> binary_list;

			for (int i = 7; i >= 0; i--)
			{
				int t = (p_pNameToConvert >> i) & 1;
				binary_list.push_back(t);
			}

			int binary = 0;

			//Not finished

			return(binary);
		}

		/*************************************************************************/
		/* --- Char [] conversion methods --- */
		/*************************************************************************/

		// convert from char[] to char *
		static char * ConvertCharBufferToCharPointer(char p_pNameToConvert[])
		{
			return static_cast<char *>(p_pNameToConvert);
		}

		// convert from char[] to char *
		static const char * ConvertCharBufferToConstCharPointer(char p_pNameToConvert[])
		{
			return static_cast<const char *>(p_pNameToConvert);
		}

		// convert from char[] to bool
		static bool ConvertCharBufferToBool(char p_pNameToConvert[])
		{
			if (p_pNameToConvert[0] == 't' || p_pNameToConvert[0] == 'T') return true;
			if (p_pNameToConvert[0] == 'f' || p_pNameToConvert[0] == 'F') return false;
		}

		// convert from char[] to std::string
		static string ConvertCharBufferToString(char p_pNameToConvert[])
		{
			string s;
			s += p_pNameToConvert;
			return s;
		}

		/*************************************************************************/
		/* --- string conversion methods --- */
		/*************************************************************************/

		// convert from const char * to char *
		static char * ConvertConstCharPointerToCharPointer(const char * p_pNameToConvert)
		{
			return const_cast<char *>(p_pNameToConvert);
		}

		// convert from string to char *
		static char * ConvertStringToCharPointer(string p_pNameToConvert)
		{
			char* c = new char[p_pNameToConvert.size() + 1];
			strcpy_s(c, p_pNameToConvert.size() + 1, p_pNameToConvert.c_str());
			return c;
		}

		// convert from string to const char *
		static const char * ConvertStringToConstCharPointer(string p_pNameToConvert)
		{
			return p_pNameToConvert.c_str();
		}

		// convert from string to float
		static double ConvertStringToDouble(string p_pNameToConvert)
		{
			return atof(p_pNameToConvert.c_str());
		}

		// convert from string to int
		static int ConvertStringToInteger(string p_pNameToConvert)
		{
			return atoi(p_pNameToConvert.c_str());
		}

		static float ConvertStringToFloat(string p_pNameToConvert)
		{
			//return (float)atof(p_pNameToConvert.c_str());

			istringstream istr(p_pNameToConvert);
			float out;
			istr >> out;
			return out;
		}

		static char ConvertStringToChar(string p_pNameToConvert)
		{
			return p_pNameToConvert[0];
		}

		static std::string ConvertWStringToString(const std::wstring &wstr)
		{
			// ZShadeSandboxGlobal::Convert a Unicode string to an ASCII string
			std::string strTo;
			char *szTo = new char[wstr.length() + 1];
			szTo[wstr.size()] = '\0';
			WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
			strTo = szTo;
			delete[] szTo;
			return strTo;
		}

		static std::wstring ConvertStringToWString(const std::string &str)
		{
			// ZShadeSandboxGlobal::Convert an ASCII string to a Unicode String
			std::wstring wstrTo;
			wchar_t *wszTo = new wchar_t[str.length() + 1];
			wszTo[str.size()] = L'\0';
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length());
			wstrTo = wszTo;
			delete[] wszTo;
			return wstrTo;
		}

		/*************************************************************************/
		/* --- char * conversion methods --- */
		/*************************************************************************/

		template <typename T>
		static void ConvertCharPointerToTChar(char const * pc, T * pt, size_t len)
		{
			std::wstringstream ss;
			ss << pc;

			wcsncpy(pt, ss.str().c_str(), len);
		}

		template<>
		static void ConvertCharPointerToTChar<char>(char const * pc, char * pt, size_t len)
		{
			strncpy_s(pt, len, pc, _TRUNCATE);
		}

		// convert from char * to bool
		static bool ConvertCharPointerToBool(char * p_pNameToConvert)
		{
			if ((strcmp(p_pNameToConvert, "true") == 0) || (strcmp(p_pNameToConvert, "TRUE") == 0))
				return true;
			if ((strcmp(p_pNameToConvert, "false") == 0) || (strcmp(p_pNameToConvert, "FALSE") == 0))
				return true;
		}

		// convert from char * to int
		static int ConvertCharPointerToInteger(char * p_pNameToConvert)
		{
			return atoi(p_pNameToConvert);
		}

		// convert from const char * to int
		static int ConvertConstCharPointerToInteger(const char * p_pNameToConvert)
		{
			return atoi(p_pNameToConvert);
		}

		// convert from char * to long
		static long ConvertCharPointerToLong(char * p_pNameToConvert)
		{
			return atol(p_pNameToConvert);
		}

		// convert from char * to double
		static double ConvertCharPointerToDouble(char * p_pNameToConvert)
		{
			return atof(p_pNameToConvert);
		}

		// convert from char * to string
		static string ConvertCharPointerToString(char * p_pNameToConvert)
		{
			return p_pNameToConvert;
		}

		// convert from char * to const char *
		static const char * ConvertCharPointerToConstCharPointer(char * p_pNameToConvert)
		{
			string temp;
			temp = p_pNameToConvert;
			return temp.c_str();
		}

		/*************************************************************************/
		/* --- int conversion methods --- */
		/*************************************************************************/

		// convert from int to double
		static double ConvertIntToDouble(int p_pNameToConvert)
		{
			return static_cast<double>(p_pNameToConvert);
		}

		// convert from int to char * in decimal
		static char * ConvertIntToCharPointerInDecimal(int p_pNameToConvert)
		{
			int xDecimal = 10;

			char buffer[1024];

			_itoa_s(p_pNameToConvert, buffer, xDecimal);

			return ConvertCharBufferToCharPointer(buffer);
		}

		// convert from int to char * in hexidecimal
		static char * ConvertIntToCharPointerInHex(int p_pNameToConvert)
		{
			int xHex = 16;

			char buffer[1024];

			_itoa_s(p_pNameToConvert, buffer, xHex);

			return ConvertCharBufferToCharPointer(buffer);
		}

		// convert from int to char * in binary
		static char * ConvertIntToCharPointerInBinary(int p_pNameToConvert)
		{
			int xBinary = 2;

			char buffer[1024];

			_itoa_s(p_pNameToConvert, buffer, xBinary);

			return ConvertCharBufferToCharPointer(buffer);
		}

		// convert from int to string
		static string ConvertIntToString(int p_pNameToConvert)
		{
			stringstream out;
			out << p_pNameToConvert;
			return(out.str());
		}

		static string ConvertFloatToString(float p_pNameToConvert)
		{
			stringstream ss;
			ss << p_pNameToConvert;
			return ss.str();
		}

		/*************************************************************************/
		/* --- TCHAR[] conversion methods --- */
		/*************************************************************************/

		static string ConvertTCharToString(TCHAR* p_pNameToConvert)
		{
			//std::wstring arr_w(p_pNameToConvert); //convert to wstring
			//std::string arr_s( arr_w.begin(), arr_w.end() ); //and convert to string.
			//return arr_s;

			std::basic_string<TCHAR> strName = p_pNameToConvert;
			return strName;
		}
	};
}

#pragma unmanaged
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__CONVERT_H