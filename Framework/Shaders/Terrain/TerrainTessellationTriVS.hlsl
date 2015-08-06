//==============================================================================
// Tessellation Texture Vertex Shader
//
//==============================================================================
// History
//
// -Created on 4/18/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

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
// Vertex Shader
//

struct VertexInput
{
	float3 position		: POSITION;
	//float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

struct HullInput
{
	float3 position		: POSITION;
	//float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

HullInput TerrainTessellationTriVS(VertexInput input)
{
	HullInput output;
	
	// Pass vertex position into the hull shader
	output.position = input.position;
	
	// Displace the patch corners to world space.  This is to make 
	// the eye to patch distance calculation more accurate.
	output.position.y = g_HeightMap.SampleLevel( samHeightmap, input.uv, 0 ).r;
	
	//output.normal = input.normal;
	
	// Pass texture uv into the hull shader
	output.uv = input.uv;
	
	return output;
}