//=====================================================================================
//WaterShader.h
//=====================================================================================
#ifndef __WATERSHADER_H
#define __WATERSHADER_H
//==============================================================================================================================
//==============================================================================================================================
#include "D3D.h"
#include "Shader.h"
#include "Camera.h"
#include "Vertex.h"
#include "WaterParameters.h"
//==============================================================================================================================
//==============================================================================================================================
class WaterShader : public ZShadeSandboxShader::Shader
{
	struct cbWaterBuffer
	{
		float 		g_WaveHeight;
		float 		g_SpecularShininess;
		float 		g_Time;
		float 		g_HalfCycle;
		XMFLOAT3 	g_CamPos;
		float		g_FlowMapOffset0;
		XMFLOAT3 	g_LightDirection;
		float		g_FlowMapOffset1;
		XMFLOAT4 	g_RefractionTint;
		XMFLOAT4  	g_DullColor;
		XMFLOAT2  	g_wpadding;
		float		g_FarPlane;
		float 		g_TexScale;
	};

	struct cbMatrixBuffer
	{
		XMFLOAT4X4	g_matWorld;
		XMFLOAT4X4	g_matView;
		XMFLOAT4X4	g_matProj;
		XMFLOAT4X4	g_matReflView;
	};
	
public:
	
	WaterShader(D3D* d3d);
	WaterShader(const WaterShader& other);
	~WaterShader();
	
	bool Initialize();
	void Shutdown();
	
	void SetFarPlane(float farPlane) { fFarPlane = farPlane; }

	void SetWaterParameters(WaterParameters* wp) { mWaterParameters = wp; }
	
	void SetFlowMap(ID3D11ShaderResourceView* srv) { m_FlowMap = srv; }
	void SetNoiseMap(ID3D11ShaderResourceView* srv) { m_NoiseMap = srv; }
	void SetWaveMap0(ID3D11ShaderResourceView* srv) { m_WaveMap0 = srv; }
	void SetWaveMap1(ID3D11ShaderResourceView* srv) { m_WaveMap1 = srv; }
	void SetReflectionMap(ID3D11ShaderResourceView* srv) { m_ReflectionMap = srv; }
	void SetRefractionMap(ID3D11ShaderResourceView* srv) { m_RefractionMap = srv; }
	
	void RenderFunc(int indexCount, ZShadeSandboxMesh::MeshRenderParameters mrp, ZShadeSandboxLighting::ShaderMaterial* material);
	
	void UseGBuffer(bool use) { mUseGBuffer = use; }
	void UseFlowMap(bool use) { mUseFlowMap = use; }

private:
	
	//
	// Variables
	//
	
	bool mUseGBuffer;
	bool mUseFlowMap;
	
	float fFarPlane;

	ID3D11Buffer* m_pWaterBufferCB;
	ID3D11Buffer* m_pMatrixBufferCB;
	
	WaterParameters* mWaterParameters;
	
	ID3D11ShaderResourceView* m_FlowMap;
	ID3D11ShaderResourceView* m_NoiseMap;
	ID3D11ShaderResourceView* m_WaveMap0;
	ID3D11ShaderResourceView* m_WaveMap1;
	ID3D11ShaderResourceView* m_ReflectionMap;
	ID3D11ShaderResourceView* m_RefractionMap;
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__WATERSHADER_H