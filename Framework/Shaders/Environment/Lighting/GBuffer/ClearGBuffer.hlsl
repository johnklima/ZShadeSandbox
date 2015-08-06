//==============================================================================
// Clears the GBuffer values
//
// http://www.catalinzima.com/xna/tutorials/deferred-rendering-in-xna/creating-the-g-buffer/
//==============================================================================
// History
//
// -Created on 6/4/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

//======================================================================================================

//
// Samplers and textures
//

//======================================================================================================

//
// Structs
//

struct PixelInput
{
	float4 position	: SV_POSITION;
};

struct PixelOutput
{
	float4 color 	: SV_Target0;
	float4 normal 	: SV_Target1;
	float4 depth	: SV_Target2;
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
// Vertex Shader
//

PixelInput ClearGBufferVS(uint vertexID : SV_VertexID)
{
	PixelInput output;
	
	output.position = float4(arrPos[vertexID].xy, 0, 1);
	
	return output;
}

//======================================================================================================

//
// Pixel Shader
//

PixelOutput ClearGBufferPS(PixelInput input) : SV_Target
{
	PixelOutput output;
	
	output.color = float4(0, 0, 0, 0);
	
	output.normal = float4(0.5f, 0.5f, 0.5f, 0);
	
	output.depth = float4(1, 0, 0, 0);
	
	return output;
}