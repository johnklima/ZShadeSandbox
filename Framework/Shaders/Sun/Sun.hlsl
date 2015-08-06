//==============================================================================
// Sun Shader
//
//==============================================================================
// History
//
// -Created on 5/1/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbSunConst : register(b0)
{
	float2 padding;
	float g_Alpha;
	float g_Glow;
};

cbuffer cbPerFrame : register(b1)
{
	matrix g_World;
	matrix g_View;
	matrix g_Proj;
};

//======================================================================================================

//
// Globals
//

Texture2D g_SunTexture : register(t0);

SamplerState TextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

//======================================================================================================


struct VertexIn
{
	float3 position : POSITION;
	float2 uv		: TEXCOORD0;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float2 uv		: TEXCOORD0;
};

VertexOut SunVS(VertexIn input)
{
	VertexOut output;
	
	output.position = mul(float4(input.position, 1.0), g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Proj);
	
	output.uv = input.uv;
	
	return output;
}

float4 SunPS(VertexOut input) : SV_Target
{
	float4 finalColor = g_SunTexture.Sample(TextureSampler, input.uv);
	
	finalColor.rgb += (1.0 - finalColor.a) / 2;
	
	finalColor.rgb += g_Glow;
	
	return finalColor * g_Alpha;
}

float4 SunWireframePS(VertexOut input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}
