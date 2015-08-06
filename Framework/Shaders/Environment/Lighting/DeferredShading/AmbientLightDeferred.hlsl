//==============================================================================
// Directional Light Shader for Deferred Shading
//
//==============================================================================
// History
//
// -Created on 6/2/2015 by Dustin Watson
//==============================================================================
#include "../GBuffer/UnpackGBuffer.hlsl"

//
// Constants
//

cbuffer cbLightBuffer : register(b0)
{
	float4 g_AmbientLightColor;
	float3 g_AmbientDown;
	float  padding1;
	float3 g_AmbientUp;
	float  padding2;
	matrix g_InvViewProj;
};

//======================================================================================================

//
// Samplers and textures
//

Texture2D   g_ColorTexture	: register(t0);
Texture2D   g_NormalTexture	: register(t1);
Texture2D   g_DepthTexture	: register(t2);

// Use a point sampler to sample out exact per-pixel data from the two render textures
SamplerState pointSampler	: register(s0);
SamplerState linearSampler	: register(s1);

//======================================================================================================

//
// Structs
//

struct PixelInput
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD0;
};

//======================================================================================================

//
// Vertex Shader
//

PixelInput AmbientLightDeferredVS(uint vertexID : SV_VertexID)
{
	PixelInput output;
	
	output.uv = float2((vertexID << 1) & 2, vertexID & 2);
	output.position = float4(output.uv * float2(2, -2) + float2(-1, 1), 0, 1);
	
	return output;
}

//======================================================================================================

//
// Pixel Shader
//

float4 AmbientLightDeferredPS(PixelInput input) : SV_Target
{
	float4 finalColor;
	
	//
	// Unpack the GBuffer
	//
	
	GBufferData data = UnpackGBuffer
	(	input.uv
	,	g_InvViewProj
	,	g_ColorTexture
	,	g_NormalTexture
	,	g_DepthTexture
	,	pointSampler
	);
	
	//
	// Perform Ambient Lighting
	//
	
	float4 linearDiffuseColor = float4(data.color.rgb * data.color.rgb, data.color.a);
	
	// Convert from [-1, 1] to [0, 1]
	float up = data.normal.y * 0.5 + 0.5;
	
	// Calculate the ambient value
	float3 ambient = g_AmbientDown + up * g_AmbientUp;
	
	// Apply the ambient color to the diffuse color
	finalColor.rgb = ambient * linearDiffuseColor.rgb * g_AmbientLightColor.rgb;
	finalColor.a = 1.0f;
	
	return finalColor;
}

float4 AmbientLightDeferredWireframePS(PixelInput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}