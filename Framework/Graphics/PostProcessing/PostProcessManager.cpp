#include "PostProcessManager.h"
//==================================================================================================================================
//==================================================================================================================================
vector<PostProcessManager::RenderTarget*> PostProcessManager::mCreatedTargets;
D3D* PostProcessManager::mD3DSystem = 0;
ZShadeSandboxShader::Shader* PostProcessManager::mShader = 0;
ID3D11RenderTargetView* PostProcessManager::mSwapChainRTV = 0;
ID3D11DepthStencilView* PostProcessManager::mDSView = 0;
DXGI_SURFACE_DESC* PostProcessManager::mBackBufferSurfaceDesc = 0;
//RenderTarget* PostProcessManager::backbufferTarget = 0;
//==================================================================================================================================
PostProcessManager::PostProcessManager(D3D* d3d)
{
	if (PostProcessManager::mD3DSystem == 0)
	{
		PostProcessManager::mD3DSystem = d3d;
	}
	
	Init();
}
//==================================================================================================================================
PostProcessManager::~PostProcessManager()
{
}
//==================================================================================================================================
void PostProcessManager::Init()
{
	// The fullscreen quad vertex shader that will be rendered to is called PostProcessQuadVS in PostProcessVS.hlsl
	PostProcessManager::mShader = new ZShadeSandboxShader::Shader(mD3DSystem);
	PostProcessManager::mShader->LoadVertexShader("PostProcessQuadVS");
}
//==================================================================================================================================
void PostProcessManager::AddPostProcess(PostProcess* pp)
{
	m_CurrentPostProcesses.push_back(pp);
}
//==================================================================================================================================
void PostProcessManager::ClearCurrentPostProcesses()
{
	m_CurrentPostProcesses.clear();
}
//==================================================================================================================================
void PostProcessManager::Begin()
{
	PostProcessManager::mD3DSystem->GetDeviceContext()->OMGetRenderTargets(1, &PostProcessManager::mSwapChainRTV, &PostProcessManager::mDSView);
	
	//for (int i = 0; i < m_CurrentPostProcesses.size(); i++)
	//{
	//	PostProcessManager::mD3DSystem->GetDeviceContext()->OMSetRenderTargets(1, &m_CurrentPostProcesses[i]->Target->RTView, &PostProcessManager::mDSView);
	//}
}
//==================================================================================================================================
void PostProcessManager::Render(ID3D11ShaderResourceView* sceneSRV)
{
	/*
		To chain different pp together set input texture m_CurrentPostProcesses[i].inputSRV to m_CurrentPostProcesses[i - 1].lastOutputSRV
		to get the prev srv that was generated into the new process.
	*/
	
	// This should be done before anything is drawn
	//ID3D11RenderTargetView* swapChainRTV;
	//ID3D11DepthStencilView* dsView;
	//PostProcessManager::mD3DSystem->GetDeviceContext()->OMGetRenderTargets(1, &swapChainRTV, &dsView);
	//Instead of passing in the scenes render target
	//Do This
	//pD3DImmediateContext->OMGetRenderTargets(1, &(m_pSwapChainRTV), &m_pDSView);
	//pD3DImmediateContext->OMSetRenderTargets(1, &(m_HDRResource.m_ppRTV[0]), m_pDSView);
	
	//
	// Grab the back buffer target info
	//
	
	/*if (PostProcessManager::backbufferTarget == 0)
	{
		PostProcessManager::backbufferTarget = new RenderTarget();
		
		PostProcessManager::backbufferTarget->Texture = PostProcessManager::mD3DSystem->GetBackbufferRenderTarget()->RTVTexture;
		PostProcessManager::backbufferTarget->RTView = PostProcessManager::mD3DSystem->GetBackbufferRenderTarget()->RTView;
		PostProcessManager::backbufferTarget->SRView = PostProcessManager::mD3DSystem->GetBackbufferRenderTarget()->SRView;
	}*/
	
	// Set attributes and render the post process
	for (int i = 0; i < m_CurrentPostProcesses.size(); i++)
	{
		switch (m_CurrentPostProcesses[i]->Type())
		{
			case EPostProcessTypes::Type::eColor:
			{
				//m_CurrentPostProcesses[i]->RenderInit(mShader);
			}
			break;
			case EPostProcessTypes::Type::eHDR:
			{
				m_CurrentPostProcesses[i]->AddInputTexture(sceneSRV);
				m_CurrentPostProcesses[i]->AddOutputTarget(PostProcessManager::mSwapChainRTV);
				m_CurrentPostProcesses[i]->DSView() = PostProcessManager::mDSView;
				m_CurrentPostProcesses[i]->ProcessInternal();
			}
			break;
			case EPostProcessTypes::Type::eNone:
			{
				return;
			}
			break;
		}
	}
}
//==================================================================================================================================
/*PostProcessManager::RenderTarget* PostProcessManager::CreateRenderTarget
(	int textureWidth
,	int textureHeight
,	float screenDepth
,	float screenNear
,	DXGI_FORMAT format
,	int multiSamples
,	int msQuality
,	int mipLevels
,	bool generateMipMaps
,	bool useAsUAV
)
{
	for (int i = 0; i < PostProcessManager::mCreatedTargets.size(); i++)
	{
		RenderTarget* rt = PostProcessManager::mCreatedTargets[i];
		if (rt->Width == textureWidth && rt->Height == textureHeight && rt->Format == format
		&&	!rt->bInUse)
		{
			rt->bInUse = true;
			return rt;
		}
	}
	
	RenderTarget* rt = new RenderTarget();
	
	HRESULT result;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	
	// Initialize the render target texture description.
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = textureWidth;
	desc.Height = textureHeight;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	if (useAsUAV)
		desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	desc.CPUAccessFlags = 0;
	desc.Format = format;
	desc.MipLevels = mipLevels;
	desc.MiscFlags = generateMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
	desc.SampleDesc.Count = multiSamples;
	desc.SampleDesc.Quality = msQuality;
	desc.Usage = D3D11_USAGE_DEFAULT;
	PostProcessManager::mD3DSystem->GetDevice11()->CreateTexture2D(&desc, NULL, &rt->Texture);
	//PostProcessManager::mD3DSystem->GetDevice11()->CreateRenderTargetView(rt->Texture, NULL, &rt->RTView);
	
	D3D11_RENDER_TARGET_VIEW_DESC rDesc;
	rDesc.Format = format;
	rDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rt->RTView = (ID3D11RenderTargetView**)malloc(sizeof(ID3D11RenderTargetView*) * m_mipLevels);
	for(int i = 0; i < m_mipLevels; i++)
	{
		rDesc.Texture2D.MipSlice = i;
		PostProcessManager::mD3DSystem->GetDevice11()->CreateRenderTargetView(rt->Texture, &rDesc, &rt->RTView[i]);
	}
	
	PostProcessManager::mD3DSystem->GetDevice11()->CreateShaderResourceView(rt->Texture, NULL, &rt->SRView);

	if (useAsUAV)
	{
		PostProcessManager::mD3DSystem->GetDevice11()->CreateUnorderedAccessView(rt->Texture, NULL, &rt->UAView);
	}
	else
		rt->UAView = NULL;
    
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
	result = PostProcessManager::mD3DSystem->GetDevice11()->CreateTexture2D(&depthBufferDesc, NULL, &rt->DSTexture);
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
	result = PostProcessManager::mD3DSystem->GetDevice11()->CreateDepthStencilView(rt->DSTexture, &depthStencilViewDesc, &rt->DSView);
	if(FAILED(result))
	{
		return false;
	}
	
	// Setup the viewport for rendering.
    rt->viewport.Width = (float)textureWidth;
    rt->viewport.Height = (float)textureHeight;
    rt->viewport.MinDepth = 0.0f;
    rt->viewport.MaxDepth = 1.0f;
    rt->viewport.TopLeftX = 0.0f;
    rt->viewport.TopLeftY = 0.0f;
	
	rt->Width = textureWidth;
	rt->Height = textureWidth;
	rt->Format = format;
	rt->bInUse = true;
	
	PostProcessManager::mCreatedTargets.push_back(rt);
	
	return PostProcessManager::mCreatedTargets[PostProcessManager::mCreatedTargets.size() - 1];
}*/
PostProcessManager::RenderTarget* PostProcessManager::CreateRenderTarget
(	int width
,	int height
,	DXGI_FORMAT format
,	UINT bindflags
,	int mipLevels
)
{
	RenderTarget* rt = new RenderTarget();
	
	rt->bindFlags = bindflags;
	rt->mipLevels = mipLevels;
	
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC tDesc;
    tDesc.ArraySize = 1;
    tDesc.BindFlags = rt->bindFlags;
	tDesc.CPUAccessFlags = 0;
    tDesc.Format = format;
    tDesc.Height = height;
    tDesc.Width = width;
    tDesc.MipLevels = rt->mipLevels;
    tDesc.MiscFlags = 0;
    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;
    tDesc.Usage = D3D11_USAGE_DEFAULT;
	
	if(rt->mipLevels > 1 && (D3D11_BIND_RENDER_TARGET & rt->bindFlags) != 0)
	{
		tDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}
	
	hr = PostProcessManager::mD3DSystem->GetDevice11()->CreateTexture2D(&tDesc, NULL, &rt->Texture);
    
	if(rt->bindFlags & D3D11_BIND_RENDER_TARGET)
	{
		D3D11_RENDER_TARGET_VIEW_DESC rDesc;
		rDesc.Format = format;
		rDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rt->RTView = (ID3D11RenderTargetView**)malloc(sizeof(ID3D11RenderTargetView*) * rt->mipLevels);
		for(int i = 0; i < rt->mipLevels; i++)
		{
			rDesc.Texture2D.MipSlice = i;
			PostProcessManager::mD3DSystem->GetDevice11()->CreateRenderTargetView(rt->Texture, &rDesc, &rt->RTView[i]);
		}
	}
	
	if(rt->bindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.Flags = 0;
		uavDesc.Buffer.NumElements = width * height;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Format = format;
		rt->UAView = (ID3D11UnorderedAccessView**)malloc(sizeof(ID3D11UnorderedAccessView*) * rt->mipLevels);
		
		for(int i = 0; i < rt->mipLevels; i++)
		{
			uavDesc.Texture2D.MipSlice = i;
			PostProcessManager::mD3DSystem->GetDevice11()->CreateUnorderedAccessView(rt->Texture, &uavDesc, &(rt->UAView[i]));
		}
	}
	
	if(rt->bindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC sDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC();
		sDesc.Format = format;
		sDesc.Texture2D.MipLevels = rt->mipLevels;
		sDesc.Texture2D.MostDetailedMip = 0;
		sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		hr = PostProcessManager::mD3DSystem->GetDevice11()->CreateShaderResourceView(rt->Texture, &sDesc, &rt->SRView);
	}
	
	return rt;
}
//==================================================================================================================================
void PostProcessManager::ClearRenderTargets()
{
	PostProcessManager::mCreatedTargets.clear();
}
//==================================================================================================================================
void PostProcessManager::RenderTarget::Set()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	//PostProcessManager::mD3DSystem->GetDeviceContext()->OMSetRenderTargets(1, &RTView, DSView);
	
	// Set the viewport.
	//PostProcessManager::mD3DSystem->GetDeviceContext()->RSSetViewports(1, &viewport);
}
//==================================================================================================================================
void PostProcessManager::RenderTarget::Clear(float red, float green, float blue, float alpha)
{
	float color[4];
	
	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;
	
	// Clear the back buffer.
	//PostProcessManager::mD3DSystem->GetDeviceContext()->ClearRenderTargetView(RTView, color);
    
	// Clear the depth buffer.
	//PostProcessManager::mD3DSystem->GetDeviceContext()->ClearDepthStencilView(DSView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
//==================================================================================================================================
