#include "SpotLightDeferredShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
SpotLightDeferredShader::SpotLightDeferredShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
SpotLightDeferredShader::SpotLightDeferredShader(const SpotLightDeferredShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
SpotLightDeferredShader::~SpotLightDeferredShader()
{
}
//==============================================================================================================================
bool SpotLightDeferredShader::Initialize()
{
	ConstantBuffer<cbLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(cbLightBuffer)));
	m_pLightCB = lightCB.Buffer();
	
	LoadVertexShader("SpotLightDeferredVS");
	LoadPixelShader("SpotLightDeferredPS");
	LoadPixelShader("SpotLightDeferredWireframePS");
	
	return true;
}
//==============================================================================================================================
void SpotLightDeferredShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool SpotLightDeferredShader::Render11
(	Camera* camera
,	ZShadeSandboxLighting::SpotLight* light
,	ID3D11ShaderResourceView* colorTexture
,	ID3D11ShaderResourceView* normalTexture
,	ID3D11ShaderResourceView* depthTexture
)
{
	cbLightBuffer cLB;
	cLB.g_SpotLightColor = light->DiffuseColor();
	cLB.g_SpotLightPosition = light->Position();
	cLB.padding = 0;
	cLB.g_SpotLightRange = light->Range();
	cLB.g_SpotCosOuterCone = light->SpotCosOuterCone();
	cLB.g_SpotInnerConeReciprocal = light->SpotInnerConeReciprocal();
	cLB.g_SpotCosineAngle = light->SpotAngle();
	cLB.g_EyePosition = camera->Position();
	cLB.g_SpotLightIntensity = 0.8f;// light->Intensity();
	cLB.g_InvViewProj = camera->InvViewProj4x4();
	// Map the light buffer
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

		SwitchTo("SpotLightDeferredPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("SpotLightDeferredWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
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

	m_pD3DSystem->TurnOnCulling();

	return true;
}
//==============================================================================================================================