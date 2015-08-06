//==============================================================================
// Static OBJ Mesh Tessellation Shader
//
//==============================================================================
// History
//
// -Created on 4/15/2015 by Dustin Watson
// -Updated on 6/7/2015 by Dustin Watson
//==============================================================================
#include "../../Environment/Lighting/Lighting.hlsl"

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
	float	materialpadding;
	float	g_FarPlanes;
	int		g_SpecularToggle;
	int		g_EnableLighting;
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

SamplerState g_LinearSampler	: register(s0);

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

struct HullInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

HullInput OBJMeshTessellationVS(VertexInput input)
{
	HullInput output;
	
	// Pass vertex position into the hull shader
	output.position = input.position;
	
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
	float edges[3]		: SV_TessFactor;
	float inside		: SV_InsideTessFactor;
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
	output.inside = factor;
}

void HCTessellate(out PatchConstOutput output, float3 e0, float3 e1, float3 e2, float3 c)
{
	output.edges[0] = CalcTessFactor(e0);
	output.edges[1] = CalcTessFactor(e1);
	output.edges[2] = CalcTessFactor(e2);
	output.inside = CalcTessFactor(c);
}

PatchConstOutput ConstHS(InputPatch<HullInput, 3> inputPatch, uint patchID : SV_PrimitiveID)
{
	PatchConstOutput output;
	
	if (g_EnableDistTess == 1)
	{
		float3 WorldPosition0 = inputPatch[0].position;
		float3 WorldPosition1 = inputPatch[1].position;
		float3 WorldPosition2 = inputPatch[2].position;

		// Compute midpoint on edges, and patch center
		float3 e0 = 0.5f * (WorldPosition0 + WorldPosition1);
		float3 e1 = 0.5f * (WorldPosition1 + WorldPosition2);
		float3 e2 = 0.5f * (WorldPosition2 + WorldPosition0);

		float3  c = (WorldPosition0 + WorldPosition1 + WorldPosition2) / 3;

		HCTessellate(output, e0, e1, e2, c);
	}
	else
	{
		HCTessellate(output, g_NoDistTessFactor);
	}

	return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstHS")]
DomainInput OBJMeshTessellationHS(InputPatch<HullInput, 3> inputPatch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	DomainInput output;
	
	output.position = inputPatch[pointID].position;
	
	output.normal = inputPatch[pointID].normal;
	
	output.uv = inputPatch[pointID].uv;
	
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
	float clip			: TEXCOORD1;
};

[domain("tri")]
PixelInput OBJMeshTessellationDS(PatchConstOutput input, float3 uvw : SV_DomainLocation, const OutputPatch<DomainInput, 3> inputPatch)
{
	PixelInput output;
	
	// Find the position of the new vertex
	float3 vPos = uvw.x * inputPatch[0].position + uvw.y * inputPatch[1].position + uvw.z * inputPatch[2].position;
	
	// Find the normal of the new vertex
	float3 vNormal = uvw.x * inputPatch[0].normal + uvw.y * inputPatch[1].normal + uvw.z * inputPatch[2].normal;
	
	// Find the texture coordinate of the new vertex
	float2 vTex = uvw.x * inputPatch[0].uv + uvw.y * inputPatch[1].uv + uvw.z * inputPatch[2].uv;
	
	output.positionW = vPos;
	
	// Calculate the new position of the new vertex with WVP matrices
	output.position = mul(float4(vPos, 1.0f), g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Proj);
	
	// Logarithmic z buffer
	// DirectX Depth range 0..1
	int C = 1;
	output.pixelDepth = log2(max(1e-6, C * output.position.z + 1)) * 2 / log2(C * g_FarPlane + 1) * output.position.z;
	
	// Send the normal to the pixel shader
	output.normal = vNormal;
	
	// Send the texture uv to the pixel shader
	output.uv = vTex;
	
	output.clip = dot(output.position, g_ClipPlane);
	
	return output;
}

//======================================================================================================

//
// Pixel Shader
//

float4 OBJMeshTessellationPS(PixelInput input) : SV_Target
{
	float4 finalColor;
	
	//============================================ Texture Color
	
	finalColor = (g_UsingDiffuseTexture == 1) ? g_MaterialDiffuseTexture.Sample(g_LinearSampler, input.uv) : g_MaterialDiffuseColor;
	finalColor += (g_UsingAmbientTexture == 1) ? g_MaterialAmbientTexture.Sample(g_LinearSampler, input.uv) : g_MaterialAmbientColor;
	finalColor += (g_UsingSpecularTexture == 1) ? g_MaterialSpecularTexture.Sample(g_LinearSampler, input.uv) : g_MaterialSpecularColor;
	finalColor += (g_UsingEmissiveTexture == 1) ? g_MaterialEmissiveTexture.Sample(g_LinearSampler, input.uv) : g_MaterialEmissiveColor;
	
	//============================================ Normal Mapping
	
	float3 normal = input.normal;
	
	if (g_UsingNormalMapTexture == 1)
	{
		float4 normalMap = g_MaterialNormalMapTexture.Sample(g_LinearSampler, input.uv);
		normal = (normalMap.rgb * 2.0f) - 1.0f;
		normal = normalize(normal);
	}
	
	//============================================ Detail Mapping
	
	if (g_UsingDetailMapTexture == 1)
	{
		float4 detailColor = g_MaterialDetailMapTexture.Sample(g_LinearSampler, input.uv);
		finalColor *= detailColor * g_DetailBrightness;
	}
	
	//============================================ Alpha Map Blending (Alpha-to-Coverage)
	
	if (g_UsingTransparency == 1)
	{
		if (g_UsingAlphaMapTexture == 1)
		{
			float4 alphaMapColor = g_MaterialAlphaMapTexture.Sample(g_LinearSampler, input.uv);
			if (alphaMapColor.x == g_Alpha && alphaMapColor.y == g_Alpha && alphaMapColor.z == g_Alpha)
			{
				clip(-1);
			}
		}
	}
	
	clip( input.clip < 0.0 ? -1 : 1 );
	
	/*finalColor.rgb *= CalculateLightColor
	(	input.positionW
	,	g_EyePosition
	,	finalColor
	,	normal
	,	g_MaterialDiffuseColor
	,	g_MaterialAmbientColor
	,	g_MaterialSpecularPower
	,	g_MaterialSpecularIntensity
	,   1.0f
	,   1.0f
	);*/
	
	finalColor.a = 1.0f;

	return finalColor;
	
	//float3 toLight = normalize(float3(2, -5, 3));
	//float I = saturate(dot(normalize(normal), toLight));
	//I += 0.2f; // Ambient
	//return finalColor * I;
}

float4 OBJMeshTessellationWireframePS(PixelInput input) : SV_Target
{
	clip( input.clip < 0.0 ? -1 : 1 );
	
    return float4(0.9f, 0.9f, 0.9f, 1);
}