#include "TerrainTessellationQuadShader.h"
#include "ConstantBuffer.h"
#include "TerrainParameters.h"
//#include "LightShadingBuffers.h"
//==============================================================================================================================
TerrainTessellationQuadShader::TerrainTessellationQuadShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
TerrainTessellationQuadShader::TerrainTessellationQuadShader(const TerrainTessellationQuadShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
TerrainTessellationQuadShader::~TerrainTessellationQuadShader()
{
}
//==============================================================================================================================
bool TerrainTessellationQuadShader::Initialize()
{
	ConstantBuffer<ZShadeSandboxTerrain::cbTessellationBuffer> tessellationCB(m_pD3DSystem);
	tessellationCB.Initialize(PAD16(sizeof(ZShadeSandboxTerrain::cbTessellationBuffer)));
	m_pTessellationCB = tessellationCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxTerrain::cbDomainConstBuffer> domainCB(m_pD3DSystem);
	domainCB.Initialize(PAD16(sizeof(ZShadeSandboxTerrain::cbDomainConstBuffer)));
	m_pDomainCB = domainCB.Buffer();

	ConstantBuffer<ZShadeSandboxTerrain::cbShadingConstBuffer> shadingCB(m_pD3DSystem);
	shadingCB.Initialize(PAD16(sizeof(ZShadeSandboxTerrain::cbShadingConstBuffer)));
	m_pShadingCB = shadingCB.Buffer();

	ConstantBuffer<ZShadeSandboxLighting::cbLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbLightBuffer)));
	m_pLightCB = lightCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbSunLightBuffer> sunCB(m_pD3DSystem);
	sunCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbSunLightBuffer)));
	m_pSunCB = sunCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("TerrainTessellationQuadShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	
	LoadVertexShader("TerrainTessellationQuadVS");
	LoadHullShader("TerrainTessellationQuadHS");
	LoadDomainShader("TerrainTessellationQuadDS");
	LoadPixelShader("TerrainTessellationQuadPS");
	LoadPixelShader("TerrainTessellationWireframePS");

	AssignVertexShaderLayout("TerrainTessellationQuadShader");

	return true;
}
//==============================================================================================================================
void TerrainTessellationQuadShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool TerrainTessellationQuadShader::Render
(	int indexCount
,	Camera* camera
,	ZShadeSandboxLighting::Light* light
,	ZShadeSandboxTerrain::TerrainShadingConst terrainShadingConst
,	ID3D11ShaderResourceView* heightMapSRV
,	ZShadeSandboxLighting::ShaderMaterial* terrainMaterial
)
{
	ID3D11ShaderResourceView* diffuseArrayTexture = 0;
	ID3D11ShaderResourceView* diffuseTexture = 0;
	ID3D11ShaderResourceView* ambientTexture = 0;
	ID3D11ShaderResourceView* specularTexture = 0;
	ID3D11ShaderResourceView* emissiveTexture = 0;
	ID3D11ShaderResourceView* normalMapTexture = 0;
	ID3D11ShaderResourceView* blendMapTexture = 0;
	ID3D11ShaderResourceView* detailMapTexture = 0;
	ID3D11ShaderResourceView* alphaMapTexture = 0;
	ID3D11ShaderResourceView* shadowMapTexture = 0;
	ID3D11ShaderResourceView* ssaoTexture = 0;
	ID3D11ShaderResourceView* displacementTexture = 0;

	terrainMaterial->GetTextures(
		diffuseArrayTexture,
		diffuseTexture,
		ambientTexture,
		specularTexture,
		emissiveTexture,
		normalMapTexture,
		blendMapTexture,
		detailMapTexture,
		alphaMapTexture,
		shadowMapTexture,
		ssaoTexture,
		displacementTexture
	);

	ZShadeSandboxTerrain::BuildTerrainTessellationConstantBuffer(m_pD3DSystem, m_pTessellationCB, terrainShadingConst);
	ZShadeSandboxTerrain::BuildTerrainDomainConstantBuffer(m_pD3DSystem, m_pDomainCB, camera->World(), camera, light, terrainShadingConst);

	XMFLOAT4X4 view;
	view = (terrainShadingConst.g_useReflection) ? camera->ReflectionView4x4() : camera->View4x4();

	ZShadeSandboxTerrain::BuildTerrainShadingConstantBuffer(m_pD3DSystem, m_pShadingCB, view, terrainShadingConst);

	ZShadeSandboxLighting::LightManager::Instance()->BuildFinalLightBuffers(m_pLightCB, m_pSunCB);

	// Set the tessellation constant buffer into the Hull Shader
	ID3D11Buffer* hs_cbs[1] = { m_pTessellationCB };
	m_pD3DSystem->GetDeviceContext()->HSSetConstantBuffers(2, 1, hs_cbs);
	
	// Set the domain constant buffer into the Domain Shader
	// Set the matrix constant buffer into the Domain Shader
	ID3D11Buffer* ds_cbs[1] = { m_pDomainCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(3, 1, ds_cbs);

	// Set the shading constant buffer into the Pixel Shader
	ID3D11Buffer* ps_cbs[5] = { m_pLightCB, m_pSunCB, m_pTessellationCB, m_pDomainCB, m_pShadingCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 5, ps_cbs);

	// Set the heightmap texture into the vertex shader and domain shader
	ID3D11ShaderResourceView* vsds_srvs[1] = { heightMapSRV };
	
	// Set the textures into the Pixel Shader
	ID3D11ShaderResourceView* ps_srvs[7] = {
		diffuseArrayTexture, blendMapTexture, normalMapTexture, heightMapSRV, detailMapTexture, shadowMapTexture, ssaoTexture
	};
	ID3D11SamplerState* ps_samp[4] = { m_pD3DSystem->Point(), m_pD3DSystem->Linear(), m_pD3DSystem->LinearClamp(), m_pD3DSystem->ShadowMapPCF() };
	
	// Tell the shader what input layout to use
	SetInputLayout("TerrainTessellationQuadShader");

	if (!m_Wireframe)
	{
		m_pD3DSystem->GetDeviceContext()->VSSetSamplers(0, 4, ps_samp);
		m_pD3DSystem->GetDeviceContext()->DSSetSamplers(0, 4, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 4, ps_samp);
		
		m_pD3DSystem->GetDeviceContext()->VSSetShaderResources(3, 1, vsds_srvs);
		m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(3, 1, vsds_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 7, ps_srvs);

		SwitchTo("TerrainTessellationQuadPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("TerrainTessellationWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
		m_pD3DSystem->TurnOnWireframe();
	}

	// Assign the shaders to render the mesh
	SetVertexShader();
	SetHullShader();
	SetDomainShader();
	SetPixelShader();
	
	//Perform Drawing
	RenderIndex11(indexCount);
	
	// Unbind
	if (!m_Wireframe)
	{
		ps_samp[0] = NULL;
		ps_samp[1] = NULL;
		ps_samp[2] = NULL;
		ps_samp[3] = NULL;
		m_pD3DSystem->GetDeviceContext()->VSSetSamplers(0, 4, ps_samp);
		m_pD3DSystem->GetDeviceContext()->DSSetSamplers(0, 4, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 4, ps_samp);

		vsds_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->VSSetShaderResources(0, 1, vsds_srvs);
		m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(0, 1, vsds_srvs);
		
		for (int i = 0; i < 7; i++) ps_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 7, ps_srvs);
	}
	
	return true;
}
//==============================================================================================================================