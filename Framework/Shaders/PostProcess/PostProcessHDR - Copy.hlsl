//==============================================================================
// Post Processing HDR Shader
//
//==============================================================================
// History
//
// -Created on 6/3/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

// g_Res : backbuffer width and height divided by 4
// g_Domain : backbuffer width times height divided by 16
// g_GroupSize : backbuffer width times height divided by 16 times 1024
cbuffer cbDownscaleBuffer : register(b0)
{
	// Resolution of the down scaled target
	uint2 g_Res			: packoffset(c0);
	
	// Total pixel in the downscaled image
	uint g_Domain		: packoffset(c0.z);
	
	// Number of groups dispached on the first pass
	uint g_GroupSize	: packoffset(c0.w);
};

cbuffer cbFinalPassConstants : register(b1)
{
	float g_MiddleGrey		: packoffset(c0);
	
	// Multiply the white value with the middle grey value and square the result
	float g_LumWhiteSqr		: packoffset(c0.y);
};

//======================================================================================================

//
// Textures, samplers and global buffers
//

Texture2D g_HDRTexture							: register(t0);
RWStructuredBuffer<float> g_AverageLuminance	: register(u0);

Texture2D<float4> g_FinalHDRTexture				: register(t1);
StructuredBuffer<float> g_FinalAvgLuminance		: register(t2);

SamplerState g_PointSampler						: register(s0);

groupshared float SharedPositions[1024];

#define MAX_GROUPS	64

groupshared float SharedAvgFinal[MAX_GROUPS];

static const float3 LUMINACNE_FACTOR = float3(0.299, 0.587, 0.114);

//======================================================================================================

//
// Global Functions
//

float3 HDRToneMapping(float3 hdrColor)
{
	// Find the luminance scale for the pixel
	float lScale = dot(hdrColor, LUMINACNE_FACTOR);
	
	lScale *= g_MiddleGrey / g_FinalAvgLuminance[0];
	
	lScale = (lScale + lScale * lScale / g_LumWhiteSqr) / (1.0 + lScale);
	
	return hdrColor * lScale;
}

//======================================================================================================

//
// Structs
//

struct PostProcessPixelInput
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD0;
};

//======================================================================================================

//
// Compute Shader
//

//======================================================================================================

//
// 1st HDR Pass
//

float DownScale4x4(uint2 CurPixel, uint groupThreadId)
{
	float avgLum = 0.0;
	
	// Skip any pixels that are out of bounds
	if (CurPixel.y < g_Res.y)
	{
		// Sum a group of 4x4 pixels
		int3 nFullResPos = int3(CurPixel * 4, 0);
		
		float4 downScaled = float4(0.0, 0.0, 0.0, 0.0);
		
		[unroll]
		for (int i = 0; i < 4; i++)
		{
			[unroll]
			for (int j = 0; j < 4; j++)
			{
				downScaled += g_HDRTexture.Load(nFullResPos, int2(j, i));
			}
		}
		
		downScaled /= 16.0;
		
		// Calculate the luminance value for this pixel
		avgLum = dot(downScaled, LUMINACNE_FACTOR);
		
		// Write the result to the shared memory
		SharedPositions[groupThreadId] = avgLum;
	}
	
	// Synchronize before next step
	GroupMemoryBarrierWithGroupSync();
	
	return avgLum;
}

float DownScale1024to4(uint dispatchThreadId, uint groupThreadId, float avgLum)
{
	[unroll]
	for (uint groupSize = 4, step1 = 1, step2 = 2, step3 = 3;
		 groupSize < 1024;
		 groupSize *= 4, step1 *= 4, step2 *= 4, step3 *= 4)
	{
		// Skip any pixels that are out of bounds
		if (groupThreadId % groupSize == 0)
		{
			// Calculate the luminance sum for this step
			float stepAvgLum = avgLum;
			stepAvgLum += (dispatchThreadId + step1) < g_Domain ? SharedPositions[groupThreadId + step1] : avgLum;
			stepAvgLum += (dispatchThreadId + step2) < g_Domain ? SharedPositions[groupThreadId + step2] : avgLum;
			stepAvgLum += (dispatchThreadId + step3) < g_Domain ? SharedPositions[groupThreadId + step3] : avgLum;
			
			// Store the results
			avgLum = stepAvgLum;
			
			// Write the result to the shared memory
			SharedPositions[groupThreadId] = avgLum;
		}
		
		// Synchronize before next step
		GroupMemoryBarrierWithGroupSync();
	}
	
	return avgLum;
}

void DownScale4to1(uint dispatchThreadId, uint groupThreadId, uint groupId, float avgLum)
{
	if (groupThreadId == 0)
	{
		// Calculate the average luminance for this thread group
		float fFinalAvgLuminance = avgLum;
		fFinalAvgLuminance += (dispatchThreadId + 256) < g_Domain ? SharedPositions[groupThreadId + 256] : avgLum;
		fFinalAvgLuminance += (dispatchThreadId + 512) < g_Domain ? SharedPositions[groupThreadId + 512] : avgLum;
		fFinalAvgLuminance += (dispatchThreadId + 768) < g_Domain ? SharedPositions[groupThreadId + 768] : avgLum;
		fFinalAvgLuminance /= 1024.0;
		
		// Write the final value into a 1D UAV
		g_AverageLuminance[groupId] = fFinalAvgLuminance;
	}
	
	
}

