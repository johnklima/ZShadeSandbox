#include "HDR.h"
#include "ConstantBuffer.h"
#include "StructuredBuffer.h"
#include "PostProcessManager.h"
#include "StructuredBuffer.h"
//==================================================================================================================================
//==================================================================================================================================
int ThreadGroups(int workItems, int groupItems)
{
	int groups = workItems / groupItems;
	
	if(workItems % groupItems > 0)
	{
		groups++;
	}
	
	return groups;
}
//==================================================================================================================================
HDR::HDR(D3D* d3d)
:   PostProcess(d3d)
//,	hdrTarget(0)
,	luminance(0)
,	bright(0)
,	summed(0)
,	blurredH(0)
,	blurredHV(0)
,	m_pBloomConstants(0)
,	m_pCSConstants(0)
{
	mType = EPostProcessTypes::Type::eHDR;
	
	ConstantBuffer<cbBloomConstants> bloomCB(mD3DSystem);
	bloomCB.Initialize(PAD16(sizeof(cbBloomConstants)));
	m_pBloomConstants = bloomCB.Buffer();
	
	ConstantBuffer<cbConstants> constCB(mD3DSystem);
	constCB.Initialize(PAD16(sizeof(cbConstants)));
	m_pCSConstants = constCB.Buffer();
	
	int backbufferWidth = mD3DSystem->GetEngineOptions()->m_screenWidth;
	int backbufferHeight = mD3DSystem->GetEngineOptions()->m_screenHeight;
	
	// Float format will change the bandwidth needed. 
	//#define FLOAT_FORMAT DXGI_FORMAT_R32G32B32A32_FLOAT
	//#define FLOAT_FORMAT DXGI_FORMAT_R16G16B16A16_FLOAT
	#define FLOAT_FORMAT DXGI_FORMAT_R11G11B10_FLOAT
	
	UINT bindflags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	
	//target = PostProcessManager::CreateRenderTarget(backbufferWidth, backbufferHeight, FLOAT_FORMAT, bindflags, 2);
	
	bindflags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	
	luminance = PostProcessManager::CreateRenderTarget(backbufferWidth / 2, backbufferHeight / 2, DXGI_FORMAT_R32_FLOAT, bindflags, 1);
	summed = PostProcessManager::CreateRenderTarget(backbufferWidth / 2, backbufferHeight / 2, FLOAT_FORMAT, bindflags, BLUR_LEVELS);
	blurredH = PostProcessManager::CreateRenderTarget(backbufferWidth / 2, backbufferHeight / 2, FLOAT_FORMAT, bindflags, BLUR_LEVELS);
	blurredHV = PostProcessManager::CreateRenderTarget(backbufferWidth / 2, backbufferHeight / 2, FLOAT_FORMAT, bindflags, BLUR_LEVELS);
	bright = PostProcessManager::CreateRenderTarget(backbufferWidth / 2, backbufferHeight / 2, FLOAT_FORMAT, bindflags, BLUR_LEVELS);
	
	fMiddleGreyValue = 1.0f;
	fBloomThreshold = 1.0f;
	fBloomMultiplier = 1.0f;
	
	PostProcessManager::mShader->LoadComputeShader("GetAvgLum");
	PostProcessManager::mShader->LoadComputeShader("BlurHorizontal");
	PostProcessManager::mShader->LoadComputeShader("BlurVertical");
	PostProcessManager::mShader->LoadComputeShader("Add");
	PostProcessManager::mShader->LoadComputeShader("ComputeLuminanceAndBright");
	PostProcessManager::mShader->LoadPixelShader("ToneMapWithBloom");
}
//==================================================================================================================================
HDR::~HDR()
{
}
//==================================================================================================================================
void HDR::ProcessInternal()
{
	//hdrTarget = PostProcessManager::backbufferTarget;
	
	HRESULT hr = S_OK;
	
	UpdateBloomConstants(fMiddleGreyValue, fBloomThreshold, fBloomMultiplier);
	
	ID3D11SamplerState* pSamplerState[] = {mD3DSystem->Linear(), mD3DSystem->Point()};
	mD3DSystem->GetDeviceContext()->CSSetSamplers(0, 2, pSamplerState);
	mD3DSystem->GetDeviceContext()->PSSetSamplers(0, 2, pSamplerState);
	
	mD3DSystem->GetDeviceContext()->OMSetRenderTargets(1, &mOutputTargets[0], NULL);
	
	int width = mD3DSystem->GetEngineOptions()->m_screenWidth / 2;
	int height = mD3DSystem->GetEngineOptions()->m_screenHeight / 2;
	int mipLevel0 = 0;
	int mipLevel1 = 0;
	
	ComputeLuminanceAndBrightPass(
		//target->SRView,// I think this is the scene SRV or the input SRV
		mInputTextures[0],// This is the SRV taken from the scene
		bright->UAView[0],
		bright->UAView[1],
		bright->UAView[2],
		bright->UAView[3],
		width,
		height
	);
	
	mipLevel0 = 3;
	mipLevel1 = 4;
	width = mD3DSystem->GetEngineOptions()->m_screenWidth >> (mipLevel0 + 1);
	height = mD3DSystem->GetEngineOptions()->m_screenHeight >> (mipLevel0 + 1);
	
	UpdateContantBuffer(mipLevel0, mipLevel1, width, height);
	BlurHorizontal(bright->SRView, blurredH->UAView[mipLevel0], width, height);
	BlurVertical(blurredH->SRView, blurredHV->UAView[mipLevel0], width, height);
	
	mipLevel0--;
	mipLevel1--;
	width = mD3DSystem->GetEngineOptions()->m_screenWidth >> (mipLevel0 + 1);
	height = mD3DSystem->GetEngineOptions()->m_screenHeight >> (mipLevel0 + 1);
	
	UpdateContantBuffer(mipLevel0, mipLevel1, width, height);
	Add(bright->SRView, blurredHV->SRView, summed->UAView[mipLevel0], width, height);
	BlurHorizontal(bright->SRView, blurredH->UAView[mipLevel0], width, height);
	BlurVertical(blurredH->SRView, blurredHV->UAView[mipLevel0], width, height);
	
	mipLevel0--;
	mipLevel1--;
	width = mD3DSystem->GetEngineOptions()->m_screenWidth >> (mipLevel0 + 1);
	height = mD3DSystem->GetEngineOptions()->m_screenHeight >> (mipLevel0 + 1);
	
	UpdateContantBuffer(mipLevel0, mipLevel1, width, height);
	Add(bright->SRView, blurredHV->SRView, summed->UAView[mipLevel0], width, height);
	BlurHorizontal(summed->SRView, blurredH->UAView[mipLevel0], width, height);
	BlurVertical(blurredH->SRView, blurredHV->UAView[mipLevel0], width, height);
	
	mipLevel0--;
	mipLevel1--;
	width = mD3DSystem->GetEngineOptions()->m_screenWidth >> (mipLevel0 + 1);
	height = mD3DSystem->GetEngineOptions()->m_screenHeight >> (mipLevel0 + 1);
	
	UpdateContantBuffer(mipLevel0, mipLevel1, width, height);
	Add(bright->SRView, blurredHV->SRView, summed->UAView[mipLevel0], width, height);
	BlurHorizontal(summed->SRView, blurredH->UAView[mipLevel0], width, height);
	BlurVertical(blurredH->SRView, blurredHV->UAView[mipLevel0], width, height);
	
	ID3D11UnorderedAccessView* pNULLUAV[] = {NULL, NULL, NULL};
	mD3DSystem->GetDeviceContext()->CSSetUnorderedAccessViews(0, 3, pNULLUAV, NULL);
	//ToneMapWithBloom(target->SRView, blurredHV->SRView, NULL);
	ToneMapWithBloom(mInputTextures[0], blurredHV->SRView, NULL);
	
	mD3DSystem->GetDeviceContext()->OMSetRenderTargets(1, &mOutputTargets[0], mDSView);
	
	SAFE_RELEASE(mOutputTargets[0]);
	SAFE_RELEASE(mDSView);
	
	/*int backbufferWidth = mD3DSystem->GetEngineOptions()->m_screenWidth;
	int backbufferHeight = mD3DSystem->GetEngineOptions()->m_screenHeight;
	int TotalBackbufferPixels = backbufferWidth * backbufferHeight;
	
	// Run the compute shaders for HDR
	hdrTarget = PostProcessManager::CreateRenderTarget(backbufferWidth, backbufferHeight, 0, 0, DXGI_FORMAT_R16G16B16A16_FLOAT);
	{
		AddOutputTarget(hdrTarget->RTView);
		AddOutputViewport(hdrTarget->viewport);
		
		ID3D11Buffer* cs_cbs[1] = { m_pDownscaleCB };
		mD3DSystem->GetDeviceContext()->CSSetConstantBuffers(0, 1, cs_cbs);
		
		ID3D11ShaderResourceView* cs0_srvs[1] = { mInputTextures[0] };
		mD3DSystem->GetDeviceContext()->CSSetShaderResources(0, 1, cs0_srvs);
		ID3D11UnorderedAccessView* cs0_uavs[1] = { mIntermediateLuminance->UAView };
		mD3DSystem->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, cs0_uavs, (UINT*)(&cs0_uavs[0]));
		
		PostProcessManager::mShader->SwitchTo("DownScaleFirstPass", ZShadeSandboxShader::EShaderTypes::ST_COMPUTE);
		PostProcessManager::mShader->RenderCS11(1024, 1, 1);
		
		ID3D11ShaderResourceView* cs1_srvs[1] = { mIntermediateLuminance->SRView };
		mD3DSystem->GetDeviceContext()->CSSetShaderResources(0, 1, cs1_srvs);
		ID3D11UnorderedAccessView* cs1_uavs[1] = { mAverageLuminance->UAView };
		mD3DSystem->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, cs1_uavs, (UINT*)(&cs1_uavs[0]));
		
		PostProcessManager::mShader->SwitchTo("DownScaleSecondPass", ZShadeSandboxShader::EShaderTypes::ST_COMPUTE);
		PostProcessManager::mShader->RenderCS11(64, 1, 1);
		
		// Set the final pass constant buffer for the pixel shader
		{
			cbFinalPassConstants cFPC;
			cFPC.g_MiddleGrey = fMiddleGreyValue;
			cFPC.g_LumWhiteSqr = SQR(fWhiteValue * fMiddleGreyValue);
			D3D11_MAPPED_SUBRESOURCE mapped_res;
			mD3DSystem->GetDeviceContext()->Map(m_pFinalPassCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
			{
				assert(mapped_res.pData);
				*(cbFinalPassConstants*)mapped_res.pData = cFPC;
			}
			mD3DSystem->GetDeviceContext()->Unmap(m_pFinalPassCB, 0);
		}
		ID3D11Buffer* ps_cbs[1] = { m_pFinalPassCB };
		mD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
		
		// Set the input textures for HDR and average luminance
		AddInputTexture(mAverageLuminance->SRView);
		
		// Process the HDR pixel shader
		Process("PostProcessHDR");
		
		// Cleanup
		cs0_srvs[0] = { NULL };
		mD3DSystem->GetDeviceContext()->CSSetShaderResources(0, 1, cs0_srvs);
		cs0_uavs[0] = { NULL };
		mD3DSystem->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, cs0_uavs, (UINT*)(&cs0_uavs[0]));
		
		cs1_srvs[0] = { NULL };
		mD3DSystem->GetDeviceContext()->CSSetShaderResources(0, 1, cs1_srvs);
		cs1_uavs[0] = { NULL };
		mD3DSystem->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, cs1_uavs, (UINT*)(&cs1_uavs[0]));
	}*/
}
//==================================================================================================================================
void HDR::ComputeLuminanceAndBrightPass
(	ID3D11ShaderResourceView* pInput0
,	ID3D11UnorderedAccessView* pOutput0
,	ID3D11UnorderedAccessView* pOutput1
,	ID3D11UnorderedAccessView* pOutput2
,	ID3D11UnorderedAccessView* pOutput3
,	int width
,	int height
)
{
	ID3D11DeviceContext* context = mD3DSystem->GetDeviceContext();
	ID3D11ShaderResourceView* ppSRV[] = { pInput0 };
	context->CSSetShaderResources(0, 1, ppSRV);
	ID3D11UnorderedAccessView* ppUAV[] = { pOutput0, pOutput1, pOutput2, pOutput3, luminance->UAView[0] };
	context->CSSetUnorderedAccessViews(0, 5, ppUAV, NULL);
	
	PostProcessManager::mShader->SwitchTo("ComputeLuminanceAndBright", ZShadeSandboxShader::EShaderTypes::ST_COMPUTE);
	PostProcessManager::mShader->SetComputeShader();
	PostProcessManager::mShader->RenderCS11(ThreadGroups(width / 2, LUM_AND_BRIGHT_THREADS), ThreadGroups(height / 2, LUM_AND_BRIGHT_THREADS), 1);
	
	ppSRV[0] = NULL;
	context->CSSetShaderResources(0, 1, ppSRV);
	
	PostProcessManager::mShader->SwitchTo("GetAvgLum", ZShadeSandboxShader::EShaderTypes::ST_COMPUTE);
	PostProcessManager::mShader->SetComputeShader();
	PostProcessManager::mShader->RenderCS11(1, 1, 1);
	
	ppUAV[0] = NULL;
	ppUAV[1] = NULL;
	ppUAV[2] = NULL;
	ppUAV[3] = NULL;
	ppUAV[4] = NULL;
	context->CSSetUnorderedAccessViews(0, 5, ppUAV, NULL);
}
//==================================================================================================================================
void HDR::Add
(	ID3D11ShaderResourceView* pInput0
,	ID3D11ShaderResourceView* pInput1
,	ID3D11UnorderedAccessView* pOutput
,	int width
,	int height
)
{
	ID3D11DeviceContext* context = mD3DSystem->GetDeviceContext();
	ID3D11UnorderedAccessView* ppUAV[] = { NULL, NULL, NULL };
	context->CSSetUnorderedAccessViews(0, 3, ppUAV, NULL);
	ID3D11ShaderResourceView* ppSRV[] = { pInput0, pInput1 };
	context->CSSetShaderResources(0, 2, ppSRV);
	ppUAV[0] = pOutput;
	context->CSSetUnorderedAccessViews(0, 3, ppUAV, NULL);
	
	PostProcessManager::mShader->SwitchTo("Add", ZShadeSandboxShader::EShaderTypes::ST_COMPUTE);
	PostProcessManager::mShader->SetComputeShader();
	PostProcessManager::mShader->RenderCS11(ThreadGroups(width, ADD_THREADS_X), ThreadGroups(height, ADD_THREADS_Y), 1);
}
//==================================================================================================================================
void HDR::BlurHorizontal(ID3D11ShaderResourceView* pInput0, ID3D11UnorderedAccessView* pOutput, int width, int height)
{
	ID3D11DeviceContext* context = mD3DSystem->GetDeviceContext();
	ID3D11UnorderedAccessView* ppUAV[] = { pOutput, NULL, NULL };
	context->CSSetUnorderedAccessViews(0, 3, ppUAV, NULL);
	ID3D11ShaderResourceView* ppSRV[] = { pInput0, NULL };
	context->CSSetShaderResources(0, 2, ppSRV);
	
	PostProcessManager::mShader->SwitchTo("BlurHorizontal", ZShadeSandboxShader::EShaderTypes::ST_COMPUTE);
	PostProcessManager::mShader->SetComputeShader();
	PostProcessManager::mShader->RenderCS11(ThreadGroups(width, BLURH_THREADS_X), ThreadGroups(height, BLURH_THREADS_Y), 1);
}
//==================================================================================================================================
void HDR::BlurVertical(ID3D11ShaderResourceView* pInput0, ID3D11UnorderedAccessView* pOutput, int width, int height)
{
	ID3D11DeviceContext* context = mD3DSystem->GetDeviceContext();
	ID3D11UnorderedAccessView* ppUAV[] = { pOutput, NULL, NULL };
	context->CSSetUnorderedAccessViews(0, 3, ppUAV, NULL);
	ID3D11ShaderResourceView* ppSRV[] = { pInput0, NULL };
	context->CSSetShaderResources(0, 2, ppSRV);
	
	PostProcessManager::mShader->SwitchTo("BlurVertical", ZShadeSandboxShader::EShaderTypes::ST_COMPUTE);
	PostProcessManager::mShader->SetComputeShader();
	PostProcessManager::mShader->RenderCS11(ThreadGroups(width, BLURV_THREADS_X), ThreadGroups(height, BLURV_THREADS_Y), 1);
}
//==================================================================================================================================
void HDR::ToneMapWithBloom(ID3D11ShaderResourceView* pInput, ID3D11ShaderResourceView* pBloom, ID3D11RenderTargetView* pOutput)
{
	ID3D11DeviceContext* context = mD3DSystem->GetDeviceContext();
	ID3D11ShaderResourceView* ppSRV[] = { pInput, luminance->SRView, pBloom };
	context->PSSetShaderResources(0, 3, ppSRV);
	
	PostProcessManager::mShader->SwitchTo("ToneMapWithBloom", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	
	mD3DSystem->TurnOnAdditiveBlending();
	{
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
		//UnloadResources();
		
		ppSRV[0] = NULL; ppSRV[1] = NULL;
		context->PSSetShaderResources(0, 2, ppSRV);
	}
	mD3DSystem->TurnOffAdditiveBlending();
}
//==================================================================================================================================
void HDR::UpdateContantBuffer(int mipLevel0, int mipLevel1, unsigned int width, unsigned int height)
{
	cbConstants cConst;
	cConst.width = width;
	cConst.height = height;
	cConst.mipLevel0 = mipLevel0;
	cConst.mipLevel1 = mipLevel1;
	D3D11_MAPPED_SUBRESOURCE mapped_res;
	mD3DSystem->GetDeviceContext()->Map(m_pCSConstants, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
	{
		assert(mapped_res.pData);
		*(cbConstants*)mapped_res.pData = cConst;
	}
	mD3DSystem->GetDeviceContext()->Unmap(m_pCSConstants, 0);
}
//==================================================================================================================================
void HDR::UpdateBloomConstants(float middleGrey, float bloomThreshold, float bloomMultiplier)
{
	cbBloomConstants cBC;
	cBC.g_MiddleGrey = middleGrey;
	cBC.g_BloomThreshold = bloomThreshold;
	cBC.g_BloomMultiplier = bloomMultiplier;
	cBC.padding = 0;
	D3D11_MAPPED_SUBRESOURCE mapped_res;
	mD3DSystem->GetDeviceContext()->Map(m_pBloomConstants, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
	{
		assert(mapped_res.pData);
		*(cbBloomConstants*)mapped_res.pData = cBC;
	}
	mD3DSystem->GetDeviceContext()->Unmap(m_pBloomConstants, 0);
}
//==================================================================================================================================
/*void HDR::RenderInit(ZShadeSandboxShader::Shader*& shader)
{
	if (!bPixelShaderSet)
	{
		shader->LoadComputeShader("DownScaleFirstPass");
		shader->LoadComputeShader("DownScaleSecondPass");
		shader->LoadPixelShader("PostProcessHDR");
		bPixelShaderSet = true;
	}
	
	// Run the compute shaders for HDR
	//PostProcessManager::RenderTarget* rt0 = PostProcessManager::CreateRenderTarget(mD3DSystem->GetEngineOptions()->m_screenWidth, mD3DSystem->GetEngineOptions()->m_screenHeight,
	//	0, 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 1, 1, 0, false, true);
	//rt0->Set();
	//rt0->Clear(0, 0, 0, 1);

	ID3D11Buffer* cs_cbs[1] = { m_pDownscaleCB };
	mD3DSystem->GetDeviceContext()->CSSetConstantBuffers(0, 1, cs_cbs);
	
	ID3D11ShaderResourceView* cs0_srvs[1] = { mInputTextures[0] };
	mD3DSystem->GetDeviceContext()->CSSetShaderResources(0, 1, cs0_srvs);
	ID3D11UnorderedAccessView* cs0_uavs[1] = { mAvgLuminanceUAV };
	mD3DSystem->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, cs0_uavs, (UINT*)(&cs0_uavs[0]));
	
	shader->SwitchTo("DownScaleFirstPass", ZShadeSandboxShader::EShaderTypes::ST_COMPUTE);
	shader->RenderCS11(1024, 1, 1);
	
	//cs0_srvs[1] = { NULL };
	//mD3DSystem->GetDeviceContext()->CSSetShaderResources(0, 1, cs0_srvs);
	//cs0_uavs[1] = { NULL };
	//mD3DSystem->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, cs0_uavs, (UINT*)(&cs0_uavs[0]));
	
	ID3D11ShaderResourceView* cs1_srvs[1] = { mAvgLuminanceSRV };
	mD3DSystem->GetDeviceContext()->CSSetShaderResources(0, 1, cs1_srvs);
	ID3D11UnorderedAccessView* cs1_uavs[1] = { mFinalAvgLuminanceUAV };
	mD3DSystem->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, cs1_uavs, (UINT*)(&cs1_uavs[0]));
	
	shader->SwitchTo("DownScaleSecondPass", ZShadeSandboxShader::EShaderTypes::ST_COMPUTE);
	shader->RenderCS11(64, 1, 1);
	
	//cs1_srvs[1] = { NULL };
	//mD3DSystem->GetDeviceContext()->CSSetShaderResources(0, 1, cs1_srvs);
	//cs1_uavs[1] = { NULL };
	//mD3DSystem->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, cs1_uavs, (UINT*)(&cs1_uavs[0]));
	
	// Set the final pass constant buffer for the pixel shader
	{
		cbFinalPassConstants cFPC;
		cFPC.g_MiddleGrey = fMiddleGreyValue;
		cFPC.g_LumWhiteSqr = SQR(fWhiteValue * fMiddleGreyValue);
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		mD3DSystem->GetDeviceContext()->Map(m_pFinalPassCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbFinalPassConstants*)mapped_res.pData = cFPC;
		}
		mD3DSystem->GetDeviceContext()->Unmap(m_pFinalPassCB, 0);
	}
	ID3D11Buffer* ps_cbs[1] = { m_pFinalPassCB };
	mD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
	
	// Set the result of the compute shaders (the srv) to the pixel shader
	ID3D11ShaderResourceView* ps_srvs[1] = { mFinalAvgLuminanceSRV };
	ID3D11SamplerState* ps_samp[1] = { mD3DSystem->Point() };
	
	mD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
	mD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);

	shader->SwitchTo("PostProcessHDR", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);

	//rt0->bInUse = false;
}*/
//==================================================================================================================================
//void HDR::UnloadResources()
//{
//	PostProcess::UnloadResources();
	
	//ID3D11ShaderResourceView* ps_srvs[1] = { NULL };
	//mD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
//}
//==================================================================================================================================