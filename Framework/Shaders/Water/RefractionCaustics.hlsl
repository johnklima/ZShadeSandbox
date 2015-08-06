//==============================================================================
// Water Caustic calculation helper
//
// Calculates refraction caustics to use on the surface of what is underwater
//
// http://my-3d-engine.googlecode.com/svn-history/r36/trunk/bin/Data/Shaders/Island11.fx
//==============================================================================
// History
//
// -Created on 5/28/2015 by Dustin Watson
//==============================================================================

static const float g_Amplitude = 0.01f;
static const float g_Frequency = 10.0f;
static const float g_factor = 1.0f;
static const float g_speed = 2.0f;
static const float g_octaves = 5;

float WaterCausticIntensity
(	float3 eye
,	float3 worldPos
, 	float3 lightPosition
,	Texture2D waterNormal0
,	Texture2D waterNormal1
,	SamplerState linearSampler
)
{
	float3 worldpos = worldPos;
	float camera_distance = length(eye - worldpos);
	float3 pixel_to_light_vector = normalize(lightPosition - worldpos);
	float2 refraction_disturbance;
	float3 n;
	float m = 0.2;
	float cc = 0;
	float k = 0.15;
	float water_depth = 0.5 - worldpos.y;
	
	worldpos.xz -= worldpos.y * pixel_to_light_vector.xz;
	
	float3 pixel_to_water_surface_vector = pixel_to_light_vector * water_depth;
	float3 refracted_pixel_to_light_vector;

	// tracing approximately refracted rays back to light
	for(float i = -3; i <= 3;i += 1)
	{
		for(float j = -3; j <= 3;j += 1)
		{
			float3 wn0 = waterNormal0.SampleLevel(linearSampler, (worldpos.xz - eye.xz - float2(200.0, 200.0) + float2(i * k,j * k) * m * water_depth) / 400.0, 0).rgb;
			float3 wn1 = waterNormal1.SampleLevel(linearSampler, (worldpos.xz - eye.xz - float2(200.0, 200.0) + float2(i * k,j * k) * m * water_depth) / 400.0, 0).rgb;
			
			n = (wn0 + wn1) - float3(1.0f, 1.0f, 1.0f);
			
			refracted_pixel_to_light_vector = m * (pixel_to_water_surface_vector + float3(i * k, 0, j * k)) - 0.5 * float3(n.x, 0, n.z);
			
			cc += 0.05 * max(0, pow(max(0, dot(normalize(refracted_pixel_to_light_vector), normalize(pixel_to_light_vector))), 500.0f));
		}
	}
	
	return cc;
}

float MicroBumpNormalFactor
(	Texture2D microBump0
,	Texture2D microBump1
,	SamplerState linearSampler
,	float2 uv
,	float3 normal
)
{
	float microBumpNormal;
	
	microBumpNormal = normalize(2 * microBump0.Sample(linearSampler, uv).rgb - float3(1.0, 1.0, 1.0));
	microBumpNormal = normalize(lerp(microBumpNormal, 2 * microBump1.Sample(linearSampler, uv).rgb - float3(1.0, 1.0, 1.0), 0.2));
	
	// Calculate the base normal rotation matrix
	float3x3 normal_rotation_matrix;
	normal_rotation_matrix[1] = normal;
	normal_rotation_matrix[2] = normalize(cross(float3(-1.0, 0.0, 0.0), normal_rotation_matrix[1]));
	normal_rotation_matrix[0] = normalize(cross(normal_rotation_matrix[2], normal_rotation_matrix[1]));
	
	microBumpNormal = mul( microBumpNormal, normal_rotation_matrix );
	
	return microBumpNormal;
}

