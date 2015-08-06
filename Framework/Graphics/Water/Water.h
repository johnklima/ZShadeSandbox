//=====================================================================================
//Water.h
//=====================================================================================
#ifndef __WATER_H
#define __WATER_H
//==============================================================================================================================
//==============================================================================================================================
#include "D3D.h"
#include "Vertex.h"
#include "Camera.h"
#include "WaterShader.h"
#include "PlaneMesh.h"
#include "WaterParameters.h"
//==============================================================================================================================
//==============================================================================================================================
class Water
{
public:
	Water();
	Water(const Water& other);
	~Water();
	
	bool Initialize(D3D* d3d, WaterParameters wp);
	void Update(float speed, float seaLevel);
	void Render(Camera* pCamera, bool renderDeferred, ID3D11ShaderResourceView* pReflectionMap, ID3D11ShaderResourceView* pRefractionMap);
	
	float GetWaterHeight();
	
	void SetUseFlowmap(bool set) 						{ bUseFlowmap = set; }
	void SetWireframe(bool set) 						{ m_Wireframe = set; }
	
	void SetWaveHeight(float waveHeight) 				{ mWaterParameters->g_WaveHeight = waveHeight; }
	void SetSpecularShininess(float SpecularShininess) 	{ mWaterParameters->g_SpecularShininess = SpecularShininess; }
	void SetLightDirection(XMFLOAT3 LightDirection) 	{ mWaterParameters->g_LightDirection = LightDirection; }
	void SetRefractionTint(XMFLOAT4 RefractionTint) 	{ mWaterParameters->g_RefractionTint = RefractionTint; }
	void SetWaterHeight(float waterHeight) 				{ m_waterHeight = waterHeight; mWaterParameters->g_waterHeight = waterHeight; }
	
	void SetFlowMapSRV(ID3D11ShaderResourceView* srv) { m_pFlowMapSRV = srv; }
	void SetNoiseMapSRV(ID3D11ShaderResourceView* srv) { m_pNoiseMapSRV = srv; }
	void SetWaveMap0SRV(ID3D11ShaderResourceView* srv) { m_pWaveMap0SRV = srv; }
	void SetWaveMap1SRV(ID3D11ShaderResourceView* srv) { m_pWaveMap1SRV = srv; }
	
	void LoadFlowMapSRV(string flowMapFilename);
	void LoadNoiseMapSRV(string noiseMapFilename);
	void LoadWaveMap0SRV(string waveMap0Filename);
	void LoadWaveMap1SRV(string waveMap1Filename);
	
	void RepositionWater(XMFLOAT3 pos);
	void ResizeWater(float radius);
	void SetFarPlane(float farPlane) { fFarPlane = farPlane; }

	// Lets you know when something touches the water
	bool IsTouching(XMFLOAT3 point);
	
private:
	
	ZShadeSandboxMesh::PlaneMesh* mPlane;

	bool bUseFlowmap;
	bool m_Wireframe;
	float m_waterHeight;
	float mWaterRadius;
	
	float fFarPlane;
	float fCycle;
	float fHalfCycle;
	float fFlowSpeed;
	float fWaterTranslation;
	float fFlowMapOffset0;
	float fFlowMapOffset1;
	
	ID3D11ShaderResourceView* m_pFlowMapSRV;
	ID3D11ShaderResourceView* m_pNoiseMapSRV;
	ID3D11ShaderResourceView* m_pWaveMap0SRV;
	ID3D11ShaderResourceView* m_pWaveMap1SRV;
	
	WaterParameters* mWaterParameters;
	XMFLOAT4X4 mWorld;
	WaterShader* mWaterShader;
	D3D* m_d3d;
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__WATER_H