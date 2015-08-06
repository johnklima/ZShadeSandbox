#include "PostProcessManager.h"
//==================================================================================================================================
//==================================================================================================================================
vector<PostProcessManager::RenderTarget*> PostProcessManager::mCreatedTargets;
D3D* PostProcessManager::mD3DSystem = 0;
ZShadeSandboxShader::Shader* PostProcessManager::mShader = 0;
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
void PostProcessManager::Render(ID3D11ShaderResourceView* sceneSRV, ID3D11RenderTargetView*& outputRTV)
{
	/*
		To chain different pp together set input texture m_CurrentPostProcesses[i].inputSRV to m_CurrentPostProcesses[i - 1].lastOutputSRV
		to get the prev srv that was generated into the new process.
	*/
	
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
				m_CurrentPostProcesses[i]->AddOutputTarget(outputRTV);
				// Add the viewport for the outputRTV
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
PostProcessManager::RenderTarget* PostProcessManager::CreateRenderTarget
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
	PostProcessManager::mD3DSystem->GetDevice11()->CreateRenderTargetView(rt->Texture, NULL, &rt->RTView);
	PostProcessManager::mD3DSystem->GetDevice11()->CreateShaderResourceView(rt->Texture, NULL, &rt->SRView);

	if (useAsUAV)
		PostProcessManager::mD3DSystem->GetDevice11()->CreateUnorderedAccessView(rt->Texture, NULL, &rt->UAView);
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
	PostProcessManager::mD3DSystem->GetDeviceContext()->OMSetRenderTargets(1, &RTView, DSView);
	
	// Set the viewport.
	PostProcessManager::mD3DSystem->GetDeviceContext()->RSSetViewports(1, &viewport);
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
	PostProcessManager::mD3DSystem->GetDeviceContext()->ClearRenderTargetView(RTView, color);
    
	// Clear the depth buffer.
	PostProcessManager::mD3DSystem->GetDeviceContext()->ClearDepthStencilView(DSView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
//==================================================================================================================================
