//==============================================================================
// Sky Shader
//
//==============================================================================
// History
//
// -Created on 4/8/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbPerFrame : register(b0)
{
	matrix g_World;
	matrix g_View;
	matrix g_Proj;
};

//======================================================================================================

//
// Globals
//

TextureCube g_CubeMap 	 	 : register(t0);
SamplerState samTriLinearSam : register(s0);

//======================================================================================================

//
// Normal Sky
//

struct VertexInput
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 uv		: TEXCOORD0;
};

struct VertexOutput
{
	float4 position  : SV_POSITION;
    float3 positionL : POSITION;
	float3 normal	 : NORMAL;
	float2 uv		 : TEXCOORD0;
};

VertexOutput SkyVS(VertexInput input)
{
	VertexOutput output;
	
	output.position = mul(float4(input.position, 1.0f), g_World);
    output.position = mul(output.position, g_View);
    output.position = mul(output.position, g_Proj);
	
	// Use local vertex position as cubemap lookup vector.
	output.positionL = input.position;
	output.uv = input.uv;
	
	output.normal = input.normal;
	
	return output;
}

float4 SkyPS(VertexOutput input) : SV_Target
{
	return g_CubeMap.Sample(samTriLinearSam, input.positionL);
}

float4 SkyWireframePS(VertexOutput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}
