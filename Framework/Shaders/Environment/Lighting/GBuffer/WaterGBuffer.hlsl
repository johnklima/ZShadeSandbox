//==============================================================================
// Water GBuffer
//
//==============================================================================
// History
//
// -Created on 6/13/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbWaterBuffer : register(b0)
{
	float 	g_WaveHeight;
	float 	g_SpecularShininess;
	float 	g_Time;
	float 	g_HalfCycle;
	float3 	g_CamPos;
	float	g_FlowMapOffset0;
	float3 	g_LightDirection;
	float	g_FlowMapOffset1;
	float4 	g_RefractionTint;
	float4  g_DullColor;
	float2  g_wpadding;
	float	g_FarPlane;
	float 	g_TexScale;
};

cbuffer cbMatrixBuffer : register(b1)
{
	matrix g_World;
	matrix g_View;
	matrix g_Projection;
	matrix g_ReflectionView;
};

static const float	  R0 = 0.02037f;

//======================================================================================================

//
// Samplers and textures
//

Texture2D g_FlowMap		  : register(t0);
Texture2D g_NoiseMap 	  : register(t1);
Texture2D g_WaveMap0 	  : register(t2);
Texture2D g_WaveMap1 	  : register(t3);
Texture2D g_ReflectionMap : register(t4);
Texture2D g_RefractionMap : register(t5);

SamplerState mirrorSampler	: register(s0);
SamplerState linearSampler	: register(s1);

//======================================================================================================

//
// Structs
//

struct WaterVertexInput
{
	float4 position  : POSITION;
	float2 uv        : TEXCOORD0;
};

struct WaterNormalPixelInput
{
	float4 position                 : SV_POSITION;
	float2 bumpMapSamplingPos1      : TEXCOORD0;
	float3 normal					: NORMAL;
	float pixelDepth				: DEPTH;
	float2 bumpMapSamplingPos2      : TEXCOORD1;
	float4 reflectionMapSamplingPos : TEXCOORD2;
	float4 refractionMapSamplingPos : TEXCOORD3;
	float4 position3D               : TEXCOORD4;
	float3 viewDirection            : TEXCOORD5;
	float2 depth					: TEXCOORD6;
};

struct WaterFlowPixelInput
{
	float4 position                 : SV_POSITION;
	float2 uv       				: TEXCOORD0;
	float3 normal					: NORMAL;
	float pixelDepth				: DEPTH;
	float4 reflectionMapSamplingPos : TEXCOORD1;
	float4 refractionMapSamplingPos : TEXCOORD2;
	float4 position3D               : TEXCOORD3;
	float3 viewDirection            : TEXCOORD4;
	float2 depth					: TEXCOORD5;
};

struct PixelOutput
{
	float4 colors 	: SV_Target0;
	float4 normals 	: SV_Target1;
	float4 depth	: SV_Target2;
};

//======================================================================================================

//
// Vertex Shader
//

WaterFlowPixelInput WaterGBufferFlowVS(WaterVertexInput input)
{
	WaterFlowPixelInput output;

	float4x4 viewProj = mul(g_View, g_Projection);
	float4x4 worldViewProj = mul(g_World, viewProj);
	float4x4 reflectionViewProjection = mul(g_ReflectionView, g_Projection);
	float4x4 worldReflectionViewProjection = mul(g_World, reflectionViewProjection);

	//
	//Calculate the refraction and reflection view matrices the same as before also. 
	//

	// Calculate the input position against the viewProjectWorld matrix.
	output.position = mul(input.position, worldViewProj);
	output.reflectionMapSamplingPos = mul(input.position, worldReflectionViewProjection);
	output.refractionMapSamplingPos = mul(input.position, worldViewProj);
	output.position3D = mul(input.position, g_World);

	// Logarithmic z buffer
	// DirectX Depth range 0..1
	int C = 1;
	output.pixelDepth = log2(max(1e-6, C * output.position.z + 1)) * 2 / log2(C * g_FarPlane + 1) * output.position.z;

	//
	//Calculate the camera's view direction for fresnel and specular calculations.
	//

	// Calculate the position of the vertex in the world.
	float3 worldPosition = mul(input.position, g_World);

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
	output.viewDirection = g_CamPos.xyz - worldPosition.xyz;

	// Normalize the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);

	output.uv = input.uv;

	output.normal = mul(float3(0, 1, 0), (float3x3)g_World);
	output.normal = normalize(output.normal);

	output.depth.x = output.position.z;
	output.depth.y = output.position.w;

	return output;
}

