#include "TreeShader.h"
#include "ConstantBuffer.h"
#include "ZMath.h"
//==============================================================================================================================
TreeShader::TreeShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
TreeShader::TreeShader(const TreeShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
TreeShader::~TreeShader()
{
}
//==============================================================================================================================
bool TreeShader::Initialize()
{
	ConstantBuffer<cbTreeBuffer> treeCB(m_pD3DSystem);
	treeCB.Initialize(PAD16(sizeof(cbTreeBuffer)));
	m_pTreeBufferCB = treeCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixBufferCB(m_pD3DSystem);
	matrixBufferCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixBufferCB = matrixBufferCB.Buffer();
	
	D3D11_INPUT_ELEMENT_DESC leafLayout[5] =
	{
		{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, NUM_LEAVES_PER_TREE }
	};
	
	ClearInputLayout();
	SetInputLayoutDesc("TreeShaderLeaf", leafLayout, 5);
	LoadVertexShader("TreeVS");
	LoadGeometryShader("TreeBillboardGS");
	LoadPixelShader("TreePS");
	LoadPixelShader("TreeWireframePS");
	AssignVertexShaderLayout("TreeShaderLeaf");
	
	// Still use the same vertex shader but create a new layout
	SetInputLayoutDesc("TreeShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex_tan_instance, 5);
	AssignVertexShaderLayout("TreeShader");
	
	return true;
}
//==============================================================================================================================
void TreeShader::InitializeLeafBuffer(InternalTreeRenderParameters trp)
{
	ConstantBuffer<cbLeafBuffer> leafCB(m_pD3DSystem);
	leafCB.Initialize(PAD16(sizeof(cbLeafBuffer)));
	m_pLeafBufferCB = leafCB.Buffer();
	
	cbLeafBuffer cLB;
	
	for (int i = 0; i < NUM_LEAVES_PER_TREE; i++)
	{
		cLB.g_LeafOnTree[i] = XMMatrixTranspose(trp.g_LeafOnTree[i]);
	}
	
	ID3D11DeviceContext* context = m_pD3DSystem->GetDeviceContext();
	
	D3D11_MAPPED_SUBRESOURCE mapped_res;
	context->Map(m_pLeafBufferCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
	{
		assert(mapped_res.pData);
		*(cbLeafBuffer*)mapped_res.pData = cLB;
	}
	context->Unmap(m_pLeafBufferCB, 0);
}
//==============================================================================================================================
void TreeShader::Shutdown()
{
	//ShutdownShader();
}
//==============================================================================================================================
void TreeShader::RenderLeaves
(	int indexCount
,	int instanceCount
,	ZShadeSandboxMesh::MeshRenderParameters mrp
,	ID3D11ShaderResourceView* texture
)
{
	cbTreeBuffer cTB;
	cTB.g_EyePos = mrp.camera->Position();
	cTB.g_TreeBillWidth = 0;
	cTB.g_TreeBillHeight = 0;
	cTB.padding1 = 0;
	cTB.padding2 = 0;
	cTB.g_DiffuseColor = XMFLOAT4(0, 0, 0, 0);
	cTB.g_HasTexture = 1;
	cTB.g_HasNormalMap = 0;
	cTB.g_IsInstance = (mrp.useInstancing) ? 1 : 0;
	cTB.g_IsLeaf = 1;
	// Map the tree parameter constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pTreeBufferCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbTreeBuffer*)mapped_res.pData = cTB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pTreeBufferCB, 0);
	}
	
	cbMatrixBuffer cMB;
	g_matWorld = mrp.world;
	g_matView = camera->View4x4();
	g_matProj = camera->Proj4x4();
	// Map the matrix parameter constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixBufferCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbMatrixBuffer*)mapped_res.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixBufferCB, 0);
	}
	
	ID3D11Buffer* constBuffers[3] = {m_pTreeBufferCB, m_pMatrixBufferCB, m_pLeafBufferCB};
	
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 3, constBuffers);
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 3, constBuffers);
	
	ID3D11ShaderResourceView* ps_srvs[1] = { texture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Linear() };
	
	m_pD3DSystem->TurnOffCulling();
	
	if (!m_Wireframe)
	{
		// Assign Texture

		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);

		//if (mUseGBuffer)
		//{
		//	SwitchTo("TreeGBufferPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
		//}
		//else
		{
			SwitchTo("TreePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
		}
	}
	else
	{
		//if (mUseGBuffer)
		//{
		//	SwitchTo("TreeGBufferWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
		//}
		//else
		{
			SwitchTo("TreeWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
		}
	}

	//if (mUseGBuffer)
	//{
	//	SwitchTo("TreeGBufferVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
	//}
	//else
	{
		SwitchTo("TreeVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
	}

	SetInputLayout("TreeShaderLeaf");

	SetVertexShader();
	SetPixelShader();

	//Perform Drawing
	if (cTB.g_IsInstance == 1)
	{
		RenderIndex11(indexCount, instanceCount);
	}
	else
	{
		RenderIndex11(indexCount);
	}
	
	// Unbind
	if (!m_Wireframe)
	{
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
	}
	
	// Reset the default Input Layout
	SetInputLayout("TreeShader");
	
	m_pD3DSystem->TurnOnCulling();
}
//==============================================================================================================================
/*void TreeShader::RenderFunc(int indexCount, ZShadeSandboxMesh::MeshRenderParameters mrp, ZShadeSandboxLighting::ShaderMaterial* material)
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

	SetInputLayout("TreeShader");

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
}*/
//==============================================================================================================================