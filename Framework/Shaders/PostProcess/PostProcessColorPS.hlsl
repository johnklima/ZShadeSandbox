//==============================================================================
// Post Processing Pixel Shader that turns the scene into a different color
//
//==============================================================================
// History
//
// -Created on 6/2/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbColorBuffer : register(b0)
{
	float4 g_Color;
};

//======================================================================================================

//
// Structs
//

struct PostProcessPixelInput
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD0;
};

//======================================================================================================

//
// Pixel Shader
//

float4 PostProcessColorPS(PostProcessPixelInput input) : SV_Target
{
	return g_Color;
}