//===============================================================================================================================
// AmbientLightDeferredShader
//
//===============================================================================================================================
// History
//
// -Created on 6/7/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __AMBIENTLIGHTDEFERREDSHADER_H
#define __AMBIENTLIGHTDEFERREDSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "AmbientLight.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class AmbientLightDeferredShader : public ZShadeSandboxShader::Shader
{
	struct cbDeferredLightBuffer
	{
		XMFLOAT4	g_AmbientLightColor;
		XMFLOAT3	g_AmbientDown;
		float		padding1;
		XMFLOAT3	g_AmbientUp;
		float		padding2;
		XMFLOAT4X4	g_InvViewProj;
	};
	
public:
	
	AmbientLightDeferredShader(D3D* d3d);
	AmbientLightDeferredShader(const AmbientLightDeferredShader& other);
	~AmbientLightDeferredShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	Camera* camera
	,	ZShadeSandboxLighting::AmbientLight* light
	,	XMFLOAT3 ambientUp
	,	XMFLOAT3 ambientDown
	,	ID3D11ShaderResourceView* colorTexture
	,	ID3D11ShaderResourceView* normalTexture
	,	ID3D11ShaderResourceView* depthTexture
	);
	
private:
	
	ID3D11Buffer* m_pLightCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__AMBIENTLIGHTDEFERREDSHADER_H