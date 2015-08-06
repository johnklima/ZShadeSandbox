//===============================================================================================================================
// LakeBedShader.h
//
//===============================================================================================================================
// History
//
// -Created on 5/29/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __LAKEBEDSHADER_H
#define __LAKEBEDSHADER_H
//==============================================================================================================================
//==============================================================================================================================
#include "D3D.h"
#include "Shader.h"
#include "Camera.h"
#include "Vertex.h"
#include "LightCamera.h"
//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxShader {
class LakeBedShader : public ZShadeSandboxShader::Shader
{
	struct cbFrameBuffer
	{
		XMFLOAT4	g_ClipPlane;
		XMFLOAT3	g_EyePos;
		float		g_CausticTimer;
		XMFLOAT3	g_LightDirection;
		float		g_dummy2;
	};

	struct cbMatrixBuffer
	{
		XMFLOAT4X4	g_matWorld;
		XMFLOAT4X4	g_matView;
		XMFLOAT4X4	g_matProj;
		XMFLOAT4X4	g_matLightViewProj;
		XMFLOAT4X4	g_matShadow;
	};
	
public:
	
	LakeBedShader(D3D* d3d);
	LakeBedShader(const LakeBedShader& other);
	~LakeBedShader();
	
	bool Initialize();
	void Shutdown();
	
	void SetDiffuseMap(ID3D11ShaderResourceView* srv) 	{ m_DiffuseMap = srv; }
	void SetWaveMap0(ID3D11ShaderResourceView* srv) 	{ m_WaveMap0 = srv; }
	void SetWaveMap1(ID3D11ShaderResourceView* srv) 	{ m_WaveMap1 = srv; }
	void SetMicroBump0(ID3D11ShaderResourceView* srv) 	{ g_MicroBump0 = srv; }
	void SetMicroBump1(ID3D11ShaderResourceView* srv) 	{ g_MicroBump1 = srv; }
	void SetDepthMap(ID3D11ShaderResourceView* srv) 	{ g_DepthMap = srv; }
	void SetCausticTexture(ID3D11ShaderResourceView* srv) 	{ g_CausticTexture = srv; }
	
	bool Render(int indexCount, float causticTimer, Camera* camera, LightCamera* lightCamera, XMMATRIX world, XMFLOAT4 clipplane, bool renderDeferred, bool reflect);
	
private:
	
	//
	// Variables
	//
	
	ID3D11Buffer* m_pFrameBufferCB;
	ID3D11Buffer* m_pMatrixBufferCB;
	
	ID3D11ShaderResourceView* m_DiffuseMap;
	ID3D11ShaderResourceView* m_WaveMap0;
	ID3D11ShaderResourceView* m_WaveMap1;
	ID3D11ShaderResourceView* g_MicroBump0;
	ID3D11ShaderResourceView* g_MicroBump1;
	ID3D11ShaderResourceView* g_DepthMap;
	ID3D11ShaderResourceView* g_CausticTexture;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__LAKEBEDSHADER_H