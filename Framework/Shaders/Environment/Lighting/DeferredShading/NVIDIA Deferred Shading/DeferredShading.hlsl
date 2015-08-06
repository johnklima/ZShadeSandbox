//==============================================================================
// DeferredShading
//
// Render the scene into a GBuffer (Geometry Buffer) that contains everything
// needed for full shading and post processing. This includes minimum of depth,
// normal, per-pixel mesh color, specular power and specular intensity.
// For each light source, find the affected pixels, read the corresponding GBuffer
// data, calculate the pixels lit color, and store it in an accumulation buffer
//==============================================================================
// History
//
// -Created on 4/21/2015 by Dustin Watson
//==============================================================================
#include "LightingHelper.hlsl"
#include "lighting.hlsl"

//
// GBuffer (Geometry Buffer)
//
//==============================================================================
// Minimum of data you can store in it is depth, base color, normal, specular power
// and specular intensity and you need between three to four render targets.
// Storing depth and base color: use a D24S8 render target for depth and stencil.
// Use A8R8G8B8 render target sampled from 8 bit per-pixel texture for base color.
// The alpha channel of the color target can be used as the specular intensity.
// Storing normals requires planning.
// Encode normalized normals from range [-1, 1] into the range [0, 1] then write it
// to three channels of a render target.
// Specular power storage: single 8 bit channel of a fourth render target
//
// 1st) Allocate GBuffer render targets
// =====================================================
// 24-Bit Depth             | 8-Bit Stencil
// =====================================================
// 24-Bit Base Color RGB    | 8-Bit Specular Intensity
// =====================================================
// 32-Bit Normals
// =====================================================
// 8-Bit Specular Power
// =====================================================
// 
// Format Types:
// ========================================================================
// Render Target | Use                               | Format
// 0             | Depth and Stencil                 | Texture: R24G8_TYPELESS
//													   Depth Stencil View: D24_UNORM_S8_UINT
//													   Resource View: R24_UNORM_X8_TYPELESS
// 1             | Base color, specular intensity    | Texture: R8G8B8A8_UNORM
//													   Resource View: R24_UNORM_X8_TYPELESS
// 2             | Normal                            | Texture: R11G11B10_FLOAT
//													   Resource View: R24_UNORM_X8_TYPELESS
// 3             | Specular power                    | Texture: R8G8B8A8_UNORM
//													   Resource View: R24_UNORM_X8_TYPELESS
// 
// Use the games back-buffer depth render target instead of allocating a new one.
//
// Filling the GBuffer:
// Start by clearing the depth view with depth equal to 1 and other targets to black.
// Set all four render targets in order.
// Once the targets are ready render the opaque elements of the scene.
//
// SIDE NOTE: Terrain normals are calculated in the shader so I currently cannot pass in a normal for terrain here

//
// Globals
//
//==============================================================================

static const g_SpecPowerRange = { 0.1, 250.0 };

static const float2 g_ArrBasePos[4] =
{
	float2(-1.0,  1.0),
	float2( 1.0,  1.0),
	float2(-1.0, -1.0),
	float2( 1.0,  1.0)
};

//
// Constant Buffers
//
//==============================================================================

cbuffer cbGBufferUnpackConst : register(b0)
{
	// XY components should be filled with one over the projection matrix diagonal values.
	// 		g_PerspectiveValues.x = 1 / Pm[0][0]
	// Z component is the projection matrix Pm[3][2] value.
	// W component is the negative projection matrix Pm[2][2] value.
	float4 g_PerspectiveValues : packoffset(c0);
	float4 g_x4ViewInv		   : packoffset(c1);
};

cbuffer cbMatrixConst : register(b1)
{
    matrix gWorld;
    matrix gView;
    matrix gProj;
};

cbuffer cbDeferredShaderConst : register(b2)
{
	float3 g_EyePos					: packoffset(c0);
	float3 g_DirectionToLight		: packoffset(c1);
	float4 g_DirectionLightColor	: packoffset(c2);
	float3 g_AmbientDown			: packoffset(c3);
	float3 g_AmbientUp				: packoffset(c4);
};

cbuffer cbDeferredShaderMatrixConst : register(b3)
{
	matrix g_InvView;
};

//
// Textures and Samplers
//
//==============================================================================

Texture2D diffuseTexture 		: register(t0);
Texture2D g_DepthTexture 		: register(t1);
Texture2D g_ColorSpecIntTexture : register(t2);
Texture2D g_NormalTexture 		: register(t3);
Texture2D g_SpecPowTexture 		: register(t4);

SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

//
// Shader Structs
//
//==============================================================================

struct GBufferVertexInput
{
	float3 position			: POSITION;
	float3 normal			: NORMAL;
	float2 uv				: TEXCOORD0;
};

struct GBufferPixelInput
{
	float4 position			: SV_POSITION;
	float3 normal			: NORMAL;
	float2 uv				: TEXCOORD0;
};

struct PSGBufferOut
{
	float4 ColorSpecInt		: SV_TARGET0;
	float4 Normal			: SV_TARGET1;
	float4 SpecPow			: SV_TARGET2;
};

struct UnpackedSurfaceData
{
	float linearDepth;
	float3 color;
	float3 normal;
	float specularIntensity;
	float specularPower;
};

