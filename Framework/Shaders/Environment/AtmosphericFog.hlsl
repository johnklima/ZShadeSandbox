//==============================================================================
// Atmospheric Fog helper
//
// http://my-3d-engine.googlecode.com/svn-history/r36/trunk/bin/Data/Shaders/Island11.fx
//==============================================================================
// History
//
// -Created on 5/29/2015 by Dustin Watson
//==============================================================================

// darkColor = float3(0.6, 0.6, 0.7)
// brightColor = float3(1.0, 1.1, 1.4)
float3 AtmosphericFogColor(float3 vertexPosition, float3 lightPosition, float3 eye, float3 darkColor, float3 brightColor)
{
	float cam_dist = length(eye - vertexPosition);
	float3 pixel_to_eye_vector = normalize(eye - vertexPosition);
	float3 pixel_to_light_vector = normalize(lightPosition - vertexPosition);
	
	return lerp(float4(darkColor, 1), float4(brightColor, 1), 0.5 * dot(pixel_to_light_vector, -pixel_to_eye_vector) + 0.5);
}
