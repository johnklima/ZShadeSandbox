//==============================================================================
// SkyPlaneShader.h
//
// Sky Plane of moving clouds
//==============================================================================
// History
//
// -Created on 4/13/2015 by Dustin Watson
//==============================================================================
#ifndef __SKYPLANESHADER_H
#define __SKYPLANESHADER_H
//==============================================================================================================================
//==============================================================================================================================
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "SkyPlaneParameters.h"
//==============================================================================================================================
//==============================================================================================================================
class SkyPlaneShader : public ZShadeSandboxShader::Shader
{
	struct cbSkyPlaneBuffer
	{
		float g_Translation;
		float g_Scale;
		float g_Brightness;
		float padding;
	};
	
	/*struct cbMatrixBuffer
	{
		XMFLOAT4X4 g_matWorld;
		XMFLOAT4X4 g_matView;
		XMFLOAT4X4 g_matProj;
	};*/
	
public:
	
	SkyPlaneShader(D3D* d3d);
	SkyPlaneShader(const SkyPlaneShader& other);
	~SkyPlaneShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	int indexCount
	,	XMMATRIX world
	,	ZShadeSandboxMath::XMMath4 clipplane
	,	Camera* camera
	,	ID3D11ShaderResourceView* cloudTexture
	,	ID3D11ShaderResourceView* perturbTexture
	,	float translation
	,	float scale
	,	float brightness
	);
	
	//bool Render11(int indexCount, Camera* camera, ID3D11ShaderResourceView* cloudTexture, ID3D11ShaderResourceView* perturbTexture,
	//	SkyPlaneParameters spp);

	/*void RenderFunc(int indexCount, Camera* pCamera, XMMATRIX view, XMMATRIX world, XMFLOAT4 clipplane,
		ID3D11ShaderResourceView* cloudTexture, ID3D11ShaderResourceView* perturbTexture,
		SkyPlaneParameters spp, bool useReflection);
	
	bool Render11(int indexCount, Camera* camera, ID3D11ShaderResourceView* cloudTexture, ID3D11ShaderResourceView* perturbTexture,
		SkyPlaneParameters spp);
	bool Render11Reflection(int indexCount, Camera* camera, XMFLOAT3 eye, XMMATRIX reflection,
		ID3D11ShaderResourceView* cloudTexture, ID3D11ShaderResourceView* perturbTexture,
		SkyPlaneParameters spp);*/
	
private:
	
	ID3D11Buffer* m_pSkyPlaneCB;
	ID3D11Buffer* m_pMatrixCB;
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__SKYPLANESHADER_H