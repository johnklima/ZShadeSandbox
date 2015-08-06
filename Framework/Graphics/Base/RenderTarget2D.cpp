#include "RenderTarget2D.h"
#include "CGlobal.h"
//==============================================================================================================================
RenderTarget2D::RenderTarget2D(D3D* d3d)
:	m_pD3DSystem(d3d)
{
	id = 0;
	InUse = false;
	//m_depthStencilBuffer = 0;
	DSView = 0;
}
//==============================================================================================================================
RenderTarget2D::RenderTarget2D(const RenderTarget2D& other)
{
}
//==============================================================================================================================
RenderTarget2D::~RenderTarget2D()
{
	Shutdown();
}
//==============================================================================================================================
ID3D11Texture2D* RenderTarget2D::CreateTexture2D
(	int width
,	int height
,	DXGI_FORMAT format
,	bool rtv
,	bool srv
,	bool dsv
,	bool uav
,	UINT multiSamples
,	UINT msQuality
,	UINT numMipLevels
,	bool autoGenMipMaps
)
{
	ID3D11Texture2D* tex;
	
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = 1;
	if (rtv) desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	if (srv) desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	if (dsv) desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	if (uav) desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	desc.CPUAccessFlags = 0;
	desc.Format = format;
	desc.MipLevels = numMipLevels;
	desc.MiscFlags = autoGenMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
	desc.SampleDesc.Count = multiSamples;
	desc.SampleDesc.Quality = msQuality;
	desc.Usage = D3D11_USAGE_DEFAULT;
	m_pD3DSystem->GetDevice11()->CreateTexture2D(&desc, NULL, &tex);
	
	return tex;
}
//==============================================================================================================================
bool RenderTarget2D::Initialize
(	int textureWidth
,	int textureHeight
//,	float screenDepth
//,	float screenNear
,	DXGI_FORMAT format
,	UINT multiSamples
,	UINT msQuality
,	UINT numMipLevels
,	bool autoGenMipMaps
,	bool createUAV
,	bool srvAsCube
)
{
	/*D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	
	m_depth = screenDepth;
	m_near = screenNear;
	this->createUAV = createUAV;

	Width = textureWidth;
	Height = textureHeight;

	Format = format;
    MSCount = multiSamples;
    MSQuality = msQuality;

	// Initialize the render target texture description.
	D3D11_TEXTURE2D_DESC desc;
    desc.Width = Width;
    desc.Height = Height;
    desc.ArraySize = 1;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET;
    if(createUAV)
        desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
    desc.CPUAccessFlags = 0;
    desc.Format = format;
    desc.MipLevels = numMipLevels;
    desc.MiscFlags = autoGenMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
    desc.SampleDesc.Count = multiSamples;
    desc.SampleDesc.Quality = msQuality;
    desc.Usage = D3D11_USAGE_DEFAULT;
    m_pD3DSystem->GetDevice11()->CreateTexture2D(&desc, NULL, &Texture);
    m_pD3DSystem->GetDevice11()->CreateRenderTargetView(Texture, NULL, &RTView);
    m_pD3DSystem->GetDevice11()->CreateShaderResourceView(Texture, NULL, &SRView);
    
    if(createUAV)
        m_pD3DSystem->GetDevice11()->CreateUnorderedAccessView(Texture, NULL, &UAView);
    else
        UAView = NULL;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	// Set up the description of the depth buffer.
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	// Create the texture for the depth buffer using the filled out description.
	result = m_pD3DSystem->GetDevice11()->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	// Create the depth stencil view.
	result = m_pD3DSystem->GetDevice11()->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &DSView);
	if(FAILED(result))
	{
		return false;
	}
	*/
	
	//m_depth = screenDepth;
	//m_near = screenNear;
	this->createUAV = createUAV;
	Width = textureWidth;
	Height = textureHeight;
	Format = format;
    MSCount = multiSamples;
    MSQuality = msQuality;
	
	//SRVTexture = CreateTexture2D(width, height, format, false, true, false, false, multiSamples, msQuality, numMipLevels, autoGenMipMaps);
	RTVTexture = CreateTexture2D(Width, Height, format, true, true, false, false, multiSamples, msQuality, numMipLevels, autoGenMipMaps);
	DSVTexture = CreateTexture2D(Width, Height, DXGI_FORMAT_D24_UNORM_S8_UINT, false, false, true, false, multiSamples, msQuality, numMipLevels, autoGenMipMaps);
	
	if (createUAV)
	{
		//UAVTexture = CreateTexture2D(width, height, format, false, false, false, true, multiSamples, msQuality, numMipLevels, autoGenMipMaps);
		RTVTexture = CreateTexture2D(Width, Height, format, true, true, false, true, multiSamples, msQuality, numMipLevels, autoGenMipMaps);
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
		ZeroMemory(&UAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
		UAVDesc.Format = format;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		m_pD3DSystem->GetDevice11()->CreateUnorderedAccessView(RTVTexture, &UAVDesc, &UAView);
	}
	else
        UAView = NULL;
	
	if(format == DXGI_FORMAT_R24G8_TYPELESS)
	{
		format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	}
	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	SRVDesc.Format = format;
	if (srvAsCube)
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	else
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	m_pD3DSystem->GetDevice11()->CreateShaderResourceView(RTVTexture, &SRVDesc, &SRView);
	
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	RTVDesc.Format = format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	m_pD3DSystem->GetDevice11()->CreateRenderTargetView(RTVTexture, &RTVDesc, &RTView);
    
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	ZeroMemory(&DSVDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	m_pD3DSystem->GetDevice11()->CreateDepthStencilView(DSVTexture, &DSVDesc, &DSView);
	
	// Setup the viewport for rendering.
    viewport.Width = (float)textureWidth;
    viewport.Height = (float)textureHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
	
	return true;
}
//==============================================================================================================================
bool RenderTarget2D::Initialize
(	ID3D11Texture2D* texture
,	int textureWidth
,	int textureHeight
,	UINT multiSamples
,	UINT msQuality
,	UINT numMipLevels
,	bool autoGenMipMaps
,	bool createUAV
)
{
	/*D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	
	this->createUAV = createUAV;

	Width = textureWidth;
	Height = textureHeight;

    MSCount = multiSamples;
    MSQuality = msQuality;
	
	Texture = texture;

    m_pD3DSystem->GetDevice11()->CreateRenderTargetView(Texture, NULL, &RTView);
    m_pD3DSystem->GetDevice11()->CreateShaderResourceView(Texture, NULL, &SRView);
    
    if(createUAV)
        m_pD3DSystem->GetDevice11()->CreateUnorderedAccessView(Texture, NULL, &UAView);
    else
        UAView = NULL;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	// Set up the description of the depth buffer.
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	// Create the texture for the depth buffer using the filled out description.
	result = m_pD3DSystem->GetDevice11()->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	// Create the depth stencil view.
	result = m_pD3DSystem->GetDevice11()->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &DSView);
	if(FAILED(result))
	{
		return false;
	}*/
	
	this->createUAV = createUAV;
	Width = textureWidth;
	Height = textureHeight;
    MSCount = multiSamples;
    MSQuality = msQuality;
	
	//SRVTexture = texture;
	RTVTexture = texture;
	DSVTexture = CreateTexture2D(Width, Height, DXGI_FORMAT_D24_UNORM_S8_UINT, false, false, true, false, multiSamples, msQuality, numMipLevels, autoGenMipMaps);
	
	if (createUAV)
	{
		//UAVTexture = texture;
		m_pD3DSystem->GetDevice11()->CreateUnorderedAccessView(RTVTexture, NULL, &UAView);
	}
	else
        UAView = NULL;
	
	m_pD3DSystem->GetDevice11()->CreateShaderResourceView(RTVTexture, NULL, &SRView);
	m_pD3DSystem->GetDevice11()->CreateRenderTargetView(RTVTexture, NULL, &RTView);
    
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	ZeroMemory(&DSVDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	m_pD3DSystem->GetDevice11()->CreateDepthStencilView(DSVTexture, &DSVDesc, &DSView);
	
	// Setup the viewport for rendering.
    viewport.Width = (float)textureWidth;
    viewport.Height = (float)textureHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
	
	return true;
}
//==============================================================================================================================
bool RenderTarget2D::Compare
(	int textureWidth
,	int textureHeight
,	DXGI_FORMAT format
,	UINT multiSamples
,	UINT msQuality
)
{
	return (Width == textureWidth &&
			Height == textureHeight &&
			MSCount == multiSamples &&
			MSQuality == msQuality &&
			Format == format);
}
//==============================================================================================================================
void RenderTarget2D::Shutdown()
{
	/*SAFE_RELEASE(m_pRTT);
	SAFE_RELEASE(m_pRTV);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pUAV);
	SAFE_RELEASE(DSView);
	SAFE_RELEASE(m_depthStencilBuffer);*/
}
//==============================================================================================================================
void RenderTarget2D::SetRenderTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_pD3DSystem->GetDeviceContext()->OMSetRenderTargets(1, &RTView, DSView);

	// Set the viewport.
	m_pD3DSystem->GetDeviceContext()->RSSetViewports(1, &viewport);
}
//==============================================================================================================================
void RenderTarget2D::ClearRenderTarget(float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_pD3DSystem->GetDeviceContext()->ClearRenderTargetView(RTView, color);
    
	// Clear the depth buffer.
	m_pD3DSystem->GetDeviceContext()->ClearDepthStencilView(DSView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}
//==============================================================================================================================
void RenderTarget2D::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}
//==============================================================================================================================
void RenderTarget2D::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}
//==============================================================================================================================
void RenderTarget2D::GetTexDesc
(	int& textureWidth
,	int& textureHeight
//,	float& screenDepth
//,	float& screenNear
,	DXGI_FORMAT& format
,	UINT& multiSamples
,	UINT& msQuality
,	UINT& numMipLevels
,	bool& autoGenMipMaps
,	bool& createUAV
,	UINT& arraySize
)
{
	textureWidth = Width;
	textureHeight = Height;
	//screenDepth = m_depth;
	//screenNear = m_near;
	format = Format;
	multiSamples = MSCount;
	msQuality = MSQuality;
	numMipLevels = NumMipLevels;
	autoGenMipMaps = AutoGenMipMaps;
	createUAV = createUAV;
	arraySize = ArraySize;
}
//==============================================================================================================================
void RenderTarget2D::ResetView()
{
	//Reset the render target to the original back buffer and not the render to texture anymore
	m_pD3DSystem->SetBackBufferRenderTarget();

	//Reset the viewport back to the original
	m_pD3DSystem->ResetViewport();
}
//==============================================================================================================================