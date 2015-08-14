//==================================================================================================================================
// HDR.h
//
// Based on the ComputeShaderHDR Sample 
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

struct HDRShaderParams
{
	int reduceGroupSize;
	int reduceTileSize;
	int fullscreenTileSize;
	int fullscreenGroupSize;
};

struct cbConstants
{
	int width;
	int height;
	int mipLevel0;
	int mipLevel1;
};

struct cbBloomConstants
{
	float g_MiddleGrey;
	float g_BloomThreshold;
	float g_BloomMultiplier;
	float padding;
};

#define BLUR_LEVELS 4

//LUM_WEIGHTS is used to calculate the intensity of a color value
// (note green contributes more than red contributes more than blue)
#define LUM_WEIGHTS float3(0.27f, 0.67f, 0.06f)
//MIDDLE_GREY is used to scale the color values for tone mapping
#define MIDDLE_GREY 0.08f
//BLOOM_THRESHOLD is the lower limit for intensity values that will contribute to bloom
#define BLOOM_THRESHOLD 0.5f
//BLOOM_MULTIPLIER scales the bloom value prior to adding the bloom value to the material color
#define BLOOM_MULTIPLIER	0.6f
#define GAMMA_POW 2.2
#define DELTA 0.00000001f
#define BLURH_THREADS_X 4
#define BLURH_THREADS_Y 64
#define BLURV_THREADS_X 64
#define	BLURV_THREADS_Y 4
#define ADD_THREADS_X 64
#define ADD_THREADS_Y 2
#define LUM_AND_BRIGHT_THREADS 8u
#define REDUCE_GROUP_SIZE 64
#define GET_AVG_LUM_THREADS 16u
#define REDUCE_Y_PER_THREAD 8

//==================================================================================================================================
class HDR : public PostProcess
{
public:
	
	HDR(D3D* d3d);
	~HDR();
	
	void ProcessInternal();
	
private:
	
	// This method computes the average log luminance of the scene and performs the bright pass plus downscaling of the bright pass output.
	// the implementation combines these steps so that the back buffer is only read once
	void ComputeLuminanceAndBrightPass(
		ID3D11ShaderResourceView* pInput0,
		ID3D11UnorderedAccessView* pOutput0,
		ID3D11UnorderedAccessView* pOutput1,
		ID3D11UnorderedAccessView* pOutput2,
		ID3D11UnorderedAccessView* pOutput3,
		int width, int height
	);
	
	//
	//The following methods operate on textures with mipmaps. Because the methods are called together to operate on the same mip levels, 
	//the mip levels are assigned to the shaders through a constant buffer outside these methods in order to avoid redundant updates to the
	//constant buffer resource.
	//
	
	// Adds input0 and input1 and stores the result. the width and height are the width and height of the input0 and output buffers.
	// Input1 buffer is accessed through the texture sampler.
	void Add(
		ID3D11ShaderResourceView* pInput0,
		ID3D11ShaderResourceView* pInput1,
		ID3D11UnorderedAccessView* pOutput,
		int width, int height
	);
	
	// Performs a Gaussian blur in the horizontal direction.
	void BlurHorizontal(ID3D11ShaderResourceView* pInput0, ID3D11UnorderedAccessView* pOutput, int width, int height);

	// Performs a Gaussian blur in the vertical direction.
	void BlurVertical(ID3D11ShaderResourceView* pInput0, ID3D11UnorderedAccessView* pOutput, int width, int height); 
	
	// Adds the bloom value to the original render output and then performs tone mapping.
	// This will be done in PostProcess.cpp in the Process function to process the input and output
	void ToneMapWithBloom(ID3D11ShaderResourceView* pInput, ID3D11ShaderResourceView* pBloom, ID3D11RenderTargetView* pOutput);
	
	// Update the constant buffer used by the compute shaders that define the width and height of the
	// 	output buffers and the mip levels that should be accessed for the add and blur operations.
	void UpdateContantBuffer(int mipLevel0, int mipLevel1, unsigned int width, unsigned int height);

	// Updates the constant buffer used by the shaders that specify the HDR (middle grey) and bloom (threshold and multiplier) parameters.
	void UpdateBloomConstants(float middleGrey, float bloomThreshold, float bloomMultiplier);
	
private:
	
	float fMiddleGreyValue;
	float fBloomThreshold;
	float fBloomMultiplier;
	
	// floating point resource used for temporary render target
	//PostProcessManager::RenderTarget* hdrTarget;
	
	// downscaled luminance values for the scene.
	PostProcessManager::RenderTarget* luminance;
	
	// average log luminance (this could also be a constant buffer)
	//PostProcessManager::RenderTarget* avgLuminance;
	
	// bright pass resource
	PostProcessManager::RenderTarget* bright;
	
	// temporary buffer to store sum of a blurred level and the next bright output
	PostProcessManager::RenderTarget* summed;
	
	// temporary buffer that has been blurred horizontally
	PostProcessManager::RenderTarget* blurredH;
	
	// temporary buffer that has been blurred horizontally and vertically
	PostProcessManager::RenderTarget* blurredHV;
	
	// HDR and bloom constant buffer for shaders
	ID3D11Buffer* m_pBloomConstants;
	
	// Buffer constants (dimensions and miplevels) for compute shaders
	ID3D11Buffer* m_pCSConstants;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__HDR_H