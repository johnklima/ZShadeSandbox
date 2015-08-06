//==============================================================================
// Terrain Tessellation Hull Shader for Shadow Mapping
//
//==============================================================================
// History
//
// -Created on 5/14/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cTessellationBuffer : register(b0)
{
	float3 	g_EyePosW;
	int   	g_FrustumCull;
	
	float 	g_MinDist;
	float 	g_MaxDist;
	float 	g_MinTess;
	float 	g_MaxTess;
	
	float2  padding;
	float	g_TerrSize;
	float	g_TerrScale;
};

//======================================================================================================

//
// Globals
//

float DistanceToPoint(float3 p, float3 vmin, float3 vmax)
{
	float dx = max(vmin.x - p.x, p.x - vmax.x);
	float dz = max(vmin.z - p.z, p.z - vmax.z);
	return sqrt(dx * dx + dz * dz);
}

//======================================================================================================

//
// Hull Shader
//

struct HullInput
{
	float3 position		: POSITION;
	//float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

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

void HCTessellate(out PatchConstOutput output, float factor)
{
	output.edges[0] = factor;
	output.edges[1] = factor;
	output.edges[2] = factor;
	
	output.inside = factor;
}

PatchConstOutput ConstShadowHS(InputPatch<HullInput, 3> inputPatch, uint patchID : SV_PrimitiveID)
{
	PatchConstOutput output;
	
	if (g_FrustumCull == 0)
	{
		// Use maximum tessellation value
		HCTessellate(output, g_MaxTess);
	}
	else
	{
		HCTessellate(output, 0.0f);
	}
	
	return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstShadowHS")]
DomainInput TerrainTessellationTriShadowHS(InputPatch<HullInput, 3> inputPatch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	DomainInput output;
	
	output.position = inputPatch[pointID].position;
	
	//output.normal = inputPatch[pointID].normal;
	
	output.uv = inputPatch[pointID].uv;
	
	return output;
}