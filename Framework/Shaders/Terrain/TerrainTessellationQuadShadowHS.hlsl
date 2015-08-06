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
	float 	g_TessellationFactor;
	
	// Test the camera frustum against the terrain
	float4  g_WorldFrustumPlanes[6];
	
	float 	g_MinDist;
	float 	g_MaxDist;
	float 	g_MinTess;
	float 	g_MaxTess;
	
	// AABB of the current node rendered
	float3  g_BoxMin;
	int   	g_FrustumCull;
	float3  g_BoxMax;
	int   	g_UseTessellation;
	
	float2  padding;
	float	g_TerrSize;
	float	g_TerrScale;
};

//======================================================================================================

//
// Globals
//

// Returns true if the box is completely behind (in negative half space) of plane.
bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane)
{
	float3 n = abs(plane.xyz);
	
	// This is always positive.
	float r = dot(extents, n);
	
	// signed distance from center point to plane.
	float s = dot( float4(center, 1.0f), plane );
	
	// If the center point of the box is a distance of e or more behind the
	// plane (in which case s is negative since it is behind the plane),
	// then the box is completely in the negative half space of the plane.
	return (s + r) < 0;//-(g_TerrSize * 100.0f);
}

// Returns true if the box is completely outside the frustum.
bool AabbOutsideFrustumTest(float3 center, float3 extents, float4 frustumPlanes[6])
{
	for(int i = 0; i < 6; ++i)
	{
		// If the box is completely behind any of the frustum planes
		// then it is outside the frustum.
		if( AabbBehindPlaneTest(center, extents, frustumPlanes[i]) )
		{
			return true;
		}
	}
	
	return false;
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
	float edges[4]		: SV_TessFactor;
	float inside[2]		: SV_InsideTessFactor;
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
	output.edges[3] = factor;
	output.inside[0] = factor;
	output.inside[1] = factor;
}

PatchConstOutput ConstShadowHS(InputPatch<HullInput, 4> inputPatch, uint patchID : SV_PrimitiveID)
{
	PatchConstOutput output;
	
	float3 boxCenter  = 0.5f * (g_BoxMin + g_BoxMax);
	float3 boxExtents = 0.5f * (g_BoxMax - g_BoxMin);
	
	bool cull = AabbOutsideFrustumTest(boxCenter, boxExtents, g_WorldFrustumPlanes);
	
	if (g_UseTessellation == 1)
	{
		if( cull == true )
			HCTessellate(output, 0.0f);
		else
		{
			// Use maximum tessellation value
			HCTessellate(output, g_MaxTess);
		}
	}
	else
	{
		HCTessellate(output, g_MaxTess);
	}
	
	return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstShadowHS")]
DomainInput TerrainTessellationQuadShadowHS(InputPatch<HullInput, 4> inputPatch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	DomainInput output;
	
	output.position = inputPatch[pointID].position;
	
	//output.normal = inputPatch[pointID].normal;
	
	output.uv = inputPatch[pointID].uv;
	
	return output;
}