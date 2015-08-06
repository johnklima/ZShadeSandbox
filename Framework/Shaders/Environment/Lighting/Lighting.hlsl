//==============================================================================
// Lighting
//
// An Emissive Shader is a mesh that represents the light source
//==============================================================================
// History
//
// -Created on 4/20/2015 by Dustin Watson
//==============================================================================

// Maximum amount of lights that can be seen in the scene
#define MAX_LIGHTS  16

struct AmbientLight
{
	float4	g_AmbientColor;
	float3	ambientpadding;
	int		g_Toggle;
};

struct DirectionalLight
{
	float3	g_LightDirection;
	float	g_Intensity;
	float4	g_AmbientColor;
	float4	g_DiffuseColor;
	float3	directionalpadding;
	int		g_Toggle;
};

struct SpotLight
{
	float4	g_AmbientColor;
	float4	g_DiffuseColor;
	float3	g_LightPosition;
	float	g_Intensity;
	float	g_LightRange;
	float	g_SpotCosOuterCone;
	float	g_SpotInnerConeReciprocal;
	float	g_CosineAngle;
	float3	spotpadding;
	int		g_Toggle;
};

struct PointLight
{
	float3	g_LightPosition;
	float	g_LightRange;
	float3	g_Attenuation;
	float	g_Intensity;
	float4	g_AmbientColor;
	float4	g_DiffuseColor;
	float3	pointpadding;
	int		g_Toggle;
};

struct CapsuleLight
{
	float3 	g_LightPosition;
	float 	g_LightRange;
	float3 	g_LightDirection;
	float 	g_LightLength;
	float3 	g_CapsuleDirectionLength;
	float 	g_CapsuleIntensity;
	float4	g_AmbientColor;
	float4	g_DiffuseColor;
	float3	capsulepadding;
	int		g_Toggle;
};

cbuffer cbLightBuffer : register(b0)
{
	AmbientLight	 g_AmbientLight[MAX_LIGHTS];
	DirectionalLight g_DirectionalLight[MAX_LIGHTS];
	SpotLight		 g_SpotLight[MAX_LIGHTS];
	PointLight		 g_PointLight[MAX_LIGHTS];
	CapsuleLight	 g_CapsuleLight[MAX_LIGHTS];
	
	int				 g_DirectionalLightCount;
	int				 g_SpotLightCount;
	int				 g_PointLightCount;
	int              g_CapsuleLightCount;
	
	float3			 g_AmbientDown;
	int				 g_AmbientLightCount;
	float3			 g_AmbientUp;
	float			 lightpadding;
};

cbuffer cbSunLightBuffer : register(b1)
{
	float3		g_SunDir;
	int			g_EnableSun;
	float4		g_SunDiffuseColor;
	float3		sunpadding1;
	float		g_SunShineness;
	float3		sunpadding2;
	float		g_SunIntensity;
};

//
// Global Functions
//

float DotProduct(float3 pos, float3 pos3D, float3 normal)
{
    float3 dir = normalize(pos3D - pos);
    return dot(-dir, normal);    
}

float4 DepthColor(float4 pos2D)
{
	return pos2D.z / pos2D.w;
}

float4 dot4x4(float4 aX, float4 aY, float4 aZ, float4 bX, float4 bY, float4 bZ)
{
	return aX * bX + aY * bY + aZ * bZ;
}

float4 dot4x1(float4 aX, float4 aY, float4 aZ, float3 b)
{
	return aX * b.xxxx + aY * b.yyyy + aZ * b.zzzz;
}

//==============================================================================
// Ambient Light

float4 DiffuseColorToLinearSpace(float4 diffuseColor)
{
	return float4(diffuseColor.rgb * diffuseColor.rgb, diffuseColor.a);
}

// Calculate the ambient value of a given pixel
float3 CalculateAmbientColor(float3 normal, float3 diffuseColor, float3 ambientDown, float3 ambientUp)
{
	// Convert from [-1, 1] to [0, 1]
	float up = normal.y * 0.5 + 0.5;
	
	// Calculate the ambient value
	float3 ambient = ambientDown + up * ambientUp;
	
	// Apply the ambient color to the diffuse color
	return ambient * diffuseColor;
}

float3 FinalAmbientColor(float3 normal, float3 diffuseColor, float3 ambientDown, float3 ambientUp)
{
	float3 ambientColor = DiffuseColorToLinearSpace(float4(diffuseColor, 1)).rgb;
	
	// Convert from [-1, 1] to [0, 1]
	float up = normal.y * 0.5 + 0.5;
	
	// Calculate the ambient value
	float3 ambient = ambientDown + up * ambientUp;
	
	// Apply the ambient color to the diffuse color
	return ambientColor + (ambient * diffuseColor);
}