WaterNormalPixelInput WaterGBufferNormalVS(WaterVertexInput input)
{
	WaterNormalPixelInput output;

	float4x4 viewProj = mul(g_View, g_Projection);
	float4x4 worldViewProj = mul(g_World, viewProj);
	float4x4 reflectionViewProjection = mul(g_ReflectionView, g_Projection);
	float4x4 worldReflectionViewProjection = mul(g_World, reflectionViewProjection);

	//
	//Calculate the refraction and reflection view matrices the same as before also. 
	//

	// Calculate the input position against the viewProjectWorld matrix.
	output.position = mul(input.position, worldViewProj);
	output.reflectionMapSamplingPos = mul(input.position, worldReflectionViewProjection);
	output.refractionMapSamplingPos = mul(input.position, worldViewProj);
	output.position3D = mul(input.position, g_World);

	// Logarithmic z buffer
	// DirectX Depth range 0..1
	int C = 1;
	output.pixelDepth = log2(max(1e-6, C * output.position.z + 1)) * 2 / log2(C * g_FarPlane + 1) * output.position.z;

	//
	//Calculate the camera's view direction for fresnel and specular calculations.
	//

	// Calculate the position of the vertex in the world.
	float3 worldPosition = mul(input.position, g_World);

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
	output.viewDirection = g_CamPos.xyz - worldPosition.xyz;

	// Normalize the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);

	output.bumpMapSamplingPos1 = input.uv / 0.01f;
	output.bumpMapSamplingPos2 = input.uv / 0.02f;

	output.normal = mul(float3(0, 1, 0), (float3x3)g_World);
	output.normal = normalize(output.normal);

	output.depth.x = output.position.z;
	output.depth.y = output.position.w;

	return output;
}

//======================================================================================================

//
// Pixel Shader
//

