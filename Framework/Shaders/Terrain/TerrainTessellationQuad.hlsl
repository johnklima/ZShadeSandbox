//==============================================================================
// Terrain Tessellation Shader
//
//==============================================================================
// History
//
// -Created on 4/8/2015 by Dustin Watson
//==============================================================================
#include "..\Environment\Lighting\Lighting.hlsl"

//
// Constants
//

cbuffer cbTessellationBuffer : register(b2)
{
	float3 	g_EyePosW;
	int  	g_FrustumCull;
	float 	g_MinDist;
	float 	g_MaxDist;
	float 	g_MinTess;
	float 	g_MaxTess;
};

cbuffer cbDomainConst : register(b3)
{
	float2  g_TexScale;
	float 	g_HeightScale;
	float 	g_TerrainZScale;
	float4 	g_ClipPlane;
	float3  g_tpadding;
	float   g_FarPlane;
	matrix	g_WorldMatrix;
	matrix	g_ViewMatrix;
	matrix	g_ProjMatrix;
	matrix	g_TexSpaceMatrix;
	matrix	g_ShadowMatrix;
};

cbuffer cbShadingConst : register(b4)
{
	// Position of the camera
	float3		g_EyePosition;
	float		g_DetailBrightness;

	// The parameters for fog
	float		g_fogThinning;
	float		g_fogAltitude;
	float		g_fogNear;
	float		g_fogFar;
	float4		g_fogColor;

	// The Terrain texture parameters
	float		g_TextureAmount;
	float		g_TextureWidth;
	float		g_TextureHeight;

	// The parameters for terrain/water updating
	float		g_seaLevel;
	float4		g_waterBodyColor;

	// Terrain Material
	float4		g_MaterialDiffuseColor;
	float4		g_MaterialAmbientColor;
	float 		g_TexelCellSpaceU;
	float 		g_TexelCellSpaceV;
	float		g_MaterialSpecularPower;
	float		g_MaterialSpecularIntensity;

	// Control variables
	int			g_useClippingPlane;
	int			g_useNormalMap;
	int			g_UseSobelFilter;
	int			g_useShadowMap;
	float3		tpadding;
	int			g_useSSAO;

	matrix		g_View;
};

static const float SMAP_SIZE = 256.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;

//======================================================================================================

//
// Textures and Samplers
//

Texture2DArray g_LayerMapArray  : register(t0);
Texture2D      g_BlendMap 		: register(t1);
Texture2D      g_NormalMap 		: register(t2);
Texture2D      g_HeightMap 		: register(t3);
Texture2D	   g_DetailMap		: register(t4);
Texture2D	   g_ShadowMap		: register(t5);
Texture2D	   g_SSAOMap		: register(t6);

SamplerState g_PointSampler							: register(s0);
SamplerState g_LinearSampler						: register(s1);
SamplerComparisonState ShadowMapSamplerComparison	: register(s3);

//======================================================================================================

//
// Globals
//

float CalcTessFactor(float3 p)
{
	float d = distance(p, g_EyePosW);

	// max norm in xz plane (useful to see detail levels from a bird's eye).
	//float d = max( abs(p.x - gEyePosW.x), abs(p.z - gEyePosW.z) );

	float s = saturate((d - g_MinDist) / (g_MaxDist - g_MinDist));

	return pow(2, (lerp(g_MaxTess, g_MinTess, s)));
}

//======================================================================================================

//
// Vertex Shader
//

struct VertexInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD0;
};

struct HullInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD0;
};

HullInput TerrainTessellationQuadVS(VertexInput input)
{
	HullInput output;
	
	// Pass vertex position into the hull shader
	output.position = input.position;
	
	// Displace the patch corners to world space.  This is to make 
	// the eye to patch distance calculation more accurate.
	output.position.y = g_HeightMap.SampleLevel(g_PointSampler, input.uv, 0).r;
	
	// Pass texture uv into the hull shader
	output.uv = input.uv;
	
	return output;
}

