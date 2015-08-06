//===============================================================================================================================
// DirectionalLightDeferredShader
//
//===============================================================================================================================
// History
//
// -Created on 6/2/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __DIRECTIONALLIGHTDEFERREDSHADER_H
#define __DIRECTIONALLIGHTDEFERREDSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "DirectionalLight.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class DirectionalLightDeferredShader : public ZShadeSandboxShader::Shader
{
	struct cbLightBuffer
	{
		XMFLOAT4	g_DirectionLightColor;
		XMFLOAT3	g_LightDirection;
		float		g_LightIntensity;
		XMFLOAT3	g_EyePosition;
		float		padding2;
		XMFLOAT4X4	g_InvViewProj;
	};
	
public:
	
	DirectionalLightDeferredShader(D3D* d3d);
	DirectionalLightDeferredShader(const DirectionalLightDeferredShader& other);
	~DirectionalLightDeferredShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	Camera* camera
	,	ZShadeSandboxLighting::DirectionalLight* light
	,	ID3D11ShaderResourceView* colorTexture
	,	ID3D11ShaderResourceView* normalTexture
	,	ID3D11ShaderResourceView* depthTexture
	);
	
private:
	
	ID3D11Buffer* m_pLightCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__DIRECTIONALLIGHTDEFERREDSHADER_H