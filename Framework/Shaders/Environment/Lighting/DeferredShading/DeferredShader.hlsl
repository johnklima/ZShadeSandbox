//==============================================================================
// Deferred Shader
//
//==============================================================================
// History
//
// -Created on 6/2/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbPackBuffer : register(b0)
{
	float2 g_SpecularPowerRange;
	float g_SpecularIntensity;
	float g_SpecularPower;
};

cbuffer cbMatrixBuffer : register(b1)
{
	matrix		g_World;
	matrix		g_View;
	matrix		g_Proj;
};

cbuffer cbMatrixBuffer2 : register(b2)
{
	matrix		g_WVP;
};

//======================================================================================================

//
// Samplers and textures
//

Texture2D g_TexDiffuse		: register(t0);
SamplerState linearSampler	: register(s0);

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
	float4 position	: SV_POSITION;
	float3 normal	: NORMAL;
	float2 uv		: TEXCOORD0;
	float2 depth	: TEXCOORD1;
};

struct PixelOutput
{
	float4 color0 	: SV_Target0;
	float4 color1 	: SV_Target1;
	float4 normal 	: SV_Target2;
	float4 depth	: SV_Target3;
};

//======================================================================================================

//
// Vertex Shader
//

PixelInput DeferredShaderVS(VertexInput input)
{
	PixelInput output;
	
	input.position.w = 1.0f;
	
	output.position = mul(input.position, g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Proj);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3)g_World);
	output.normal = normalize(output.normal);
	
	output.depth.x = output.position.z;
	output.depth.y = output.position.w;
	
	return output;
}

PixelInput DeferredShaderVS2(VertexInput input)
{
	PixelInput output;
	
	input.position.w = 1.0f;
	
	output.position = mul(input.position, g_WVP);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3)g_World);
	//output.normal = 0.5f * (normalize(output.normal) + 1.0f);
	output.normal = normalize(output.normal);
	
	output.depth.x = output.position.z;
	output.depth.y = output.position.w;
	
	return output;
}

//======================================================================================================

//
// Pixel Shader
//

PixelOutput DeferredShaderPS(PixelInput input) : SV_Target
{
	PixelOutput output;
	
	// Normalize the specular power
	//float specularPowerNormal = (g_SpecularPower - g_SpecularPowerRange.x) / g_SpecularPowerRange.y;
	
	// Output white color for the cube texture target
	output.color0 = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	output.color1 = g_TexDiffuse.Sample(linearSampler, input.uv);
	output.color1.a = g_SpecularIntensity;
	
	output.normal = float4(input.normal, g_SpecularPower);
	
	output.depth = float4(input.depth.x / input.depth.y, 0.0, 0.0, 0.0);
	
	return output;
}

PixelOutput DeferredShaderWireframePS(PixelInput input) : SV_Target
{
	PixelOutput output;
    
	output.color0 = float4(0.9f, 0.9f, 0.9f, 0);
	output.color1 = float4(0.9f, 0.9f, 0.9f, 0);
	output.normal = float4(0.5f, 0.5f, 0.5f, 0);
	output.depth = float4(1, 0, 0, 0);
	
	return output;
}