PixelOutput WaterGBufferNormalPS(WaterNormalPixelInput input) : SV_Target
{
	float4 color = float4(1, 1, 1, 1);

	float2 windForce = float2(1, 1);
	float2 moveVector = float2(0, 1);
	input.bumpMapSamplingPos1 += g_Time * windForce * moveVector;
	input.bumpMapSamplingPos2 += g_Time * windForce * moveVector;

	//
	//Sample the water normal map two times using the two different texture sampling coordinates.
	//

	// Sample the normal from the normal map texture using the two different tiled and translated coordinates.
	float4 bumpColor1 = g_WaveMap0.Sample(mirrorSampler, input.bumpMapSamplingPos1);
	float4 bumpColor2 = g_WaveMap1.Sample(mirrorSampler, input.bumpMapSamplingPos2);

	// Expand the range of the normal from (0,1) to (-1,+1).
	float3 normal1 = (bumpColor1.rgb * 2.0f) - 1.0f;
	float3 normal2 = (bumpColor2.rgb * 2.0f) - 1.0f;

	//
	//Now combine the two normal map results to get an animated water ripple effect instead of just a single rotated normal map ripple.
	//

	// Combine the normals to add the normal maps together.
	float3 normal = normalize(normal1 + normal2);

	float2 perturbation = g_WaveHeight * (bumpColor1.rg - 0.5f) * 2.0f;

	//
	// Set the reflection color
	//

	float2 projectedCoords;
	projectedCoords.x = input.reflectionMapSamplingPos.x / input.reflectionMapSamplingPos.w / 2.0f + 0.5f;
	projectedCoords.y = -input.reflectionMapSamplingPos.y / input.reflectionMapSamplingPos.w / 2.0f + 0.5f;

	// Re-position the texture coordinate sampling position by the scaled normal map value to simulate the rippling wave effect.
	float2 reflectTexCoord = projectedCoords + (normal.xy * 0.03f);
	float2 perturbatedTexCoords = projectedCoords + perturbation;

	float4 reflectiveColor = g_ReflectionMap.Sample(mirrorSampler, reflectTexCoord);

	//
	// Set the refraction color
	//

	float2 projectedRefrCoords;
	projectedRefrCoords.x = input.refractionMapSamplingPos.x / input.refractionMapSamplingPos.w / 2.0f + 0.5f;
	projectedRefrCoords.y = -input.refractionMapSamplingPos.y / input.refractionMapSamplingPos.w / 2.0f + 0.5f;

	// Re-position the texture coordinate sampling position by the scaled normal map value to simulate the rippling wave effect.
	float2 refractTexCoord = projectedRefrCoords + (normal.xy * 0.03f);
	float2 perturbatedRefrTexCoords = projectedRefrCoords + perturbation;

	float4 refractiveColor = g_RefractionMap.Sample(mirrorSampler, refractTexCoord);

	//
	//Add a water color tint to the refraction.
	//

	// Combine the tint with the refraction color.
	//float4(0.0f, 0.8f, 1.0f, 1.0f) is refraction tint
	refractiveColor = saturate(refractiveColor * g_RefractionTint);

	//
	// Calculate the fresnel term
	//
	float3 eyeVector = normalize(g_CamPos - input.position3D);
	float3 normalVectorUp = float3(0, 1, 0);
	float fangle = 1 + dot(eyeVector, normalVectorUp);
	fangle = pow(fangle, 5);
	float fresnelTerm = 1 / fangle;
	fresnelTerm = fresnelTerm * 3;
	fresnelTerm = fresnelTerm < 0 ? 0 : fresnelTerm;
	fresnelTerm = fresnelTerm > 1 ? 1 : fresnelTerm;
	float4 combinedColor = refractiveColor * (1 - fresnelTerm) + reflectiveColor * fresnelTerm;

	//
	// Add some dull color
	//

	//float4 dullColor = float4(0.1f, 0.1f, 0.2f, 1.0f);
	//float4 dullColor = float4(0.3f, 0.3f, 0.5f, 1.0f);
	color = lerp(combinedColor, g_DullColor, 0.2f);

	//
	//Finally do a specular light calculation using the water normals and add it to the final color result to get the specular effect on just the water ripples.
	//

	// Calculate the reflection vector using the normal and the direction of the light.
	float3 reflection = -reflect(normalize(g_LightDirection), normal);

	// Calculate the specular light based on the reflection and the camera position.
	float specular = dot(normalize(reflection), normalize(input.viewDirection));

	// Check to make sure the specular was positive so we aren't adding black spots to the water.
	if (specular > 0.0f)
	{
		// Increase the specular light by the shininess value.
		specular = pow(specular, g_SpecularShininess);

		// Add the specular to the final color.
		color = saturate(color + specular);
	}

	PixelOutput output;

	output.colors = float4(color.rgb, 0.0f);
	output.colors.a = 1.0f;
	output.normals = float4(input.normal, 500.0f);
	output.depth = float4(input.depth.x / input.depth.y, 0.0, 0.0, 1.0);

	return output;
}

