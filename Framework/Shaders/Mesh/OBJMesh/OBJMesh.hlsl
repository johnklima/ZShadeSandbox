//==============================================================================
// Static OBJ Mesh Shader
//
//==============================================================================
// History
//
// -Created on 4/14/2015 by Dustin Watson
//==============================================================================
#include "../../Environment/Lighting/Lighting.hlsl"

//
// Constants
//

cbuffer cbShadingBuffer : register(b2)
{
	float3	g_EyePosition;
	float	g_Alpha;
	float4	g_ClipPlane;
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
	float	g_FarPlane;
	int		g_SpecularToggle;
	int		g_EnableLighting;
};

cbuffer cbMatrixBuffer : register(b3)
{
	matrix g_World;
	matrix g_View;
	matrix g_Proj;
};

//======================================================================================================

//
// Globals
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

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
	float3 normal		: NORMAL;
	float pixelDepth	: DEPTH;
	float2 uv			: TEXCOORD0;
	float clip 			: TEXCOORD1;
};

PixelInput OBJMeshVS(VertexInput input)
{
	PixelInput output;
	
	output.position = mul(float4(input.position, 1.0f), g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Proj);

	output.positionW = input.position;

	output.normal = input.normal;
    output.uv = input.uv;
    
	output.clip = dot(output.position, g_ClipPlane);
	
	// Logarithmic z buffer
	// DirectX Depth range 0..1
	int C = 1;
	output.pixelDepth = log2(max(1e-6, C * output.position.z + 1)) * 2 / log2(C * g_FarPlane + 1) * output.position.z;
	
	return output;
}

//
// Pixel Shader
//

float4 OBJMeshPS(PixelInput input) : SV_Target
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

	//if (g_UsingTransparency == 1)
	//{
	//	if (g_UsingAlphaMapTexture == 1)
	//	{
	//		float4 alphaMapColor = g_MaterialAlphaMapTexture.Sample(g_LinearSampler, input.uv);
	//		if (alphaMapColor.x == g_Alpha && alphaMapColor.y == g_Alpha && alphaMapColor.z == g_Alpha)
	//		{
	//			clip(-1);
	//		}
	//	}
	//}
	
	clip( input.clip < 0.0 ? -1 : 1 );
	
	/*
	finalColor.rgb *= CalculateLightColor
	(	input.positionW
	,	g_EyePosition
	,	finalColor
	,	normal
	,	g_MaterialDiffuseColor
	,	g_MaterialAmbientColor
	,	g_MaterialSpecularPower
	,	g_MaterialSpecularIntensity
	,	1.0f
	,	1.0f
	);
	*/
	
	finalColor.a = 1.0f;

	return finalColor;

	//float3 toLight = normalize(float3(2,-5,3));
	//float I = saturate(dot(normalize(normal), toLight));
	//I += 0.2f; // Ambient
	//return finalColor * I;
}

float4 OBJMeshWireframePS(PixelInput input) : SV_Target
{
	clip( input.clip < 0.0 ? -1 : 1 );
	
	return float4(0.9f, 0.9f, 0.9f, 1);
}