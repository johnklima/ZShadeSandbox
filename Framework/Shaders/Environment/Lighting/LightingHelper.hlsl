//==============================================================================
// LightingHelper.hlsl
//
// Helper functions for shader files
//==============================================================================
// History
//
// -Created on 4/20/2015 by Dustin Watson
//==============================================================================

// Maximum amount of lights that can be in the scene
#define MAX_LIGHTS  16

struct AmbientLight
{
	float4	g_AmbientColor;
};

struct DirectionalLight
{
	float3	g_LightDirection;
	float	padding;
	float4	g_AmbientColor;
	float4	g_DiffuseColor;
};

struct SpotLight
{
	float4		g_AmbientColor;
	float4		g_DiffuseColor;
	float3		g_LightPosition;
	float		padding;
	float		g_LightRange;
	float		g_SpotCosOuterCone;
	float		g_SpotInnerConeReciprocal;
	float		g_CosineAngle;
};

struct PointLight
{
	float3	g_LightPosition;
	float	g_LightRange;
	float3	g_Attenuation;
	float	padding;
	float4	g_AmbientColor;
	float4	g_DiffuseColor;
};

struct CapsuleLight
{
	float3 	g_LightPosition;
	float 	g_LightRange;
	float3 	g_LightDirection;
	//float 	g_LightRangeReciprocal;
	//float3 	g_LightColor;
	float 	g_LightLength;
	float3 	g_CapsuleDirectionLength;
	float 	g_CapsuleIntensity;
	float4	g_AmbientColor;
	float4	g_DiffuseColor;
};

cbuffer cbLightBuffer : register(b1)
{
	AmbientLight	 g_AmbientLight[MAX_LIGHTS];
	DirectionalLight g_DirectionalLight[MAX_LIGHTS];
	SpotLight		 g_SpotLight[MAX_LIGHTS];
	PointLight		 g_PointLight[MAX_LIGHTS];
	CapsuleLight	 g_CapsuleLight[MAX_LIGHTS];
	
	int				 g_DirectionalLightCount;
	int				 g_SpotLightCount;
	int				 g_PointLightCount;
	int              g_CapsuleLightCount;
	
	float3			 g_AmbientDown;
	int				 g_AmbientLightCount;
	float3			 g_AmbientUp;
	float			 padding2;
};

cbuffer cbSunLightBuffer : register(b2)
{
	float3		g_SunDir;
	int			g_EnableSun;
	float4		g_SunDiffuseColor;
	float3		padding;
	float		g_SunShineness;
};

//
// Global Functions
//

float DotProduct(float3 pos, float3 pos3D, float3 normal)
{
    float3 dir = normalize(pos3D - pos);
    return dot(-dir, normal);    
}

float4 DepthColor(float4 pos2D)
{
	return pos2D.z / pos2D.w;
}

float4 dot4x4(float4 aX, float4 aY, float4 aZ, float4 bX, float4 bY, float4 bZ)
{
	return aX * bX + aY * bY + aZ * bZ;
}

float4 dot4x1(float4 aX, float4 aY, float4 aZ, float3 b)
{
	return aX * b.xxxx + aY * b.yyyy + aZ * b.zzzz;
}

//int GetLightIndexInt1(int arr[MAX_LIGHTS], int i)
//{
//	return ((int[MAX_LIGHTS])(arr[i/MAX_LIGHTS]))[i%MAX_LIGHTS];
//}
//
//int GetLightIndexFloat4(float4 arr[MAX_LIGHTS], int i)
//{
//	return ((float4[MAX_LIGHTS])(arr[i/MAX_LIGHTS]))[i%MAX_LIGHTS];
//}
//
//int GetLightIndexFloat3(float3 arr[MAX_LIGHTS], int i)
//{
//	return ((float3[MAX_LIGHTS])(arr[i/MAX_LIGHTS]))[i%MAX_LIGHTS];
//}
//
//int GetLightIndexFloat1(float arr[MAX_LIGHTS], int i)
//{
//	return ((float[MAX_LIGHTS])(arr[i/MAX_LIGHTS]))[i%MAX_LIGHTS];
//}
