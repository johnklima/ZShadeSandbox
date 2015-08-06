//==============================================================================
// Debugs the GBuffer and prints each target to the screen
//
//==============================================================================
// History
//
// -Created on 6/11/2015 by Dustin Watson
//==============================================================================

cbuffer cbMatrixBuffer : register(b0)
{
	matrix invViewProj;
};

//
// Samplers and textures
//

Texture2D   g_GBufferTexture : register(t0);

// Use a point sampler to sample out exact per-pixel data from the two render textures
SamplerState pointSampler	 : register(s0);

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

PixelInput DebugGBufferDeferredVS(uint vertexID : SV_VertexID)
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

float4 DebugGBufferDeferredPS(PixelInput input) : SV_Target
{
	//
	// Unpack the GBuffer information
	//
	
	float4 colors = g_GBufferTexture.Sample(pointSampler, input.uv);
	
	// Make the color fully opaque
	colors.a = 1.0;
	
	//
	// Just return the scene color information
	//
	
	return colors;
}

float4 DebugGBufferDepthDeferredPS(PixelInput input) : SV_Target
{
	//
	// Unpack the GBuffer information
	//

	float4 colors = g_GBufferTexture.Sample(pointSampler, input.uv);

	// Compute to screen-space position
	float4 position;
	position.x = input.uv.x * 2.0f - 1.0f;
	position.y = -(input.uv.y * 2.0f - 1.0f);
	position.z = colors.x;
	position.w = 1.0;

	// Transform to world space
	position = mul(position, invViewProj);
	position /= position.w;

	colors = position;

	// Make the color fully opaque
	colors.a = 1.0;

	//
	// Just return the scene color information
	//

	return colors;
}

float4 DebugGBufferDeferredWireframePS(PixelInput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}