float CausticShadowFactor
(	Texture2D depthTexture
,	SamplerComparisonState depthSampler
,	float3 worldSpacePosition
,	float4x4 lightViewProjMatrix
)
{
	// calculating pixel position in light view space
	float4 positionLS = mul(float4(worldSpacePosition, 1), lightViewProjMatrix);
	positionLS.xyz /= positionLS.w;
	positionLS.x = (positionLS.x + 1) * 0.5;
	positionLS.y = (1 - positionLS.y) * 0.5;
	
	float shadowFactor;
	
	float dsf = 0.75f / 4096.0f;
	//SampleCmpLevelZero
	shadowFactor  = 0.2 * depthTexture.SampleCmp(depthSampler, positionLS.xy, positionLS.z * 0.995f);
	shadowFactor += 0.2 * depthTexture.SampleCmp(depthSampler, positionLS.xy + float2(dsf, dsf), positionLS.z * 0.995f);
	shadowFactor += 0.2 * depthTexture.SampleCmp(depthSampler, positionLS.xy + float2(-dsf, dsf), positionLS.z * 0.995f);
	shadowFactor += 0.2 * depthTexture.SampleCmp(depthSampler, positionLS.xy + float2(dsf, -dsf), positionLS.z * 0.995f);
	shadowFactor += 0.2 * depthTexture.SampleCmp(depthSampler, positionLS.xy + float2(-dsf, -dsf), positionLS.z * 0.995f);
	
	return shadowFactor;
}

float SurfaceRefractionColor
(	float3 eye
,	float3 vertexPosition
, 	float3 lightPosition
,	Texture2D waterNormal0
,	Texture2D waterNormal1
,	SamplerState linearSampler
,	float waterHeightBumpScale// = 1.0f
,	Texture2D microBump0
,	Texture2D microBump1
,	float2 uv
,	float3 normal
,	Texture2D depthTexture
,	SamplerComparisonState depthSampler
,	float4x4 lightViewProjMatrix
)
{
	float camera_distance = length(eye - vertexPosition);
	float3 pixel_to_light_vector = normalize(lightPosition - vertexPosition);
	
	// Fetch the shadowmap shading for the position the light does not touch
	float3 microBumpNormal = MicroBumpNormalFactor(microBump0, microBump1, linearSampler, uv, normal);
	float shadowFactor = CausticShadowFactor(depthTexture, depthSampler, vertexPosition, lightViewProjMatrix);
	float finalColor = max(0, dot(pixel_to_light_vector, microBumpNormal)) * shadowFactor + 0.2;
	
	// Add some light color
	finalColor += (0.2 * max(0, (dot(float3(0, 1, 0), microBumpNormal)))) * float3(0.2, 0.2, 0.3);
	
	// Make it look brighter and the wet surface appear darker than normal
	finalColor *= 0.5 + 0.8 * max(0, min(1, vertexPosition.y * 0.5 + 0.5));
	
	return finalColor;
}

float2 GradiantWave(float x, float y, float timer)
{
	float dZx = 0.0f;
	float dZy = 0.0f;
	float octaves = g_octaves;
	float factor = g_factor;
	float d = sqrt(x * x + y * y);
	
	do
	{
		dZx += d * sin(timer * g_speed + (1 / factor) * x * y * g_Frequency) * y * g_Frequency - factor * cos(timer * g_speed + (1 / factor) * x * y * g_Frequency) * x / d;
		dZy += d * sin(timer * g_speed + (1 / factor) * x * y * g_Frequency) * x * g_Frequency - factor * cos(timer * g_speed + (1 / factor) * x * y * g_Frequency) * y / d;
		factor = factor / 2;
		octaves--;
	} while (octaves > 0);
	
	return float2(2 * g_Amplitude * dZx, 2 * g_Amplitude * dZy);
}

float3 line_plane_intercept(float3 lineP, float3 lineN, float3 planeN, float3 planeD)
{
	float distance = (planeD - lineP.z) / lineN.z;
	return lineP + lineN * distance;
}

float3 CausticDance(float4 positionV, Texture2D lightMap, SamplerState linearSampler, float timer)
{
	float2 dxdy = GradiantWave(positionV.x, positionV.y, timer);
	
	float3 intercept = line_plane_intercept(positionV.xyz, float3(dxdy, saturate(positionV.w)), float3(0, 0, 1), -0.8);
	
	return lightMap.Sample(linearSampler, intercept.xy * 0.8);
}

