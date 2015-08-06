//==============================================================================
// Quad Material Tessellation Shader
//
//==============================================================================
// History
//
// -Created on 5/12/2015 by Dustin Watson
//==============================================================================
#include "../Environment/Lighting/Lighting.hlsl"

//======================================================================================================

//
// Constants
//

cbuffer cbTessellationBuffer : register(b2)
{
	float3 	g_EyePosW;
	float 	g_NoDistTessFactor;
	float 	g_MinTessDist;
	float 	g_MaxTessDist;
	float 	g_MinTessFactor;
	float 	g_MaxTessFactor;
	float3	tesspadding;
	int		g_EnableDistTess;
};

cbuffer cbDomainBuffer : register(b3)
{
	float4  g_ClipPlane;
	float3	domainpadding;
	float	g_FarPlane;
	matrix 	g_World;
	matrix 	g_View;
	matrix 	g_Proj;
};

cbuffer cbShadingBuffer : register(b4)
{
	float3	g_EyePosition;
	float	g_Alpha;
	float4	g_ClipPlanes;
	float4	g_MaterialDiffuseColor;
	float4	g_MaterialAmbientColor;
	float4	g_MaterialSpecularColor;
	float4	g_MaterialEmissiveColor;
	float	g_Emissivity;
	float	g_Reflectivity;
	float	g_Transmissivity;
	float	g_RefractionIndex;
	float3	g_AlphaToCoverageValue;
	float	g_DetailBrightness;
	float3	g_TransmissionFilter;
	float	g_MaterialSpecularPower;
	float	g_MaterialSpecularIntensity;
	int		g_IlluminationModel;
	int	   	g_FlipTextureH;
	int	   	g_FlipTextureV;
	int		g_UsingDiffuseArrayTexture;
	int	   	g_UsingDiffuseTexture;
	int	   	g_UsingAmbientTexture;
	int	   	g_UsingSpecularTexture;
	int	   	g_UsingEmissiveTexture;
	int	   	g_UsingNormalMapTexture;
	int	   	g_UsingBlendMapTexture;
	int	   	g_UsingDetailMapTexture;
	int		g_UsingAlphaMapTexture;
	int		g_UsingTransparency;
	int		g_UsingShadowMap;
	int		g_UsingSSAOMap;
	int		g_UsingDisplacementMap;
	float	g_FarPlanes;
	int		g_SpecularToggle;
	int		g_EnableLighting;
	float3	matpadding;
	float	g_BlendAmount;
};

//======================================================================================================

//
// Texturs and Samplers
//

Texture2DArray g_MaterialDiffuseArrayTexture 	: register(t0);
Texture2D g_MaterialDiffuseTexture 				: register(t1);
Texture2D g_MaterialAmbientTexture 				: register(t2);
Texture2D g_MaterialSpecularTexture 			: register(t3);
Texture2D g_MaterialEmissiveTexture 			: register(t4);
Texture2D g_MaterialNormalMapTexture 			: register(t5);
Texture2D g_MaterialBlendMapTexture 			: register(t6);
Texture2D g_MaterialDetailMapTexture 			: register(t7);
Texture2D g_MaterialAlphaMapTexture 			: register(t8);
Texture2D g_ShadowMap							: register(t9);
Texture2D g_SSAOMap								: register(t10);
Texture2D g_DisplacementMap						: register(t11);

SamplerState g_PointSampler		: register(s0);
SamplerState g_LinearSampler	: register(s1);

//======================================================================================================

//
// Vertex Shader
//

struct VertexInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

struct VertexInputInstance
{
	float3 position				: POSITION;
	float3 normal				: NORMAL;
	float2 uv					: TEXCOORD0;
	float3 instancePosition		: INSTANCEPOS;
};

struct HullInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

HullInput QuadMaterialTessellationVS(VertexInput input)
{
	HullInput output;
	
	// Pass vertex position into the hull shader
	output.position = input.position;

	if (g_UsingDisplacementMap == 1)
	{
		output.position.y = g_DisplacementMap.SampleLevel(g_PointSampler, input.uv, 0).r;
	}

	// Pass normal into the hull shader
	output.normal = input.normal;

	// Pass texture uv into the hull shader
	output.uv = input.uv;
	
	return output;
}

HullInput QuadMaterialTessellationInstanceVS(VertexInputInstance input, uint instanceID : SV_InstanceID)
{
	HullInput output;
	
	input.position.x += input.instancePosition.x;
	input.position.y += input.instancePosition.y;
	input.position.z += input.instancePosition.z;
	
	// Pass vertex position into the hull shader
	output.position = input.position;

	if (g_UsingDisplacementMap == 1)
	{
		output.position.y = g_DisplacementMap.SampleLevel(g_PointSampler, input.uv, 0).r;
	}

	// Pass normal into the hull shader
	output.normal = input.normal;

	// Pass texture uv into the hull shader
	output.uv = input.uv;
	
	return output;
}

//======================================================================================================

//
// Hull Shader
//

struct PatchConstOutput
{
	float edges[4]		: SV_TessFactor;
	float inside[2]		: SV_InsideTessFactor;
};

struct DomainInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

