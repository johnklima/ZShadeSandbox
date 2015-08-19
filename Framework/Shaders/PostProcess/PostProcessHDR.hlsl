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

cbuffer cbConstants : register(b0)
{
	uint width;
	uint height;
	uint mipLevel0;
	uint mipLevel1;
};

cbuffer cbBloomConstants : register(b1)
{
	float g_MiddleGrey;
	float g_BloomThreshold;
	float g_BloomMultiplier;
	float padding;
};

//======================================================================================================

//
// Textures, samplers and global buffers
//

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

Texture2D<float4> 	g_Input0 : register( t0 );
Texture2D<float4> 	g_Input1 : register( t1 );
RWTexture2D<float4> g_Output0 : register( u0 );
RWTexture2D<float4> g_Output1 : register( u1 );
RWTexture2D<float4> g_Output2 : register( u2 );
RWTexture2D<float4> g_Output3 : register( u3 );
RWTexture2D<float> 	g_LuminanceTemp : register(u4);

SamplerState	sampleLinear			: register( s0 );
SamplerState	samplePoint				: register( s1 );

//======================================================================================================

//
// Global Functions
//

float4 BrightClampAndLuminance(float2 samplePoint, int2 offset)
{
	float4 sample = g_Input0.SampleLevel(sampleLinear, samplePoint, 0, offset);
	sample.a = log(dot(sample.rgb, LUM_WEIGHTS) + DELTA);
	sample.rgb = max(sample.rgb - g_BloomThreshold, (float3)0.0f);
	return sample;
}

//======================================================================================================

//
// Compute Shaders
//

groupshared float4 temp0[LUM_AND_BRIGHT_THREADS][LUM_AND_BRIGHT_THREADS];
groupshared float4 temp1[LUM_AND_BRIGHT_THREADS][LUM_AND_BRIGHT_THREADS];

