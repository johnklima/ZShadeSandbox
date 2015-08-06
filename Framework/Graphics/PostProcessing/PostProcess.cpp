#include "PostProcess.h"
#include "PostProcessManager.h"
//==================================================================================================================================
//==================================================================================================================================
PostProcess::PostProcess(D3D* d3d)
:   mD3DSystem(d3d)
,   bPixelShaderSet(false)
{
}
//==================================================================================================================================
PostProcess::~PostProcess()
{
}
//==================================================================================================================================
void PostProcess::AddInputTexture(ID3D11ShaderResourceView* srv)
{
	mInputTextures.push_back(srv);
}
//==================================================================================================================================
void PostProcess::AddOutputTarget(ID3D11RenderTargetView* rtv)
{
	mOutputTargets.push_back(rtv);
}
//==================================================================================================================================
void PostProcess::AddOutputUAV(ID3D11UnorderedAccessView* uav)
{
	mOutputUAVs.push_back(uav);
}
//==================================================================================================================================
void PostProcess::AddOutputViewport(D3D11_VIEWPORT viewport)
{
	mOutputViewports.push_back(viewport);
}
//==================================================================================================================================
EPostProcessTypes::Type PostProcess::Type()
{
	return mType;
}
//==================================================================================================================================
void PostProcess::PopInputTextures()
{
	if (mInputTextures.size() > 0)
		mInputTextures.clear();
}
//==================================================================================================================================
void PostProcess::PopOutputTargets()
{
	if (mOutputTargets.size() > 0)
		mOutputTargets.clear();
}
//==================================================================================================================================
void PostProcess::PopOutputUAVs()
{
	if (mOutputUAVs.size() > 0)
		mOutputUAVs.clear();
}
//==================================================================================================================================
void PostProcess::PopOutputViewports()
{
	if (mOutputViewports.size() > 0)
		mOutputViewports.clear();
}
//==================================================================================================================================
void PostProcess::UnloadResources()
{
	PopInputTextures();
	PopOutputTargets();
	PopOutputUAVs();
	PopOutputViewports();
}
//==================================================================================================================================
void PostProcess::ProcessCompute(string computeShaderName, int threadX, int threadY, int threadZ)
{
	// Set the render targets
	/*int numTargets = mOutputTargets.size();
	ID3D11RenderTargetView** renderTargets = reinterpret_cast<ID3D11RenderTargetView**>(&mOutputTargets[0]);
	if (mOutputUAVs.size() == 0)
	{
		mD3DSystem->GetDeviceContext()->OMSetRenderTargets(numTargets, renderTargets, NULL);
	}
	else
    {
        ID3D11UnorderedAccessView** uavs = reinterpret_cast<ID3D11UnorderedAccessView**>(&mOutputUAVs[0]);
        UINT numUAVs = static_cast<UINT>(mOutputUAVs.size());
        UINT initialCounts[D3D11_PS_CS_UAV_REGISTER_COUNT] = { 0 };
        mD3DSystem->GetDeviceContext()->OMSetRenderTargetsAndUnorderedAccessViews(numTargets, renderTargets, NULL, numTargets, numUAVs, uavs, initialCounts);
    }
	
	// Set the input textures
	int numSRVs = mInputTextures.size();
	ID3D11ShaderResourceView** textures = reinterpret_cast<ID3D11ShaderResourceView**>(&mInputTextures[0]);
	mD3DSystem->GetDeviceContext()->PSSetShaderResources(0, numSRVs, textures);
	
	// Set the compute shader for the post process
	PostProcessManager::mShader->SwitchTo(computeShaderName, ZShadeSandboxShader::EShaderTypes::ST_COMPUTE);
	
	// Set the viewports
	int numViewports = mOutputViewports.size();
	mD3DSystem->GetDeviceContext()->RSSetViewports(numViewports, &mOutputViewports[0]);
	
	PostProcessManager::mShader->RenderCS11(threadX, threadY, threadZ);
	
	// Need to unload resources
	UnloadResources();*/
}
//==================================================================================================================================
void PostProcess::Process(string pixelShaderName)
{
	// Set the render targets
	/*int numTargets = mOutputTargets.size();
	ID3D11RenderTargetView** renderTargets = reinterpret_cast<ID3D11RenderTargetView**>(&mOutputTargets[0]);
	if (mOutputUAVs.size() == 0)
	{
		mD3DSystem->GetDeviceContext()->OMSetRenderTargets(numTargets, renderTargets, NULL);
	}
	else
    {
        ID3D11UnorderedAccessView** uavs = reinterpret_cast<ID3D11UnorderedAccessView**>(&mOutputUAVs[0]);
        UINT numUAVs = static_cast<UINT>(mOutputUAVs.size());
        UINT initialCounts[D3D11_PS_CS_UAV_REGISTER_COUNT] = { 0 };
        mD3DSystem->GetDeviceContext()->OMSetRenderTargetsAndUnorderedAccessViews(numTargets, renderTargets, NULL, numTargets, numUAVs, uavs, initialCounts);
    }
	
	// Set the input textures
	int numSRVs = mInputTextures.size();
	ID3D11ShaderResourceView** textures = reinterpret_cast<ID3D11ShaderResourceView**>(&mInputTextures[0]);
	mD3DSystem->GetDeviceContext()->PSSetShaderResources(0, numSRVs, textures);
	
	// Set the pixel shader for the post process
	PostProcessManager::mShader->SwitchTo(pixelShaderName, ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	
	// Set the viewports
	int numViewports = mOutputViewports.size();
	mD3DSystem->GetDeviceContext()->RSSetViewports(numViewports, &mOutputViewports[0]);
	
	mD3DSystem->TurnOnAdditiveBlending();
	
	// Set all IA stage inputs to NULL, since we're not using it at all.
	ID3D11Buffer* vertexBuffers[1] = { NULL };
	UINT strides[1] = { 0 };
	UINT offsets[1] = { 0 };
	mD3DSystem->GetDeviceContext()->IASetVertexBuffers(0, 1, vertexBuffers, strides, offsets);
	mD3DSystem->GetDeviceContext()->IASetInputLayout(NULL);
	mD3DSystem->GetDeviceContext()->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
	
	// Set the vertex shader for the fullscreen quad
	PostProcessManager::mShader->SetVertexShader();
	
	// Set the pixel shader being used for the post process
	PostProcessManager::mShader->SetPixelShader();
	
	// Bind a NULL Input Layout for the fullscreen quad and draw with 4 points
	PostProcessManager::mShader->RenderDraw11(4);
	
	// Need to unload resources
	UnloadResources();
	
	mD3DSystem->TurnOffAdditiveBlending();*/
}
//==================================================================================================================================