//======================================================================================================

//
// Hull Shader
//

struct PatchConstOutput
{
	float edges[4]		: SV_TessFactor;
	float inside[2]		: SV_InsideTessFactor;
};

struct DomainInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD0;
};

void HCTessellate(out PatchConstOutput output, float factor)
{
	output.edges[0] = factor;
	output.edges[1] = factor;
	output.edges[2] = factor;
	output.edges[3] = factor;
	output.inside[0] = factor;
	output.inside[1] = factor;
}

void HCTessellate(out PatchConstOutput output, float3 e0, float3 e1, float3 e2, float3 e3, float3 c)
{
	output.edges[0] = CalcTessFactor(e0);
	output.edges[1] = CalcTessFactor(e1);
	output.edges[2] = CalcTessFactor(e2);
	output.edges[3] = CalcTessFactor(e3);
	output.inside[0] = CalcTessFactor(c);
	output.inside[1] = output.inside[0];
}

PatchConstOutput ConstHS(InputPatch<HullInput, 4> inputPatch, uint patchID : SV_PrimitiveID)
{
	PatchConstOutput output;

	if (g_FrustumCull == 0)
	{
		// Compute midpoint on edges, and patch center
		float3 e0 = 0.5f * (inputPatch[0].position + inputPatch[2].position);
		float3 e1 = 0.5f * (inputPatch[0].position + inputPatch[1].position);
		float3 e2 = 0.5f * (inputPatch[1].position + inputPatch[3].position);
		float3 e3 = 0.5f * (inputPatch[2].position + inputPatch[3].position);
		float3  c = 0.25f * (inputPatch[0].position + inputPatch[1].position + inputPatch[2].position + inputPatch[3].position);

		HCTessellate(output, e0, e1, e2, e3, c);
	}
	else
	{
		HCTessellate(output, 0.0f);
	}

	return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstHS")]
DomainInput TerrainTessellationQuadHS(InputPatch<HullInput, 4> inputPatch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	DomainInput output;

	output.position = inputPatch[pointID].position;

	output.uv = inputPatch[pointID].uv;

	return output;
}

//======================================================================================================

//
// Domain Shader
//

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
	float pixelDepth	: DEPTH;
	float2 uv			: TEXCOORD0;
	float2 TiledTex 	: TEXCOORD1;
	float4 worldPos 	: TEXCOORD2;
	float clip			: TEXCOORD3;
	float4 depth		: TEXCOORD4;
	float4 shadowPos	: TEXCOORD5;
	float4 ssaoPos		: TEXCOORD6;
};

[domain("quad")]
PixelInput TerrainTessellationQuadDS(PatchConstOutput input, float2 uv : SV_DomainLocation, const OutputPatch<DomainInput, 4> inputPatch)
{
	PixelInput output;

	// Bilinear interpolation.

	// Find the position of the new vertex
	float3 vPos = lerp(lerp(inputPatch[0].position, inputPatch[1].position, uv.x), lerp(inputPatch[2].position, inputPatch[3].position, uv.x), uv.y);

	// Find the texture coordinate of the new vertex
	float2 vTex = lerp(lerp(inputPatch[0].uv, inputPatch[1].uv, uv.x), lerp(inputPatch[2].uv, inputPatch[3].uv, uv.x), uv.y);

	// Send the texture uv to the pixel shader
	output.uv = vTex;

	// Tile layer textures over terrain.
	output.TiledTex = output.uv * g_TexScale;

	// Sample the height for this vertex
	float4 SampleValue = g_HeightMap.SampleLevel(g_PointSampler, output.uv, 0);
	vPos.y = ((SampleValue.r * g_HeightScale * 100.0) / 255.0) / ((g_TerrainZScale * 2) + 1);

	// Calculate independent world position
	output.worldPos = mul(float4(vPos, 1.0f), g_WorldMatrix);

	// Get the non-homogenous (untransformed) position of the final vertex
	output.positionW = vPos;

	// Calculate the new position of the new vertex with WVP matrices
	output.position = mul(float4(vPos, 1.0f), g_WorldMatrix);
	output.position = mul(output.position, g_ViewMatrix);
	output.position = mul(output.position, g_ProjMatrix);

	// Logarithmic z buffer
	// DirectX Depth range 0..1
	int C = 1;
	output.pixelDepth = log2(max(1e-6, C * output.position.z + 1)) * 2 / log2(C * g_FarPlane + 1) * output.position.z;

	// Determine where this vertex lies on the clipping plane
	output.clip = dot(output.position, g_ClipPlane);

	// Store the positions for depth calculations
	output.depth = output.position;

	// Calculate the position for shadow mapping
	output.shadowPos = mul(float4(vPos, 1.0f), g_ShadowMatrix);

	// Calculate the new SSAO position
	output.ssaoPos = mul(float4(vPos, 1.0f), g_ViewMatrix);
	output.ssaoPos = mul(output.ssaoPos, g_ProjMatrix);
	output.ssaoPos = mul(output.ssaoPos, g_TexSpaceMatrix);

	return output;
}

