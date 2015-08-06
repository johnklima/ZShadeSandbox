//================================================================================================================
// CGlobal.h
//
//================================================================================================================
// History
//
// Created on 10/9/2014 by Dustin Watson
//================================================================================================================
#ifndef __CGLOBAL_H
#define __CGLOBAL_H
//================================================================================================================
// C/C++ Includes
//=========================================================================================================================
#define _CRT_SECURE_NO_WARNINGS
#define _XM_NO_INTRINSICS_
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

#include <filesystem>
namespace filesystem = std::tr2::sys;
/* or namespace filesystem = boost::filesystem */

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
//#include <tchar.h>
//#include <strsafe.h>
#include <iomanip>
#include <numeric>
#include <chrono>
#include <string>
#include <cstring>
#include <vector>
#include <io.h>         // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <sstream>
#include "BetterString.h"
#include "D3DIncludes.h"
#include "GameDirectory.h"
#include "EngineOptions.h"
#include "ZShadeMessageCenter.h"
#include "Convert.h"
using namespace std;
//=========================================================================================================================
//================================================================================================================
// Macros
#define MAX_PATH_LENGTH 1024
#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete [] (x); (x) = NULL; }
#define SHOWERROR(s,f,l) char buf[1024]; sprintf( buf, "File: %s\nLine: %d\n%s",f,l,s); MessageBox( 0, buf, "Error", 0 );
#define PAD16(n) (((n)+15)/16*16)
//=========================================================================================================================
typedef signed char        int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;
//=========================================================================================================================
//=========================================================================================================================
namespace DXColors
{
	XMGLOBALCONST XMVECTORF32 White     = {1.0f, 1.0f, 1.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}
//=========================================================================================================================
struct uint2
{
	uint32 x;
	uint32 y;
};
//=========================================================================================================================
// Returns true if a file exits
inline bool FileExists(const CHAR* fileName)
{
    _ASSERT(fileName);

    DWORD fileAttr;
    fileAttr = GetFileAttributes(fileName);
    if (fileAttr == INVALID_FILE_ATTRIBUTES)
        return false;

    return true;
}
//=========================================================================================================================
// Clamps a value to the specified range
inline float Clamp(float val, float min, float max)
{
	_ASSERT(max >= min);

	if (val < min)
		val = min;
	else if (val > max)
		val = max;
	return val;
}
//=========================================================================================================================
inline XMFLOAT2 Clamp(const XMFLOAT2& val, const XMFLOAT2 min, const XMFLOAT2 max)
{
	XMFLOAT2 retVal;
	retVal.x = Clamp(val.x, min.x, max.x);
	retVal.y = Clamp(val.y, min.y, max.y);
}
//=========================================================================================================================
inline XMFLOAT3 Clamp(const XMFLOAT3& val, const XMFLOAT3 min, const XMFLOAT3 max)
{
	XMFLOAT3 retVal;
	retVal.x = Clamp(val.x, min.x, max.x);
	retVal.y = Clamp(val.y, min.y, max.y);
	retVal.z = Clamp(val.z, min.z, max.z);
}
//=========================================================================================================================
inline XMFLOAT4 Clamp(const XMFLOAT4& val, const XMFLOAT4 min, const XMFLOAT4 max)
{
	XMFLOAT4 retVal;
	retVal.x = Clamp(val.x, min.x, max.x);
	retVal.y = Clamp(val.y, min.y, max.y);
	retVal.z = Clamp(val.z, min.z, max.z);
	retVal.w = Clamp(val.w, min.w, max.w);
}
//=========================================================================================================================
// Returns a size suitable for creating a constant buffer, by rounding up
// to the next multiple of 16
inline UINT CBSize(UINT size)
{
    UINT cbsize = size + (16 - (size % 16));
    return cbsize;
}
//=========================================================================================================================
//=========================================================================================================================
template <typename T>
inline void ProfilerRun (T&&  func, const std::string& routine_name = "unknown",
                  const char* file = "unknown", unsigned line = 0)
{
    using std::chrono::duration_cast;
    using std::chrono::microseconds;
    using std::chrono::steady_clock;
    using std::cerr;
    using std::endl;

    steady_clock::time_point t_begin = steady_clock::now();
	
    // Call the function
    func();

    steady_clock::time_point t_end = steady_clock::now();
    
	/*
	cerr << "[" << std::setw (20)
         << (std::strrchr (file, '/') ?
             std::strrchr (file, '/') + 1 : file)
         << ":" << std::setw (5) << line << "]   "
         << std::setw (10) << std::setprecision (6) << std::fixed
         << static_cast<float> (duration_cast<microseconds>
                                (t_end - t_begin).count()) / 1e6
         << "s  --> " << routine_name << endl;

    cerr.unsetf (std::ios_base::floatfield);
	*/
	
	BetterString str("[");
	str += (std::strrchr (file, '/') ? std::strrchr (file, '/') + 1 : file);
	str += ":";
	str += ZShadeSandboxGlobal::Convert::ConvertIntToString(line);
	str += "]   ";
	float t = static_cast<float> (duration_cast<microseconds>(t_end - t_begin).count()) / 1e6;
	str += ZShadeSandboxGlobal::Convert::ConvertFloatToString(t);
	str += "s --> ";
	str += routine_name;
	
	ZShadeMessageCenter::MsgBoxOK(NULL, str, "Profiler");
}
//=========================================================================================================================
//http://stackoverflow.com/questions/20168051/why-push-back-is-slower-than-operator-for-an-previous-allocated-vector
#define PROFILE(BLOCK, ROUTNAME) ProfilerRun([&](){do {BLOCK;} while(0);}, ROUTNAME, __FILE__, __LINE__);
//=========================================================================================================================
inline void OutputSum(uint64_t sum, BetterString caption)
{
	std::ostringstream o;
	o << sum;
	BetterString str = "sum -> ";
	str += o.str();
	ZShadeMessageCenter::MsgBoxOK(NULL, str, caption);
}
//=========================================================================================================================
template<class T>
void adjust_size(std::vector<T>& v, int k, double factor)
{
    if(k >= v.size())
    {
        v.resize(v.size() < 10 ? 10 : k*factor);
    }
}
//=========================================================================================================================
static unsigned int x = 0;
template<class T>
void /*__attribute__((noinline))*/ silly_branch(std::vector<T>& v, int k)
{
    if(k == x)
    {
        x = x < 10 ? 10 : x*2;
    }
    //++x;
}
//=========================================================================================================================
inline void ProfilerTest()
{
	const int N = (1 << 28);
	const int PAGESIZE = 4096;
	
	// Test 1
	{
		uint64_t sum;
		int* bigarray = new int[N];
		
		PROFILE(
			{
				for (int k = 0, *p = bigarray; p != bigarray + N; ++p, ++k)
					*p = k;
			},
			"new (routine1)"
		);
		
		sum = std::accumulate(bigarray, bigarray + N, 0ULL);
		
		delete [] bigarray;
		
		//OutputSum(sum, "routine1 ProfilerTest");
	}
	
	// Test 2
	{
		uint64_t sum;
		int* bigarray = new int[N];
		
		memset(bigarray, 0, sizeof(int) * N);
		
		PROFILE(
			{
				for (int k = 0, *p = bigarray; p != bigarray + N; ++p, ++k)
					*p = k;
			},
			"new + full memset (routine2)"
		);
		
		sum = std::accumulate(bigarray, bigarray + N, 0ULL);
		
		delete [] bigarray;
		
		//OutputSum(sum, "routine2 ProfilerTest");
	}
	
	// Test 3
	{
		uint64_t sum;
		int* bigarray = new int[N];
		
		for (int k = 0; k < N; k += PAGESIZE / 2 / sizeof(int))
			bigarray[k] = 0;
		
		PROFILE(
			{
				for (int k = 0, *p = bigarray; p != bigarray + N; ++p, ++k)
					*p = k;
			},
			"new + strided memset (every page half) (routine3)"
		);
		
		sum = std::accumulate(bigarray, bigarray + N, 0ULL);
		
		delete [] bigarray;
		
		//OutputSum(sum, "routine3 ProfilerTest");
	}
	
	// Test 4
	{
		uint64_t sum;
		int* bigarray = new int[N];
		
		for (int k = 0; k < N; k += PAGESIZE / 1 / sizeof(int))
			bigarray[k] = 0;
		
		PROFILE(
			{
				for (int k = 0, *p = bigarray; p != bigarray + N; ++p, ++k)
					*p = k;
			},
			"new + strided memset (every page) (routine4)"
		);
		
		sum = std::accumulate(bigarray, bigarray + N, 0ULL);
		
		delete [] bigarray;
		
		//OutputSum(sum, "routine4 ProfilerTest");
	}
	
	// Test 5
	{
		uint64_t sum;
		int* bigarray = new int[N];
		
		for (int k = 0; k < N; k += PAGESIZE * 2 / sizeof(int))
			bigarray[k] = 0;
		
		PROFILE(
			{
				for (int k = 0, *p = bigarray; p != bigarray + N; ++p, ++k)
					*p = k;
			},
			"new + strided memset (every other page) (routine5)"
		);
		
		sum = std::accumulate(bigarray, bigarray + N, 0ULL);
		
		delete [] bigarray;
		
		//OutputSum(sum, "routine5 ProfilerTest");
	}
	
	// Test 6
	{
		uint64_t sum;
		int* bigarray = new int[N];
		
		for (int k = 0; k < N; k += PAGESIZE * 4 / sizeof(int))
			bigarray[k] = 0;
		
		PROFILE(
			{
				for (int k = 0, *p = bigarray; p != bigarray + N; ++p, ++k)
					*p = k;
			},
			"new + strided memset (every 4th page) (routine6)"
		);
		
		sum = std::accumulate(bigarray, bigarray + N, 0ULL);
		
		delete [] bigarray;
		
		//OutputSum(sum, "routine6 ProfilerTest");
	}
	
	// Test 7
	{
		uint64_t sum;
		vector<int> bigarray(N);
		
		PROFILE(
			{
				for (int k = 0; k < N; ++k)
					bigarray[k] = k;
			},
			"vector, using ctor to initialize (routine7)"
		);
		
		sum = std::accumulate(begin(bigarray), end(bigarray), 0ULL);
		
		//OutputSum(sum, "routine7 ProfilerTest");
	}
	
	// Test 8
	{
		uint64_t sum;
		vector<int> bigarray(N);
		
		PROFILE(
			{
				for (int k = 0; k < N; ++k)
					bigarray.push_back(k);
			},
			"vector (+ no reserve) + push_back (routine8)"
		);
		
		sum = std::accumulate(begin(bigarray), end(bigarray), 0ULL);
		
		//OutputSum(sum, "routine8 ProfilerTest");
	}
	
	// Test 9
	{
		uint64_t sum;
		vector<int> bigarray;
		bigarray.reserve(N);
		
		PROFILE(
			{
				for (int k = 0; k < N; ++k)
					bigarray.push_back(k);
			},
			"vector + reserve + push_back (routine9)"
		);
		
		sum = std::accumulate(begin(bigarray), end(bigarray), 0ULL);
		
		//OutputSum(sum, "routine9 ProfilerTest");
	}
	
	// Test 10
	{
		uint64_t sum;
		vector<int> bigarray;
		bigarray.reserve(N);
		
		memset(bigarray.data(), 0, sizeof(int) * N);
		
		PROFILE(
			{
				for (int k = 0; k < N; ++k)
					bigarray.push_back(k);
			},
			"vector + reserve + memset (UB) + push_back (routine10)"
		);
		
		sum = std::accumulate(begin(bigarray), end(bigarray), 0ULL);
		
		//OutputSum(sum, "routine10 ProfilerTest");
	}
	
	// Test 11
	{
		uint64_t sum;
		vector<int> bigarray;
		
		PROFILE(
			{
				for (int k = 0; k < N; ++k)
				{
					adjust_size(bigarray, k, 1.5);
					bigarray[k] = k;
				}
			},
			"vector + custom emplace_back @ factor 1.5 (routine11)"
		);
		
		sum = std::accumulate(begin(bigarray), end(bigarray), 0ULL);
		
		//OutputSum(sum, "routine11 ProfilerTest");
	}
	
	// Test 12
	{
		uint64_t sum;
		vector<int> bigarray;
		
		PROFILE(
			{
				for (int k = 0; k < N; ++k)
				{
					adjust_size(bigarray, k, 2);
					bigarray[k] = k;
				}
			},
			"vector + custom emplace_back @ factor 2 (routine12)"
		);
		
		sum = std::accumulate(begin(bigarray), end(bigarray), 0ULL);
		
		//OutputSum(sum, "routine12 ProfilerTest");
	}
	
	// Test 13
	{
		uint64_t sum;
		vector<int> bigarray;
		
		PROFILE(
			{
				for (int k = 0; k < N; ++k)
				{
					adjust_size(bigarray, k, 3);
					bigarray[k] = k;
				}
			},
			"vector + custom emplace_back @ factor 3 (routine13)"
		);
		
		sum = std::accumulate(begin(bigarray), end(bigarray), 0ULL);
		
		//OutputSum(sum, "routine13 ProfilerTest");
	}
	
	// Test 14
	{
		uint64_t sum;
		vector<int> bigarray;
		
		PROFILE(
			{
				for (int k = 0; k < N; ++k)
					bigarray.emplace_back(k);
			},
			"vector + (+ no reserve) + emplace_back (routine14)"
		);
		
		sum = std::accumulate(begin(bigarray), end(bigarray), 0ULL);
		
		//OutputSum(sum, "routine14 ProfilerTest");
	}
	
	// Test 15
	{
		uint64_t sum;
		vector<int> bigarray;
		bigarray.reserve(N);
		
		PROFILE(
			{
				for (int k = 0; k < N; ++k)
					bigarray.emplace_back(k);
			},
			"vector + reserve + emplace_back (routine15)"
		);
		
		sum = std::accumulate(begin(bigarray), end(bigarray), 0ULL);
		
		//OutputSum(sum, "routine15 ProfilerTest");
	}
	
	// Test 16
	{
		uint64_t sum;
		vector<int> bigarray;
		bigarray.reserve(N);
		
		memset(bigarray.data(), 0, sizeof(bigarray[0]) * N);
		
		PROFILE(
			{
				for (int k = 0; k < N; ++k)
					bigarray.emplace_back(k);
			},
			"vector + reserve + memset (UB) + emplace_back (routine16)"
		);
		
		sum = std::accumulate(begin(bigarray), end(bigarray), 0ULL);
		
		//OutputSum(sum, "routine16 ProfilerTest");
	}
	
	// Test 17
	{
		uint64_t sum;
		vector<int> bigarray(N);
		
		PROFILE(
			{
				for (int k = 0; k < N; ++k)
				{
					silly_branch(bigarray, k);
					bigarray[k] = k;
				}
			},
			"vector, using ctor to initialize + silly branch (routine17)"
		);
		
		sum = std::accumulate(begin(bigarray), end(bigarray), 0ULL);
		
		//OutputSum(sum, "routine17 ProfilerTest");
	}
}
//=========================================================================================================================
//=========================================================================================================================
template<typename T>
class ArrayDeleter
{
public:
	ArrayDeleter(T* ptr)
	{
		_ASSERT(ptr);
		array = ptr;
	}

