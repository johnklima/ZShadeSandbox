//==============================================================================
// GBuffer Unpacking Helper
//
//==============================================================================
// History
//
// -Created on 6/7/2015 by Dustin Watson
//==============================================================================

struct GBufferData
{
	float4 	color;
	float3 	normal;
	float 	specularPower;
	float 	specularIntensity;
	float4 	position;
};

//
// Global Functions
//

GBufferData UnpackGBuffer
(	float2 uv
,	matrix invViewProj
,	Texture2D colorTexture
,	Texture2D normalTexture
,	Texture2D depthTexture
,	SamplerState pointSampler
)
{
	float4 colors = colorTexture.Sample(pointSampler, uv);
	
	// Make the color fully opaque
	colors.a = 1.0;
	
	float4 normals = normalTexture.Sample(pointSampler, uv);
	float3 normal = normals.xyz;
	
	float specularPower = normals.w;
	float specularIntensity = colorTexture.Sample(pointSampler, uv).a;
	
	// Sample the depth
	float depth = depthTexture.Sample(pointSampler, uv).r;
	
	// Compute to screen-space position
	float4 position;
	position.x = uv.x * 2.0f - 1.0f;
	position.y = -(uv.y * 2.0f - 1.0f);
	position.z = depth;
	position.w = 1.0;
	
	// Transform to world space
	position = mul(position, invViewProj);
	position /= position.w;
	
	GBufferData data;
	
	data.color = colors;
	data.normal = normal;
	data.specularPower = specularPower;
	data.specularIntensity = specularIntensity;
	data.position = position;
	
	return data;
}

GBufferData UnpackGBuffer
(	float2 uv
,	float2 screenPos
,	matrix invViewProj
,	Texture2D colorTexture
,	Texture2D normalTexture
,	Texture2D depthTexture
,	SamplerState pointSampler
)
{
	float4 colors = colorTexture.Sample(pointSampler, uv);
	
	// Make the color fully opaque
	colors.a = 1.0;
	
	float4 normals = normalTexture.Sample(pointSampler, uv);
	float3 normal = normals.xyz;
	
	float specularPower = normals.w;
	float specularIntensity = colorTexture.Sample(pointSampler, uv).a;
	
	// Sample the depth
	float depth = depthTexture.Sample(pointSampler, uv).r;
	
	// Compute to screen-space position
	float4 position;
	position.xy = screenPos.xy;
	position.z = depth;
	position.w = 1.0;
	
	// Transform to world space
	position = mul(position, invViewProj);
	position /= position.w;
	
	GBufferData data;
	
	data.color = colors;
	data.normal = normal;
	data.specularPower = specularPower;
	data.specularIntensity = specularIntensity;
	data.position = position;
	
	return data;
}