//==============================================================================
// Directional Light (sun or moon)
// When time of day is dynamic we need multiple values for different parts of the day.
// To do this, pick values for a group of specific times in the day/night and interpolate
// between them and they have to match the sky rendering.
// The direction is the inversed direction (direction to the light).
// To calculate negate the light direction.
// Normal, DiffuseColor, specularExp and specularIntensity is defined as the material.

// Blinn-Phong Directional Light
float3 DirectionalLightColor
(	float3 pixelPosition
,   float3 eyePosition
,   float3 lightDirection
,	float lightIntensity
,   float3 directionLightColor
,   float3 materialNormal
,   float4 materialDiffuseColor
,   float materialSpecularExponent
,   float materialSpecularIntensity
,   float shadowColor
)
{
	//
	// Phong diffuse
	//
	
	float NDotL = dot(materialNormal, lightDirection);
	float3 lightColor = directionLightColor.rgb * saturate(NDotL);
	
	//
	// Blinn specular
	//
	
	float3 toEye = eyePosition.xyz - pixelPosition;
	toEye = normalize(toEye);
	float3 halfWay = normalize(toEye + lightDirection);
	float NDotH = saturate(dot(halfWay, materialNormal));
	lightColor += directionLightColor.rgb * pow(NDotH, materialSpecularExponent) * materialSpecularIntensity;// * shadowColor;
	
	return lightColor * materialDiffuseColor.rgb * lightIntensity;
}
//==============================================================================
// Point Light
// Emits light equally in all directions (light bulb or torch)
// The position is the center of the light source.
// The range is the furthest distance light can travel from the source.

float3 PointLightColor
(	float3 pixelPosition
,   float3 eyePosition
,   float3 pointColor
,   float3 lightPosition
,	float lightIntensity
,   float pointLightRangeReciprocal
,   float3 materialNormal
,   float4 materialDiffuseColor
,   float materialSpecularExponent
,   float materialSpecularIntensity
)
{
	float3 toLight = lightPosition.xyz - pixelPosition;
	float3 toEye = eyePosition.xyz - pixelPosition;
	float distToLight = length(toLight);
	
	//
	// Phong Diffuse
	//
	
	toLight /= distToLight;
	float NDotL = saturate(dot(toLight, materialNormal));
	float3 lightColor = pointColor * NDotL;
	
	//
	// Blinn specular
	//
	
	toEye = normalize(toEye);
	float3 halfWay = normalize(toEye + toLight);
	float NDotH = saturate(dot(halfWay, materialNormal));
	lightColor += pointColor * pow(NDotH, materialSpecularExponent) * materialSpecularIntensity;
	
	//
	// Attenuation
	//
	
	float distToLightNormal = 1.0 - saturate(distToLight * pointLightRangeReciprocal);
	float attenuation = distToLightNormal * distToLightNormal;
	lightColor *= materialDiffuseColor.rgb * attenuation * lightIntensity;
	
	return lightColor;
}
//==============================================================================
// Spot Light

// Blinn-Phong Spotlight
float3 SpotLightColor
(	float3 pixelPosition
,	float3 eyePosition
,	float3 spotColor
,   float3 spotLightPosition
,	float lightIntensity
,	float spotLightRangeReciprocal
,	float3 spotLightDirection // Inverted Direction (Not Used)
,	float spotCosOuterCone
,	float spotInnerConeReciprocal
,	float cosAng
,   float3 materialNormal
,   float4 materialDiffuseColor
,   float materialSpecularExponent
,   float materialSpecularIntensity
)
{
	float3 toLight = spotLightPosition - pixelPosition;
	float3 toEye = eyePosition.xyz - pixelPosition;
	float distToLight = length(toLight);
	
	//
	// Phong Diffuse
	//
	
	toLight /= distToLight;
	float NDotL = saturate(dot(toLight, materialNormal));
	float3 lightColor = spotColor * NDotL;
	
	//
	// Blinn specular
	//
	
	toEye = normalize(toEye);
	float3 halfWay = normalize(toEye + toLight);
	float NDotH = saturate(dot(halfWay, materialNormal));
	lightColor += spotColor * pow(NDotH, materialSpecularExponent) * materialSpecularIntensity;
	
	//
	// Cone Attenuation
	//
	
	float coneAtt = saturate((cosAng - spotCosOuterCone) * spotInnerConeReciprocal);
	coneAtt *= coneAtt;
	
	//
	// Attenuation
	//
	
	float distToLightNormal = 1.0 - saturate(distToLight * spotLightRangeReciprocal);
	float attenuation = distToLightNormal * distToLightNormal;
	lightColor *= materialDiffuseColor.rgb * attenuation * coneAtt * lightIntensity;
	
	return lightColor;
}

