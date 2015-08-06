#include "Water.h"
#include "TextureManager.h"
#include "MaterialManager.h"
//==============================================================================================================================
Water::Water()
{
	bUseFlowmap = false;
	m_Wireframe = false;
	
	fWaterTranslation = 0;
	fFlowMapOffset0 = 0;
	fFlowMapOffset1 = 0;
	
	m_pFlowMapSRV = 0;
	m_pNoiseMapSRV = 0;
	m_pWaveMap0SRV = 0;
	m_pWaveMap1SRV = 0;
}
//==============================================================================================================================
Water::Water(const Water& other)
{
}
//==============================================================================================================================
Water::~Water()
{
	if (mWaterParameters != NULL)
	{
		delete mWaterParameters;
		mWaterParameters = NULL;
	}
}
//==============================================================================================================================
bool Water::Initialize(D3D* d3d, WaterParameters wp)
{
	bool result;
	
	m_d3d = d3d;
	
	// Cycle for movement of a flowmap
	fCycle = 0.15f;
	fHalfCycle = fCycle * 0.5f;
	fFlowSpeed = 0.05f;
	
	mWaterParameters = new WaterParameters();
	mWaterParameters->g_WaveHeight = wp.g_WaveHeight;
	mWaterParameters->g_SpecularShininess = wp.g_SpecularShininess;
	mWaterParameters->g_LightDirection = wp.g_LightDirection;
	mWaterParameters->g_RefractionTint = wp.g_RefractionTint;
	mWaterParameters->g_waterHeight = wp.g_waterHeight;
	mWaterParameters->g_waterRadius = wp.g_waterRadius;
	mWaterParameters->g_DullColor = wp.g_DullColor;
	mWaterParameters->g_TexScale = wp.g_TexScale;
	mWaterParameters->g_HalfCycle = fHalfCycle;
	
	//Initialize the water shader
	mWaterShader = new WaterShader(d3d);
	
	// Store the water height and radius
	m_waterHeight = mWaterParameters->g_waterHeight;
	mWaterRadius = mWaterParameters->g_waterRadius;

	ZShadeSandboxMesh::MeshParameters mp;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.in2D = false;
	mp.useCustomShader = true;
	mp.shader = mWaterShader;
	mp.material = 0;
	
	mPlane = new ZShadeSandboxMesh::PlaneMesh(mWaterRadius, m_waterHeight, m_d3d, mp);

	//Load the world matrix
	XMStoreFloat4x4(&mWorld, XMMatrixIdentity());

	return true;
}
//==============================================================================================================================
void Water::LoadFlowMapSRV(string flowMapFilename)
{
	m_pFlowMapSRV = TextureManager::Instance()->GetTexture(flowMapFilename);
}
//==============================================================================================================================
void Water::LoadNoiseMapSRV(string noiseMapFilename)
{
	m_pNoiseMapSRV = TextureManager::Instance()->GetTexture(noiseMapFilename);
}
//==============================================================================================================================
void Water::LoadWaveMap0SRV(string waveMap0Filename)
{
	m_pWaveMap0SRV = TextureManager::Instance()->GetTexture(waveMap0Filename);
}
//==============================================================================================================================
void Water::LoadWaveMap1SRV(string waveMap1Filename)
{
	m_pWaveMap1SRV = TextureManager::Instance()->GetTexture(waveMap1Filename);
}
//==============================================================================================================================
bool Water::IsTouching(XMFLOAT3 point)
{
	return mPlane->IntersectsAABB(point);
}
//==============================================================================================================================
void Water::RepositionWater(XMFLOAT3 pos)
{
	mPlane->Position() = pos;
	
	// Update the AABB
	mPlane->UpdateAABB(pos, XMFLOAT3(mWaterRadius, m_waterHeight, mWaterRadius));
}
//==============================================================================================================================
void Water::ResizeWater(float radius)
{
	mWaterRadius = radius;
	mWaterParameters->g_waterRadius = mWaterRadius;
	mPlane->Scale() = XMFLOAT3(mWaterRadius, m_waterHeight, mWaterRadius);
	
	// Update the AABB
	mPlane->UpdateAABB(mPlane->Position(), XMFLOAT3(mWaterRadius, m_waterHeight, mWaterRadius));
}
//==============================================================================================================================
void Water::Update(float speed, float seaLevel)
{
	// If a change in sea level happens then update the water height
	if (m_waterHeight != seaLevel)
	{
		m_waterHeight = seaLevel;

		mPlane->UpdateBuffers(m_waterHeight);
	}

	if (bUseFlowmap)
	{
		//update the flow map offsets for both layers
		fFlowMapOffset0 += fFlowSpeed * speed;
		fFlowMapOffset1 += fFlowSpeed * speed;
		
		//if ( fFlowMapOffset0 >= fCycle )
		//	fFlowMapOffset0 = 0.0f;
		
		//if ( fFlowMapOffset1 >= fCycle )
		//	fFlowMapOffset1 = 0.0f;
	}
	else
	{
		// Update the position of the water to simulate motion.
		fWaterTranslation += fFlowSpeed * speed;
		
		//if (fWaterTranslation > 1.0f)
		//{
		//	fWaterTranslation -= 1.0f;
		//}
	}
	
	return;
}
//==============================================================================================================================
void Water::Render(Camera* pCamera, bool renderDeferred, ID3D11ShaderResourceView* pReflectionMap, ID3D11ShaderResourceView* pRefractionMap)
{
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	
	mWaterParameters->g_CamPos = pCamera->Position();
	mWaterParameters->g_Time = fWaterTranslation;
	mWaterParameters->g_FlowMapOffset0 = fFlowMapOffset0;
	mWaterParameters->g_FlowMapOffset1 = fFlowMapOffset1;
	
	mWaterShader->SetFarPlane(fFarPlane);
	mWaterShader->UseFlowMap(bUseFlowmap);
	mWaterShader->SetWaterParameters(mWaterParameters);
	mWaterShader->SetFlowMap(m_pFlowMapSRV);
	mWaterShader->SetNoiseMap(m_pNoiseMapSRV);
	mWaterShader->SetWaveMap0(m_pWaveMap0SRV);
	mWaterShader->SetWaveMap1(m_pWaveMap1SRV);
	mWaterShader->SetReflectionMap(pReflectionMap);
	mWaterShader->SetRefractionMap(pRefractionMap);
	mWaterShader->UseGBuffer(renderDeferred);
	mWaterShader->Wireframe() = m_Wireframe;

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = pCamera;
	mPlane->Render(mrp);

	return;
}
//==============================================================================================================================
float Water::GetWaterHeight()
{
	return m_waterHeight;
}
//==============================================================================================================================