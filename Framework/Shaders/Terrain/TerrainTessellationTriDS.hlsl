//==============================================================================
// Terrain Tessellation Domain Shader
//
//==============================================================================
// History
//
// -Created on 4/18/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cDomainConst : register(b0)
{
	float2  g_TexScale;
	float 	g_HeightScale;
	float 	g_TerrainZScale;
	float 	g_TexelCellSpaceU;
	float 	g_TexelCellSpaceV;
	float2  padding;
	float4 	g_ClipPlane;
};

cbuffer cMatrixBuffer : register(b1)
{
	matrix g_WorldMatrix;
	matrix g_ViewMatrix;
	matrix g_ProjMatrix;
};

//======================================================================================================

//
// Globals
//

Texture2D g_HeightMap : register(t0);

//------------------------------------------------------------------
//Sample States
//------------------------------------------------------------------
SamplerState samHeightmap
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

//======================================================================================================

//
// Domain Shader
//

struct PatchConstOutput
{
	float edges[3]		: SV_TessFactor;
	float inside		: SV_InsideTessFactor;
};

struct DomainInput
{
	float3 position		: POSITION;
	//float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
	//float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
	float2 TiledTex 	: TEXCOORD1;
	float4 worldPos 	: TEXCOORD2;
	float clip      	: TEXCOORD3;
	float leftY     	: TEXCOORD4;
	float rightY   		: TEXCOORD5;
	float bottomY   	: TEXCOORD6;
	float topY      	: TEXCOORD7;
};

[domain("tri")]
PixelInput TerrainTessellationTriDS(PatchConstOutput input, float3 uv : SV_DomainLocation, const OutputPatch<DomainInput, 3> inputPatch)
{
	PixelInput output;
	
	// Bilinear interpolation.
	
	// Find the position of the new vertex
	float3 vPos = inputPatch[0].position * uv.x + inputPatch[1].position * uv.y + inputPatch[2].position * uv.z;
	
	// Find the normal of the new vertex
	//float3 vNormal = inputPatch[0].normal * uv.x + inputPatch[1].normal * uv.y + inputPatch[2].normal * uv.z;
	
	// Find the texture coordinate of the new vertex
	float2 vTex = inputPatch[0].uv * uv.x + inputPatch[1].uv * uv.y + inputPatch[2].uv * uv.z;
	
	// Send the normal to the pixel shader
	//output.normal = vNormal;
	
	// Send the texture uv to the pixel shader
	output.uv = vTex;
	
	// Tile layer textures over terrain.
	output.TiledTex = output.uv * g_TexScale;
	
	// Sample the height for this vertex
	float4 SampleValue = g_HeightMap.SampleLevel( samHeightmap, output.uv, 0 );
	vPos.y = ((SampleValue.r * g_HeightScale * 100.0) / 255.0) / ((g_TerrainZScale * 2) + 1);
	
	// Calculate independent world position
	output.worldPos = mul(float4(vPos, 1.0f), g_WorldMatrix);
	
	// Get the non-homogenous (untransformed) position of the final vertex
	output.positionW = vPos;
	
	// Calculate the new position of the new vertex with WVP matrices
	output.position = mul(float4(vPos, 1.0f), g_WorldMatrix);
	output.position = mul(output.position, g_ViewMatrix);
	output.position = mul(output.position, g_ProjMatrix);
	
	// Determine where this vertex lies on the clipping plane
	output.clip = dot(output.position, g_ClipPlane);
	
	float2 leftTex   = output.uv + float2(-g_TexelCellSpaceU, 0.0f);
	float2 rightTex  = output.uv + float2(g_TexelCellSpaceU, 0.0f);
	float2 bottomTex = output.uv + float2(0.0f, g_TexelCellSpaceV);
	float2 topTex    = output.uv + float2(0.0f, -g_TexelCellSpaceV);
	
	output.leftY   = g_HeightMap.SampleLevel( samHeightmap, leftTex, 0 ).r;
	output.rightY  = g_HeightMap.SampleLevel( samHeightmap, rightTex, 0 ).r;
	output.bottomY = g_HeightMap.SampleLevel( samHeightmap, bottomTex, 0 ).r;
	output.topY    = g_HeightMap.SampleLevel( samHeightmap, topTex, 0 ).r;
	
	return output;
}