//==============================================================================
// Ocean Surface Vertex Shader
//
//==============================================================================
// History
//
// -Created on 5/30/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

//======================================================================================================

//
// Structs
//

struct VertexInput
{
	float3 position  : POSITION;
	float2 uv        : TEXCOORD0;
};

struct HullInput
{
	float3 position  : POSITION;
	float2 uv        : TEXCOORD0;
};

//======================================================================================================

//
// Vertex Shader
//

HullInput OceanSurfaceVS(VertexInput input)
{
	HullInput output;
	
	output.uv = input.uv;
	
	output.position = input.position;
	
	return output;
}