//==============================================================================
// Capsule Light Shader for Deferred Shading
//
//==============================================================================
// History
//
// -Created on 6/7/2015 by Dustin Watson
//==============================================================================
#include "../GBuffer/UnpackGBuffer.hlsl"

//
// Constants
//

cbuffer cbLightBuffer : register(b0)
{
	float3 	g_CapsuleLightPosition;
	float 	g_CapsuleLightRange;
	float3 	g_CapsuleLightDirection;
	float 	g_CapsuleLightLength;
	float3 	g_CapsuleDirectionLength;
	float 	g_CapsuleIntensity;
	float4	g_CapsuleDiffuseColor;
	float3	g_EyePosition;
	float	padding2;
	matrix	g_InvViewProj;
};

//======================================================================================================

//
// Samplers and textures
//

Texture2D g_ColorTexture	: register(t0);
Texture2D g_NormalTexture	: register(t1);
Texture2D g_DepthTexture	: register(t2);

// Use a point sampler to sample out exact per-pixel data from the two render textures
SamplerState pointSampler	: register(s0);

//======================================================================================================

//
// Structs
//

struct PixelInput
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD0;
};

//======================================================================================================

//
// Vertex Shader
//

PixelInput CapsuleLightDeferredVS(uint vertexID : SV_VertexID)
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

float4 CapsuleLightDeferredPS(PixelInput input) : SV_Target
{
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
	// Perform Blinn Phong Capsule Light
	//
	
	float3 toEye = g_EyePosition.xyz - data.position;
	
	//
	// Find the shortest distance between the pixel and capsules segment
	//
	
	float3 toCapsuleStart = data.position - g_CapsuleLightPosition;
	float distOnLine = dot(toCapsuleStart, g_CapsuleDirectionLength.xyz) / g_CapsuleLightRange;
	distOnLine = saturate(distOnLine) * g_CapsuleLightRange;
	float3 pointOnLine = g_CapsuleLightPosition + g_CapsuleLightDirection * distOnLine;
	float3 toLight = pointOnLine - data.position;
	float distToLight = length(toLight);
	
	//
	// Phong Diffuse
	//
	
	toLight /= distToLight;
	float NDotL = saturate(dot(toLight, data.normal));
	float3 finalLightColor = data.color.rgb * NDotL;
	
	//
	// Blinn Specular
	//
	
	toEye = normalize(toEye);
	float3 halfWay = normalize(toEye + toLight);
	float NDotH = saturate(dot(halfWay, data.normal));
	finalLightColor += pow(NDotH, data.specularPower) * data.specularIntensity;
	
	//
	// Attenuation
	//
	
	float distToLightNormal = 1.0 - saturate(distToLight * (1 / g_CapsuleLightRange));
	float attenuation = distToLightNormal * distToLightNormal;
	finalLightColor *= g_CapsuleDiffuseColor.rgb * g_CapsuleIntensity * attenuation;
	
	return float4(finalLightColor, 1.0);
}

float4 CapsuleLightDeferredWireframePS(PixelInput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}