struct DeferredShaderPixelInput
{
	float4 position			: SV_POSITION;
	float2 csPos			: TEXCOORD0;
};

struct Material
{
	float3 normal;
	float4 diffuseColor;
	float specularIntensity;
};

//
// GBuffer Packer (Uses the Geometry Buffer)
//
//==============================================================================

// Pack all three values into the GBuffer format.
PSGBufferOut PackGBuffer(float3 baseColor, float3 normal, float specularIntensity, float specularPower)
{
	PSGBufferOut output;
	
	// Normalize specular power
	float specularPowerNormal = (specularPower - g_SpecPowerRange.x) / g_SpecPowerRange.y;
	
	// Pack all the data into the GBuffer
	output.ColorSpecInt = float4(baseColor.rgb, specularIntensity);
	output.normal = float4(normal.xyz * 0.5 + 0.5, 0.0);
	output.SpecPow = float4(specularPowerNormal, 0.0, 0.0, 0.0);
}

GBufferPixelInput GBufferVS(GBufferVertexInput input)
{
	GBufferPixelInput output;
	
	output.position = mul(float4(input.position, 1.0), gWorld);
	output.position = mul(output.position, gView);
	output.position = mul(output.position, gProj);
	
	output.normal = mul(input.normal, gWorld);
	output.uv = input.uv;
	
	return output;
}

PSGBufferOut GBufferPS(GBufferPixelInput input)
{
	float3 diffuseColor;
	float3 normal;
	float specularIntensity;
	float specularPower;
	
	// Fill in all values from vertex shader
	
	diffuseColor = diffuseTexture.Sample(SampleType, input.uv);
	
	specularIntensity = 0.8;
	specularPower = 0.5;
	
	normal = input.normal;
	
	return PackGBuffer(diffuseColor, normalize(normal), specularIntensity, specularPower);
}

//
// GBuffer Unpacker
//
//==============================================================================

float DepthToLinear(float depth)
{
	return g_PerspectiveValues.z / (depth + g_PerspectiveValues.w);
}

UnpackedSurfaceData UnpackGBuffer(int2 location)
{
	UnpackedSurfaceData output;
	
	// Cast the location to 3 components
	int3 location3 = int3(location, 0);
	
	// Convert the depth to linear depth
	float depth = g_DepthTexture.Load(location3).x;
	output.linearDepth = DepthToLinear(depth);
	
	// Base color and specular intensity
	float4 baseColorSpecIntensity = g_ColorSpecIntTexture.Load(location3);
	output.color = baseColorSpecIntensity.xyz;
	output.specularIntensity = baseColorSpecIntensity.w;
	
	// Sample the normal, convert it to the full range and normalize it
	output.normal = g_NormalTexture.Load(location3).xyz;
	output.normal = normalize(output.normal * 2.0 - 1.0);
	
	// Scale the specular power back to the original image
	float specularPowerNormal = g_SpecPowTexture.Load(location3).x;
	output.specularPower = specularPowerNormal.x + specularPowerNormal * g_SpecPowerRange.y;
	
	return output;
}

//
// Deferred Shader
//
//==============================================================================
// Need to disable depth and nullify the depth buffer for the fullscreen quad

#define EyePositionViewInv[3].xyz;

float3 CalculateWorldPosition(float2 csPos, float linearDepth)
{
	float4 position;
	
	position.xy = csPos.xy * g_PerspectiveValues.xy * linearDepth;
	position.z = linearDepth;
	position.w = 1.0;
	
	return mul(position, g_InvView).xyz;
}

DeferredShaderPixelInput DeferredShaderDirectionalLightVS(uint VertexID : SV_VertexID)
{
	DeferredShaderPixelInput output;
	
	output.position = float4(g_ArrBasePos[VertexID].xy, 0.0 1.0);
	
	output.csPos = output.position.xy;
	
	return output;
}

float4 DeferredShaderDirectionalLightPS(DeferredShaderPixelInput input) : SV_Target
{
	//
	// Unpack the GBuffer
	//
	
	UnpackedSurfaceData gbuffer = UnpackGBuffer(input.position.xy);
	
	//
	// Convert the data into a material
	//
	
	Material mat;
	mat.normal = gbuffer.normal;
	mat.diffuseColor.xyz = gbuffer.color;
	mat.diffuseColor.w = 1.0; // Fully Opaque
	mat.specularPower = g_SpecPowerRange.x + g_SpecPowerRange.y * gbuffer.specularPower;
	mat.specularIntensity = gbuffer.specularIntensity;
	
	//
	// Reconstruct the world position
	//
	
	float3 position = CalculateWorldPosition(input.cpPos, gbuffer.linearDepth);
	
	//
	// Calculate ambient and directional lights
	//
	
	float4 lightColor;
	
	lightColor.xyz = CalculateAmbientColor(mat.normal, mat.diffuseColor.xyz, g_AmbientDown, g_AmbientUp);
	
	lightColor.xyz += DirectionalLightColor(
		position,
		g_EyePos,
		g_DirectionToLight,
		g_DirectionLightColor,
		mat.normal,
		mat.diffuseColor,
		mat.specularPower,
		mat.specularIntensity
	);
	
	lightColor.w = 1.0;
	
	return lightColor;
}