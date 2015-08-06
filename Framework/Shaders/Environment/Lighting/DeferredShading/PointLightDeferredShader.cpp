#include "PointLightDeferredShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
PointLightDeferredShader::PointLightDeferredShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
PointLightDeferredShader::PointLightDeferredShader(const PointLightDeferredShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
PointLightDeferredShader::~PointLightDeferredShader()
{
}
//==============================================================================================================================
bool PointLightDeferredShader::Initialize()
{
	ConstantBuffer<cbLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(cbLightBuffer)));
	m_pLightCB = lightCB.Buffer();
	
	LoadVertexShader("PointLightDeferredVS");
	LoadPixelShader("PointLightDeferredPS");
	LoadPixelShader("PointLightDeferredWireframePS");
	
	return true;
}
//==============================================================================================================================
void PointLightDeferredShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool PointLightDeferredShader::Render11
(	Camera* camera
,	ZShadeSandboxLighting::PointLight* light
,	ID3D11ShaderResourceView* colorTexture
,	ID3D11ShaderResourceView* normalTexture
,	ID3D11ShaderResourceView* depthTexture
)
{
	cbLightBuffer cLB;
	cLB.g_PointLightColor = light->DiffuseColor();
	cLB.g_EyePosition = camera->Position();
	cLB.g_PointLightIntensity = 0.8f;
	cLB.g_PointLightPosition = light->Position();
	cLB.g_PointLightRange = light->Range();
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

		SwitchTo("PointLightDeferredPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("PointLightDeferredWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
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

/*bool PointLightDeferredShader::Render11
(	Camera* camera
,	ZShadeSandboxLighting::PointLight* light
,	ID3D11ShaderResourceView* colorTexture
,	ID3D11ShaderResourceView* normalTexture
,	ID3D11ShaderResourceView* depthTexture
)
{
	XMFLOAT3 lightPos = light->Position();
	float range = light->Range();
	XMMATRIX cameraView = camera->View();
	XMMATRIX cameraProj = camera->Proj();

	float eps = 0.001f;
	XMMATRIX scale = XMMatrixScaling(range + eps, range + eps, range + eps);
	XMMATRIX translation = XMMatrixTranslation(lightPos.x, lightPos.y, lightPos.z);

	XMVECTOR scal; //for scaling
	XMVECTOR quat; //for rotation
	XMVECTOR tran; //for translation
	XMMatrixDecompose(&scal, &quat, &tran, cameraView);

	XMFLOAT4 transV;
	XMStoreFloat4(&transV, tran);
	XMFLOAT4 rotzV;
	XMStoreFloat4(&rotzV, quat);
	XMFLOAT4 scaleV;
	XMStoreFloat4(&scaleV, scal);

	XMMATRIX viewTranslation = XMMatrixTranslation(transV.x, transV.y, transV.z);

	XMMATRIX lightProj = scale * translation * viewTranslation * cameraProj;

	cbDomainBuffer cDB;
	//cDB.g_LightProjection = light->Perspective()->LightProj4x4();
	//cDB.g_LightProjection = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(lightProj));
	cDB.g_LightProjection = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(lightProj));
	// Map the domain buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pDomainCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbDomainBuffer*)mapped_res2.pData = cDB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pDomainCB, 0);
	}
	
	cbPixelBuffer cPB;
	cPB.g_PointLightPosition = light->Position();
	cPB.g_PointLightRange = light->Range();
	cPB.g_PointLightColor = XMFLOAT3(light->DiffuseColor().x, light->DiffuseColor().y, light->DiffuseColor().z);
	cPB.padding1 = 0;
	cPB.g_PointLightIntensity = light->Attenuation();
	cPB.padding2 = 0;
	cPB.g_EyePosition = camera->Position();
	cPB.padding3 = 0;
	// XY components should be filled with one over the projection matrix diagonal values.
	// 		g_PerspectiveValues.x = 1 / Pm[0][0]
	// Z component is the projection matrix Pm[3][2] value.
	// W component is the negative projection matrix Pm[2][2] value.
	ZShadeSandboxMath::XMMatrix proj(camera->Proj());
	cPB.g_PerspectiveValues.x = 1 / proj._11;
	cPB.g_PerspectiveValues.y = 1 / proj._11;
	cPB.g_PerspectiveValues.z = proj._32;
	cPB.g_PerspectiveValues.w = proj._22;
	cPB.g_ViewInv = camera->InvView4x4();
	// Map the pixel buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pPixelCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbPixelBuffer*)mapped_res2.pData = cPB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pPixelCB, 0);
	}
	
	ID3D11Buffer* ds_cbs[1] = { m_pDomainCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(0, 1, ds_cbs);
	
	ID3D11Buffer* ps_cbs[1] = { m_pPixelCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(1, 1, ps_cbs);
	
	// Assign Texture
	ID3D11ShaderResourceView* ps_srvs[3] = { colorTexture, normalTexture, depthTexture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Point() };
	
	if (!m_Wireframe)
	{
		// Set new rasterizer and stencil states
		
		// Helps to avoid culling some or all of the point light volume faces when the camera is inside the volume
		//m_pD3DSystem->SetDepthStencilState(m_pD3DSystem->ReverseDepthEnabled());
		//m_pD3DSystem->SetRasterizerState(m_pD3DSystem->BackFaceCull());
		
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 3, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		
		SwitchTo("PointLightDeferredPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("PointLightDeferredWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	// Use additive blending to accumulate the light values from all other lights

	m_pD3DSystem->TurnOnAdditiveBlending();
	
	SetVertexShader();
	SetHullShader();
	SetDomainShader();
	SetPixelShader();
	
	// Enable the tessellation feature
	m_pD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);
	
	RenderDraw11(1);
	
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
}*/
//==============================================================================================================================