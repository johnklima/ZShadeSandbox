//=====================================================================================
// SunShader.h
//=====================================================================================
#ifndef __SUNSHADER_H
#define __SUNSHADER_H
//==============================================================================================================================
//==============================================================================================================================
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "CustomMeshParameters.h"
//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxShader {
class SunShader : public ZShadeSandboxShader::Shader
{
	struct cbSunConst
	{
		XMFLOAT2 padding;
		float g_Alpha;
		float g_Glow;
	};
	
	struct Const_Per_Frame
	{
		XMFLOAT4X4 g_World;
		XMFLOAT4X4 g_View;
		XMFLOAT4X4 g_Proj;
	};
	
public:
	SunShader(D3D* d3d);
	SunShader(const SunShader& other);
	~SunShader();

	bool Initialize();
	void Shutdown();
	
	bool Render11(int indexCount, float alpha, float glow, Camera* camera, ID3D11ShaderResourceView* textureSRV);

private:
	
	ID3D11Buffer* m_pSunCB;
	ID3D11Buffer* m_pPerFrameCB;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__SUNSHADER_H