// diffuse: Diffuse color of the object to light
float4 SpotLightColor
(	float3 worldPos
, 	float3 normal
,	float4 diffuse
,	float3 lightPosition
,	float  lightRange
,	float3 lightDirection
,	float  lightCone
,	float3 lightAttenuation
,	float4 lightAmbient
,	float4 lightDiffuse
)
{
	float3 lightColor = float3(0.0f, 0.0f, 0.0f);
	
	// Create a vector from the light position and pixel position
	float3 lightPixelVec = lightPosition - worldPos;
	
	// Find the distance between the light and pixel
	float d = length(lightPixelVec);
	
	// Add ambient light
	float3 ambientColor = diffuse.rgb * lightAmbient.rgb;
	
	// If pixel is too far then return pixel color with ambient light
	if (d > lightRange)
	{
		return float4(ambientColor, diffuse.w);
	}
	
	// Turn lightPixelVec into a unit vector
	lightPixelVec /= d;
	
	// Calculate how much light the pixel gets by the angle striking the pixel surface
	float intensity = dot(lightPixelVec, normal);
	
	// If the light is striking front of the pixel
	if (intensity > 0.0f)
	{
		lightColor += diffuse.rgb * lightDiffuse.rgb;
		
		// Light falloff factor
		lightColor /= (lightAttenuation.x + (lightAttenuation.y * d)) + (lightAttenuation.z * (d * d));
		
		// Calculate falloff from center to the edge of a point light cone
		lightColor *= pow(max(dot(-lightPixelVec, lightDirection), 0.0f), lightCone);
	}
	
	// Make sure the values fall between 1 and 0
	lightColor = saturate(lightColor + ambientColor);
	
	return float4(lightColor, diffuse.w);
}
//==============================================================================
// Capsule Light (Fluorescent tubes or lightsaber)
// Light source shaped like a capsule

// Blinn-Phong CapsuleLight
float3 CapsuleLightColor
(	float3 pixelPosition
,	float3 eyePosition
,	float3 lightPosition
,	float lightRange
,	float lightRangeReciprocal
,	float3 lightDirection
,	float lightLength
,	float3 lightColor
,	float3 capsuleDirectionLength
,	float capsuleIntensity
,   float3 materialNormal
,   float4 materialDiffuseColor
,   float materialSpecularExponent
,   float materialSpecularIntensity
)
{
	lightDirection = -lightDirection;
	
	float3 toEye = eyePosition.xyz - pixelPosition;
	
	//
	// Find the shortest distance between the pixel and capsules segment
	//
	
	float3 toCapsuleStart = pixelPosition - lightPosition;
	float distOnLine = dot(toCapsuleStart, capsuleDirectionLength.xyz) / lightRange;
	distOnLine = saturate(distOnLine) * lightRange;
	float3 pointOnLine = lightPosition + lightDirection * distOnLine;
	float3 toLight = pointOnLine - pixelPosition;
	float distToLight = length(toLight);
	
	//
	// Phong Diffuse
	//
	
	toLight /= distToLight;
	float NDotL = saturate(dot(toLight, materialNormal));
	float3 finalLightColor = materialDiffuseColor.rgb * NDotL;
	
	//
	// Blinn Specular
	//
	
	toEye = normalize(toEye);
	float3 halfWay = normalize(toEye + toLight);
	float NDotH = saturate(dot(halfWay, materialNormal));
	finalLightColor += pow(NDotH, materialSpecularExponent) * materialSpecularIntensity;
	
	//
	// Attenuation
	//
	
	float distToLightNormal = 1.0 - saturate(distToLight * lightRangeReciprocal);
	float attenuation = distToLightNormal * distToLightNormal;
	finalLightColor *= lightColor.rgb * capsuleIntensity * attenuation;
	
	return finalLightColor;
}
//==============================================================================
// Projected Texture - Point Light

