//===============================================================================================================================
// CapsuleLightDeferredShader
//
//===============================================================================================================================
// History
//
// -Created on 6/7/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __CAPSULELIGHTDEFERREDSHADER_H
#define __CAPSULELIGHTDEFERREDSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "CapsuleLight.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class CapsuleLightDeferredShader : public ZShadeSandboxShader::Shader
{
	struct cbLightBuffer
	{
		XMFLOAT3 	g_CapsuleLightPosition;
		float 		g_CapsuleLightRange;
		XMFLOAT3 	g_CapsuleLightDirection;
		float 		g_CapsuleLightLength;
		XMFLOAT3 	g_CapsuleDirectionLength;
		float 		g_CapsuleIntensity;
		XMFLOAT4	g_CapsuleDiffuseColor;
		XMFLOAT3	g_EyePosition;
		float		padding2;
		XMFLOAT4X4	g_ViewInvProj;
	};
	
public:
	
	CapsuleLightDeferredShader(D3D* d3d);
	CapsuleLightDeferredShader(const CapsuleLightDeferredShader& other);
	~CapsuleLightDeferredShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	Camera* camera
	,	ZShadeSandboxLighting::CapsuleLight* light
	,	ID3D11ShaderResourceView* colorTexture
	,	ID3D11ShaderResourceView* normalTexture
	,	ID3D11ShaderResourceView* depthTexture
	);
	
private:
	
	ID3D11Buffer* m_pLightCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__CAPSULELIGHTDEFERREDSHADER_H