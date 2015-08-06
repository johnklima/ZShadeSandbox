//==============================================================================
// GBuffer
//
//==============================================================================
// History
//
// -Created on 5/22/2015 by Dustin Watson
//==============================================================================
#include "DeferredCommon.hlsl"

//
// Constants
//

cbuffer cbMatrixBuffer : register(b0)
{
	matrix		g_LocalToProjectedMatrix;
	matrix		g_LocalToWorldMatrix;
	matrix		g_WorldToViewMatrix;
};

cbuffer cbSubMeshBuffer : register(b1)
{
	float3	g_DiffuseColor;
	int		g_IsTextured;
};

//======================================================================================================

//
// Samplers and textures
//

Texture2D texDiffuse		: register(t0);

SamplerState textureSampler	: register(s0);

//======================================================================================================

//
// Structs
//

struct VertexInput
{
	float4 position	: POSITION;
	float3 normal	: NORMAL;
	float2 uv		: TEXCOORD0;
};

struct PixelInput
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD0;
	float3 worldNorm	: TEXCOORD1;
	float viewDepth		: TEXCOORD2;
};

struct PixelOutput
{
	float4 normalDepth 	 : SV_Target0;
	float4 diffuseAlbedo : SV_Target1;
	uint coverage		 : SV_Target2;
};

//======================================================================================================

//
// Vertex Shader
//

PixelInput GBufferVS(VertexInput input)
{
	PixelInput output;
	
	float4 worldPos = mul(input.position, g_LocalToWorldMatrix);
	
	output.position = mul(input.position, g_LocalToProjectedMatrix);
	output.uv = input.uv;
	output.worldNorm = mul(input.normal, (float3x3)g_LocalToWorldMatrix);
	output.viewDepth = mul(worldPos, g_WorldToViewMatrix).z;
	
	return output;
}

//======================================================================================================

//
// Pixel Shader
//

PixelOutput GBufferPS(PixelInput input, uint coverage : SV_Coverage) : SV_Target
{
	PixelOutput output;
	
	float3 worldNormal = normalize(input.worldNorm);
	float viewDepth = input.viewDepth / 1000.0f;
	
	float3 diffuse = g_IsTextured > 0 ? texDiffuse.Sample(textureSampler, float2(1, 1) - input.uv).rgb : g_DiffuseColor;
	
	float edge = coverage != 0xf;
	
	output.normalDepth = float4(worldNormal, viewDepth);
	output.diffuseAlbedo = float4(diffuse, edge);
	output.coverage = coverage
	
	return output;
}