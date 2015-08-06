#include "TerrainTessellationTriShader.h"
#include "ConstantBuffer.h"
//==============================================================================================================================
TerrainTessellationTriShader::TerrainTessellationTriShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
TerrainTessellationTriShader::TerrainTessellationTriShader(const TerrainTessellationTriShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
TerrainTessellationTriShader::~TerrainTessellationTriShader()
{
}
//==============================================================================================================================
bool TerrainTessellationTriShader::Initialize()
{
	ConstantBuffer<cTessellationBuffer> tessellationCB(m_pD3DSystem);
	tessellationCB.Initialize(PAD16(sizeof(cTessellationBuffer)));
	m_pTessellationCB = tessellationCB.Buffer();
	
	ConstantBuffer<cMatrixBuffer> domainCB(m_pD3DSystem);
	domainCB.Initialize(PAD16(sizeof(cMatrixBuffer)));
	m_pDomainCB = domainCB.Buffer();

	ConstantBuffer<cMatrixBuffer> matrixBufferCB(m_pD3DSystem);
	matrixBufferCB.Initialize(PAD16(sizeof(cMatrixBuffer)));
	m_pMatrixBufferCB = matrixBufferCB.Buffer();

	ConstantBuffer<cMatrixBuffer> shadingCB(m_pD3DSystem);
	shadingCB.Initialize(PAD16(sizeof(cMatrixBuffer)));
	m_pShadingCB = shadingCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("TerrainTessellationTriShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	
	LoadVertexShader("TerrainTessellationTriVS");
	LoadHullShader("TerrainTessellationTriHS");
	LoadDomainShader("TerrainTessellationTriDS");
	LoadPixelShader("TerrainTessellationTriPS");

	AssignVertexShaderLayout("TerrainTessellationTriShader");

	return true;
}
//==============================================================================================================================
void TerrainTessellationTriShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool TerrainTessellationTriShader::Render(int indexCount, Camera* camera, LightCamera* lightcamera, bool bReflect, ZShadeSandboxTerrain::TerrainShadingConst terrainShadingConst,
	ID3D11ShaderResourceView* layerMapSRV, ID3D11ShaderResourceView* blendMapSRV,
	ID3D11ShaderResourceView* heightMapSRV, ID3D11ShaderResourceView* normalMapSRV,
	ID3D11ShaderResourceView* shadowMapSRV)
{
	cTessellationBuffer cTB;
	cMatrixBuffer cMB;
	cDomainConstBuffer cDCB;
	cShadingConstBuffer cSCB;
	
	//if (!m_Wireframe)
	//	m_pD3DSystem->TurnOnCulling();

	/*if (m_UseCustomWorld)
		cMB.g_WorldMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mWorld.Get()));
	else
		cMB.g_WorldMatrix = camera->World4x4();*/
	
	if (bReflect)
		cMB.g_ViewMatrix = camera->ReflectionView4x4();
	else
		cMB.g_ViewMatrix = camera->View4x4();

	cMB.g_ProjMatrix = camera->Proj4x4();
	
	cTB.g_EyePosW = terrainShadingConst.g_EyePosW;
	cTB.g_MinDist = terrainShadingConst.g_MinDist;
	cTB.g_MaxDist = terrainShadingConst.g_MaxDist;
	cTB.g_MinTess = terrainShadingConst.g_MinTess;
	cTB.g_MaxTess = terrainShadingConst.g_MaxTess;
	cTB.g_FrustumCull = terrainShadingConst.g_FrustumCull;
	cTB.padding = XMFLOAT2(0, 0);
	cTB.g_TerrSize = terrainShadingConst.g_MapSize;
	cTB.g_TerrScale = terrainShadingConst.g_TerrSize;

	cDCB.g_TexScale = XMFLOAT2(66.0f, 66.0f);
	cDCB.g_HeightScale = terrainShadingConst.g_HeightScale;
	cDCB.g_TerrainZScale = terrainShadingConst.g_TerrSize;
	cDCB.g_TexelCellSpaceU = terrainShadingConst.g_TexelCellSpaceU;
	cDCB.g_TexelCellSpaceV = terrainShadingConst.g_TexelCellSpaceV;
	cDCB.padding = XMFLOAT2(0, 0);
	cDCB.g_ClipPlane = terrainShadingConst.g_ClipPlane;
	
	cSCB.g_EyePosW = terrainShadingConst.g_EyePosW;
	cSCB.padding1 = 0;
	cSCB.g_fogThinning = terrainShadingConst.g_fogThinning;
	cSCB.g_fogAltitude = terrainShadingConst.g_fogAltitude;
	cSCB.g_fogNear = terrainShadingConst.g_fogNear;
	cSCB.g_fogFar = terrainShadingConst.g_fogFar;
	cSCB.g_fogColor = terrainShadingConst.g_fogColor;
	cSCB.g_TextureAmount = terrainShadingConst.g_TextureAmount;
	cSCB.g_TextureWidth = terrainShadingConst.g_TextureWidth;
	cSCB.g_TextureHeight = terrainShadingConst.g_TextureHeight;
	cSCB.g_seaLevel = terrainShadingConst.g_seaLevel;
	cSCB.g_waterBodyColor = terrainShadingConst.g_waterBodyColor;
	//cSCB.g_SunDir = terrainShadingConst.g_SunDir;
	cSCB.padding2 = 0;
	//cSCB.g_AmbientColor = terrainShadingConst.g_AmbientColor;
	//cSCB.g_SpecularColor = terrainShadingConst.g_SpecularColor;
	//cSCB.g_DiffuseColor = terrainShadingConst.g_DiffuseColor;
	cSCB.g_useClippingPlane = terrainShadingConst.g_useClippingPlane;
	cSCB.g_useNormalMap = terrainShadingConst.g_useNormalMap;
	cSCB.g_UseSobelFilter = terrainShadingConst.g_UseSobelFilter;
	cSCB.g_useShadowMap = terrainShadingConst.g_useShadowMap;
	
	// Map tessellation constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pTessellationCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cTessellationBuffer*)mapped_res.pData = cTB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pTessellationCB, 0);
	}
	
	// Map domain shader constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pDomainCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cDomainConstBuffer*)mapped_res.pData = cDCB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pDomainCB, 0);
	}
	
	// Map matrix constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixBufferCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cMatrixBuffer*)mapped_res.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixBufferCB, 0);
	}
	
	// Map pixel shading constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pShadingCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cShadingConstBuffer*)mapped_res.pData = cSCB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pShadingCB, 0);
	}

	// Set the tessellation constant buffer into the Hull ZShadeSandboxShader::Shader
	ID3D11Buffer* hs_cbs[1] = { m_pTessellationCB };
	m_pD3DSystem->GetDeviceContext()->HSSetConstantBuffers(0, 1, hs_cbs);
	
	// Set the domain constant buffer into the Domain ZShadeSandboxShader::Shader
	// Set the matrix constant buffer into the Domain ZShadeSandboxShader::Shader
	ID3D11Buffer* ds_cbs[2] = { m_pDomainCB, m_pMatrixBufferCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(0, 2, ds_cbs);

	// Set the shading constant buffer into the Pixel ZShadeSandboxShader::Shader
	ID3D11Buffer* ps_cbs[1] = { m_pShadingCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);

	// Set the heightmap texture into the vertex shader
	ID3D11ShaderResourceView* vs_srvs[1] = { heightMapSRV };
	m_pD3DSystem->GetDeviceContext()->VSSetShaderResources(0, 1, vs_srvs);
	
	// Set the heightmap texture into the domain shader
	ID3D11ShaderResourceView* ds_srvs[1] = { heightMapSRV };
	m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(0, 1, ds_srvs);

	// Set the textures into the Pixel ZShadeSandboxShader::Shader
	ID3D11ShaderResourceView* ps_srvs[4] = { layerMapSRV, blendMapSRV, normalMapSRV, heightMapSRV };
	m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 4, ps_srvs);
	
	// Tell the shader what input layout to use
	SetInputLayout("TerrainTessellationTriShader");

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
		vs_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->VSSetShaderResources(0, 1, vs_srvs);

		ds_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(0, 1, ds_srvs);
		
		for (int i = 0; i < 4; i++) ps_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 4, ps_srvs);
	}
	
	//m_pD3DSystem->TurnOffCulling();

	return true;
}
//==============================================================================================================================