float CalcTessFactor(float3 p)
{
	float d = distance(p, g_EyePosW);

	float s = saturate((d - g_MinTessDist) / (g_MaxTessDist - g_MinTessDist));

	return pow(2, (lerp(g_MaxTessFactor, g_MinTessFactor, s)));
}

void HCTessellate(out PatchConstOutput output, float factor)
{
	output.edges[0] = factor;
	output.edges[1] = factor;
	output.edges[2] = factor;
	output.edges[3] = factor;
	output.inside[0] = factor;
	output.inside[1] = factor;
}

void HCTessellate(out PatchConstOutput output, float3 e0, float3 e1, float3 e2, float3 e3, float3 c)
{
	output.edges[0] = CalcTessFactor(e0);
	output.edges[1] = CalcTessFactor(e1);
	output.edges[2] = CalcTessFactor(e2);
	output.edges[3] = CalcTessFactor(e3);
	output.inside[0] = CalcTessFactor(c);
	output.inside[1] = output.inside[0];
}

PatchConstOutput ConstHSMaterialQuad(InputPatch<HullInput, 4> inputPatch, uint patchID : SV_PrimitiveID)
{
	PatchConstOutput output;

	if (g_EnableDistTess == 1)
	{
		float3 WorldPosition0 = inputPatch[0].position;
		float3 WorldPosition1 = inputPatch[1].position;
		float3 WorldPosition2 = inputPatch[2].position;
		float3 WorldPosition3 = inputPatch[3].position;

		// Compute midpoint on edges, and patch center
		float3 e0 = 0.5f * (WorldPosition0 + WorldPosition2);
		float3 e1 = 0.5f * (WorldPosition0 + WorldPosition1);
		float3 e2 = 0.5f * (WorldPosition1 + WorldPosition3);
		float3 e3 = 0.5f * (WorldPosition2 + WorldPosition3);

		float3  c = 0.25f * (WorldPosition0 + WorldPosition1 + WorldPosition2 + WorldPosition3);

		HCTessellate(output, e0, e1, e2, e3, c);
	}
	else
	{
		HCTessellate(output, g_NoDistTessFactor);
	}

	return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstHSMaterialQuad")]
DomainInput QuadMaterialTessellationHS(InputPatch<HullInput, 4> inputPatch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	DomainInput output;

	output.position = inputPatch[pointID].position;

	output.uv = inputPatch[pointID].uv;

	output.normal = inputPatch[pointID].normal;

	return output;
}

//======================================================================================================

//
// Domain Shader
//

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
	float3 normal		: NORMAL;
	float pixelDepth	: DEPTH;
	float2 uv			: TEXCOORD0;
	float  clip			: TEXCOORD1;
	float4 depth		: TEXCOORD2;
};

[domain("quad")]
PixelInput QuadMaterialTessellationDS(PatchConstOutput input, float2 uv : SV_DomainLocation, const OutputPatch<DomainInput, 4> inputPatch)
{
	PixelInput output;

	// Bilinear interpolation.

	// Find the position of the new vertex
	float3 vPos = lerp(lerp(inputPatch[0].position, inputPatch[1].position, uv.x), lerp(inputPatch[2].position, inputPatch[3].position, uv.x), uv.y);

	// Find the normal of the new vertex
	float3 vNormal = lerp(lerp(inputPatch[0].normal, inputPatch[1].normal, uv.x), lerp(inputPatch[2].normal, inputPatch[3].normal, uv.x), uv.y);

	// Find the texture coordinate of the new vertex
	float2 vTex = lerp(lerp(inputPatch[0].uv, inputPatch[1].uv, uv.x), lerp(inputPatch[2].uv, inputPatch[3].uv, uv.x), uv.y);

	if (g_UsingDisplacementMap == 1)
	{
		vPos.y = g_DisplacementMap.SampleLevel(g_PointSampler, vTex, 0).r;
	}

	output.positionW = vPos;

	// Calculate the new position of the new vertex with WVP matrices
	output.position = mul(float4(vPos, 1.0f), g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Proj);

	// Send the texture uv to the pixel shader
	output.uv = vTex;

	// Send the normal to the pixel shader
	output.normal = vNormal;

	output.clip = dot(output.position, g_ClipPlane);
	output.depth = output.position;

	// Logarithmic z buffer
	// DirectX Depth range 0..1
	int C = 1;
	output.pixelDepth = log2(max(1e-6, C * output.position.z + 1)) * 2 / log2(C * g_FarPlane + 1) * output.position.z;

	return output;
}

//======================================================================================================

//
// Pixel Shader
//