[numthreads(1024, 1, 1)]
void DownScaleFirstPass(uint3 groupId : SV_GroupID, uint3 dispatchThreadId : SV_DispatchThreadID, uint3 groupThreadId : SV_GroupThreadID)
{
	uint2 CurPixel = uint2(dispatchThreadId.x % g_Res.x, dispatchThreadId.x / g_Res.x);
	
	// Reduce a group of 16 pixels to a single pixel and store in the shared memory
	float avgLum = DownScale4x4(CurPixel, groupThreadId.x);
	
	// Downscale from 1024 to 4
	avgLum = DownScale1024to4(dispatchThreadId.x, groupThreadId.x, avgLum);
	
	// Downscale from 4 to 1
	DownScale4to1(dispatchThreadId.x, groupThreadId.x, groupId.x, avgLum);
}

//======================================================================================================

//
// 2nd HDR Pass
//

[numthreads(MAX_GROUPS, 1, 1)]
void DownScaleSecondPass(uint3 groupId : SV_GroupID, uint3 dispatchThreadId : SV_DispatchThreadID, uint3 groupThreadId : SV_GroupThreadID)
{
	float avgLum = 0.0;
	
	// Fill the shared memory with the 1D values
	if (dispatchThreadId.x < g_GroupSize)
	{
		avgLum = g_AverageLuminance[dispatchThreadId.x];
	}
	
	SharedAvgFinal[dispatchThreadId.x] = avgLum;
	
	// Downscale from 64 to 16
	if (dispatchThreadId.x % 4 == 0)
	{
		// Calculate the luminance sum for this step
		float stepAvgLum = avgLum;
		stepAvgLum += (dispatchThreadId.x + 1) < g_GroupSize ? SharedAvgFinal[groupThreadId.x + 1] : avgLum;
		stepAvgLum += (dispatchThreadId.x + 2) < g_GroupSize ? SharedAvgFinal[groupThreadId.x + 2] : avgLum;
		stepAvgLum += (dispatchThreadId.x + 3) < g_GroupSize ? SharedAvgFinal[groupThreadId.x + 3] : avgLum;
		
		// Store the results
		avgLum = stepAvgLum;
		
		// Write the result to the shared memory
		SharedAvgFinal[groupThreadId.x] = stepAvgLum;
	}
	
	// Synchronize before next step
	GroupMemoryBarrierWithGroupSync();
	
	// Downscale from 16 to 4
	if (dispatchThreadId.x % 16 == 0)
	{
		// Calculate the luminance sum for this step
		float stepAvgLum = avgLum;
		stepAvgLum += (dispatchThreadId.x + 4) < g_GroupSize ? SharedAvgFinal[groupThreadId.x + 4] : avgLum;
		stepAvgLum += (dispatchThreadId.x + 8) < g_GroupSize ? SharedAvgFinal[groupThreadId.x + 8] : avgLum;
		stepAvgLum += (dispatchThreadId.x + 12) < g_GroupSize ? SharedAvgFinal[groupThreadId.x + 12] : avgLum;
		
		// Store the results
		avgLum = stepAvgLum;
		
		// Write the result to the shared memory
		SharedAvgFinal[groupThreadId.x] = stepAvgLum;
	}
	
	// Synchronize before next step
	GroupMemoryBarrierWithGroupSync();
	
	// Downscale from 4 to 1
	if (dispatchThreadId.x == 0)
	{
		// Calculate the average luminance
		float fFinalAvgLuminance = avgLum;
		fFinalAvgLuminance += (dispatchThreadId.x + 16) < g_GroupSize ? SharedAvgFinal[groupThreadId.x + 16] : avgLum;
		fFinalAvgLuminance += (dispatchThreadId.x + 32) < g_GroupSize ? SharedAvgFinal[groupThreadId.x + 32] : avgLum;
		fFinalAvgLuminance += (dispatchThreadId.x + 48) < g_GroupSize ? SharedAvgFinal[groupThreadId.x + 48] : avgLum;
		
		fFinalAvgLuminance /= 64.0;
		
		// Write the final value into a 1D UAV
		g_AverageLuminance[groupId.x] = fFinalAvgLuminance;
	}
}

//======================================================================================================

//
// Pixel Shader
//

float4 PostProcessHDR(PostProcessPixelInput input) : SV_Target
{
	// Get the hdr color sample
	float3 color = g_FinalHDRTexture.Sample(g_PointSampler, input.uv.xy).xyz;
	
	// Apply tone mapping and output the LDR value
	color = HDRToneMapping(color);
	
	return float4(color, 1.0);
}
