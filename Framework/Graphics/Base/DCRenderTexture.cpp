//#include "stdafx.h"
#include "DCRenderTexture.h"
#include "CGlobal.h"
//===============================================================================================================================
//===============================================================================================================================
DCRenderTexture::DCRenderTexture(D3D* d3d, int textureWidth, int textureHeight, DXGI_FORMAT format)
:	m_pD3DSystem(d3d)
{
	//
	// Create 2D texture
	//

	D3D11_TEXTURE2D_DESC tex_desc;
	tex_desc.Width = textureWidth;
	tex_desc.Height = textureHeight;
	tex_desc.MipLevels = 0;
	tex_desc.ArraySize = 1;
	tex_desc.Format = format;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	m_pD3DSystem->GetDevice11()->CreateTexture2D(&tex_desc, NULL, &m_renderTargetTexture);
	assert(m_renderTargetTexture);

	// Create shader resource view
	m_renderTargetTexture->GetDesc(&tex_desc);
	//if (m_shaderResourceView)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		srv_desc.Format = format;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = tex_desc.MipLevels;
		srv_desc.Texture2D.MostDetailedMip = 0;

		m_pD3DSystem->GetDevice11()->CreateShaderResourceView(m_renderTargetTexture, &srv_desc, &m_shaderResourceView);
		assert(m_shaderResourceView);
	}

	// Create render target view
	//if (m_renderTargetView)
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
		rtv_desc.Format = format;
		rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtv_desc.Texture2D.MipSlice = 0;

		m_pD3DSystem->GetDevice11()->CreateRenderTargetView(m_renderTargetTexture, &rtv_desc, &m_renderTargetView);
		assert(m_renderTargetView);
	}

	//
	// Create the viewport from the width and height
	//

	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = (float)textureWidth;
	m_viewport.Height = (float)textureHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
}
//===============================================================================================================================
DCRenderTexture::DCRenderTexture(const DCRenderTexture&)
{

}
//===============================================================================================================================
DCRenderTexture::~DCRenderTexture()
{

}
//===============================================================================================================================
void DCRenderTexture::Shutdown()
{
	SAFE_RELEASE(m_shaderResourceView);
	SAFE_RELEASE(m_renderTargetView);
	SAFE_RELEASE(m_renderTargetTexture);
}
//===============================================================================================================================
void DCRenderTexture::SetRenderTarget()
{
	m_pD3DSystem->GetDeviceContext()->OMSetRenderTargets(1, &m_renderTargetView, NULL);

	// Set the viewport.
	m_pD3DSystem->GetDeviceContext()->RSSetViewports(1, &m_viewport);
}
//===============================================================================================================================
void DCRenderTexture::ResetView()
{
	//Reset the render target to the original back buffer and not the render to texture anymore
	m_pD3DSystem->SetBackBufferRenderTarget();

	//Reset the viewport back to the original
	m_pD3DSystem->ResetViewport();
}
//===============================================================================================================================
ID3D11ShaderResourceView* DCRenderTexture::GetSRV()
{
	return m_shaderResourceView;
}
//===============================================================================================================================