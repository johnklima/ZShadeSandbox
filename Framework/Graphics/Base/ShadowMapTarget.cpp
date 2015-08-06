#include "ShadowMapTarget.h"
#include "CGlobal.h"
//==============================================================================================================================
ShadowMapTarget::ShadowMapTarget(D3D* d3d)
:   m_pD3DSystem(d3d)
,   SRView(0)
,   RTView(0)
,   DSView(0)
,   RTVTexture(0)
//,   SRVTexture(0)
,   DSVTexture(0)
{
}
//==============================================================================================================================
ShadowMapTarget::ShadowMapTarget(const ShadowMapTarget& other)
{
}
//==============================================================================================================================
ShadowMapTarget::~ShadowMapTarget()
{
	Shutdown();
}
//==============================================================================================================================
bool ShadowMapTarget::Initialize(int textureWidth, int textureHeight)
{
	Width = textureWidth;
	Height = textureHeight;
	
	// Create the RTV Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = Width;
		desc.Height = Height;
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.Format = DXGI_FORMAT_R16_TYPELESS;
		desc.MipLevels = 1;
		desc.MiscFlags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		m_pD3DSystem->GetDevice11()->CreateTexture2D(&desc, NULL, &RTVTexture);
	}
	
	// Create the SRV Texture
	//{
	//	D3D11_TEXTURE2D_DESC desc;
	//	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	//	desc.Width = Width;
	//	desc.Height = Height;
	//	desc.ArraySize = 1;
	//	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//	desc.CPUAccessFlags = 0;
	//	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	//	desc.MipLevels = 1;
	//	desc.MiscFlags = 0;
	//	desc.SampleDesc.Count = 1;
	//	desc.SampleDesc.Quality = 0;
	//	desc.Usage = D3D11_USAGE_DEFAULT;
	//	m_pD3DSystem->GetDevice11()->CreateTexture2D(&desc, NULL, &SRVTexture);
	//}
	
	// Create the DSV Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = Width;
		desc.Height = Height;
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.Format = DXGI_FORMAT_R16_TYPELESS;//DXGI_FORMAT_D24_UNORM_S8_UINT
		desc.MipLevels = 1;
		desc.MiscFlags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		m_pD3DSystem->GetDevice11()->CreateTexture2D(&desc, NULL, &DSVTexture);
	}
	
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	RTVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	m_pD3DSystem->GetDevice11()->CreateRenderTargetView(RTVTexture, NULL, &RTView);
	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	SRVDesc.Format = DXGI_FORMAT_R16_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	m_pD3DSystem->GetDevice11()->CreateShaderResourceView(RTVTexture, &SRVDesc, &SRView);
	
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	ZeroMemory(&DSVDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	DSVDesc.Format = DXGI_FORMAT_D16_UNORM;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	m_pD3DSystem->GetDevice11()->CreateDepthStencilView(DSVTexture, &DSVDesc, &DSView);
	
	// Setup the viewport for rendering.
	m_viewport.Width = (float)Width;
	m_viewport.Height = (float)Height;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	return true;
}
//==============================================================================================================================
void ShadowMapTarget::Shutdown()
{
	/*SAFE_RELEASE(m_pRTT);
	SAFE_RELEASE(m_pRTV);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pUAV);
	SAFE_RELEASE(m_depthStencilView);
	SAFE_RELEASE(m_depthStencilBuffer);*/
}
//==============================================================================================================================
void ShadowMapTarget::SetRenderTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_pD3DSystem->GetDeviceContext()->OMSetRenderTargets(1, &RTView, DSView);
	
	// Clear the back buffer.
	float color[4];
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
	color[3] = 0.0f;
	m_pD3DSystem->GetDeviceContext()->ClearRenderTargetView(RTView, color);
	
	// Set the viewport.
	m_pD3DSystem->GetDeviceContext()->RSSetViewports(1, &m_viewport);
	
	// Clear the depth buffer.
	m_pD3DSystem->GetDeviceContext()->ClearDepthStencilView(DSView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	m_pD3DSystem->SetRasterizerState(m_pD3DSystem->RSDepth());
}
//==============================================================================================================================
void ShadowMapTarget::ResetView()
{
	//Reset the render target to the original back buffer and not the render to texture anymore
	m_pD3DSystem->SetBackBufferRenderTarget();

	//Reset the viewport back to the original
	m_pD3DSystem->ResetViewport();
}
//==============================================================================================================================