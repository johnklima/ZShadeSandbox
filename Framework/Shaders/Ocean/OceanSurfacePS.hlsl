//==============================================================================
// Ocean Surface Pixel Shader
//
// Add Perlin Noise and Tessendorf's FFT later
//==============================================================================
// History
//
// -Created on 5/30/2015 by Dustin Watson
// -Updated 8/5/2015 by John Klima
//==============================================================================

//<JPK> not sure how best to use these
#define PATCH_BLEND_BEGIN		800
#define PATCH_BLEND_END			20000

//
// Constants
//

cbuffer cbPerFrame : register(b0)
{
	float2	g_padding;
	float	   g_SeaLevel;
	float 	g_WaveHeight;
	float3 	g_LightDirection;
	float 	g_SpecularShininess;
	float3 	g_CamPos;
	float 	g_Time;
	float4 	g_RefractionTint;
	float4   g_DullColor;
	
	//<JPK>  perlin
	float2	g_PerlinMovement;
	float2	g_UVBase;
	float	   g_PerlinSize;
	float3	g_PerlinAmplitude;
	float3	g_PerlinOctave;
	float3	g_PerlinGradient;
	float    g_TexelLength_x2;
	float    g_UVScale;
	float    g_UVOffset;
};

//======================================================================================================

//
// Globals
//

Texture2D g_texDisplacement	: register(t0); // FFT wave displacement map in VS (wave displacement from ocean heightfield)
Texture2D g_texPerlin			: register(t1); // FFT wave gradient map in PS
Texture2D g_texGradient			: register(t2); // Perlin wave displacement & gradient map in both VS & PS
Texture2D g_WaveMap0 	  		: register(t3); // First Normal map
Texture2D g_WaveMap1 	  		: register(t4); // Second Normal map
Texture2D g_ReflectionMap 		: register(t5);
Texture2D g_RefractionMap 		: register(t6);
Texture2D g_FresnelMap			: register(t7);
TextureCube g_texReflectCube 	: register(t8);// A small skybox cube texture for reflection

SamplerState cubeSampler		: register(s0);
SamplerState perlinSampler		: register(s1);
SamplerState gradientSampler	: register(s2);
SamplerState fresnelSampler	: register(s3);
SamplerState mirrorSampler		: register(s4);

//======================================================================================================

//
// Pixel Shader
//

struct PixelInput
{
	float4 position					: SV_POSITION;
	float3 positionW				: POSITION;
	float2 uv						: TEXCOORD0;
	float2 bumpMapSamplingPos1      : TEXCOORD1;
	float2 bumpMapSamplingPos2      : TEXCOORD2;
	float4 reflectionMapSamplingPos : TEXCOORD3;
	float4 refractionMapSamplingPos : TEXCOORD4;
	float4 position3D               : TEXCOORD5;
	float3 viewDirection            : TEXCOORD6;
	float2 uv1						: TEXCOORD7;
};

