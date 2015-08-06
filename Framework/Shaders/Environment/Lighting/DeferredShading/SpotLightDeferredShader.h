//===============================================================================================================================
// SpotLightDeferredShader
//
//===============================================================================================================================
// History
//
// -Created on 6/7/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __SPOTLIGHTDEFERREDSHADER_H
#define __SPOTLIGHTDEFERREDSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "SpotLight.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class SpotLightDeferredShader : public ZShadeSandboxShader::Shader
{
	struct cbLightBuffer
	{
		XMFLOAT4	g_SpotLightColor;
		XMFLOAT3	g_SpotLightPosition;
		float		padding;
		float		g_SpotLightRange;
		float		g_SpotCosOuterCone;
		float		g_SpotInnerConeReciprocal;
		float		g_SpotCosineAngle;
		XMFLOAT3 	g_EyePosition;
		float		g_SpotLightIntensity;
		XMFLOAT4X4	g_InvViewProj;
	};
	
public:

	SpotLightDeferredShader(D3D* d3d);
	SpotLightDeferredShader(const SpotLightDeferredShader& other);
	~SpotLightDeferredShader();

	bool Initialize();
	void Shutdown();

	bool Render11
	(	Camera* camera
	,	ZShadeSandboxLighting::SpotLight* light
	,	ID3D11ShaderResourceView* colorTexture
	,	ID3D11ShaderResourceView* normalTexture
	,	ID3D11ShaderResourceView* depthTexture
	);

private:

	ID3D11Buffer* m_pLightCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__SPOTLIGHTDEFERREDSHADER_H