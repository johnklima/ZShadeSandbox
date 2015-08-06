//==============================================================================
// Ocean Surface Hull Shader
//
//==============================================================================
// History
//
// -Created on 5/30/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbTessellationBuffer : register(b0)
{
	float3 	g_EyePosW;
	int	  	g_FrustumCull;
	float 	g_MinDist;
	float 	g_MaxDist;
	float 	g_MinTess;
	float 	g_MaxTess;
};

//======================================================================================================

//
// Hull Shader
//

struct HullInput
{
	float3 position		: POSITION;
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
	float2 uv			: TEXCOORD0;
};

float CalcTessFactor(float3 p)
{
	float d = distance(p, g_EyePosW);

	// max norm in xz plane (useful to see detail levels from a bird's eye).
	//float d = max( abs(p.x - gEyePosW.x), abs(p.z - gEyePosW.z) );
	
	float s = saturate( (d - g_MinDist) / (g_MaxDist - g_MinDist) );
	
	return pow(2, (lerp(g_MaxTess, g_MinTess, s)) );
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

void HCTessellate(out PatchConstOutput output, float tessFactor)
{
	output.edges[0] = tessFactor;
	output.edges[1] = tessFactor;
	output.edges[2] = tessFactor;
	output.edges[3] = tessFactor;
	output.inside[0] = tessFactor;
	output.inside[1] = tessFactor;
}

PatchConstOutput ConstHSQuad(InputPatch<HullInput, 4> inputPatch, uint patchID : SV_PrimitiveID)
{
	PatchConstOutput output;
	
	if (g_FrustumCull == 0)
	{
		// Compute midpoint on edges, and patch center
		float3 e0 = 0.5f * (inputPatch[0].position + inputPatch[2].position);
		float3 e1 = 0.5f * (inputPatch[0].position + inputPatch[1].position);
		float3 e2 = 0.5f * (inputPatch[1].position + inputPatch[3].position);
		float3 e3 = 0.5f * (inputPatch[2].position + inputPatch[3].position);
		float3  c = 0.25f * (inputPatch[0].position + inputPatch[1].position + inputPatch[2].position + inputPatch[3].position);
		
		HCTessellate(output, e0, e1, e2, e3, c);
	}
	else
	{
		HCTessellate(output, -1);
	}
	
	return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstHSQuad")]
DomainInput OceanSurfaceHS(InputPatch<HullInput, 4> inputPatch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	DomainInput output;
	
	output.position = inputPatch[pointID].position;
	
	output.uv = inputPatch[pointID].uv;
	
	return output;
}