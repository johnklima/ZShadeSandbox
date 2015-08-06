//#include "stdafx.h"
#include "WaterShader.h"
#include "ConstantBuffer.h"
//==============================================================================================================================
WaterShader::WaterShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
,   mUseFlowMap(false)
{
	Initialize();
}
//==============================================================================================================================
WaterShader::WaterShader(const WaterShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
WaterShader::~WaterShader()
{
}
//==============================================================================================================================
bool WaterShader::Initialize()
{
	ConstantBuffer<cbWaterBuffer> waterBufferCB(m_pD3DSystem);
	waterBufferCB.Initialize(PAD16(sizeof(cbWaterBuffer)));
	m_pWaterBufferCB = waterBufferCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixBufferCB(m_pD3DSystem);
	matrixBufferCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixBufferCB = matrixBufferCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("WaterShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	LoadVertexShader("WaterNormalVS");
	LoadVertexShader("WaterFlowVS");
	LoadPixelShader("WaterNormalPS");
	LoadPixelShader("WaterFlowPS");
	LoadPixelShader("WaterNormalWireframePS");
	LoadPixelShader("WaterFlowWireframePS");
	LoadVertexShader("WaterGBufferNormalVS");
	LoadVertexShader("WaterGBufferFlowVS");
	LoadPixelShader("WaterGBufferNormalPS");
	LoadPixelShader("WaterGBufferFlowPS");
	LoadPixelShader("WaterGBufferNormalWireframePS");
	LoadPixelShader("WaterGBufferFlowWireframePS");
	AssignVertexShaderLayout("WaterShader");

	return true;
}
//==============================================================================================================================
void WaterShader::Shutdown()
{
	//ShutdownShader();
}
//==============================================================================================================================
//bool WaterShader::RenderWater11(int indexCount, WaterParameters wp, Camera* camera,
//	ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* pReflectionMap, ID3D11ShaderResourceView* pRefractionMap)
void WaterShader::RenderFunc(int indexCount, ZShadeSandboxMesh::MeshRenderParameters mrp, ZShadeSandboxLighting::ShaderMaterial* material)
{
	cbWaterBuffer cWB;
	cbMatrixBuffer cMB;

	cMB.g_matWorld = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mrp.world));
	cMB.g_matView = mrp.camera->View4x4();
	cMB.g_matReflView = mrp.camera->ReflectionView4x4();

	if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_2D)
		cMB.g_matProj = mrp.camera->Ortho4x4();
	else if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_3D)
		cMB.g_matProj = mrp.camera->Proj4x4();

	cWB.g_WaveHeight = mWaterParameters->g_WaveHeight;
	cWB.g_SpecularShininess = mWaterParameters->g_SpecularShininess;
	cWB.g_Time = mWaterParameters->g_Time;
	cWB.g_HalfCycle = mWaterParameters->g_HalfCycle;
	cWB.g_CamPos = mWaterParameters->g_CamPos;
	cWB.g_FlowMapOffset0 = mWaterParameters->g_FlowMapOffset0;
	cWB.g_LightDirection = mWaterParameters->g_LightDirection;
	cWB.g_FlowMapOffset1 = mWaterParameters->g_FlowMapOffset1;
	cWB.g_RefractionTint = mWaterParameters->g_RefractionTint;
	cWB.g_DullColor = mWaterParameters->g_DullColor;
	cWB.g_wpadding = XMFLOAT2(0, 0);
	cWB.g_FarPlane = fFarPlane;
	cWB.g_TexScale = mWaterParameters->g_TexScale;

	// Map the water parameter constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pWaterBufferCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbWaterBuffer*)mapped_res.pData = cWB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pWaterBufferCB, 0);
	}

	// Map the matrix constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixBufferCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbMatrixBuffer*)mapped_res.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixBufferCB, 0);
	}

	ID3D11Buffer* vs_cbs[2] = { m_pWaterBufferCB, m_pMatrixBufferCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 2, vs_cbs);

	ID3D11Buffer* ps_cbs[1] = { m_pWaterBufferCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);

	ID3D11ShaderResourceView* ps_srvs[6] = { m_FlowMap, m_NoiseMap, m_WaveMap0, m_WaveMap1, m_ReflectionMap, m_RefractionMap };
	ID3D11SamplerState* ps_samp[2] = { m_pD3DSystem->Mirror(), m_pD3DSystem->Linear() };

	if (!m_Wireframe)
	{
		// Assign Texture

		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 2, ps_samp);

		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 6, ps_srvs);

		if (mUseFlowMap)
		{
			SwitchTo("WaterFlowPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
		}
		else
		{
			if (mUseGBuffer)
			{
				SwitchTo("WaterGBufferNormalPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
			}
			else
			{
				SwitchTo("WaterNormalPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
			}
		}
	}
	else
	{
		if (mUseFlowMap)
		{
			SwitchTo("WaterFlowWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
		}
		else
		{
			if (mUseGBuffer)
			{
				SwitchTo("WaterGBufferNormalWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
			}
			else
			{
				SwitchTo("WaterNormalWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
			}
		}
	}

	if (mUseFlowMap)
	{
		SwitchTo("WaterFlowVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
	}
	else
	{
		if (mUseGBuffer)
		{
			SwitchTo("WaterGBufferNormalVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
		}
		else
		{
			SwitchTo("WaterNormalVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
		}
	}

	SetInputLayout("WaterShader");

	SetVertexShader();
	SetPixelShader();

	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		ps_samp[0] = NULL;
		ps_samp[1] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 2, ps_samp);

		for (int i = 0; i < 6; i++) ps_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 6, ps_srvs);
	}
}
//==============================================================================================================================