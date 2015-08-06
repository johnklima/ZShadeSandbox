#include "HDR.h"
#include "ConstantBuffer.h"
#include "StructuredBuffer.h"
#include "PostProcessManager.h"
#include "StructuredBuffer.h"
//==================================================================================================================================
//==================================================================================================================================
HDR::HDR(D3D* d3d)
:   PostProcess(d3d)
{
	mType = EPostProcessTypes::Type::eHDR;
	
	int backbufferWidth = mD3DSystem->GetEngineOptions()->m_screenWidth;
	int backbufferHeight = mD3DSystem->GetEngineOptions()->m_screenHeight;
	int TotalBackbufferPixels = backbufferWidth * backbufferHeight;
	
	cbDownscaleBuffer cdb;
	cdb.g_Res.x = backbufferWidth / 4;
	cdb.g_Res.y = backbufferHeight / 4;
	cdb.g_Domain = TotalBackbufferPixels / 16;
	cdb.g_GroupSize = TotalBackbufferPixels / (16 * 1024);
	
	ConstantBuffer<cbDownscaleBuffer> downscaleCB(mD3DSystem);
	downscaleCB.Initialize(PAD16(sizeof(cbDownscaleBuffer)), &cdb, true);
	m_pDownscaleCB = downscaleCB.Buffer();
	
	ConstantBuffer<cbFinalPassConstants> finalPassCB(mD3DSystem);
	finalPassCB.Initialize(PAD16(sizeof(cbFinalPassConstants)));
	m_pFinalPassCB = finalPassCB.Buffer();
	
	mIntermediateLuminance = new StructuredBuffer(mD3DSystem);
	mAverageLuminance = new StructuredBuffer(mD3DSystem);

	mIntermediateLuminance->Initialize(4 * sizeof(float), 4, 4 * TotalBackbufferPixels / (16 * 1024), 0, true, TotalBackbufferPixels / (16 * 1024));
	mAverageLuminance->Initialize(4 * sizeof(float), 4, 4, 0, true, 1);
	
	fMiddleGreyValue = 1.5f;
	fWhiteValue = 1.0f;
	
	PostProcessManager::mShader->LoadComputeShader("DownScaleFirstPass");
	PostProcessManager::mShader->LoadComputeShader("DownScaleSecondPass");
	PostProcessManager::mShader->LoadPixelShader("PostProcessHDR");
}
//==================================================================================================================================
HDR::~HDR()
{
}
//==================================================================================================================================
void HDR::ProcessInternal()
{
	int backbufferWidth = mD3DSystem->GetEngineOptions()->m_screenWidth;
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
	}
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
void HDR::UnloadResources()
{
	PostProcess::UnloadResources();
	
	//ID3D11ShaderResourceView* ps_srvs[1] = { NULL };
	//mD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
}
//==================================================================================================================================