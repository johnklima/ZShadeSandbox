//===============================================================================================================================
// ShaderCompiler.h
//
// Compiles all shaders for the engine
//===============================================================================================================================
// History
//
// -Created on 4/10/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __SHADERCOMPILER_H
#define __SHADERCOMPILER_H
//=================================================================================================================
//=================================================================================================================

//
//Includes
//

#include <D3Dcompiler.h>
#include <map>
#include <string>
#include "D3D.h"
#include "ShaderStruct.h"
#include "BetterString.h"
using std::map;
using std::string;

#pragma comment(lib, "D3DCompiler.lib")

/*
D3DCompile - pInclude [in, optional]

Optional. A pointer to an ID3DInclude for handling include files.
Setting this to NULL will cause a compile error if a shader contains a #include.
You can pass the D3D_COMPILE_STANDARD_FILE_INCLUDE macro, which is a pointer to a
default include handler. This default include handler includes files that are relative
to the current directory and files that are relative to the directory of the initial source file.
When you use D3D_COMPILE_STANDARD_FILE_INCLUDE, you must specify the source file name in
the pSourceName parameter; the compiler will derive the initial relative directory from pSourceName.
*/
#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)

//=================================================================================================================
//=================================================================================================================
namespace ZShadeSandboxShader {
class ShaderCompiler
{
	class ShaderMacros
	{
		//Macro definition support
		static const UINT32 MaxDefines = 16;
		static const UINT32 BufferSize = 1024;

		D3D_SHADER_MACRO defines[MaxDefines + 1];
		CHAR buffer[BufferSize];
		UINT32 numDefines;
		UINT32 bufferIdx;

		void ClearMacroDefines();

	public:

		ShaderMacros();
		const UINT32 NumDefines() const { return numDefines; }
		const D3D10_SHADER_MACRO* Defines() const { return defines; }
		void AddMacro(const std::string& name, UINT32 value);
	};

	static map<string, ShaderMacros*> m_ShaderMacros;
	static vector<ZShadeSandboxShader::ShaderBase*> m_Shaders;
	
	static ID3DBlob* Compile(LPCWSTR name, BetterString shaderFuncName, LPCSTR profile, UINT flags, ID3DBlob*& errorBlob, ShaderMacros* macros = 0, bool loadPrecompiledShader = false);
	static void OutputShaderErrorMessage(ID3D10Blob* blob, HWND hwnd, BetterString shaderFuncName, string shaderFilename);
	
public:
	
	static void AddMacro(BetterString macroName, const std::string& name, UINT32 value);

	// Use this to compile all shaders and recompile all shaders in the engine
	static void CompileAll(D3D* d3d);
	
	// Compile a single shader
	static HRESULT Compile(D3D* d3d, BetterString filename, BetterString shaderFuncName, int type, BetterString macroName = "", bool loadPrecompiledShader = false);
	
	static ZShadeSandboxShader::ShaderBase* GetShaderByName(BetterString name);
};
}
//=================================================================================================================
//=================================================================================================================
#endif//__SHADERCOMPILER_H