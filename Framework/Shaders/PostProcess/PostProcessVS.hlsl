//==============================================================================
// Post Processing Vertex Shader
//
//==============================================================================
// History
//
// -Created on 6/2/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

//======================================================================================================

//
// Structs
//

struct PostProcessPixelInput
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD0;
};

static const float2 arrPos[4] =
{
	float2(-1.0,  1.0),
	float2( 1.0,  1.0),
	float2(-1.0, -1.0),
	float2( 1.0, -1.0)
};

static const float2 arrUV[4] =
{
	float2(0.0, 0.0),
	float2(1.0, 0.0),
	float2(0.0, 1.0),
	float2(1.0, 1.0)
};

//======================================================================================================

//
// Vertex Shader for a fullscreen quad
//

PostProcessPixelInput PostProcessQuadVS(uint vertexID : SV_VertexID)
{
	PostProcessPixelInput output;
	
	//output.position = float4(arrPos[vertexID].xy, 0, 1);
	//output.uv = arrUV[vertexID].xy;
	
	output.uv = float2((vertexID << 1) & 2, vertexID & 2);
	output.position = float4(output.uv * float2(2, -2) + float2(-1, 1), 0, 1);
	
	return output;
}