[numthreads(LUM_AND_BRIGHT_THREADS, LUM_AND_BRIGHT_THREADS, 1)]
void ComputeLuminanceAndBright(uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{
	float2 samplepoint = float2((2 * DTid.xy + 0.5f) / float2(width, height));
	float4 bright = 0;
	float4 sampleVlaue = 0;
	float4 b = 0;
	int2 outPos = 2 * DTid.xy;
	int2 offset = int2(0, 0);
	b = BrightClampAndLuminance(samplepoint, offset * 2);
	g_Output0[outPos + offset] = b;
	bright += b;

	offset = int2(0, 1);
	b = BrightClampAndLuminance(samplepoint, offset * 2);
	g_Output0[outPos + offset] = b;
	bright += b;
	
	offset = int2(1, 0);
	b = BrightClampAndLuminance(samplepoint, offset * 2);
	g_Output0[outPos + offset] = b;
	bright += b;
	
	offset = int2(1, 1);
	b = BrightClampAndLuminance(samplepoint, offset * 2);
	g_Output0[outPos + offset] = b;
	bright += b;
	
	bright = bright / 4;
	temp0[GTid.x][GTid.y] = bright;
	
	g_Output1[GTid.xy + Gid.xy * LUM_AND_BRIGHT_THREADS] = bright;
	
	GroupMemoryBarrierWithGroupSync();
	
	if(GTid.x < LUM_AND_BRIGHT_THREADS/2 && GTid.y < LUM_AND_BRIGHT_THREADS / 2)
	{
		float4 nextLevel;
		nextLevel = temp0[GTid.x * 2][GTid.y * 2];
		nextLevel += temp0[GTid.x * 2 + 1][GTid.y * 2];
		nextLevel += temp0[GTid.x * 2][GTid.y * 2 + 1];
		nextLevel += temp0[GTid.x * 2 + 1][GTid.y * 2 + 1];
		nextLevel = nextLevel / 4;
		temp1[GTid.x][GTid.y] = nextLevel;
		g_Output2[Gid.xy * LUM_AND_BRIGHT_THREADS / 2 + GTid.xy] = nextLevel;
	}
	
	GroupMemoryBarrierWithGroupSync();
	
	if(GTid.x < LUM_AND_BRIGHT_THREADS/4 && GTid.y < LUM_AND_BRIGHT_THREADS / 4)
	{
		float4 nextLevel;
		nextLevel =  temp1[GTid.x * 2][GTid.y * 2];
		nextLevel += temp1[GTid.x * 2 + 1][GTid.y * 2];
		nextLevel += temp1[GTid.x * 2][GTid.y * 2 + 1];
		nextLevel += temp1[GTid.x * 2 + 1][GTid.y * 2 + 1];
		nextLevel = nextLevel / 4;
		g_Output3[Gid.xy * LUM_AND_BRIGHT_THREADS/4 + GTid.xy] = nextLevel;
		temp0[GTid.x][GTid.y] = nextLevel;
	}
	
	GroupMemoryBarrierWithGroupSync();
	
	if(GTid.x == 0 && GTid.y == 0)
	{
		float4 nextLevel;
		nextLevel =  temp0[0][0];
		nextLevel += temp0[1][0];
		nextLevel += temp0[0][1];
		nextLevel += temp0[1][1];
		nextLevel = nextLevel / 4;
		g_LuminanceTemp[Gid.xy] = nextLevel.a;
	}
}


groupshared float avgTemp[GET_AVG_LUM_THREADS];

[numthreads(GET_AVG_LUM_THREADS, 1, 1)]
void GetAvgLum(uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{
	float totalLuminance = 0.0f;
	
	for(uint i = 0; i < width / (LUM_AND_BRIGHT_THREADS * GET_AVG_LUM_THREADS); i++)
	{
		for(uint j = 0; j < height / 16u; j++)
		{
			totalLuminance += g_LuminanceTemp[int2(DTid.x + GET_AVG_LUM_THREADS * i, j)];
		}
	}
	
	avgTemp[DTid.x] = totalLuminance;
	
	GroupMemoryBarrierWithGroupSync();
	
	if (DTid.x == 0)
	{
		for(uint i = 1; i < GET_AVG_LUM_THREADS; i++)
		{
			totalLuminance += avgTemp[i];
		}
		
		float luminance = totalLuminance / ((width / GET_AVG_LUM_THREADS) * (height / GET_AVG_LUM_THREADS));
		
		g_LuminanceTemp[int2(0, 0)] = g_MiddleGrey / (exp(luminance) - DELTA);
	}
}


[numthreads(ADD_THREADS_X, ADD_THREADS_Y, 1)]
void Add( uint3 DTid : SV_DispatchThreadID)
{
	g_Output0[DTid.xy] = 0.5f * (g_Input0.Load(float3(DTid.xy, mipLevel0)) + g_Input1.SampleLevel(sampleLinear, (DTid.xy + 0.5f) / float2(width, height), mipLevel1));
}	

#define BLUR_WIDTH 9
#define HALF_BLUR_WIDTH 4


[numthreads(1, BLURH_THREADS_Y, 1)]
void BlurHorizontal(uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{
	static const float g_BlurWeights[] = {
		0.004815026f,
		0.028716039f,
		0.102818575f,
		0.221024189f,
		0.28525234f,
		0.221024189f,
		0.102818575f,
		0.028716039f,
		0.004815026f
	};
	
	int3 base = int3(DTid.x * BLURH_THREADS_X, DTid.y, mipLevel0);
	float4 input[BLUR_WIDTH + BLURH_THREADS_X];
	
	[unroll]
	for(int i = 0; i < 9 + BLURH_THREADS_X; i++)
	{
		input[i] = g_Input0.Load(base, int2(i - HALF_BLUR_WIDTH, 0));
	}
	
	[unroll]
	for(int x = 0; x < BLURH_THREADS_X; x++)
	{
		float4 output = 0.0f;
		
		[unroll]
		for(int i = 0; i < BLUR_WIDTH; i++)
		{
			output += input[(x + i)] * g_BlurWeights[i];
		}
		
		g_Output0[base.xy + int2(x, 0)] = output;
	} 
}


[numthreads(BLURV_THREADS_X, 1, 1)]
void BlurVertical(uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{
	static const float g_BlurWeights[] = {
		0.004815026f,
		0.028716039f,
		0.102818575f,
		0.221024189f,
		0.28525234f,
		0.221024189f,
		0.102818575f,
		0.028716039f,
		0.004815026f
	};
	
	float4 input[BLUR_WIDTH + BLURV_THREADS_Y];
	int3 base = int3(DTid.x, DTid.y * BLURV_THREADS_Y, mipLevel0);
	
	[unroll]
	for(int i = 0; i < BLUR_WIDTH + BLURV_THREADS_Y; i++)
	{
		input[i] = g_Input0.Load(base, int2(0, i - HALF_BLUR_WIDTH));
	}
	
	[unroll]
	for(int y = 0; y < BLURV_THREADS_Y; y++)
	{
		float4 output = 0.0f;
		
		[unroll]
		for(int i = 0; i < BLUR_WIDTH; i++)
		{
			output += input[y + i] * g_BlurWeights[i];
		}
		
		g_Output0[base.xy + int2(0, y)] = output;
	} 
}

//======================================================================================================

//
// Pixel Shader
//

#ifndef BLOOM
#define BLOOM true
#endif 

Texture2D g_Input		 : register( t0 );
Texture2D g_ToneMapScale : register( t1 );
Texture2D g_Bloom		 : register( t2 );

struct PostProcessPixelInput
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD0;
};

float4 ToneMapWithBloom(PostProcessPixelInput input) : SV_Target
{
	int3 index = int3(input.position.xy, 0);
	float3 Lw = g_Input.Load(index).xyz;
	
	//if (BLOOM)
	{
		Lw += g_BloomMultiplier * g_Bloom.SampleLevel(sampleLinear, input.uv, 0).rgb;
	}
	
	float scale = g_ToneMapScale.Load(int3(0, 0, 0)).r;
	float3 L = scale * Lw;
	float3 Ld = (L / (1 + L));
	return float4(Ld, 1.0f);
}