// gets the final refraction caustic color for a surface
float RefractionCaustics
(	float3 eye
,	float3 vertexPosition
, 	float3 lightPosition
,	Texture2D waterNormal0
,	Texture2D waterNormal1
,	SamplerState linearSampler
,	float waterHeightBumpScale// = 1.0f
,	Texture2D microBump0
,	Texture2D microBump1
,	float2 uv
,	float3 normal
,	Texture2D depthTexture
,	SamplerComparisonState depthSampler
,	float4x4 lightViewProjMatrix
)
{
	float camera_distance = length(eye - vertexPosition);
	float3 pixel_to_light_vector = normalize(lightPosition - vertexPosition);
	
	// Fetch the shadowmap shading for the position the light does not touch
	float3 microBumpNormal = MicroBumpNormalFactor(microBump0, microBump1, linearSampler, uv, normal);
	float shadowFactor = CausticShadowFactor(depthTexture, depthSampler, vertexPosition, lightViewProjMatrix);
	float finalCausticColor = max(0, dot(pixel_to_light_vector, microBumpNormal)) * shadowFactor + 0.2;
	
	// Add some light color to the caustics
	finalCausticColor += (0.2 * max(0, (dot(float3(0, 1, 0), microBumpNormal)))) * float3(0.2, 0.2, 0.3);
	
	// Make it look brighter and the wet surface appear darker than normal
	finalCausticColor *= 0.5 + 0.8 * max(0, min(1, vertexPosition.y * 0.5 + 0.5));
	
	//
	// Calculate the final refraction caustics
	//
	
	float cc = WaterCausticIntensity(eye, vertexPosition, lightPosition, waterNormal0, waterNormal1, linearSampler);
	
	// Fade caustics out at distance
	cc *= (200.0 / (200.0 + camera_distance));
	
	// Fade caustics close to the water surface
	cc *= min(1, max(0, -waterHeightBumpScale - vertexPosition.y));
	
	finalCausticColor *= (1.0 + max(0, 0.4 + 0.6 * dot(pixel_to_light_vector, microBumpNormal)) * cc * (0.4 + 0.6 * shadowFactor));
	
	return finalCausticColor;
}


float ComputeShadowColor(float4 lightViewPosition, float3 normal, float3 lightDir, Texture2D shadowMap, SamplerState depthSampler)
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
		float depthValue = shadowMap.Sample(depthSampler, projectTexCoord).r;
		
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

float SampleCaustic
(	Texture2D waterNormal0
,	Texture2D waterNormal1
,	Texture2D microBump0
,	Texture2D microBump1
,	Texture2D causticTexture
,	SamplerState linearSampler
,	SamplerComparisonState depthSampler
,	float2 uv
,	float3 eye
,	float3 vertexPosition
,	float4 lightViewPosition
,	float3 lightDir
)
{
	float shadowFactor = ComputeShadowColor(lightViewPosition, float3(0, 1, 0), lightDir, causticTexture, linearSampler);
	
	float waterHeightBumpScale = 1.0f;
	
	float camera_distance = length(eye - vertexPosition);
	float3 pixel_to_light_vector = normalize(lightDir - vertexPosition);
	
	// Fetch the shadowmap shading for the position the light does not touch
	float3 microBumpNormal = MicroBumpNormalFactor(microBump0, microBump1, linearSampler, uv, float3(0, 1, 0));
	
	float finalCausticColor = max(0, dot(pixel_to_light_vector, microBumpNormal)) * shadowFactor + 0.2;
	
	// Add some light color to the caustics
	finalCausticColor += (0.2 * max(0, (dot(float3(0, 1, 0), microBumpNormal)))) * float3(0.2, 0.2, 0.3);
	
	// Make it look brighter and the wet surface appear darker than normal
	finalCausticColor *= 0.5 + 0.8 * max(0, min(1, vertexPosition.y * 0.5 + 0.5));
	
	//
	// Calculate the final refraction caustics
	//
	
	float cc = WaterCausticIntensity(eye, vertexPosition, lightDir, waterNormal0, waterNormal1, linearSampler);
	
	// Fade caustics out at distance
	cc *= (200.0 / (200.0 + camera_distance));
	
	// Fade caustics close to the water surface
	cc *= min(1, max(0, -waterHeightBumpScale - vertexPosition.y));
	
	finalCausticColor *= (1.0 + max(0, 0.4 + 0.6 * dot(pixel_to_light_vector, microBumpNormal)) * cc * (0.4 + 0.6 * shadowFactor));
	
	return finalCausticColor;
}