	~ArrayDeleter() { delete[] array; }

protected:
	T* array;
};

template<typename T>
class IUnknownReleaser
{
public:
	IUnknownReleaser(T* ptr)
	{
		_ASSERT(ptr);
		this->ptr = ptr;
	}

	~IUnknownReleaser() { ptr->Release(); }

protected:
	T* ptr;
};
//=========================================================================================================================
//=========================================================================================================================
class CFileWriter
{
	std::ofstream outputFile;

public:

	CFileWriter();
	void Open(string filename);
	void Write(string contents);
	void Close();
};
//=========================================================================================================================
//=========================================================================================================================
class CGlobal
{
public:
	
	//Searches through folders to find a media file. Adapted from the DirectX Sample Framework.
    static BOOL GetMediaFile( char* file, char path[] );
	
	// Returns a list of drives available on the system
	static vector<string> GetAvailableDrives();
	
	// If file extension is not found returns empty string
	static string GetFileExt(string filename);
	
	static string FindInitialPath();
	static string LocateAnimationDirectory(const char* pFileName, const char* pProfileName, char* pExtraType, bool pIsExt, char* pExtName);
	static void ClientResize(HWND hWnd, int nWidth, int nHeight);
	static bool DoesFolderExist(string folder);
	static bool DoesFileExist(string file);
	static bool IsMouseOverWindow(HWND hWnd, const int mx, const int my, const bool inClientSpace = false);
	static vector<string> GetFilenames(char path_in[MAX_PATH], char sub_path[MAX_PATH]);
	
	static uintmax_t FileSize(std::string filename);
	
	// Get the current username in windows
	static string GrabUserName();
	
	static GameDirectory2D* CreateGD2D(string gameFolder, GameType2D gameType2D);
	static GameDirectory2D* LoadGameDirectory2D(string game_folder);

	static GameDirectory3D* CreateGD3D(string gameFolder);
	static GameDirectory3D* LoadGameDirectory3D(string game_folder);
	
	// Get the horizontal and vertical screen sizes in pixel
	static void GetDesktopResolution(int& horizontal, int& vertical);
	
	// Builds a list of names from files from within a directory
	static vector<string> CacheNames(string filepath);
	
private:
	
	//Searches through folders to find a media file. Adapted from the DirectX Sample Framework.
    static BOOL SearchFolders( char* filename, char* exeFolder, char* exeName, char fullPath[] );
};
//=========================================================================================================================
//=========================================================================================================================
#endif//__CGLOBAL_H