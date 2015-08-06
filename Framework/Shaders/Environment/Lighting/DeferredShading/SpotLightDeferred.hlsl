//==============================================================================
// Spot Light Shader for Deferred Shading
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
	float4	g_SpotLightColor;
	float3	g_SpotLightPosition;
	float	padding;
	float	g_SpotLightRange;
	float	g_SpotCosOuterCone;
	float	g_SpotInnerConeReciprocal;
	float	g_SpotCosineAngle;
	float3 	g_EyePosition;
	float	g_SpotLightIntensity;
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

PixelInput SpotLightDeferredVS(uint vertexID : SV_VertexID)
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

float4 SpotLightDeferredPS(PixelInput input) : SV_Target
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
	// Perform Blinn Phong Spot Light
	//
	
	float3 toLight = g_SpotLightPosition - data.position;
	float3 toEye = g_EyePosition.xyz - data.position;
	float distToLight = length(toLight);
	
	//
	// Phong Diffuse
	//
	
	toLight /= distToLight;
	float NDotL = saturate(dot(toLight, data.normal));
	float3 lightColor = g_SpotLightColor * NDotL;
	
	//
	// Blinn specular
	//
	
	toEye = normalize(toEye);
	float3 halfWay = normalize(toEye + toLight);
	float NDotH = saturate(dot(halfWay, data.normal));
	lightColor += g_SpotLightColor * pow(NDotH, data.specularPower) * data.specularIntensity;
	
	//
	// Cone Attenuation
	//
	
	float coneAtt = saturate((g_SpotCosineAngle - g_SpotCosOuterCone) * g_SpotInnerConeReciprocal);
	coneAtt *= coneAtt;
	
	//
	// Attenuation
	//
	
	float distToLightNormal = 1.0 - saturate(distToLight * (1 / g_SpotLightRange));
	float attenuation = distToLightNormal * distToLightNormal;
	lightColor *= data.color.rgb * attenuation * coneAtt * g_SpotLightIntensity;
	
	return float4(lightColor, 1.0);
}

float4 SpotLightDeferredWireframePS(PixelInput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}
