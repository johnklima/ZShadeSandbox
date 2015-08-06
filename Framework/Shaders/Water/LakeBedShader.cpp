#include "LakeBedShader.h"
#include "ConstantBuffer.h"
using ZShadeSandboxShader::LakeBedShader;
//==============================================================================================================================
LakeBedShader::LakeBedShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
LakeBedShader::LakeBedShader(const LakeBedShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
LakeBedShader::~LakeBedShader()
{
}
//==============================================================================================================================
bool LakeBedShader::Initialize()
{
	ConstantBuffer<cbFrameBuffer> frameBufferCB(m_pD3DSystem);
	frameBufferCB.Initialize(PAD16(sizeof(cbFrameBuffer)));
	m_pFrameBufferCB = frameBufferCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixBufferCB(m_pD3DSystem);
	matrixBufferCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixBufferCB = matrixBufferCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("LakeBedShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	LoadVertexShader("LakeBedVS");
	LoadPixelShader("LakeBedPS");
	LoadPixelShader("LakeBedWireframePS");
	LoadVertexShader("LakeBedGBufferVS");
	LoadPixelShader("LakeBedGBufferPS");
	LoadPixelShader("LakeBedWireframeGBufferPS");
	AssignVertexShaderLayout("LakeBedShader");
	
	return true;
}
//==============================================================================================================================
void LakeBedShader::Shutdown()
{
	//ShutdownShader();
}
//==============================================================================================================================
bool LakeBedShader::Render(int indexCount, float causticTimer, Camera* camera, LightCamera* lightCamera, XMMATRIX world, XMFLOAT4 clipplane, bool renderDeferred, bool reflect)
{
	// Light must exist otherwise there is no point to the shader
	if (lightCamera == NULL) return false;
	
	cbFrameBuffer cFB;
	cbMatrixBuffer cMB;
	
	cMB.g_matWorld = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(world));
	
	if (reflect)
		cMB.g_matView = camera->ReflectionView4x4();
	else
		cMB.g_matView = camera->View4x4();
	
	cMB.g_matShadow = lightCamera->ShadowTransform4x4();
	cMB.g_matLightViewProj = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(lightCamera->LightView() * lightCamera->LightProj()));
	cMB.g_matProj = camera->Proj4x4();
	
	cFB.g_ClipPlane = clipplane;
	cFB.g_EyePos = camera->Position();
	cFB.g_CausticTimer = causticTimer;
	cFB.g_LightDirection = lightCamera->Position();
	cFB.g_dummy2 = 0;
	
	// Map the frame parameter constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pFrameBufferCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbFrameBuffer*)mapped_res.pData = cFB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pFrameBufferCB, 0);
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
	
	ID3D11Buffer* vs_cbs[2] = { m_pFrameBufferCB, m_pMatrixBufferCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 2, vs_cbs);
	
	ID3D11Buffer* ps_cbs[2] = { m_pFrameBufferCB, m_pMatrixBufferCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 2, ps_cbs);
	
	ID3D11ShaderResourceView* ps_srvs[7] = { m_DiffuseMap, m_WaveMap0, m_WaveMap1, g_MicroBump0, g_MicroBump1, g_DepthMap, g_CausticTexture };
	ID3D11SamplerState* ps_samp[2] = { m_pD3DSystem->Linear(), m_pD3DSystem->ShadowMap() };
	
	if (!m_Wireframe)
	{
		// Assign Texture
		
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 2, ps_samp);
		
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 7, ps_srvs);
		
		if (renderDeferred)
		{
			SwitchTo("LakeBedGBufferPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
		}
		else
		{
			SwitchTo("LakeBedPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
		}
	}
	else
	{
		if (renderDeferred)
		{
			SwitchTo("LakeBedGBufferWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
		}
		else
		{
			SwitchTo("LakeBedWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
		}
	}
	
	if (renderDeferred)
	{
		SwitchTo("LakeBedGBufferVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
	}
	else
	{
		SwitchTo("LakeBedVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
	}

	SetInputLayout("LakeBedShader");

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
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 7, ps_srvs);
	}
}
//==============================================================================================================================