#include "D3D.h"
#include "CGlobal.h"
#include "Shader.h"
#include "ShaderCompiler.h"
#include "DCRenderTexture.h"
#include "StructuredBuffer.h"
#include "ConstantBuffer.h"
#include "GBuffer.h"
//===============================================================================================================================
//===============================================================================================================================
D3D::D3D()
:	m_device10(0)
,	m_isFullscreen(false)
,	m_device11(0)
,	m_deviceContext(0)
,	m_swapChain(0)
//,	m_renderTargetViewBackbuffer(0)
//,	m_depthStencilBackBuffer(0)
,	m_depthStencilState(0)
//,	m_pDepthStencilBackBufferView(0)
,	mOptions(0)
,	m_pBackbufferDS(0)
,	m_pBackbufferRT(0)
,	mGBuffer(0)
{
}
//===============================================================================================================================
D3D::D3D(const D3D& other)
{
}
//===============================================================================================================================
D3D::~D3D()
{
	m_gdi->Shutdown();
}
//===============================================================================================================================
bool D3D::Initialize11(EngineOptions* options)
{
	HWND hwnd = options->hwnd;

	// Initialize the GDI Plus interface to use with DirectX
	m_gdi = new GDI();
	
	mOptions = options;

	m_isFullscreen = mOptions->fullscreen;

	m_screenWidth = options->m_screenWidth;
	m_screenHeight = options->m_screenHeight;

	//Store the vsync setting
	m_vsync_enabled = mOptions->m_vsync;

	//Initialize the DirectX interface adapters and get display modes
	InitializeAdapterAndModes();

	//Create the Swap chain and device
	InitializeDeviceAndSwapChain11();

	//Create the backbuffers and render targets
	InitializeBackbuffers11();

	//Setup the main viewport for rendering
    m_viewport.Width = static_cast<float>(m_screenWidth);
    m_viewport.Height = static_cast<float>(m_screenHeight);
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;

    m_deviceContext->RSSetViewports(1, &m_viewport);

	//Create the device states and render states
	InitRenderStates();

	//m_RenderTargets.SetInitialValue( NULL );

	return true;
}
//===============================================================================================================================
bool D3D::InitializeAdapterAndModes()
{
	HRESULT hr;
	unsigned int numModes;
	unsigned int i;
	unsigned int stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;

	//Create a DirectX Graphics interface factory
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

	if (FAILED(hr))
	{
		return false;
	}

	//Use the factory to create an adapter for the primary graphics interface (video card)
	hr = factory->EnumAdapters(0, &adapter);

	if (FAILED(hr))
	{
		return false;
	}

	//Enumerate the primary adapter output (monitor)
	hr = adapter->EnumOutputs(0, &adapterOutput);

	if (FAILED(hr))
	{
		return false;
	}

	//Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor)
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);

	if (FAILED(hr))
	{
		return false;
	}

	//Create a list to hold all the Vertexsible display modes for this monitor/video card combination
	displayModeList = new DXGI_MODE_DESC[numModes];

	if (!displayModeList)
	{
		return false;
	}

	//Now fill the display mode list structures
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

	if (FAILED(hr))
	{
		return false;
	}

	//Now go through all the display modes and find the one that matches the screen width and height
	//When a match is found store the numerator and denominator of the refresh rate for that monitor
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)m_screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)m_screenHeight)
			{
				refreshRate.Numerator = displayModeList[i].RefreshRate.Numerator;
				refreshRate.Denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//Get the adapter (video card) description
	hr = adapter->GetDesc(&adapterDesc);

	if (FAILED(hr))
	{
		return false;
	}

	//Store the dedicated video card memory in megabytes
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//ZShadeSandboxGlobal::Convert the name of the video card to a character array and store it
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);

	if (error != 0)
	{
		return false;
	}

	////Release the display mode list
	//SAFE_DELETE_ARRAY(displayModeList);

	////Release the adapter output
	//SAFE_RELEASE(adapterOutput);

	////Release the adapter
	//SAFE_RELEASE(adapter);

	////Release the factory
	//SAFE_RELEASE(factory);

	return true;
}
//===============================================================================================================================
bool D3D::InitializeDeviceAndSwapChain11()
{
	HRESULT hr;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;

	//Initialize the swap chain description
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//Set to a single back buffer
	swapChainDesc.BufferCount = 1;

	//Set the width and height of the back buffer
	swapChainDesc.BufferDesc.Width = m_screenWidth;
	swapChainDesc.BufferDesc.Height = m_screenHeight;

	//Set regular 32-bit surface for the back buffer
	//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//Set the refresh rate of the back buffer
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = refreshRate.Numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = refreshRate.Denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//Set the usage of the back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;// | DXGI_USAGE_SHADER_INPUT;

	//Set the handle for the window to render to
	swapChainDesc.OutputWindow = mOptions->hwnd;

	//Turn multisampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	//Set to fullscreen or windowd mode
	if (m_isFullscreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	//Set the scan line ordering and scaling to unspecified
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Discard the back buffer contents after presenting
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//Set the advanced flags to go fullscreen when alt+enter is pressed
	swapChainDesc.Flags = 0;// DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//Set the feature level to directx 11
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	//Create the swap chain, direct3d device, and direct3d device context
	hr = D3D11CreateDeviceAndSwapChain(NULL,
										D3D_DRIVER_TYPE_HARDWARE,
										NULL,
										0,
										&featureLevel,
										1,
										D3D11_SDK_VERSION,
										&swapChainDesc,
										&m_swapChain,
										&m_device11,
										NULL,
										&m_deviceContext);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}
//===============================================================================================================================
//bool D3D::InitializePostProcessingTargets()
//{
	//Initialize the color render target
	//m_pColorRT = new RenderTarget2D(this);
	//m_pColorRT->Initialize( m_screenWidth, m_screenHeight, 0, 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 1, 0, 1 );

	//Initialize the description of the depth buffer
	//m_pDepthStencilBuffer = new DepthStencilBuffer(this);
	//m_pDepthStencilBuffer->Initialize( m_screenWidth, m_screenHeight, DXGI_FORMAT_D24_UNORM_S8_UINT, true, 1, 0 );

//	return true;
//}
//===============================================================================================================================
bool D3D::InitializeBackbuffers11()
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDescription;

	//Get the pointer to the back buffer
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);

	if (FAILED(hr))
	{
		return false;
	}
	
	//Create the render target view with the back buffer pointer
	/*hr = m_device11->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetViewBackbuffer);

	if (FAILED(hr))
	{
		return false;
	}

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//Default Depth Stencil
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	m_backBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthBufferDesc.Width = m_screenWidth;
	depthBufferDesc.Height = m_screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//Create the texture for the depth buffer
	hr = m_device11->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBackBuffer);

	if (FAILED(hr))
	{
		return false;
	}

	//Initialize the depth stencil view
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//Setup the depth stencil view description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//Create the depth stencil view
	hr = m_device11->CreateDepthStencilView(m_depthStencilBackBuffer, &depthStencilViewDesc, &m_pDepthStencilBackBufferView);

	if (FAILED(hr))
	{
		return false;
	}

	//Bind the render target view and depth stencil buffer to the output render pipeline
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetViewBackbuffer, m_pDepthStencilBackBufferView);
	*/
	
	m_backBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	
	m_pBackbufferRT = new RenderTarget2D(this);
	m_pBackbufferRT->Initialize(backBufferPtr, m_screenWidth, m_screenHeight);

	m_pBackbufferDS = new DepthStencilBuffer(this);
	m_pBackbufferDS->Initialize(m_screenWidth, m_screenHeight, m_backBufferFormat, true);
	
	//Bind the render target view and depth stencil buffer to the output render pipeline
	m_deviceContext->OMSetRenderTargets(1, &m_pBackbufferRT->RTView, m_pBackbufferDS->GetDSView());
	
	//
	// Create the depth stencil state
	//
	
	//Initialize the description of the stencil state
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//Setup the description of the stencil state
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//Stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Create the depth stencil state
	hr = m_device11->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);

	if (FAILED(hr))
	{
		return false;
	}

	//Set the depth stencil state
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	
	// Create the GBuffer
	mGBuffer = new GBuffer(this);
	mGBuffer->Init(m_screenWidth, m_screenHeight, mOptions->fNearPlane, mOptions->fFarPlane);
	
	ZShadeSandboxShader::ShaderCompiler::CompileAll(this);

	return true;
}
//===============================================================================================================================
void D3D::Shutdown()
{
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	SAFE_RELEASE(m_swapChain);
	
	if (mOptions->m_d3dVersion == DIRECTX10)
	{
		SAFE_RELEASE(m_depthDisabledStencilState10);
		SAFE_RELEASE(m_rasterState10);
		SAFE_RELEASE(m_depthStencilView10);
		SAFE_RELEASE(m_depthStencilState10);
		SAFE_RELEASE(m_depthStencilBuffer10);
		SAFE_RELEASE(m_renderTargetView10);
		SAFE_RELEASE(m_device10);
	}
	else if (mOptions->m_d3dVersion == DIRECTX11)
	{
		//SAFE_RELEASE(m_depthStencilBackBuffer);
		SAFE_RELEASE(m_depthStencilState);
		//SAFE_RELEASE(m_renderTargetViewBackbuffer);
		SAFE_RELEASE(m_deviceContext);
		SAFE_RELEASE(m_device11);
	}
}
//===============================================================================================================================
void D3D::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
}
//===============================================================================================================================
//void D3D::SetGBufferTargets()
//{
//	mGBuffer->SetRenderTargets();
//	mGBuffer->ClearRenderTargets();
//}
//===============================================================================================================================
//ID3D11ShaderResourceView* D3D::GetGBufferColorSRV() { return mGBuffer->GetColorSRV(); }
//ID3D11ShaderResourceView* D3D::GetGBufferNormalSRV() { return mGBuffer->GetNormalSRV(); }
//ID3D11ShaderResourceView* D3D::GetGBufferDepthSRV() { return mGBuffer->GetDepthSRV(); }
//===============================================================================================================================
void D3D::GBufferBegin()
{
	mGBuffer->Begin(m_pBackbufferDS->GetDSView());
}
//===============================================================================================================================
void D3D::GBufferEnd()
{
	mGBuffer->End();
	
	//m_deviceContext->ClearDepthStencilView(m_pBackbufferDS->GetDSView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	//Set the depth stencil state
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	
	SetBackBufferRenderTarget();
	//ID3D11RenderTargetView* rtv[1] = { m_pBackbufferRT->RTView };
	//m_deviceContext->OMSetRenderTargets(1, rtv, NULL);
	
	//Reset the viewport back to the original
	ResetViewport();
}
//===============================================================================================================================
RenderTarget2D* D3D::GBufferColorTarget()
{
	return mGBuffer->ColorTarget();
}
//===============================================================================================================================
RenderTarget2D* D3D::GBufferNormalTarget()
{
	return mGBuffer->NormalTarget();
}
//===============================================================================================================================
RenderTarget2D* D3D::GBufferDepthTarget()
{
	return mGBuffer->DepthTarget();
}
//===============================================================================================================================
RenderTarget2D*& D3D::GBufferColorTargetRef()
{
	return mGBuffer->ColorTargetRef();
}
//===============================================================================================================================
RenderTarget2D*& D3D::GBufferNormalTargetRef()
{
	return mGBuffer->NormalTargetRef();
}
//===============================================================================================================================
RenderTarget2D*& D3D::GBufferDepthTargetRef()
{
	return mGBuffer->DepthTargetRef();
}
//===============================================================================================================================
bool D3D::BeginScene(float red, float green, float blue, float alpha)
{
	HRESULT hr;

	if (mOptions->m_d3dVersion == DIRECTX10)
	{
		float color[4];

		color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;

		//Clear the back buffer
		m_device10->ClearRenderTargetView(m_renderTargetView10, color);

		m_device10->ClearDepthStencilView(m_depthStencilView10, D3D10_CLEAR_DEPTH, 1.0f, 0);
	}
	else if (mOptions->m_d3dVersion == DIRECTX11)
	{
		float color[4];

		color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;
		
		ID3D11RenderTargetView* rtv[1] = { m_pBackbufferRT->RTView };
		ID3D11DepthStencilView* dsv = m_pBackbufferDS->GetDSView();
		
		m_deviceContext->OMSetRenderTargets(1, rtv, dsv);
		
		m_deviceContext->ClearRenderTargetView(rtv[0], color);
		
		m_deviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	return true;
}
//===============================================================================================================================
void D3D::ClearBackbufferStencil(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	//Clear the back buffer
	ID3D11RenderTargetView* rtv[1] = { m_pBackbufferRT->RTView };
	ID3D11DepthStencilView* dsv = m_pBackbufferDS->GetDSView();
	
	m_deviceContext->ClearRenderTargetView(rtv[0], color);
	
	m_deviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
//===============================================================================================================================
void D3D::EndScene()
{
	if (mOptions->m_d3dVersion == DIRECTX10)
	{
		//m_device->EndScene();
		//m_device->Present(NULL, NULL, NULL, NULL);
	}
	else if (mOptions->m_d3dVersion == DIRECTX11)
	{
		if (m_vsync_enabled)
		{
			//Lock to screen refresh rate
			m_swapChain->Present(1, 0);
		}
		else
		{
			ID3D11RenderTargetView* rtv[1] = { NULL };
			m_deviceContext->OMSetRenderTargets(1, rtv, NULL);
			
			//Present as fast as Possible
			m_swapChain->Present(0, 0);
		}
	}
}
//===============================================================================================================================
void D3D::SetDepthStencilState(ID3D11DepthStencilState* dst)
{
	m_deviceContext->OMSetDepthStencilState(dst, 1);
}
//===============================================================================================================================
void D3D::TurnOnZBuffer()
{
	if (mOptions->m_d3dVersion == DIRECTX10)
	{
		//m_device->SetRenderState(D3DRS_ZENABLE, true);
		m_device10->OMSetDepthStencilState(m_depthStencilState10, 1);
	}
	else if (mOptions->m_d3dVersion == DIRECTX11)
	{
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	}
}
//===============================================================================================================================
void D3D::TurnOffZBuffer()
{
	if (mOptions->m_d3dVersion == DIRECTX10)
	{
		//m_device->SetRenderState(D3DRS_ZENABLE, false);
		m_device10->OMSetDepthStencilState(m_depthDisabledStencilState10, 1);
	}
	else if (mOptions->m_d3dVersion == DIRECTX11)
	{
		m_deviceContext->OMSetDepthStencilState(m_pDepthDisabled, 1);
	}
}
//===============================================================================================================================
void D3D::SetBlendState(ID3D11BlendState* bld)
{
	if (mOptions->m_d3dVersion == DIRECTX11)
	{
		float blendFactor[4];

		blendFactor[0] = 1.0f;
		blendFactor[1] = 1.0f;
		blendFactor[2] = 1.0f;
		blendFactor[3] = 1.0f;

		//Turn on blending
		m_deviceContext->OMSetBlendState(bld, blendFactor, 0xFFFFFFFF);
	}
}
//===============================================================================================================================
void D3D::SetRasterizerState(ID3D11RasterizerState* rs)
{
	if (mOptions->m_d3dVersion == DIRECTX11)
		m_deviceContext->RSSetState(rs);
}
//===============================================================================================================================
void D3D::TurnOnAdditiveBlending()
{
	if (mOptions->m_d3dVersion == DIRECTX11)
	{
		float blendFactor[4];

		blendFactor[0] = 1.0f;
		blendFactor[1] = 1.0f;
		blendFactor[2] = 1.0f;
		blendFactor[3] = 1.0f;

		//Turn on the alpha blending
		m_deviceContext->OMSetBlendState(m_pAdditiveBlend, blendFactor, 0xFFFFFFFF);
	}
}
//===============================================================================================================================
void D3D::TurnOffAdditiveBlending()
{
	//SetBlendState(m_pBlendDisabled);

	float blendFactor[4];

	blendFactor[0] = 1.0f;
	blendFactor[1] = 1.0f;
	blendFactor[2] = 1.0f;
	blendFactor[3] = 1.0f;

	//Turn off the alpha blending
	m_deviceContext->OMSetBlendState(m_pBlendDisabled, blendFactor, 0xFFFFFFFF);
}
//===============================================================================================================================
void D3D::TurnOnAlphaBlending()
{
	if (mOptions->m_d3dVersion == DIRECTX10)
	{
		//m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		//Set the colour to come completely from the texture

		//m_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
		//m_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		//m_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);   //Ignored


		//Set the alpha to come completely from the diffuse

		//m_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
		//m_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
		//m_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TEXTURE);   //Ignored

		//m_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		//m_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		//m_device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	}
	else if (mOptions->m_d3dVersion == DIRECTX11)
	{
		float blendFactor[4];

		blendFactor[0] = 1.0f;
		blendFactor[1] = 1.0f;
		blendFactor[2] = 1.0f;
		blendFactor[3] = 1.0f;

		//Turn on the alpha blending
		m_deviceContext->OMSetBlendState(m_pAlphaBlend, blendFactor, 0xFFFFFFFF);
	}
}
//===============================================================================================================================
void D3D::TurnOffAlphaBlending()
{
	if (mOptions->m_d3dVersion == DIRECTX10)
	{
		//m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
	else if (mOptions->m_d3dVersion == DIRECTX11)
	{
		float blendFactor[4];

		blendFactor[0] = 1.0f;
		blendFactor[1] = 1.0f;
		blendFactor[2] = 1.0f;
		blendFactor[3] = 1.0f;

		//Turn off the alpha blending
		m_deviceContext->OMSetBlendState(m_pBlendDisabled, blendFactor, 0xFFFFFFFF);
	}
}
//===============================================================================================================================
void D3D::TurnOnCulling()
{
	// Set the culling rasterizer state.
	m_deviceContext->RSSetState(m_pCullBackfaces);
}
//===============================================================================================================================
void D3D::TurnOffCulling()
{
	// Set the no back face culling rasterizer state.
	m_deviceContext->RSSetState(m_pNoCull);
}
//===============================================================================================================================
void D3D::AfterReset(int width, int height)
{
	InitializeBackbuffers11();

	//Setup the main viewport for rendering
    m_viewport.Width = static_cast<float>(m_screenWidth);
    m_viewport.Height = static_cast<float>(m_screenHeight);
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;

    m_deviceContext->RSSetViewports(1, &m_viewport);
}
//===============================================================================================================================
void D3D::PrepareFullScreenSettings()
{
    assert(adapterOutput);

    // Have the Output look for the closest matching mode
    DXGI_MODE_DESC desiredMode;
    desiredMode.Format = m_backBufferFormat;
    desiredMode.Width = m_screenWidth;
    desiredMode.Height = m_screenHeight;
    desiredMode.RefreshRate.Numerator = 0;
    desiredMode.RefreshRate.Denominator = 0;
    desiredMode.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desiredMode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    DXGI_MODE_DESC closestMatch;
    adapterOutput->FindClosestMatchingMode(&desiredMode, &closestMatch, m_device11.GetInterfacePtr());

	mOptions->m_screenWidth = closestMatch.Width;
	mOptions->m_screenHeight = closestMatch.Height;
    m_backBufferFormat = closestMatch.Format;
    m_screenWidth = closestMatch.Width;
    m_screenHeight = closestMatch.Height;
    refreshRate = closestMatch.RefreshRate;
}
//===============================================================================================================================
void D3D::SetBackbufferSize(int w, int h)
{
	mOptions->m_screenWidth = w;
	mOptions->m_screenHeight = h;
	m_screenWidth = w;
	m_screenHeight = h;
}
//===============================================================================================================================
void D3D::ResizeDevice()
{
	assert(m_swapChain);

    // Release all references
    //SAFE_RELEASE(m_depthStencilBackBuffer);
    //SAFE_RELEASE(m_renderTargetViewBackbuffer);

    m_deviceContext->ClearState();

    if(!m_isFullscreen)
    {
		refreshRate.Numerator = 0;
		refreshRate.Denominator = 1;
    }

    m_swapChain->SetFullscreenState(m_isFullscreen, NULL);

    m_swapChain->ResizeBuffers(1, m_screenWidth, m_screenHeight, m_backBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

    if(m_isFullscreen)
    {
        DXGI_MODE_DESC mode;
        mode.Format = m_backBufferFormat;
        mode.Width = m_screenWidth;
        mode.Height = m_screenHeight;
        mode.RefreshRate.Numerator = 0;
        mode.RefreshRate.Denominator = 0;
        mode.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        mode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        m_swapChain->ResizeTarget(&mode);
    }
	else
	{
		int crap = 0;
		crap = 23;
	}

    AfterReset(m_screenWidth, m_screenHeight);
}
//===============================================================================================================================
void D3D::GetBackbufferSize(int & w, int & h)
{
	w = m_screenWidth;
	h = m_screenHeight;
}
//===============================================================================================================================
void D3D::SetFullscreen(bool fullscreen)
{
	m_isFullscreen = fullscreen;
	mOptions->fullscreen = m_isFullscreen;
	ResizeDevice();
}
//===============================================================================================================================
/*void D3D::SetRenderTargetNotInUse(RenderTarget2D* rt)
{
}
//===============================================================================================================================
void D3D::ClearRenderTargets()
{
	m_RenderTargets.clear();
}
//===============================================================================================================================
RenderTarget2D* D3D::CreateRenderTarget
(	int textureWidth
,	int textureHeight
,	float screenDepth
,	float screenNear
,	DXGI_FORMAT format
,	UINT numMipLevels
,	UINT multiSamples
,	UINT msQuality
,	bool autoGenMipMaps
,	bool createUAV
,	UINT arraySize
)
{
	for (int i = 0; i < m_RenderTargets.size(); i++)
	{
		RenderTarget2D* rt = m_RenderTargets[i];
		if ( !rt->InUse && rt->Compare(textureWidth, textureHeight, screenDepth, screenNear, format, multiSamples, msQuality, numMipLevels, autoGenMipMaps, createUAV, arraySize ))
		{
			rt->InUse = true;
			return rt;
		}
	}

	RenderTarget2D* rt = new RenderTarget2D(this);
	rt->Initialize( textureWidth, textureHeight, screenDepth, screenNear, format, multiSamples, msQuality, numMipLevels, autoGenMipMaps, createUAV, arraySize );

	rt->InUse = true;

	m_RenderTargets.push_back( rt );

	return m_RenderTargets[m_RenderTargets.size() - 1];
}*/
//===============================================================================================================================
void D3D::InitRenderStates()
{
	if (mOptions->m_d3dVersion == DIRECTX10)
	{
		// State blocks
		D3D10_RASTERIZER_DESC ras_desc;
		ras_desc.FillMode = D3D10_FILL_SOLID; 
		ras_desc.CullMode = D3D10_CULL_BACK;
		ras_desc.FrontCounterClockwise = FALSE; 
		ras_desc.DepthBias = 0;
		ras_desc.SlopeScaledDepthBias = 0.0f;
		ras_desc.DepthBiasClamp = 0.0f;
		ras_desc.DepthClipEnable= TRUE;
		ras_desc.ScissorEnable = FALSE;
		ras_desc.MultisampleEnable = TRUE;
		ras_desc.AntialiasedLineEnable = FALSE;

		m_device10->CreateRasterizerState(&ras_desc, &m_solidRS10);
		assert(m_solidRS10);

		ras_desc.FillMode = D3D10_FILL_WIREFRAME;
		ras_desc.CullMode = D3D10_CULL_NONE;

		m_device10->CreateRasterizerState(&ras_desc, &m_wireframeRS10);
		assert(m_wireframeRS10);
	}
	else if (mOptions->m_d3dVersion == DIRECTX11)
	{
		/*
		// State blocks
		D3D11_RASTERIZER_DESC ras_desc;
		ras_desc.FillMode = D3D11_FILL_SOLID; 
		ras_desc.CullMode = D3D11_CULL_BACK;
		ras_desc.FrontCounterClockwise = FALSE;
		ras_desc.DepthBias = 0;
		ras_desc.SlopeScaledDepthBias = 0.0f;
		ras_desc.DepthBiasClamp = 0.0f;
		ras_desc.DepthClipEnable= TRUE;
		ras_desc.ScissorEnable = FALSE;
		ras_desc.MultisampleEnable = TRUE;
		ras_desc.AntialiasedLineEnable = FALSE;

		m_device11->CreateRasterizerState(&ras_desc, &m_solidRS11);
		assert(m_solidRS11);

		ras_desc.FillMode = D3D11_FILL_WIREFRAME;
		ras_desc.CullMode = D3D11_CULL_NONE;

		m_device11->CreateRasterizerState(&ras_desc, &m_wireframeRS11);
		assert(m_wireframeRS11);

		// State blocks
		D3D11_RASTERIZER_DESC ocean_ras_desc;
		ocean_ras_desc.FillMode = D3D11_FILL_SOLID; 
		ocean_ras_desc.CullMode = D3D11_CULL_NONE;
		ocean_ras_desc.FrontCounterClockwise = FALSE;
		ocean_ras_desc.DepthBias = 0;
		ocean_ras_desc.SlopeScaledDepthBias = 0.0f;
		ocean_ras_desc.DepthBiasClamp = 0.0f;
		ocean_ras_desc.DepthClipEnable= TRUE;
		ocean_ras_desc.ScissorEnable = FALSE;
		ocean_ras_desc.MultisampleEnable = TRUE;
		ocean_ras_desc.AntialiasedLineEnable = FALSE;

		m_device11->CreateRasterizerState(&ocean_ras_desc, &m_solidOceanRS11);
		assert(m_solidOceanRS11);

		ocean_ras_desc.FillMode = D3D11_FILL_WIREFRAME;

		m_device11->CreateRasterizerState(&ocean_ras_desc, &m_wireframeOceanRS11);
		assert(m_wireframeOceanRS11);
		*/

		D3D11_BLEND_DESC blendDesc;
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		for (UINT i = 0; i < 8; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable = false;
			blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		}

		m_device11->CreateBlendState(&blendDesc, &m_pBlendDisabled);

		for (UINT i = 0; i < 8; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable = true;
			blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		}

		m_device11->CreateBlendState(&blendDesc, &m_pAdditiveBlend);

		for (UINT i = 0; i < 8; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable = true;
			blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		}

		m_device11->CreateBlendState(&blendDesc, &m_pAlphaBlend);

		for (UINT i = 0; i < 8; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable = false;
			blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		}

		m_device11->CreateBlendState(&blendDesc, &m_pPMAlphaBlend);

		for (UINT i = 0; i < 8; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable = false;
			blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = 0;
			blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		}

		m_device11->CreateBlendState(&blendDesc, &m_pNoColor);
		
		D3D11_RASTERIZER_DESC rastDesc;
		rastDesc.AntialiasedLineEnable = FALSE;
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.DepthBias = 0;
		rastDesc.DepthBiasClamp = 1.0f;
		rastDesc.DepthClipEnable = false;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable = true;
		rastDesc.ScissorEnable = false;
		rastDesc.SlopeScaledDepthBias = 0;

		m_device11->CreateRasterizerState(&rastDesc, &m_pDisableDepthClip);
		
		rastDesc.AntialiasedLineEnable = FALSE;
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.DepthBias = 0;
		rastDesc.DepthBiasClamp = 1.0f;
		rastDesc.DepthClipEnable = TRUE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable = true;
		rastDesc.ScissorEnable = false;
		rastDesc.SlopeScaledDepthBias = 0;

		m_device11->CreateRasterizerState(&rastDesc, &m_pNoCull);

		rastDesc.AntialiasedLineEnable = FALSE;
		rastDesc.CullMode = D3D11_CULL_FRONT;
		rastDesc.DepthBias = 0;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.DepthClipEnable = TRUE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable = true;
		rastDesc.ScissorEnable = false;
		rastDesc.SlopeScaledDepthBias = 0;

		m_device11->CreateRasterizerState(&rastDesc, &m_pCullFrontfaces);

		rastDesc.AntialiasedLineEnable = FALSE;
		rastDesc.CullMode = D3D11_CULL_FRONT;
		rastDesc.DepthBias = 0;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.DepthClipEnable = TRUE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = TRUE;
		rastDesc.MultisampleEnable = TRUE;
		rastDesc.ScissorEnable = TRUE;
		rastDesc.SlopeScaledDepthBias = 0;

		m_device11->CreateRasterizerState(&rastDesc, &m_pCullFrontfacesScissor);

		rastDesc.AntialiasedLineEnable = FALSE;
		rastDesc.CullMode = D3D11_CULL_BACK;
		rastDesc.DepthBias = 0;
		rastDesc.DepthBiasClamp = 1.0f;
		rastDesc.DepthClipEnable = TRUE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable = true;
		rastDesc.ScissorEnable = false;
		rastDesc.SlopeScaledDepthBias = 0;

		m_device11->CreateRasterizerState(&rastDesc, &m_pCullBackfaces);

		rastDesc.AntialiasedLineEnable = FALSE;
		rastDesc.CullMode = D3D11_CULL_BACK;
		rastDesc.DepthBias = 0;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.DepthClipEnable = TRUE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = FALSE;
		rastDesc.MultisampleEnable = TRUE;
		rastDesc.ScissorEnable = TRUE;
		rastDesc.SlopeScaledDepthBias = 0;

		m_device11->CreateRasterizerState(&rastDesc, &m_pCullBackfacesScissor);

		rastDesc.AntialiasedLineEnable = FALSE;
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.DepthBias = 0;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.DepthClipEnable = TRUE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable = false;
		rastDesc.ScissorEnable = false;
		rastDesc.SlopeScaledDepthBias = 0;

		m_device11->CreateRasterizerState(&rastDesc, &m_pNoCullNoMS);

		rastDesc.AntialiasedLineEnable = FALSE;
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.DepthBias = 0;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.DepthClipEnable = TRUE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable = true;
		rastDesc.ScissorEnable = true;
		rastDesc.SlopeScaledDepthBias = 0;

		m_device11->CreateRasterizerState(&rastDesc, &m_pNoCullScissor);

		rastDesc.AntialiasedLineEnable = FALSE;
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.DepthBias = 0;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.DepthClipEnable = TRUE;
		rastDesc.FillMode = D3D11_FILL_WIREFRAME;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable = true;
		rastDesc.ScissorEnable = false;
		rastDesc.SlopeScaledDepthBias = 0;

		m_device11->CreateRasterizerState(&rastDesc, &m_pWireframe);
		
		rastDesc.AntialiasedLineEnable = FALSE;
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.DepthBias = 100000;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.DepthClipEnable = TRUE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.MultisampleEnable = true;
		rastDesc.ScissorEnable = false;
		rastDesc.SlopeScaledDepthBias = 1.0f;

		m_device11->CreateRasterizerState(&rastDesc, &m_pDepth);
		
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		dsDesc.DepthEnable = false;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace = dsDesc.FrontFace;

		m_device11->CreateDepthStencilState(&dsDesc, &m_pDepthDisabled);

		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace = dsDesc.FrontFace;

		m_device11->CreateDepthStencilState(&dsDesc, &m_pDepthEnabled);

		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace = dsDesc.FrontFace;

		m_device11->CreateDepthStencilState(&dsDesc, &m_pRevDepthEnabled);

		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace = dsDesc.FrontFace;

		m_device11->CreateDepthStencilState(&dsDesc, &m_pDepthWriteEnabled);

		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace = dsDesc.FrontFace;

		m_device11->CreateDepthStencilState(&dsDesc, &m_pRevDepthWriteEnabled);

		D3D11_SAMPLER_DESC sampDesc;

		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		sampDesc.MipLODBias = 0.0f;
		sampDesc.MaxAnisotropy = 1;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_device11->CreateSamplerState(&sampDesc, &m_pMirror);

		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.MipLODBias = 0.0f;
		sampDesc.MaxAnisotropy = 1;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_device11->CreateSamplerState(&sampDesc, &m_pLinear);

		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.MipLODBias = 0.0f;
		sampDesc.MaxAnisotropy = 1;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_device11->CreateSamplerState(&sampDesc, &m_pLinearClamp);

		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.MipLODBias = 0.0f;
		sampDesc.MaxAnisotropy = 1;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_device11->CreateSamplerState(&sampDesc, &m_pPoint);

		sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.MipLODBias = 0.0f;
		sampDesc.MaxAnisotropy = 16;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_device11->CreateSamplerState(&sampDesc, &m_pAnisotropic);

		sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.MipLODBias = 0.0f;
		sampDesc.MaxAnisotropy = 1;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_device11->CreateSamplerState(&sampDesc, &m_pShadowMap);

		//sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;//CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;//CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;//CLAMP;
		sampDesc.MipLODBias = 0.0f;
		sampDesc.MaxAnisotropy = 1;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;//_EQUAL;
		sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 1.0;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_device11->CreateSamplerState(&sampDesc, &m_pShadowMapPCF);
	}
}
//===============================================================================================================================
void D3D::TurnOnWireframe()
{
	if (mOptions->m_d3dVersion == DIRECTX10)
		m_device10->RSSetState(m_wireframeRS10);
	else if (mOptions->m_d3dVersion == DIRECTX11)
		m_deviceContext->RSSetState(m_pWireframe);
}
//===============================================================================================================================
//void D3D::SetSolidMode()
//{
//	if (mOptions->m_d3dVersion == DIRECTX10)
//		m_device10->RSSetState(m_solidRS10);
//	else if (mOptions->m_d3dVersion == DIRECTX11)
//	{
//		if (m_IsRenderStateOcean)
//			m_deviceContext->RSSetState(m_pNoCull);
//		else
//			m_deviceContext->RSSetState(m_pCullBackfaces);
//	}
//}
//===============================================================================================================================
void D3D::SetViewport(UINT rtWidth, UINT rtHeight)
{
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<float>(rtWidth);
    viewport.Height = static_cast<float>(rtHeight);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    m_deviceContext->RSSetViewports(1, &viewport);
}
//===============================================================================================================================
void D3D::ResetViewport()
{
	// Set the viewport.
    m_deviceContext->RSSetViewports(1, &m_viewport);

	return;
}
//===============================================================================================================================
void D3D::SetBackBufferRenderTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	//m_deviceContext->OMSetRenderTargets(1, &m_renderTargetViewBackbuffer, m_pDepthStencilBackBufferView);
	
	ID3D11RenderTargetView* rtv[1] = { m_pBackbufferRT->RTView };
	ID3D11DepthStencilView* dsv = m_pBackbufferDS->GetDSView();
	
	m_deviceContext->OMSetRenderTargets(1, rtv, dsv);
}
//===============================================================================================================================