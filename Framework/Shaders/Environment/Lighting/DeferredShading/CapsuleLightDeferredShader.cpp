#include "CapsuleLightDeferredShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
CapsuleLightDeferredShader::CapsuleLightDeferredShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
CapsuleLightDeferredShader::CapsuleLightDeferredShader(const CapsuleLightDeferredShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
CapsuleLightDeferredShader::~CapsuleLightDeferredShader()
{
}
//==============================================================================================================================
bool CapsuleLightDeferredShader::Initialize()
{
	ConstantBuffer<cbLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(cbLightBuffer)));
	m_pLightCB = lightCB.Buffer();
	
	LoadVertexShader("CapsuleLightDeferredVS");
	LoadPixelShader("CapsuleLightDeferredPS");
	LoadPixelShader("CapsuleLightDeferredWireframePS");
	
	return true;
}
//==============================================================================================================================
void CapsuleLightDeferredShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool CapsuleLightDeferredShader::Render11
(	Camera* camera
,	ZShadeSandboxLighting::CapsuleLight* light
,	ID3D11ShaderResourceView* colorTexture
,	ID3D11ShaderResourceView* normalTexture
,	ID3D11ShaderResourceView* depthTexture
)
{
	cbLightBuffer cLB;
	cLB.g_CapsuleLightPosition = light->Position();
	cLB.g_CapsuleLightRange = light->Range();
	cLB.g_CapsuleLightDirection = light->Direction();
	cLB.g_CapsuleLightLength = light->LightLength();
	cLB.g_CapsuleDirectionLength = light->CapsuleDirectionLength();
	cLB.g_CapsuleIntensity = light->Intensity();
	cLB.g_CapsuleDiffuseColor = light->DiffuseColor();
	cLB.g_EyePosition = camera->Position();
	cLB.padding2 = 0;
	cLB.g_ViewInvProj = camera->InvViewProj4x4();
	// Map the pixel buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pLightCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbLightBuffer*)mapped_res2.pData = cLB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pLightCB, 0);
	}
	
	ID3D11Buffer* ps_cbs[1] = { m_pLightCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
	
	// Assign Texture
	ID3D11ShaderResourceView* ps_srvs[3] = { colorTexture, normalTexture, depthTexture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Point() };
	
	if (!m_Wireframe)
	{
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 3, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		
		SwitchTo("CapsuleLightDeferredPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("CapsuleLightDeferredWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	// Use additive blending to accumulate the light values from all other lights

	m_pD3DSystem->TurnOnAdditiveBlending();
	
	SetVertexShader();
	SetPixelShader();
	
	RenderDraw11(4);
	
	m_pD3DSystem->TurnOffAdditiveBlending();
	
	// Unbind
	if (!m_Wireframe)
	{
		ps_samp[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		ps_srvs[0] = NULL;
		ps_srvs[1] = NULL;
		ps_srvs[2] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 3, ps_srvs);
	}
	
	return true;
}
//==============================================================================================================================