// Filter			D3D11_FILTER_MIN_MAG_MIP_LINEAR
// AddressU			D3D11_TEXTURE_ADDRESS_WRAP
// AddressV			D3D11_TEXTURE_ADDRESS_WRAP
// AddressW			D3D11_TEXTURE_ADDRESS_WRAP
// MaxAnisotropy	1
// ComparisonFunc	D3D11_COMPARISON_ALWAYS
// MaxLOD			D3D11_FLOAT32_MAX

SamplerState linearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	MaxAnisotropy = 1;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

// This is done in the vertex shader
float3 DirToLight(float4x4 lightTransform, float3 worldPos)
{
	float3 toLight = lightTransform[3].xyz + worldPos;
	return mul(toLight.xyz, (float3x3)lightTransform);
}

// Pass result of this function into the PointLightColor function as the pointColor
float3 ProjTexPointLightColor(float3 direction, float pointIntensity, Texture2D projLightCubeMapTex)
{
	return pointIntensity * projLightCubeMapTex.Sample(linearSampler, direction).rgb;
}
//==============================================================================
// Projected Texture - Spot Light

// Called in the vertex shader with worldPosition.w = 1
float2 ProjectionPosition(float4 worldPosition, matrix lightViewProjection)
{
	float3 projTexXYW = mul(worldPosition, lightViewProjection).xyw;
	
	// Perspective Correction
	projTexXYW.xy /= projTexXYW.z;
	
	// Normalized Space
	float2 uv = (projTexXYW.xy + 1.0.xx) * float2(0.5, -0.5);
	
	return uv;
}

// Pass result of this function into the SpotLightColor function as the spotColor
float3 ProjTexSpotLightColor(float2 uv, float spotLightIntensity, Texture2D projLightTex)
{
	return spotLightIntensity * projLightTex.Sample(linearSampler, uv).rgb;
}
//==============================================================================
// Multiple Lights - Four Lights

// float4 contains four light sources
// can use more than four lights also because you are not limited to just four lights but it will need to be coded
float3 FourLightSourceColor
(	float4 lightPositionX
,	float4 lightPositionY
,	float4 lightPositionZ
,	float4 lightDirectionX
,	float4 lightDirectionY
,	float4 lightDirectionZ
,	float4 lightColorR
,	float4 lightColorG
,	float4 lightColorB
,	float4 lightRange
,	float4 capsuleLength 			 // Store as 0 if not using
,	float4 spotCosOuterCone 		 // Store as -2 if not using
,	float spotCosInnerConeReciprocal // Store as 1 if not using
,	float lightRangeReciprocal
,	float3 pixelPosition
,	float3 eyePosition
,   float3 materialNormal
,   float4 materialDiffuseColor
,   float materialSpecularExponent
,   float materialSpecularIntensity
)
{
	float3 toEye = eyePosition.xyz - pixelPosition;
	
	//
	// Find the shortest distance between the pixel and capsules segment
	//
	
	float4 toCapsuleStartX = pixelPosition.xxxx - lightPositionX;
	float4 toCapsuleStartY = pixelPosition.yyyy - lightPositionY;
	float4 toCapsuleStartZ = pixelPosition.zzzz - lightPositionZ;
	float4 distOnLine = dot4x4(toCapsuleStartX, toCapsuleStartY, toCapsuleStartZ, lightDirectionX, lightDirectionY, lightDirectionZ);
	float4 capsuleLengthSafe = max(capsuleLength, 1.e-6);
	distOnLine = capsuleLength * saturate(distOnLine / capsuleLengthSafe);
	float4 pointOnLineX = lightPositionX + lightDirectionX * distOnLine;
	float4 pointOnLineY = lightPositionY + lightDirectionY * distOnLine;
	float4 pointOnLineZ = lightPositionZ + lightDirectionZ * distOnLine;
	float4 toLightX = pointOnLineX - pixelPosition.xxxx;
	float4 toLightY = pointOnLineY - pixelPosition.yyyy;
	float4 toLightZ = pointOnLineZ - pixelPosition.zzzz;
	float4 distToLightSqr = dot4x4(toLightX, toLightY, toLightZ, toLightX, toLightY, toLightZ);
	float4 distToLight = sqrt(distToLightSqr);
	
	//
	// Phong Diffuse
	//
	
	toLightX /= distToLight;
	toLightY /= distToLight;
	toLightZ /= distToLight;
	float4 NDotL = saturate(dot4x1(toLightX, toLightY, toLightZ, materialNormal));
	//float3 lightColor = float3(dot(lightColorR, NDotL), dot(lightColorG, NDotL), dot(lightColorB, NDotL));
	
	//
	// Blinn Specular
	//
	
	toEye = normalize(toEye);
	float4 halfWayX = toEye.xxxx + toLightX;
	float4 halfWayY = toEye.yyyy + toLightY;
	float4 halfWayZ = toEye.zzzz + toLightZ;
	float4 halfWaySize = sqrt(dot4x4(halfWayX, halfWayY, halfWayZ, halfWayX, halfWayY, halfWayZ));
	float4 NDotH = saturate(dot4x1(halfWayX / halfWaySize, halfWayY / halfWaySize, halfWayZ / halfWaySize, materialNormal));
	float4 specularValue = pow(NDotH, materialSpecularExponent.xxxx) * materialSpecularIntensity;
	//lightColor += float3(dot(lightColorR, specularValue), dot(lightColorG, specularValue), dot(lightColorB, specularValue));
	
	//
	// Cone Attenuation
	//
	
	float4 cosAng = dot4x4(lightDirectionX, lightDirectionY, lightDirectionZ, toLightX, toLightY, toLightZ);
	float4 coneAttenuation = saturate((cosAng - spotCosOuterCone) * spotCosInnerConeReciprocal);
	coneAttenuation *= coneAttenuation;
	
	//
	// Attenuation
	//
	
	float4 distToLightNormal = 1.0 - saturate(distToLight * lightRangeReciprocal);
	float attenuation = distToLightNormal * distToLightNormal;
	attenuation *= coneAttenuation;
	
	//
	// Calculate final color value
	//
	
	float4 pixelIntensity = (NDotL + specularValue) * attenuation;
	float3 lightColor = float3(dot(lightColorR, pixelIntensity), dot(lightColorG, pixelIntensity), dot(lightColorB, pixelIntensity));
	lightColor *= materialDiffuseColor;
	
	return lightColor;
}
//==============================================================================
// Sun Light

