//==============================================================================
// Represents the lake bed or ocean bed of a floor beneath water
// with caustics on the surface
//
//==============================================================================
// History
//
// -Created on 5/29/2015 by Dustin Watson
//==============================================================================
#include "../Water/RefractionCaustics.hlsl"
#include "../Environment/AtmosphericFog.hlsl"

//
// Constants
//

cbuffer cbFrameBuffer : register(b0)
{
	float4 g_ClipPlane;
	float3 g_EyePos;
	float  g_CausticTimer;
	float3 g_LightDirection;
	float  g_dummy2;
};

cbuffer cbMatrixBuffer : register(b1)
{
	matrix g_World;
	matrix g_View;
	matrix g_Proj;
	matrix g_LightViewProj;
	matrix g_Shadow;
};

//======================================================================================================

//
// Samplers and textures
//

Texture2D g_DiffuseTexture  : register(t0);
Texture2D g_WaveMap0 	  	: register(t1);
Texture2D g_WaveMap1 	  	: register(t2);
Texture2D g_MicroBump0 	  	: register(t3);
Texture2D g_MicroBump1 	  	: register(t4);
Texture2D g_DepthTexture	: register(t5);
Texture2D g_CausticTexture	: register(t6);

SamplerState linearSampler	: register(s0);
SamplerComparisonState depthSampler	: register(s1);

//======================================================================================================

//
// Structs
//

struct VertexInput
{
	float3 position  : POSITION;
	float2 uv        : TEXCOORD0;
};

struct PixelInput
{
	float4 position  : SV_POSITION;
	float pixelDepth : DEPTH;
	float3 positionV : TEXCOORD0;
	float2 uv        : TEXCOORD1;
	float clip		 : TEXCOORD2;
	float4 lightPos	 : TEXCOORD3;
};

//======================================================================================================

//
// Vertex Shader
//

PixelInput LakeBedVS(VertexInput input)
{
	PixelInput output;
	
	output.position = mul(float4(input.position, 1.0f), g_World);
    output.position = mul(output.position, g_View);
    output.position = mul(output.position, g_Proj);
	
    output.uv = input.uv;
	
	output.positionV = input.position;
	
	// Calculate the position for shadow mapping
	output.lightPos = mul(float4(input.position, 1.0f), g_Shadow);
	
	output.clip = dot(output.position, g_ClipPlane);
	
	//g_FarPlane
	// Logarithmic z buffer
	// DirectX Depth range 0..1
	int C = 1;
	output.pixelDepth = log2(max(1e-6, C * output.position.z + 1)) * 2 / log2(C * 200000 + 1) * output.position.z;
	
	return output;
}

//======================================================================================================

//
// Pixel Shader
//

float4 LakeBedPS(PixelInput input) : SV_Target
{
	float4 textureColor;
	
	//Sample the pixel color from the texture using the sampler at this texture
    //coordinate location
    textureColor = g_DiffuseTexture.Sample(linearSampler, input.uv);
    
	// adding per-vertex lighting defined by displacement of vertex 
	textureColor *= 0.5 + 0.5 * min(1.0, max(0.0, 1.0f / 3.0f + 0.5f));
	
	// Calculate a surface refraction color
	float rc = RefractionCaustics
	(	g_EyePos
	,	input.positionV
	,	g_LightDirection
	,	g_WaveMap0
	,	g_WaveMap1
	,	linearSampler
	,	1.0f
	,	g_MicroBump0
	,	g_MicroBump1
	,	input.uv
	,	float3(0, 1, 0)
	,	g_CausticTexture
	,	depthSampler
	,	g_LightViewProj
	);
	
	//textureColor += rc;
	
	// Sample the caustic color
	//textureColor += g_CausticTexture.Sample(linearSampler, input.uv);
	//textureColor.rgb += CausticDance(input.position, g_CausticTexture, linearSampler, g_CausticTimer);
	//textureColor = lerp(textureColor, float4(1, 1, 1, 1), 0.2);
	
	float cc = SampleCaustic
	(	g_WaveMap0
	,	g_WaveMap1
	,	g_MicroBump0
	,	g_MicroBump1
	,	g_CausticTexture
	,	linearSampler
	,	depthSampler
	,	input.uv
	,	g_EyePos
	,	input.positionV
	,	input.lightPos
	,	g_LightDirection
	);
	
	textureColor += cc;
	textureColor += g_CausticTexture.Sample(linearSampler, input.uv);
	textureColor = lerp(textureColor, float4(1, 1, 1, 1), 0.2);
	
	// Add an atmospheric fog color
	float3 fogColor = AtmosphericFogColor(input.positionV, g_LightDirection, g_EyePos, float3(0.2, 0.2, 0.7), float3(1.0, 1.1, 1.4));
	float camera_distance = length(g_EyePos - input.positionV);
	float fogDensity = 1.0f / 700.0f;
	//textureColor.rgb = lerp(fogColor, textureColor.rgb, min(1, exp(-camera_distance * fogDensity)));
	//textureColor.a = length(g_EyePos - input.positionV);
	
	clip( input.clip < 0.0 ? -1 : 1 );
	
	return textureColor;
}

float4 LakeBedWireframePS(PixelInput input) : SV_Target
{
	clip( input.clip < 0.0 ? -1 : 1 );
	
	return float4(0.9f, 0.9f, 0.9f, 1);
}