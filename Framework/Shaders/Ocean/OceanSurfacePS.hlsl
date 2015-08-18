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
	float	g_SeaLevel;
	float 	g_WaveHeight;
	float3 	g_LightDirection;
	float 	g_SpecularShininess;
	float3 	g_CamPos;
	float 	g_Time;
	float4 	g_RefractionTint;
	float4  g_DullColor;
	
	//<JPK>  perlin
	float2	g_PerlinMovement;
	float2	g_UVBase;
	float	g_PerlinSize;
	float3	g_PerlinAmplitude;
	float3	g_PerlinOctave;
	float3	g_PerlinGradient;
	float   g_TexelLength_x2;
	float   g_UVScale;
	float   g_UVOffset;
};

//======================================================================================================

//
// Globals
//

Texture2D g_texDisplacement		: register(t0); // FFT wave displacement map in VS (wave displacement from ocean heightfield)
Texture2D g_texPerlin			: register(t1); // FFT wave gradient map in PS
Texture2D g_texGradient			: register(t2); // Perlin wave displacement & gradient map in both VS & PS
Texture2D g_WaveMap0 	  		: register(t3); // First Normal map
Texture2D g_WaveMap1 	  		: register(t4); // Second Normal map
Texture2D g_ReflectionMap 		: register(t5);
Texture2D g_RefractionMap 		: register(t6);
Texture2D g_FresnelMap			: register(t7);
TextureCube g_texReflectCube 	: register(t8);// A small skybox cube texture for reflection

SamplerState cubeSampler	      : register(s0);
SamplerState perlinSampler	      : register(s1);
SamplerState gradientSampler	   : register(s2);
SamplerState fresnelSampler	   : register(s3);

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

   float2 perlin_uv = input.bumpMapSamplingPos1 / 10.0f;
	
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
   float2 fft_tc = perlin_uv;
	float2 grad = g_texGradient.Sample(gradientSampler, fft_tc).xy;
	grad = lerp(perlin, grad, blend_factor);
	float3 normalPerlin = normalize(float3(grad.x, g_TexelLength_x2, grad.y));
	// Reflected ray
	float3 reflect_vec = reflect(-eye_dir, normalPerlin);
	
	float cos_angle = dot(normalPerlin, eye_dir);
	
	// A coarse way to handle transmitted light (WaterBodyColor)
	//<JPK> quick hack
	float3 body_color = float3(0.07f, 0.15f, 0.2f);
	float3 sky_color = float3(0.38f, 0.45f, 0.56f);
	float3 bend_param = float3(0.1f, -0.4f, 0.2f);
	float3 sun_color = float3(1.0f, 1.0f, 0.6f);
	// --------------- Reflected color
	
	// ramp.x for fresnel term. ramp.y for sky blending
	float4 ramp = g_FresnelMap.Sample(fresnelSampler, cos_angle).xyzw;
	
	// A workaround to deal with "indirect reflection vectors" (which are rays requiring multiple
	// reflections to reach the sky).

	/*  <JPK> THIS IS NOT CORRECT TO AXIS CHANGE!!! not sure yet what to do about it
   
   if (reflect_vec.z < bend_param.x)
	{
		ramp = lerp(ramp, bend_param.y, (bend_param.x - reflect_vec.z) / (bend_param.x - bend_param.y));
	}
	
	reflect_vec.z = max(0, reflect_vec.z);
	
   */ 

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
	
	return final_color;
}

float4 OceanSurfaceWireframePS(PixelInput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}