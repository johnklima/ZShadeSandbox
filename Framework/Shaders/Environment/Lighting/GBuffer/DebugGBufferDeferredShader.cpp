#include "DebugGBufferDeferredShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
DebugGBufferDeferredShader::DebugGBufferDeferredShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
DebugGBufferDeferredShader::DebugGBufferDeferredShader(const DebugGBufferDeferredShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
DebugGBufferDeferredShader::~DebugGBufferDeferredShader()
{
}
//==============================================================================================================================
bool DebugGBufferDeferredShader::Initialize()
{
	ConstantBuffer<cbInvMatrixBuffer> matrixCB(m_pD3DSystem);
	matrixCB.Initialize(PAD16(sizeof(cbInvMatrixBuffer)));
	m_pMatrixCB = matrixCB.Buffer();

	LoadVertexShader("DebugGBufferDeferredVS");
	LoadPixelShader("DebugGBufferDeferredPS");
	LoadPixelShader("DebugGBufferDepthDeferredPS");
	LoadPixelShader("DebugGBufferDeferredWireframePS");
	
	return true;
}
//==============================================================================================================================
void DebugGBufferDeferredShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool DebugGBufferDeferredShader::Render11(ID3D11ShaderResourceView* texture)
{
	// Assign Texture
	ID3D11ShaderResourceView* ps_srvs[1] = { texture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Point() };
	
	if (!m_Wireframe)
	{
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		
		SwitchTo("DebugGBufferDeferredPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("DebugGBufferDeferredWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	m_pD3DSystem->TurnOnAdditiveBlending();
	
	SetVertexShader();
	SetPixelShader();
	
	//Perform Drawing onto a fullscreen quad with a NULL Input Layout
	RenderDraw11(4);
	
	m_pD3DSystem->TurnOffAdditiveBlending();
	
	// Unbind
	if (!m_Wireframe)
	{
		ps_samp[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		ps_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
	}
	
	return true;
}
//==============================================================================================================================
bool DebugGBufferDeferredShader::RenderDepth11(ID3D11ShaderResourceView* texture, Camera* camera)
{
	cbInvMatrixBuffer cMB;
	cMB.g_InvViewProj = camera->InvViewProj4x4();
	// Map the matrix buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbInvMatrixBuffer*)mapped_res2.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixCB, 0);
	}

	ID3D11Buffer* ps_cbs[1] = { m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);

	// Assign Texture
	ID3D11ShaderResourceView* ps_srvs[1] = { texture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Point() };

	if (!m_Wireframe)
	{
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);

		SwitchTo("DebugGBufferDepthDeferredPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("DebugGBufferDeferredWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}

	m_pD3DSystem->TurnOnAdditiveBlending();

	SetVertexShader();
	SetPixelShader();

	//Perform Drawing onto a fullscreen quad with a NULL Input Layout
	RenderDraw11(4);

	m_pD3DSystem->TurnOffAdditiveBlending();

	// Unbind
	if (!m_Wireframe)
	{
		ps_samp[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		ps_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
	}

	return true;
}
//==============================================================================================================================