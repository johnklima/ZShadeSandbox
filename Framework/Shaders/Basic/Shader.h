//===============================================================================================================================
// Shader
//
//===============================================================================================================================
// History
//
// -Created in 2014 by Dustin Watson
//===============================================================================================================================
#ifndef __SHADER_H
#define __SHADER_H
//=================================================================================================================
//=================================================================================================================

//
//Includes
//

#include <D3Dcompiler.h>
#include <map>
#include "D3DIncludes.h"
#include "D3D.h"
#include <fstream>
#include "SkyPlaneParameters.h"
#include "ZMath.h"
//#include "BetterString.h"
#include "Camera.h"
#include "ShaderCompiler.h"
#include "CustomMeshParameters.h"
#include "ShaderMaterial.h"
#include "MeshRenderParameters.h"
using namespace std;

//=================================================================================================================
//=================================================================================================================
namespace ZShadeSandboxShader {
class Shader
{
public:
	
	Shader(D3D* d3d);
	Shader(const Shader& c);
	virtual ~Shader();
	
	//void UseOrtho(bool useOrtho) { m_UseOrtho = useOrtho; }
	//void SetOrtho(XMMATRIX ortho) { mOrtho = ortho; }
	
	//void UseCustomWorld(bool useWorld) { m_UseCustomWorld = useWorld; }
	//void SetCustomWorld(ZShadeSandboxMath::XMMatrix world) { mWorld = world; }
	
	//void UseCustomView(bool useView) { m_UseCustomView = useView; }
	//void SetCustomView(XMFLOAT4X4 view) { mView = view; }
	
	//Create a custom shader with this for the CustomMesh
	//Override this and add whatever is needed to render the shader
	virtual void RenderFunc(int indexCount, ZShadeSandboxMesh::MeshRenderParameters mrp, ZShadeSandboxLighting::ShaderMaterial* material) {}
	//virtual void RenderFunc(int indexCount, ZShadeSandboxMesh::MeshRenderParameters mrp, ID3D11ShaderResourceView* texture) {}
	
	void SetVertexShader();
	void SetPixelShader();
	void SetHullShader();
	void SetDomainShader();
	void SetGeometryShader();
	void SetComputeShader();
	void SetDefaultInputLayout();
	void SetInputLayout(string vertexShaderName);
	void AssignVertexShaderLayout(string vertexShaderName);
	void SwitchTo(string shaderFuncName, int type);
	
	void RenderDraw11(int vertexCount);
	void RenderIndex11(int indexCount);
	void RenderIndex11(int startIndex, int indexCount);
	void RenderIndexInstanced11(int indexCount, int primCount);
	void RenderCS11(int threadX, int threadY, int threadZ);
	
	//shaderFuncName is the name of the actual function to compile
	//HRESULT Compile(BetterString filename, BetterString shaderFuncName, ShaderTypes type, bool loadPrecompiledShader = false);
	
	/*
		Compiles a binary .cso file
		Usage

		std::vector<uint8_t> vertexShaderData = CompileBinary("VertexShader.cso");
		if(vertexShaderData.empty()) {  handle it }
		auto wannaKnowSize = vertexShaderData.size();
	*/
	//std::vector<uint8_t> CompileBinary(const std::string filename);

	// Loads the shader from the shader compiler
	void LoadVertexShader(string shaderFuncName);
	void LoadPixelShader(string shaderFuncName);
	void LoadHullShader(string shaderFuncName);
	void LoadDomainShader(string shaderFuncName);
	void LoadGeometryShader(string shaderFuncName);
	void LoadComputeShader(string shaderFuncName);

	ID3D11VertexShader* GetVertexShader(string shaderFuncName);
	ID3D11PixelShader* GetPixelShader(string shaderFuncName);
	ID3D11GeometryShader* GetGeometryShader(string shaderFuncName);
	ID3D11ComputeShader* GetComputeShader(string shaderFuncName);

	//void SetWireframe(bool set) { m_Wireframe = set; }
	//bool IsWireframe() { return m_Wireframe; }
	bool& Wireframe() { return m_Wireframe; }
	bool Wireframe() const { return m_Wireframe; }

	D3D* GetD3D() { return m_pD3DSystem; }
	
	//Locate the current directory the shader is at
	string LocateDir();
	
	ID3D11InputLayout* GetInputLayout(string vertexShaderName);
	void SetInputLayoutDesc(string vertexShaderName, D3D11_INPUT_ELEMENT_DESC* desc, UINT NumElements);
	void SetInputLayoutCreation(bool set) { m_UseInputLayout = set; }
	void ClearInputLayout();
	
protected:
	
	//ZShadeSandboxMath::XMMatrix mWorld;
	//XMFLOAT4X4 mView;
	//bool m_UseCustomWorld;
	//bool m_UseCustomView;
	//XMMATRIX mOrtho;
	//bool m_UseOrtho;
	bool m_Wireframe;
	bool m_UseInputLayout;
	string m_filename;
	D3D* m_pD3DSystem;
	
	struct SLayout
	{
		D3D11_INPUT_ELEMENT_DESC* m_pLayoutDesc;
		UINT m_NumElements;
		ID3D11InputLayout* m_layout11;
	};
	
	string mCurrentLayoutName;
	ID3D11InputLayout* m_CurrentLayout11;

	// Lookup key is the name of the vertex shader
	map<string, SLayout*> m_VertexShaderLayout;
	
	//
	// HLSL Shaders
	//
	
	string mCurrentVSFuncName;
	string mCurrentPSFuncName;
	string mCurrentGSFuncName;
	string mCurrentCSFuncName;

	VertexShader*   m_pVertexShader;
	HullShader*     m_pHullShader;
	DomainShader*   m_pDomainShader;
	PixelShader*    m_pPixelShader;
	GeometryShader* m_pGeometryShader;
	ComputeShader*  m_pComputeShader;
	
	// Ability to have multiple shaders to switch back and forth from
	map<string, VertexShader*>   m_VertexShaders;
	map<string, PixelShader*>    m_PixelShaders;
	map<string, GeometryShader*> m_GeometryShaders;
	map<string, ComputeShader*>  m_ComputeShaders;
};
}
//=================================================================================================================
//=================================================================================================================
#endif//__SHADER_H