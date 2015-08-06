//==============================================================================
// ShadowMap.hlsl
//
// Creates a shadow color based on a given shadow map and builds a dynamic shadow map given a shader
//==============================================================================
// History
//
// -Created on 4/20/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbShadowMapConst : register(b0)
{
	matrix g_ShadowWorld;
	matrix g_LightView;
	matrix g_LightProj;
};

float g_depthBias = 0.0001f;

//======================================================================================================

//
// Samplers and textures
//

SamplerState ShadowMapSampler
{
	Filter = COMPARISON_MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW  = CLAMP;
};

SamplerComparisonState ShadowMapSamplerComparison
{
	Filter = COMPARISON_MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW  = CLAMP;
};

//======================================================================================================

//
// Global Functions
//

float4 ComputeShadowColor(float4 DiffuseColor, float4 lightingPosition, Texture2D shadowMap)
{
    // Find the position in the shadow map for this pixel
    float2 ShadowTexCoord = 0.5 * lightingPosition.xy / lightingPosition.w + float2( 0.5, 0.5 );
	
    ShadowTexCoord.y = 1.0f - ShadowTexCoord.y;
	
    // Get the current depth stored in the shadow map
    float4 shadowInfo = shadowMap.Sample(ShadowMapSampler, ShadowTexCoord);
    float shadowdepth = shadowInfo.r;
    float shadowOpacity = 0.5f + 0.5f * (1 - shadowInfo.g);
	
    // Calculate the current pixel depth
    // The bias is used to prevent floating point errors that occur when
    // the pixel of the occluder is being drawn
    float ourdepth = (lightingPosition.z / lightingPosition.w) - g_depthBias;
	
    // Check to see if this pixel is in front or behind the value in the shadow map
    if (shadowdepth < ourdepth)
    {
        // Shadow the pixel by lowering the intensity
        DiffuseColor *= float4(shadowOpacity, shadowOpacity, shadowOpacity, 1);
    }
	
    return DiffuseColor;
}

float ComputeShadowColor(float4 lightingPosition, float3 lightPos, float3 normal, Texture2D shadowMap)
{
	lightPos = -lightPos;
	
	float shadowColor = 1.0f;// = float3(1.0f, 1.0f, 1.0f);
	
	// Find the position in the shadow map for this pixel
    float2 ShadowTexCoord = 0.5 * lightingPosition.xy / lightingPosition.w + float2( 0.5, 0.5 );
	
    ShadowTexCoord.y = 1.0f - ShadowTexCoord.y;
	
    // Get the current depth stored in the shadow map
    float4 shadowInfo = shadowMap.Sample(ShadowMapSampler, ShadowTexCoord);
    float shadowdepth = shadowInfo.r;
    float shadowOpacity = 0.5f + 0.5f * (1 - shadowInfo.g);
	
    // Calculate the current pixel depth
    // The bias is used to prevent floating point errors that occur when
    // the pixel of the occluder is being drawn
    float ourdepth = (lightingPosition.z / lightingPosition.w) - g_depthBias;
	
	// Check to see if this pixel is in front or behind the value in the shadow map
    if (shadowdepth < ourdepth)
    {
		//shadowColor = float3(shadowOpacity, shadowOpacity, shadowOpacity);
		shadowColor = shadowOpacity;
		// Calculate the amount of light on this pixel
		float intensity = saturate(dot(normal, lightPos));
		
		if (intensity > 0)
		{
			shadowColor = shadowOpacity;
			//intensity = saturate(intensity);
			//shadowColor = float3(intensity, intensity, intensity);
		}
	}
	
	return shadowColor;
}

float ShadowMapPCH(float3 lightPos, matrix toShadowmap, Texture2D shadowMap)
{
	float4 posShadowMap = mul(float4(lightPos, 1.0), toShadowmap);
	
	float3 UVD = posShadowMap.xyz / posShadowMap.w;
	
	UVD.xy = 0.5 * UVD.xy + 0.5;
	UVD.y = 1.0 - UVD.y;
	
	return shadowMap.SampleCmpLevelZero(ShadowMapSamplerComparison, UVD.xy, UVD.z);
}

float ComputeShadowColor2(float4 lightViewPosition, float3 normal, float3 lightDir, Texture2D shadowMap)
{
	lightDir = -lightDir;
	float bias = 0.001f;
	float lightIntensity = 1.0f;
	
	// Calculate the projected texture coordinates
	float2 projectTexCoord;
	projectTexCoord.x = lightViewPosition.x / lightViewPosition.w / 2.0 + 0.5f;
	projectTexCoord.y = -lightViewPosition.y / lightViewPosition.w / 2.0 + 0.5f;
	
	// Determine if the projected coordinates are in the 0 to 1 range. If so then this pixel is in the view of the light.
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		// Sample the shadow map depth value from the shadow map using the sampler at the projected texture coordinate location
		float depthValue = shadowMap.Sample(ShadowMapSampler, projectTexCoord).r;
		
		// Calculate the depth of the light
		float lightDepthValue = lightViewPosition.z / lightViewPosition.w;
		
		// Subtract the bias from the lightDepthValue
		lightDepthValue = lightDepthValue - bias;
		
		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
        // If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
		if (lightDepthValue < depthValue)
		{
			// Calculate the amount of light on this pixel
			lightIntensity = saturate(dot(normal, lightDir));
			
			if (lightIntensity > 0.0f)
			{
				return lightIntensity;
			}
		}
	}
	else
	{
		// If this is outside the area of shadow map range then draw things normally with regular lighting.
		lightIntensity = saturate(dot(normal, lightDir));
		
		if (lightIntensity > 0.0f)
		{
			return lightIntensity;
		}
	}
	
	return lightIntensity;
}

//======================================================================================================

//
// Shader Helper for buiding a dynamic shadow map
//

struct SMVertexInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD0;
	float3 normal		: NORMAL;
};

struct SMPixelInput
{
	float4 position     : SV_POSITION;
    float4 position2D   : TEXCOORD0;
};

SMPixelInput ShadowMapBuilderVS(SMVertexInput input)
{
	SMPixelInput output;
	
	//position.w = 1.0f;
	
	output.position = mul(float4(input.position, 1.0), g_ShadowWorld);
	output.position = mul(output.position, g_LightView);
	output.position = mul(output.position, g_LightProj);
    output.position2D = output.position;
	
	return output;
}

float4 ShadowMapBuilderPS(SMPixelInput input) : SV_Target
{
	float depthValue;
	float4 color;
	
	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	depthValue = input.position2D.z / input.position2D.w;

	color = float4(depthValue, depthValue, depthValue, 1.0f);

	return color;
}