float4 OceanSurfacePS(PixelInput input) : SV_TARGET
{
	//<JPK> add perlin
	// Calculate eye vector.
	float3 eye_vec = g_CamPos - input.position3D;
	float3 eye_dir = normalize(eye_vec);
	
	// --------------- Blend perlin noise for reducing the tiling artifacts
	
	// Blend displacement to avoid tiling artifact
	float dist_2d = length(eye_vec.xz);
	float blend_factor = (PATCH_BLEND_END - dist_2d) / (PATCH_BLEND_END - PATCH_BLEND_BEGIN);
	blend_factor = clamp(blend_factor * blend_factor * blend_factor, 0, 1);
	
   float2 perlin_uv = input.uv ; // input.bumpMapSamplingPos1 / 20.0f;

   // Compose perlin waves from three octaves
   float2 perlin_tc = perlin_uv;
   float2 perlin_tc0 = (blend_factor < 1) ? perlin_tc * g_PerlinOctave.x + g_PerlinMovement : 0;
   float2 perlin_tc1 = (blend_factor < 1) ? perlin_tc * g_PerlinOctave.y + g_PerlinMovement : 0;
   float2 perlin_tc2 = (blend_factor < 1) ? perlin_tc * g_PerlinOctave.z + g_PerlinMovement : 0;

   float2 perlin_0 = g_texPerlin.Sample(perlinSampler, perlin_tc0).xy;
   float2 perlin_1 = g_texPerlin.Sample(perlinSampler, perlin_tc1).xy;
   float2 perlin_2 = g_texPerlin.Sample(perlinSampler, perlin_tc2).xy;

   float2 perlin = (perlin_0 * g_PerlinGradient.x + perlin_1 * g_PerlinGradient.y + perlin_2 * g_PerlinGradient.z);

   // Texcoord mash optimization: Texcoord of FFT wave is not required when blend_factor > 1
   float2 fft_tc = (blend_factor > 0) ? perlin_uv : 0; // perlin_uv;
	
	float2 grad = g_texGradient.Sample(gradientSampler, fft_tc).xy;
	grad = lerp(perlin, grad, blend_factor);
	float3 normalPerlin = normalize(float3(grad.x, g_TexelLength_x2, grad.y));


	// Reflected ray
	float3 reflect_vec = reflect(-eye_dir, normalPerlin);
	
	float cos_angle = dot(normalPerlin, eye_dir);
	
	// A coarse way to handle transmitted light (WaterBodyColor)
	//<JPK> quick hack
   float3 body_color = float3(0.07f, 0.10f, 0.15f);
      float3 sky_color = float3(0.38f, 0.45f, 0.56f);
      float3 bend_param = float3(0.1f, -0.4f, 0.2f);
      float3 sun_color = float3(1.0f, 1.0f, 0.6f);

      // --------------- Reflected color

      // ramp.x for fresnel term. ramp.y for sky blending
      float4 ramp = g_FresnelMap.Sample(fresnelSampler, cos_angle).xyzw;

      // A workaround to deal with "indirect reflection vectors" (which are rays requiring multiple
      // reflections to reach the sky).
      
      if (reflect_vec.y < bend_param.x)
         ramp = lerp(ramp, bend_param.z, (bend_param.x - reflect_vec.y) / (bend_param.x - bend_param.y));
   
   reflect_vec.y = max(0, reflect_vec.y);
      
   reflect_vec.y = -reflect_vec.y;

   float3 reflectionP = g_texReflectCube.Sample(cubeSampler, reflect_vec).xyz;
   
   
   // Hack bit: making higher contrast
	reflectionP = reflectionP * reflectionP * 2.5f;
	
	// Blend with predefined sky color
	float3 reflected_color = lerp(sky_color, reflectionP, ramp.y);
	
	// Combine waterbody color and reflected color
	float3 water_color = lerp(body_color, reflected_color, ramp.x);
	
	float cos_spec = clamp(dot(reflect_vec, g_LightDirection), 0, 1);
	float sun_spot = pow(cos_spec, g_SpecularShininess);
	water_color += sun_color * sun_spot;
	
	float4 final_color = float4(water_color.r, water_color.g, water_color.b, 1);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Begin final color generation for the ocean
	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	float4 color = float4(1, 1, 1, 1);
	
	float2 windForce = float2(1, 1);
	float2 moveVector = float2(0, 0);
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
	float3 normal = normalize( normal1 + normal2 );
	
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
	float4 combinedColor = refractiveColor * (1 - fresnelTerm) + reflectiveColor * fresnelTerm * 0.7f;
	
	//
	// Add some dull color
	//
	
	color = lerp(combinedColor, g_DullColor, 0.2f);
	
	//
	//Finally do a specular light calculation using the water normals and add it to the final colour result
	//to get the specular effect on just the water ripples.
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
	
	color = lerp(color, final_color, 0.7f);
   
   return  final_color;  //just this for now
}

float4 OceanSurfaceWireframePS(PixelInput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}