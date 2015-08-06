//===============================================================================================================================
// BlurShader
//
//===============================================================================================================================
// History
//
// -Created on 7/2/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __BLURSHADER_H
#define __BLURSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class BlurShader : public ZShadeSandboxShader::Shader
{
	struct CB_Const
	{
		float g_Width;
		float g_Height;
		XMFLOAT2 dummy;
	};

	/*struct Const_Per_Frame
	{
		XMMATRIX g_World;
		XMMATRIX g_View;
		XMMATRIX g_Proj;
	};*/

public:
	BlurShader(D3D* d3d);
	BlurShader(const BlurShader& other);
	~BlurShader();

	bool Initialize();
	void Shutdown();
	
	bool Render11(int indexCount, Camera* camera, float w, float h,
		ID3D11ShaderResourceView* horizontalBlurTexture,
		ID3D11ShaderResourceView* verticalBlurTexture);

	/*
	void SetWorld(CXMMATRIX m)                    { pWorld->SetMatrix(reinterpret_cast<const float*>(&m)); }
	void SetView(CXMMATRIX m)                     { pView->SetMatrix(reinterpret_cast<const float*>(&m)); }
	void SetProj(CXMMATRIX m)                     { pProj->SetMatrix(reinterpret_cast<const float*>(&m)); }

	void SetScreenWidth(float m)                  { pScreenWidth->SetFloat(m); }
	void SetScreenHeight(float m)                 { pScreenHeight->SetFloat(m); }
	
	void SetHorizontalBlurTexture(ID3D11ShaderResourceView* m)  { pHorizontalBlurMap->SetResource(m); }
	void SetVerticalBlurTexture(ID3D11ShaderResourceView* m)    { pVerticalBlurMap->SetResource(m); }
	*/

	void SetHorizontalBlur(bool set) { m_HorizontalBlur = set; }

private:
	bool InitializeShader11();
	
	void ShutdownShader();
	
	void RenderShader(D3D* d3d, int indexCount);

	bool m_HorizontalBlur;

	//ID3D11InputLayout* m_layout11;
	
	ID3D11Buffer* m_pCBConst;
	ID3D11Buffer* m_pPerObjectCB;

	/*
	ID3DX11EffectTechnique* HorizontalBlurTech;
	ID3DX11EffectTechnique* VerticalBlurTech;

	ID3DX11EffectScalarVariable* pScreenWidth;
	ID3DX11EffectScalarVariable* pScreenHeight;
	
	ID3DX11EffectMatrixVariable* pWorld;
	ID3DX11EffectMatrixVariable* pView;
	ID3DX11EffectMatrixVariable* pProj;

	ID3DX11EffectShaderResourceVariable* pHorizontalBlurMap;
	ID3DX11EffectShaderResourceVariable* pVerticalBlurMap;
	*/
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__BLURSHADER_H