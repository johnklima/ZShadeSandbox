//==================================================================================================================================
// HDR.h
//
//==================================================================================================================================
// History
//
// -Created on 4/20/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __HDR_H
#define __HDR_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "CGlobal.h"
#include "PostProcess.h"
#include "PostProcessManager.h"

class StructuredBuffer;

//==================================================================================================================================
class HDR : public PostProcess
{
public:
	
	HDR(D3D* d3d);
	~HDR();
	
	void SetMiddleGreyValue(float value) 	{ fMiddleGreyValue = value; }
	void SetWhiteValue(float value) 		{ fWhiteValue = value; }
	
	void ProcessInternal();
	
	void UnloadResources();
	
private:
	
	float fMiddleGreyValue;
	float fWhiteValue;
	
	struct cbDownscaleBuffer
	{
		uint2	g_Res;
		uint32	g_Domain;
		uint32	g_GroupSize;
	};
	
	struct cbFinalPassConstants
	{
		float	g_MiddleGrey;
		float	g_LumWhiteSqr;
	};
	
	ID3D11Buffer* m_pDownscaleCB;
	ID3D11Buffer* m_pFinalPassCB;
	
	StructuredBuffer* mIntermediateLuminance;
	StructuredBuffer* mAverageLuminance;
	
	PostProcessManager::RenderTarget* hdrTarget;
	
	// Has InputSRV to HDR also
	//ID3D11ShaderResourceView* mHDROutputSRV;
	
	//ID3D11ShaderResourceView* mAvgLuminanceSRV;
	//ID3D11UnorderedAccessView* mAvgLuminanceUAV;
	
	//ID3D11ShaderResourceView* mFinalAvgLuminanceSRV;
	//ID3D11UnorderedAccessView* mFinalAvgLuminanceUAV;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__HDR_H