PixelOutput WaterGBufferFlowPS(WaterFlowPixelInput input) : SV_Target
{
	float4 color;

	//
	// Get and uncompress the flow vector for this pixel
	//

	float2 flowmap = g_FlowMap.Sample(linearSampler, input.uv).rg * 2.0f - 1.0f;

	float cycleOffset = g_NoiseMap.Sample(linearSampler, input.uv).r;

	float phase0 = cycleOffset * 0.5f + g_FlowMapOffset0;
	float phase1 = cycleOffset * 0.5f + g_FlowMapOffset1;

	//
	// Sample the normal wave maps
	//

	float3 normal0 = g_WaveMap0.Sample(linearSampler, (input.uv * g_TexScale) + flowmap * phase0);
	float3 normal1 = g_WaveMap1.Sample(linearSampler, (input.uv * g_TexScale) + flowmap * phase1);

	float flowLerp = abs(g_HalfCycle - g_FlowMapOffset0) / g_HalfCycle;

	// unroll the normals
	normal0.yz = normal0.zy;
	normal1.yz = normal1.zy;

	normal0 = 2.0f * normal0 - 1.0f;
	normal1 = 2.0f * normal1 - 1.0f;

	float3 normal = lerp(normal0, normal1, flowLerp);

	float2 perturbation = g_WaveHeight * (normal0.rg - 0.5f) * 2.0f;

	//
	// Set the reflection color
	//

	float2 projectedCoords;
	projectedCoords.x = input.reflectionMapSamplingPos.x / input.reflectionMapSamplingPos.w / 2.0f + 0.5f;
	projectedCoords.y = -input.reflectionMapSamplingPos.y / input.reflectionMapSamplingPos.w / 2.0f + 0.5f;

	// Re-position the texture coordinate sampling position by the scaled normal map value to simulate the rippling wave effect.
	float2 reflectTexCoord = projectedCoords + (normal.xy * 0.03f);
	float2 perturbatedTexCoords = projectedCoords + perturbation;

	float4 reflectiveColor = g_ReflectionMap.Sample(mirrorSampler, reflectTexCoord);

	//
	// Set the refraction color
	//

	float2 projectedRefrCoords;
	projectedRefrCoords.x = input.refractionMapSamplingPos.x / input.refractionMapSamplingPos.w / 2.0f + 0.5f;
	projectedRefrCoords.y = -input.refractionMapSamplingPos.y / input.refractionMapSamplingPos.w / 2.0f + 0.5f;

	// Re-position the texture coordinate sampling position by the scaled normal map value to simulate the rippling wave effect.
	float2 refractTexCoord = projectedRefrCoords + (normal.xy * 0.03f);
	float2 perturbatedRefrTexCoords = projectedRefrCoords + perturbation;

	float4 refractiveColor = g_RefractionMap.Sample(mirrorSampler, refractTexCoord);

	//
	//Add a water color tint to the refraction.
	//

	// Combine the tint with the refraction color.
	refractiveColor = saturate(refractiveColor * g_RefractionTint);

	//
	// Calculate the fresnel term
	//
	float3 eyeVector = normalize(g_CamPos - input.position3D);
	float3 normalVectorUp = float3(0, 1, 0);
	float fangle = 1 + dot(eyeVector, normal);
	fangle = pow(fangle, 5);
	float fresnelTerm = 1 / fangle;
	fresnelTerm = fresnelTerm * 3;
	fresnelTerm = fresnelTerm < 0 ? 0 : fresnelTerm;
	fresnelTerm = fresnelTerm > 1 ? 1 : fresnelTerm;
	float4 combinedColor = refractiveColor * (1 - fresnelTerm) + reflectiveColor * fresnelTerm;

	//
	// Add some dull color
	//

	//float4 dullColor = float4(0.1f, 0.1f, 0.2f, 1.0f);
	color = lerp(combinedColor, g_DullColor, 0.2f);

	//
	//Finally do a specular light calculation using the water normals and add it to the final color result to get the specular effect on just the water ripples.
	//

	// Calculate the reflection vector using the normal and the direction of the light.
	float3 reflection = -reflect(normalize(g_LightDirection), normal);

	// Calculate the specular light based on the reflection and the camera position.
	float specular = dot(normalize(reflection), normalize(input.viewDirection));

	// Check to make sure the specular was positive so we aren't adding black spots to the water.
	if (specular > 0.0f)
	{
		// Increase the specular light by the shininess value.
		specular = pow(specular, g_SpecularShininess);

		// Add the specular to the final color.
		color = saturate(color + specular);
	}

	PixelOutput output;

	output.colors = float4(color.rgb, 0.0f);
	output.colors.a = 1.0f;
	output.normals = float4(input.normal, 500.0f);
	output.depth = float4(input.depth.x / input.depth.y, 0.0, 0.0, 1.0);

	return output;
}

PixelOutput WaterGBufferNormalWireframePS(WaterNormalPixelInput input) : SV_Target
{
	PixelOutput output;

	output.colors = float4(0.9f, 0.9f, 0.9f, 1);
	output.normals = float4(0.5f, 0.5f, 0.5f, 1);
	output.depth = float4(1, 0, 0, 1);

	return output;
}

PixelOutput WaterGBufferFlowWireframePS(WaterFlowPixelInput input) : SV_Target
{
	PixelOutput output;

	output.colors = float4(0.9f, 0.9f, 0.9f, 1);
	output.normals = float4(0.5f, 0.5f, 0.5f, 1);
	output.depth = float4(1, 0, 0, 1);

	return output;
}