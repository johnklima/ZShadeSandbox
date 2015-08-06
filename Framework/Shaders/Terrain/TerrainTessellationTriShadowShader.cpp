#include "TerrainTessellationTriShadowShader.h"
#include "ConstantBuffer.h"
//==============================================================================================================================
TerrainTessellationTriShadowShader::TerrainTessellationTriShadowShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
TerrainTessellationTriShadowShader::TerrainTessellationTriShadowShader(const TerrainTessellationTriShadowShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
TerrainTessellationTriShadowShader::~TerrainTessellationTriShadowShader()
{
}
//==============================================================================================================================
bool TerrainTessellationTriShadowShader::Initialize()
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

	ClearInputLayout();
	SetInputLayoutDesc("TerrainTessellationTriShadowShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	
	LoadVertexShader("TerrainTessellationTriVS");
	LoadHullShader("TerrainTessellationTriShadowHS");
	LoadDomainShader("TerrainTessellationTriDS");

	AssignVertexShaderLayout("TerrainTessellationTriShadowShader");

	return true;
}
//==============================================================================================================================
void TerrainTessellationTriShadowShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool TerrainTessellationTriShadowShader::Render(int indexCount, LightCamera* lightcamera,
	ZShadeSandboxTerrain::TerrainShadingConst terrainShadingConst, ID3D11ShaderResourceView* heightMapSRV)
{
	cTessellationBuffer cTB;
	cMatrixBuffer cMB;
	cDomainConstBuffer cDCB;
	
	m_pD3DSystem->SetRasterizerState(m_pD3DSystem->RSDepth());
	
	//if (!m_Wireframe)
	//	m_pD3DSystem->TurnOnCulling();

	/*if (m_UseCustomWorld)
		cMB.g_WorldMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mWorld.Get()));
	else
		cMB.g_WorldMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(XMMatrixIdentity()));*/
	
	cMB.g_ViewMatrix = lightcamera->LightView4x4();
	cMB.g_ProjMatrix = lightcamera->LightProj4x4();
	cMB.g_ShadowMatrix = lightcamera->ShadowTransform4x4();
	
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

	// Set the tessellation constant buffer into the Hull Shader
	ID3D11Buffer* hs_cbs[1] = { m_pTessellationCB };
	m_pD3DSystem->GetDeviceContext()->HSSetConstantBuffers(0, 1, hs_cbs);
	
	// Set the domain and matrix constant buffer into the Domain Shader
	ID3D11Buffer* ds_cbs[2] = { m_pDomainCB, m_pMatrixBufferCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(0, 2, ds_cbs);

	// Set the heightmap texture into the vertex shader
	ID3D11ShaderResourceView* vs_srvs[1] = { heightMapSRV };
	m_pD3DSystem->GetDeviceContext()->VSSetShaderResources(0, 1, vs_srvs);
	
	// Set the heightmap texture into the domain shader
	ID3D11ShaderResourceView* ds_srvs[1] = { heightMapSRV };
	m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(0, 1, ds_srvs);

	// Tell the shader what input layout to use
	SetInputLayout("TerrainTessellationTriShadowShader");

	// Assign the shaders to render the mesh
	SetVertexShader();
	SetHullShader();
	SetDomainShader();
	
	m_pD3DSystem->GetDeviceContext()->PSSetShader(NULL, NULL, 0);
	
	//Perform Drawing
	RenderIndex11(indexCount);
	
	//m_pD3DSystem->TurnOffCulling();

	return true;
}
//==============================================================================================================================