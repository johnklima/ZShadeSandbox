//==============================================================================
// OBJ Mesh GBuffer Packer
//
//==============================================================================
// History
//
// -Created on 6/8/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbShadingBuffer : register(b0)
{
	float3	g_EyePosW;
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

cbuffer cbMatrixBuffer : register(b1)
{
	matrix g_World;
	matrix g_View;
	matrix g_Proj;
};

//======================================================================================================

//
// Samplers and textures
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

SamplerState linearSampler	: register(s0);

//======================================================================================================

//
// Structs
//

struct VertexInput
{
	float3 position	: POSITION;
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
	float4 colors 	: SV_Target0;
	float4 normals 	: SV_Target1;
	float4 depth	: SV_Target2;
};

//======================================================================================================

//
// Vertex Shader
//

PixelInput OBJGBufferVS(VertexInput input)
{
	PixelInput output;
	
	output.position = mul(float4(input.position, 1), g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Proj);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3)g_World);
	output.normal = normalize(output.normal);
	
	output.depth.x = output.position.z;
	output.depth.y = output.position.w;
	
	return output;
}

//======================================================================================================

//
// Pixel Shader
//

PixelOutput OBJGBufferPS(PixelInput input) : SV_Target
{
	float4 finalColor;
	
	//============================================ Texture Color
	
	finalColor = (g_UsingDiffuseTexture == 1) ? g_MaterialDiffuseTexture.Sample(linearSampler, input.uv) : g_MaterialDiffuseColor;
	finalColor += (g_UsingAmbientTexture == 1) ? g_MaterialAmbientTexture.Sample(linearSampler, input.uv) : g_MaterialAmbientColor;
	finalColor += (g_UsingSpecularTexture == 1) ? g_MaterialSpecularTexture.Sample(linearSampler, input.uv) : g_MaterialSpecularColor;
	finalColor += (g_UsingEmissiveTexture == 1) ? g_MaterialEmissiveTexture.Sample(linearSampler, input.uv) : g_MaterialEmissiveColor;
	
	//============================================ Normal Mapping
	
	float3 normal = input.normal;
	
	if (g_UsingNormalMapTexture == 1)
	{
		float4 normalMap = g_MaterialNormalMapTexture.Sample(linearSampler, input.uv);
		normal = (normalMap.rgb * 2.0f) - 1.0f;
		normal = normalize(normal);
	}
	
	//============================================ Detail Mapping
	
	if (g_UsingDetailMapTexture == 1)
	{
		float4 detailColor = g_MaterialDetailMapTexture.Sample(linearSampler, input.uv);
		finalColor *= detailColor * g_DetailBrightness;
	}
	
	//============================================ Alpha Map Blending (Alpha-to-Coverage)
	
	float doClip = 0.0;
	if (g_UsingTransparency == 1)
	{
		if (g_UsingAlphaMapTexture == 1)
		{
			float4 alphaMapColor = g_MaterialAlphaMapTexture.Sample(linearSampler, input.uv);
			if (alphaMapColor.x == g_Alpha && alphaMapColor.y == g_Alpha && alphaMapColor.z == g_Alpha)
			{
				doClip = 1.0;
			}
		}
	}
	
	//============================================ Final GBuffer pack
	
	PixelOutput output;
	
	output.colors = float4(finalColor.rgb, 0.0f);
	output.colors.a = g_MaterialSpecularIntensity;
	output.normals = float4(normal, g_MaterialSpecularPower);
	output.depth = float4(input.depth.x / input.depth.y, 0.0, 0.0, 1.0);
	
	return output;
}

PixelOutput OBJGBufferWireframePS(PixelInput input) : SV_Target
{
	PixelOutput output;
    
	output.colors = float4(0.9f, 0.9f, 0.9f, 1);
	output.normals = float4(0.5f, 0.5f, 0.5f, 1);
	output.depth = float4(1, 0, 0, 1);
	
	return output;
}