//======================================================================================================

//
// Pixel Shader
//

float4 TerrainTessellationQuadPS(PixelInput input) : SV_Target
{
	float4 finalColor;

	//=======================================================
	// Estimate normal and tangent using central differences.
	float2 leftTex = input.uv + float2(-g_TexelCellSpaceU, 0.0f);
	float2 rightTex = input.uv + float2(g_TexelCellSpaceU, 0.0f);
	float2 bottomTex = input.uv + float2(0.0f, g_TexelCellSpaceV);
	float2 topTex = input.uv + float2(0.0f, -g_TexelCellSpaceV);

	float leftY = g_HeightMap.SampleLevel(g_PointSampler, leftTex, 0).r;
	float rightY = g_HeightMap.SampleLevel(g_PointSampler, rightTex, 0).r;
	float bottomY = g_HeightMap.SampleLevel(g_PointSampler, bottomTex, 0).r;
	float topY = g_HeightMap.SampleLevel(g_PointSampler, topTex, 0).r;

	float worldCellSpace = 1.0f;
	float3 tangent = normalize(float3(2.0f * worldCellSpace, rightY - leftY, 0.0f));
	float3 bitangent = normalize(float3(0.0f, bottomY - topY, -2.0f * worldCellSpace));
	float3 normalW = cross(tangent, bitangent);

	// Get the depth value of the pixel by dividing the Z pixel by the homogeneous W coordinate
	float depth = input.depth.z / input.depth.w;

	//============================================ Calculate position of camera eye for lighting

	// The toEye vector is used in lighting.
	float3 toEye = g_EyePosition - input.positionW;

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;

	float4 camWorld = float4(input.worldPos - float4(g_EyePosW, 1));
	float d = length(camWorld);

	//=======================================================
	// Create terrain normals

	//where pixelSize.x will be the width and pixelSize.y will be the height of pixels
	float2 pixelSize = float2(1 / g_TextureWidth, 1 / g_TextureHeight);

	//============================================ Normal Mapping

	float3 normal = float4(0, 1, 0, 1);

	if (g_useNormalMap == 1)
	{
		float4 normalMap = g_NormalMap.Sample(g_LinearSampler, input.uv);
		normal = (normalMap.rgb * 2.0f) - 1.0f;
		normal = normalW + normal.x * tangent + normal.y * bitangent;
		normal = normalize(normal);
	}
	else
	{
		if (g_UseSobelFilter == 1)
		{
			/*
			Coordinates are laid out as follows:

			0,0 | 1,0 | 2,0
			----+-----+----
			0,1 | 1,1 | 2,1
			----+-----+----
			0,2 | 1,2 | 2,2
			*/

			// Compute the necessary offsets:
			float2 o00 = input.uv + float2(-pixelSize.x, -pixelSize.y);
			float2 o10 = input.uv + float2(0.0f, -pixelSize.y);
			float2 o20 = input.uv + float2(pixelSize.x, -pixelSize.y);

			float2 o01 = input.uv + float2(-pixelSize.x, 0.0f);
			float2 o21 = input.uv + float2(pixelSize.x, 0.0f);

			float2 o02 = input.uv + float2(-pixelSize.x, pixelSize.y);
			float2 o12 = input.uv + float2(0.0f, pixelSize.y);
			float2 o22 = input.uv + float2(pixelSize.x, pixelSize.y);

			// Use of the sobel filter requires the eight samples
			// surrounding the current pixel:
			float h00 = g_HeightMap.Sample(g_LinearSampler, o00).r;
			float h10 = g_HeightMap.Sample(g_LinearSampler, o10).r;
			float h20 = g_HeightMap.Sample(g_LinearSampler, o20).r;

			float h01 = g_HeightMap.Sample(g_LinearSampler, o01).r;
			float h21 = g_HeightMap.Sample(g_LinearSampler, o21).r;

			float h02 = g_HeightMap.Sample(g_LinearSampler, o02).r;
			float h12 = g_HeightMap.Sample(g_LinearSampler, o12).r;
			float h22 = g_HeightMap.Sample(g_LinearSampler, o22).r;

			// The Sobel X kernel is:
			//
			// [ 1.0  0.0  -1.0 ]
			// [ 2.0  0.0  -2.0 ]
			// [ 1.0  0.0  -1.0 ]

			float Gx = h00 - h20 + 2.0f * h01 - 2.0f * h21 + h02 - h22;

			// The Sobel Y kernel is:
			//
			// [  1.0    2.0    1.0 ]
			// [  0.0    0.0    0.0 ]
			// [ -1.0   -2.0   -1.0 ]

			float Gy = h00 + 2.0f * h10 + h20 - h02 - 2.0f * h12 - h22;

			// Generate the missing Z component - tangent
			// space normals are +Z which makes things easier
			// The 0.5f leading coefficient can be used to control
			// how pronounced the bumps are - less than 1.0 enhances
			// and greater than 1.0 smoothes.
			float Gz = 0.5f * sqrt(1.0f - Gx * Gx - Gy * Gy);

			// Make sure the returned normal is of unit length
			normal = normalize(float3(2.0f * Gx, 2.0f * Gy, Gz));
		}
		else
		{
			// Determine the offsets
			float2 o1 = float2(pixelSize.x, 0.0f);
			float2 o2 = float2(0.0f, pixelSize.y);

			// Take three samples to determine two vectors that can be
			// use to generate the normal at this pixel
			float h0 = g_HeightMap.Sample(g_LinearSampler, input.uv).r;
			float h1 = g_HeightMap.Sample(g_LinearSampler, input.uv + o1).r;
			float h2 = g_HeightMap.Sample(g_LinearSampler, input.uv + o2).r;

			float3 v01 = float3(o1, h1 - h0);
			float3 v02 = float3(o2, h2 - h0);

			float3 n = cross(v01, v02);

			// Can be useful to scale the Z component to tweak the
			// amount bumps show up, less than 1.0 will make them
			// more apparent, greater than 1.0 will smooth them out
			n.z *= 0.5f;

			normal = normalize(n);
		}
	}

	//============================================ Texturing

	/*
	Texture2D g_TextureMask;
	Texture2D g_PaintingTexture;

	float4 paintingTexColor = float4(1, 1, 1, 1);

	if (g_Painting == 1)
	{
	float maskColor = g_TextureMask.Sample(SampleTypeLinear, input.uv).r;
	if (maskColor == 1.0f)
	{
	paintingTexColor = g_PaintingTexture.Sample(SampleTypeLinear, input.uv);
	}
	}

	// Regular layer map texturing
	...

	finalColor = paintingTexColor * texColor;
	*/

	// Sample layers in texture array.
	float4 c0 = g_LayerMapArray.Sample(g_LinearSampler, float3(input.TiledTex, 0.0f));
	float4 c1 = g_LayerMapArray.Sample(g_LinearSampler, float3(input.TiledTex, 1.0f));
	float4 c2 = g_LayerMapArray.Sample(g_LinearSampler, float3(input.TiledTex, 2.0f));
	float4 c3 = g_LayerMapArray.Sample(g_LinearSampler, float3(input.TiledTex, 3.0f));

	// Sample the blend map.
	float4 t = g_BlendMap.Sample(g_LinearSampler, input.uv);

	// Blend the layers on top of each other.
	float4 texColor = c0;
	texColor = lerp(texColor, c1, t.r);
	texColor = lerp(texColor, c2, t.g);
	texColor = lerp(texColor, c3, t.b);

	finalColor = texColor;

	//============================================ Detail Mapping

	// Check if the depth value is close to the screen
	//if (depth < 500.0f)
	{
		float4 detailColor = g_DetailMap.Sample(g_LinearSampler, input.uv);
		finalColor *= detailColor * g_DetailBrightness;
	}

	//============================================ Shadow Mapping

	//float3 shadowColor = float3(1.0f, 1.0f, 1.0f);
	float shadowColor = 1.0f;// = ShadowMapPCH(g_DirectionalLight[0].g_LightDirection, g_ShadowMatrix, g_ShadowMap);

	if (g_useShadowMap == 1)
	{
		//shadowColor = ComputeShadowColor(input.shadowPos, g_DirectionalLight[0].g_LightDirection, normal, g_ShadowMap);

		//float4 lightingPosition = input.shadowPos;
		//float3 lightPos = g_DirectionalLight[0].g_LightDirection;

		//float3 shadowColor = float3(1.0f, 1.0f, 1.0f);

		// Find the position in the shadow map for this pixel
		//float2 ShadowTexCoord = 0.5 * lightingPosition.xy / lightingPosition.w + float2( 0.5, 0.5 );

		//ShadowTexCoord.y = 1.0f - ShadowTexCoord.y;

		// Get the current depth stored in the shadow map
		//float4 shadowInfo = g_ShadowMap.Sample(ShadowMapSampler, ShadowTexCoord);
		//float shadowdepth = shadowInfo.r;
		//float shadowOpacity = 0.5f + 0.5f * (1 - shadowInfo.g);

		// Calculate the current pixel depth
		// The bias is used to prevent floating point errors that occur when
		// the pixel of the occluder is being drawn
		//float ourdepth = (lightingPosition.z / lightingPosition.w) - 0.0001f;

		// Check to see if this pixel is in front or behind the value in the shadow map
		//if (shadowdepth < ourdepth)
		//{
		// Calculate the amount of light on this pixel
		//	float intensity = saturate(dot(normal, lightPos));

		//	float3 lightDirection = -lightPos;

		//	if (intensity > 0)
		//	{
		//		float3 lightColor = finalColor.rgb * saturate(intensity);
		//		float3 toEye = g_EyePosW.xyz - input.positionW;
		//		toEye = normalize(toEye);
		//		float3 halfWay = normalize(toEye + lightDirection);
		//		float NDotH = saturate(dot(halfWay, normal));
		//		lightColor += finalColor.rgb * pow(NDotH, g_MaterialSpecularPower) * g_MaterialSpecularIntensity * shadowOpacity;
		//		finalColor.rgb *= lightColor * g_MaterialDiffuseColor.rgb;

		//finalColor += (g_MaterialDiffuseColor * intensity);
		//		finalColor = saturate(finalColor);
		//	}
		//}

		float4 shadowPos = input.shadowPos;

		shadowPos.xyz /= shadowPos.w;

		float depth = shadowPos.z;

		const float dx = SMAP_DX;

		float percentLit = 0.0f;
		const float2 offsets[9] =
		{
			float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
			float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
			float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
		};

		[unroll]
		for (int i = 0; i < 9; i++)
		{
			percentLit += g_ShadowMap.SampleCmpLevelZero(ShadowMapSamplerComparison, shadowPos.xy + offsets[i], depth).r;
		}

		shadowColor = percentLit / 9.0f;
	}

	//============================================ SSAO

	float4 ssaoPos = input.ssaoPos;
	ssaoPos /= ssaoPos.w;
	float ssaoColor = 1.0f;
	if (g_useSSAO == 1)
	{
		ssaoColor = g_SSAOMap.SampleLevel(g_LinearSampler, ssaoPos.xy, 0.0f).r;
	}

	//============================================ Simple Fog Above Water

	float waterDiff = g_seaLevel - input.worldPos.y;
	float fogValue = g_fogThinning;
	float fogAlt = g_fogAltitude;
	float fogNearDist = g_fogNear;
	float fogFarDist = g_fogFar;
	float4 fogColor = g_fogColor;

	if (waterDiff > 0)
	{
		//Elevation based fog becomes distance-based
		fogValue = 1;
		fogAlt = g_seaLevel;
		fogNearDist = 0;
		fogFarDist = 30;
		d = waterDiff;

		waterDiff = min(waterDiff / 100.0f, 1.0f);

		fogColor = lerp(g_waterBodyColor, g_waterBodyColor, waterDiff);
	}

	float l = saturate((0 - fogNearDist) / (fogFarDist - fogNearDist) / clamp(input.worldPos.y / fogAlt + 1, 1, fogValue));

	//============================================ Lighting

	//finalColor *= CalculateLightColor
	//(	input.positionW
	//,	g_EyePosW
	//,	finalColor
	//,	normal
	//,	g_MaterialDiffuseColor
	//,	g_MaterialAmbientColor
	//,	g_MaterialSpecularPower
	//,	g_MaterialSpecularIntensity
	//,   shadowColor
	//,	ssaoColor
	//);

	//============================================ Clip the terrain with the water/ocean

	if (g_useClippingPlane == 1)
	{
		clip((input.clip < 0.0) ? -1 : 1);
	}

	//============================================ Combine all Colors

	return lerp(finalColor, fogColor, l);
}

