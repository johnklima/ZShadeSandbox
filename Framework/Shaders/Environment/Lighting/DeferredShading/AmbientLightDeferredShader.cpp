#include "AmbientLightDeferredShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
AmbientLightDeferredShader::AmbientLightDeferredShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
AmbientLightDeferredShader::AmbientLightDeferredShader(const AmbientLightDeferredShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
AmbientLightDeferredShader::~AmbientLightDeferredShader()
{
}
//==============================================================================================================================
bool AmbientLightDeferredShader::Initialize()
{
	ConstantBuffer<cbDeferredLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(cbDeferredLightBuffer)));
	m_pLightCB = lightCB.Buffer();
	
	LoadVertexShader("AmbientLightDeferredVS");
	LoadPixelShader("AmbientLightDeferredPS");
	LoadPixelShader("AmbientLightDeferredWireframePS");
	
	return true;
}
//==============================================================================================================================
void AmbientLightDeferredShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool AmbientLightDeferredShader::Render11
(	Camera* camera
,	ZShadeSandboxLighting::AmbientLight* light
,	XMFLOAT3 ambientUp
,	XMFLOAT3 ambientDown
,	ID3D11ShaderResourceView* colorTexture
,	ID3D11ShaderResourceView* normalTexture
,	ID3D11ShaderResourceView* depthTexture
)
{
	cbDeferredLightBuffer cLB;
	cLB.g_AmbientLightColor = light->DiffuseColor();
	cLB.g_AmbientDown = ambientDown;
	cLB.padding1 = 0;
	cLB.g_AmbientUp = ambientUp;
	cLB.padding2 = 0;
	cLB.g_InvViewProj = camera->InvViewProj4x4();
	// Map the light buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pLightCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbDeferredLightBuffer*)mapped_res2.pData = cLB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pLightCB, 0);
	}
	
	ID3D11Buffer* ps_cbs[1] = { m_pLightCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
	
	// Assign Texture
	ID3D11ShaderResourceView* ps_srvs[3] = { colorTexture, normalTexture, depthTexture };
	ID3D11SamplerState* ps_samp[2] = { m_pD3DSystem->Point(), m_pD3DSystem->Linear() };
	
	if (!m_Wireframe)
	{
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 3, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 2, ps_samp);
		
		SwitchTo("AmbientLightDeferredPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("AmbientLightDeferredWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	m_pD3DSystem->TurnOnAdditiveBlending();
	//m_pD3DSystem->TurnOnAlphaBlending();
	
	SetVertexShader();
	SetPixelShader();
	
	//Perform Drawing onto a fullscreen quad with a NULL Input Layout
	RenderDraw11(4);
	
	//m_pD3DSystem->TurnOffAlphaBlending();
	m_pD3DSystem->TurnOffAdditiveBlending();
	
	// Unbind
	if (!m_Wireframe)
	{
		ps_samp[0] = NULL;
		ps_samp[1] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 2, ps_samp);
		ps_srvs[0] = NULL;
		ps_srvs[1] = NULL;
		ps_srvs[2] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 3, ps_srvs);
	}
	
	return true;
}
//==============================================================================================================================