float4 QuadMaterialTessellationPS(PixelInput input) : SV_Target
{
	float4 finalColor;

	// 1 - u: flips the texture horizontally
	if (g_FlipTextureH == 1)
	{
		input.uv.x = 1 - input.uv.x;
	}

	// 1 - v: flips the texture vertically
	if (g_FlipTextureV == 1)
	{
		input.uv.y = 1 - input.uv.y;
	}

	//============================================ Texturing

	float4 finalTextureColor = float4(0, 0, 0, 1);
	float4 diffuseLayers0 = float4(0, 0, 0, 1);
	float4 diffuseLayers1 = float4(0, 0, 0, 1);
	float4 diffuseLayers2 = float4(0, 0, 0, 1);
	float4 diffuseLayers3 = float4(0, 0, 0, 1);

	if (g_UsingDiffuseArrayTexture == 1)
	{
		// Sample layers in texture array.
		diffuseLayers0 = g_MaterialDiffuseArrayTexture.Sample(g_LinearSampler, float3(input.uv, 0.0f));
		diffuseLayers1 = g_MaterialDiffuseArrayTexture.Sample(g_LinearSampler, float3(input.uv, 1.0f));
		diffuseLayers2 = g_MaterialDiffuseArrayTexture.Sample(g_LinearSampler, float3(input.uv, 2.0f));
		diffuseLayers3 = g_MaterialDiffuseArrayTexture.Sample(g_LinearSampler, float3(input.uv, 3.0f));
	}
	else
	{
		if (g_UsingDiffuseTexture == 1)
		{
			finalTextureColor = (g_UsingDiffuseTexture == 1) ? g_MaterialDiffuseTexture.Sample(g_LinearSampler, input.uv) : g_MaterialDiffuseColor;
			//finalTextureColor += (g_UsingAmbientTexture == 1) ? g_MaterialAmbientTexture.Sample(g_LinearSampler, input.uv) : g_MaterialAmbientColor;
			//finalTextureColor += (g_UsingSpecularTexture == 1) ? g_MaterialSpecularTexture.Sample(g_LinearSampler, input.uv) : g_MaterialSpecularColor;
			//finalTextureColor += (g_UsingEmissiveTexture == 1) ? g_MaterialEmissiveTexture.Sample(g_LinearSampler, input.uv) : g_MaterialEmissiveColor;
		}
	}

	//============================================ Blend Mapping

	if (g_UsingBlendMapTexture == 1)
	{
		if (g_UsingDiffuseArrayTexture == 1)
		{
			// Sample the blend map.
			float4 t = g_MaterialBlendMapTexture.Sample(g_LinearSampler, input.uv);

			// Blend the layers on top of each other.
			finalTextureColor = diffuseLayers0;
			finalTextureColor = lerp(finalTextureColor, diffuseLayers1, t.r);
			finalTextureColor = lerp(finalTextureColor, diffuseLayers2, t.g);
			finalTextureColor = lerp(finalTextureColor, diffuseLayers3, t.b);
		}
	}
	else
	{
		if (g_UsingDiffuseArrayTexture == 1)
			finalTextureColor = saturate(diffuseLayers0 + diffuseLayers1 + diffuseLayers2 + diffuseLayers3);
	}

	// Now add the texture color
	finalColor = finalTextureColor;

	//============================================ Normal Mapping

	float depth = input.depth.z / input.depth.w;

	float3 normal = input.normal;

	if (g_UsingNormalMapTexture == 1)
	{
		float4 normalMap = g_MaterialNormalMapTexture.Sample(g_LinearSampler, input.uv);
		normal = (normalMap.rgb * 2.0f) - 1.0f;
		normal = normalize(normal);
	}

	//============================================ Shadow Mapping

	float shadowColor = 1.0f;

	//============================================ SSAO

	float ssaoColor = 1.0f;

	//============================================ Detail Mapping

	// Check if the depth value is close to the screen
	if (g_UsingDetailMapTexture == 1)
	{
		float4 detailColor = g_MaterialDetailMapTexture.Sample(g_LinearSampler, input.uv);
		finalColor *= detailColor * g_DetailBrightness;
	}

	//============================================ Lighting

	if (g_EnableLighting == 1)
	{
		finalColor.rgb *= CalculateLightColor
		(	input.positionW
		,	g_EyePosition
		,	finalColor
		,	normal
		,	g_MaterialDiffuseColor
		,	g_MaterialAmbientColor
		,	g_MaterialSpecularPower
		,	g_MaterialSpecularIntensity
		,	shadowColor
		,	ssaoColor
		);
	}

	//============================================ Alpha Map Blending (Alpha-to-Coverage)

	if (g_UsingAlphaMapTexture == 1)
	{
		float4 alphaMapColor = g_MaterialAlphaMapTexture.Sample(g_LinearSampler, input.uv);
		if (alphaMapColor.r == g_AlphaToCoverageValue.x
		&&	alphaMapColor.g == g_AlphaToCoverageValue.y
		&&	alphaMapColor.b == g_AlphaToCoverageValue.z)
		{
			//Set the alpha value of this pixel to the blending amount to
			//create the alpha blending effect.
			if (g_UsingTransparency == 1)
				finalColor.a = g_BlendAmount;
			else
				clip(-1);
		}
	}
	else
	{
		finalColor.a = 1.0f;
	}

	//============================================ Clipping for water reflections and refractions

	clip(input.clip < 0.0 ? -1 : 1);

	return finalColor;
}

float4 QuadMaterialTessellationWireframePS(PixelInput input) : SV_Target
{
	clip(input.clip < 0.0 ? -1 : 1);

	return float4(0.9f, 0.9f, 0.9f, 1);
}