float4 TerrainTessellationWireframePS(PixelInput input) : SV_Target
{
	if (g_useClippingPlane == 1)
	{
		clip((input.clip < 0.0) ? -1 : 1);
	}

	return float4(0.9f, 0.9f, 0.9f, 1);
}

float4 TerrainTessellationShadowPS(PixelInput input) : SV_Target
{
	float depthValue = input.shadowPos.z / input.shadowPos.w;
	return float4(depthValue, depthValue, depthValue, 1.0f);
}

float4 TerrainTessellationNormalDepthPS(PixelInput input) : SV_Target
{
	//=======================================================
	// Estimate normal and tangent using central differences.
	float2 leftTex = input.uv + float2(-g_TexelCellSpaceU, 0.0f);
	float2 rightTex = input.uv + float2(g_TexelCellSpaceU, 0.0f);
	float2 bottomTex = input.uv + float2(0.0f, g_TexelCellSpaceV);
	float2 topTex = input.uv + float2(0.0f, -g_TexelCellSpaceV);

	float leftY = g_HeightMap.SampleLevel(g_PointSampler, leftTex, 0).r;
	float rightY = g_HeightMap.SampleLevel(g_PointSampler, rightTex, 0).r;
	float bottomY = g_HeightMap.SampleLevel(g_PointSampler, bottomTex, 0).r;
	float topY = g_HeightMap.SampleLevel(g_PointSampler, topTex, 0).r;

	float worldCellSpace = 0.5f;
	float3 tangent = normalize(float3(2.0f * worldCellSpace, rightY - leftY, 0.0f));
	float3 bitangent = normalize(float3(0.0f, bottomY - topY, -2.0f * worldCellSpace));
	float3 normalW = cross(tangent, bitangent);

	float3 posV = mul(float4(input.positionW, 1.0f), g_View).xyz;
	float3 normalV = mul(normalW, (float3x3)g_View);

	return float4(normalV, posV.z);
}