float4 SunSpotColor(float3 normal)
{
	if (g_EnableSun == 1)
	{
		float cos_spec = clamp(dot(normal, g_SunDir), 0, 1);
		float sun_spot = pow(cos_spec, g_SunShineness);
		return g_SunDiffuseColor * sun_spot;
	}
	
	return float4(0, 0, 0, 0);
}

float4 SunDirectionalColor
(	float3 pixelPosition
,	float3 eyePosition
,   float3 materialNormal
,   float4 materialDiffuseColor
,   float materialSpecularExponent
,   float materialSpecularIntensity
,   float shadowColor
)
{
	if (g_EnableSun == 1)
	{
		float4 sunColor;
		
		sunColor.rgb = DirectionalLightColor
		(	pixelPosition
		,	eyePosition
		,	g_SunDir
		,	g_SunIntensity
		,	g_SunDiffuseColor
		,	materialNormal
		,	materialDiffuseColor
		,	materialSpecularExponent
		,	materialSpecularIntensity
		,   shadowColor
		);
		
		sunColor.a = 1.0f;
		
		return sunColor;
	}
	
	return float4(0, 0, 0, 0);
}
//==============================================================================
// Forward Lighting

// Factors in all lights that can be contained in the scene to the final color
float4 CalculateLightColor
(	float3 pixelPosition
,	float3 eyePosition
,	float4 envDiffuseColor
,   float3 materialNormal
,   float4 materialDiffuseColor
,	float4 materialAmbientColor
,   float materialSpecularExponent
,   float materialSpecularIntensity
,   float shadowColor
,	float ssaoColor
)
{
	float4 finalColor					= float4(0, 0, 0, 1);
	float4 finalSunColor 				= float4(0, 0, 0, 1);
	float4 finalAmbientLightColor		= float4(0, 0, 0, 1);
	float4 finalDirectionalLightColor 	= float4(0, 0, 0, 1);
	float4 finalPointLightColor 		= float4(0, 0, 0, 1);
	float4 finalSpotLightColor 			= float4(0, 0, 0, 1);
	float4 finalCapsuleLightColor 		= float4(0, 0, 0, 1);
	
	float4 finalDirectionalAmbientColor = float4(0, 0, 0, 1);
	float4 finalPointAmbientColor 		= float4(0, 0, 0, 1);
	float4 finalSpotAmbientColor 		= float4(0, 0, 0, 1);
	float4 finalCapsuleAmbientColor 	= float4(0, 0, 0, 1);
	
	finalSunColor = SunDirectionalColor
	(	pixelPosition
	,	eyePosition
	,	materialNormal
	,	materialDiffuseColor
	,	materialSpecularExponent
	,	materialSpecularIntensity
	,   shadowColor
	);
	
	// Factor in the sun spot specular color
	finalSunColor += saturate(SunSpotColor(materialNormal));
	
	[loop]
	for (uint i = 0; i < g_AmbientLightCount; i++)
	{
		AmbientLight al = g_AmbientLight[i];
		
		if (al.g_Toggle == 1)
		{
			finalAmbientLightColor *= al.g_AmbientColor;
		}
	}
	
	[loop]
	for (uint i = 0; i < g_DirectionalLightCount; i++)
	{
		DirectionalLight dl = g_DirectionalLight[i];
		
		if (dl.g_Toggle == 1)
		{
			finalDirectionalLightColor += dl.g_AmbientColor;
			finalDirectionalLightColor.rgb += DirectionalLightColor
			(	pixelPosition
			,	eyePosition
			,	dl.g_LightDirection
			,	dl.g_Intensity
			,	dl.g_DiffuseColor
			,	materialNormal
			,	materialDiffuseColor
			,	materialSpecularExponent
			,	materialSpecularIntensity
			,   shadowColor
			);
			finalDirectionalLightColor.a = 1.0f;
		}
	}
	
	[loop]
	for (uint i = 0; i < g_PointLightCount; i++)
	{
		PointLight pl = g_PointLight[i];
		
		if (pl.g_Toggle == 1)
		{
			finalPointAmbientColor += pl.g_AmbientColor;
			finalPointLightColor.rgb += PointLightColor
			(	pixelPosition
			,	eyePosition
			,	pl.g_DiffuseColor
			,	pl.g_LightPosition
			,	pl.g_Intensity
			,	1.0 / pl.g_LightRange
			,	materialNormal
			,	materialDiffuseColor
			,	materialSpecularExponent
			,	materialSpecularIntensity
			);
			finalPointLightColor.a = 1.0f;
		}
	}
	
	[loop]
	for (uint i = 0; i < g_SpotLightCount; i++)
	{
		SpotLight sl = g_SpotLight[i];
		
		if (sl.g_Toggle == 1)
		{
			finalSpotAmbientColor += sl.g_AmbientColor;
			finalSpotLightColor.rgb += SpotLightColor
			(	pixelPosition
			,	eyePosition
			,	sl.g_DiffuseColor
			,	sl.g_LightPosition
			,	sl.g_Intensity
			,	1 / sl.g_LightRange
			,	float3(0,0,0)
			,	sl.g_SpotCosOuterCone
			,	sl.g_SpotInnerConeReciprocal
			,	sl.g_CosineAngle
			,	materialNormal
			,	materialDiffuseColor
			,	materialSpecularExponent
			,	materialSpecularIntensity
			);
			finalSpotLightColor.a = 1.0f;
		}
	}
	
	[loop]
	for (uint i = 0; i < g_CapsuleLightCount; i++)
	{
		CapsuleLight cl = g_CapsuleLight[i];
		
		if (cl.g_Toggle == 1)
		{
			finalCapsuleAmbientColor += cl.g_AmbientColor;
			finalCapsuleLightColor.rgb += CapsuleLightColor
			(	pixelPosition
			,	eyePosition
			,	cl.g_LightPosition
			,	cl.g_LightRange
			,	1.0 / cl.g_LightRange
			,	cl.g_LightDirection
			,	cl.g_LightLength
			,	cl.g_DiffuseColor
			,	cl.g_CapsuleDirectionLength
			,	cl.g_CapsuleIntensity
			,	materialNormal
			,	materialDiffuseColor
			,	materialSpecularExponent
			,	materialSpecularIntensity
			);
			finalCapsuleLightColor.a = 1.0f;
		}
	}
	
	// saturate the final light color with base ambient light
	float4 lightAmbientColor = (finalDirectionalAmbientColor + finalPointAmbientColor + finalSpotAmbientColor + finalCapsuleAmbientColor);
	float4 ambientColor = float4(FinalAmbientColor(materialNormal, materialDiffuseColor, g_AmbientDown, g_AmbientUp), 1);
	float4 sceneAmbience = ssaoColor * (ambientColor + materialAmbientColor + lightAmbientColor);
	float4 color = finalSunColor + finalAmbientLightColor + finalDirectionalLightColor + finalPointLightColor + finalSpotLightColor + finalCapsuleLightColor + sceneAmbience;
	
	// Combine all the light colors together
	finalColor = color * shadowColor;
	finalColor = saturate(finalColor);
	
	return finalColor;
}
//==============================================================================