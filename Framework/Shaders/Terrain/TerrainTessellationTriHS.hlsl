//==============================================================================
// Terrain Tessellation Hull Shader
//
//==============================================================================
// History
//
// -Created on 4/18/2015 by Dustin Watson
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

float CalcTessFactor(float3 p)
{
	float d = distance(p, g_EyePosW);

	// max norm in xz plane (useful to see detail levels from a bird's eye).
	//float d = max( abs(p.x - gEyePosW.x), abs(p.z - gEyePosW.z) );
	
	float s = saturate( (d - g_MinDist) / (g_MaxDist - g_MinDist) );
	
	return pow(2, (lerp(g_MaxTess, g_MinTess, s)) );
}

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
	
	if (g_FrustumCull == 0)
	{
		float3 WorldPosition0 = inputPatch[0].position;
		float3 WorldPosition1 = inputPatch[1].position;
		float3 WorldPosition2 = inputPatch[2].position;
		
		// Compute midpoint on edges, and patch center
		float3 e0 = 0.5f * (WorldPosition0 + WorldPosition2);
		float3 e1 = 0.5f * (WorldPosition0 + WorldPosition1);
		float3 e2 = 0.5f * (WorldPosition1 + WorldPosition0);
		
		float3  c = 0.25f * (WorldPosition0 + WorldPosition1 + WorldPosition2);
		
		HCTessellate(output, e0, e1, e2, c);
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
[patchconstantfunc("ConstHS")]
DomainInput TerrainTessellationTriHS(InputPatch<HullInput, 3> inputPatch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	DomainInput output;
	
	output.position = inputPatch[pointID].position;
	
	//output.normal = inputPatch[pointID].normal;
	
	output.uv = inputPatch[pointID].uv;
	
	return output;
}