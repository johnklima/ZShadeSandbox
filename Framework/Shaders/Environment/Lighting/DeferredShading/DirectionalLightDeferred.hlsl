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
	float4 g_DirectionLightColor;
	float3 g_LightDirection;
	float  g_LightIntensity;
	float3 g_EyePosition;
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

PixelInput DirectionalLightDeferredVS(uint vertexID : SV_VertexID)
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

float4 DirectionalLightDeferredPS(PixelInput input) : SV_Target
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
	// Perform Blinn Phong Directional Light
	//
	
	//
	// Phong diffuse
	//
	
	float3 lightDir = g_LightDirection;
	
	float NDotL = dot(data.normal, lightDir);
	float3 lightColor = g_DirectionLightColor.rgb * saturate(NDotL);
	
	//
	// Blinn specular
	//
	
	float3 toEye = g_EyePosition.xyz - data.position;
	toEye = normalize(toEye);
	float3 halfWay = normalize(toEye + lightDir);
	float NDotH = saturate(dot(halfWay, data.normal));
	lightColor += g_DirectionLightColor.rgb * pow(NDotH, data.specularPower) * data.specularIntensity;
	
	finalColor.rgb = lightColor * data.color.rgb * g_LightIntensity;
	finalColor.a = 1.0f;
	
	return finalColor;
}

float4 DirectionalLightDeferredWireframePS(PixelInput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}