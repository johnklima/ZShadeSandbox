//===============================================================================================================================
// PointLightDeferredShader
//
//===============================================================================================================================
// History
//
// -Created on 6/2/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __POINTLIGHTDEFERREDSHADER_H
#define __POINTLIGHTDEFERREDSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "PointLight.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
/*class PointLightDeferredShader : public ZShadeSandboxShader::Shader
{
	struct cbDomainBuffer
	{
		XMFLOAT4X4 g_LightProjection;
	};
	
	struct cbPixelBuffer
	{
		XMFLOAT3	g_PointLightPosition;
		float		g_PointLightRange;
		XMFLOAT3	g_PointLightColor;
		float		padding1;
		XMFLOAT3	g_PointLightIntensity;
		float		padding2;
		XMFLOAT3	g_EyePosition;
		float		padding3;
		XMFLOAT4	g_PerspectiveValues;
		XMFLOAT4X4	g_ViewInv;
	};
	
public:
	
	PointLightDeferredShader(D3D* d3d);
	PointLightDeferredShader(const PointLightDeferredShader& other);
	~PointLightDeferredShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	Camera* camera
	,	ZShadeSandboxLighting::PointLight* light
	,	ID3D11ShaderResourceView* colorTexture
	,	ID3D11ShaderResourceView* normalTexture
	,	ID3D11ShaderResourceView* depthTexture
	);
	
private:
	
	ID3D11Buffer* m_pDomainCB;
	ID3D11Buffer* m_pPixelCB;
};
*/
class PointLightDeferredShader : public ZShadeSandboxShader::Shader
{
	struct cbLightBuffer
	{
		XMFLOAT4 	g_PointLightColor;
		XMFLOAT3 	g_EyePosition;
		float		g_PointLightIntensity;
		XMFLOAT3	g_PointLightPosition;
		float		g_PointLightRange;
		XMFLOAT4X4	g_InvViewProj;
	};

public:

	PointLightDeferredShader(D3D* d3d);
	PointLightDeferredShader(const PointLightDeferredShader& other);
	~PointLightDeferredShader();

	bool Initialize();
	void Shutdown();

	bool Render11
	(	Camera* camera
	,	ZShadeSandboxLighting::PointLight* light
	,	ID3D11ShaderResourceView* colorTexture
	,	ID3D11ShaderResourceView* normalTexture
	,	ID3D11ShaderResourceView* depthTexture
	);

private:

	ID3D11Buffer* m_pLightCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__POINTLIGHTDEFERREDSHADER_H