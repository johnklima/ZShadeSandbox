//==============================================================================
// Tessellation Texture Pixel Shader
//
//==============================================================================
// History
//
// -Created on 4/18/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cShadingConst : register(b0)
{
	// Position of the camera
	float3		g_EyePosW;
	float		padding1;
	
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
	
	// The direction of the sun and lighting
	float3		g_SunDir;
	float		padding2;
	float4		g_AmbientColor;
	float4		g_SpecularColor;
	float4		g_DiffuseColor;
	
	// Control variables
	int			g_useClippingPlane;
	int			g_useNormalMap;
	int			g_UseSobelFilter;
	int			g_useShadowMap;
};

//======================================================================================================

//
// Globals
//

Texture2DArray g_LayerMapArray  : register(t0);
Texture2D      g_BlendMap 		: register(t1);
Texture2D      g_NormalMap 		: register(t2);
Texture2D      g_HeightMap 		: register(t3);

//------------------------------------------------------------------
//Sample States
//------------------------------------------------------------------
SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

//======================================================================================================

//
// Pixel Shader
//

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
	//float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
	float2 TiledTex 	: TEXCOORD1;
	float4 worldPos 	: TEXCOORD2;
	float clip      	: TEXCOORD3;
	float leftY     	: TEXCOORD4;
	float rightY   		: TEXCOORD5;
	float bottomY   	: TEXCOORD6;
	float topY      	: TEXCOORD7;
};

float4 TerrainTessellationTriPS(PixelInput input) : SV_Target
{
	float4 finalColor;
	
	//=======================================================
	// Create terrain normals
	
	//where pixelSize.x will be the width and pixelSize.y will be the height of pixels
	float2 pixelSize = float2(1 / g_TextureWidth, 1 / g_TextureHeight);
	
	//============================================ Normal Mapping

	float3 normal = float4(0, 1, 0, 1);
	
	if (g_useNormalMap == 1)
	{
		float4 normalMap = g_NormalMap.Sample( SampleType, input.uv );

		normal = (normalMap.rgb * 2.0f) - 1.0f;
	
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
			float2 o00 = input.uv + float2( -pixelSize.x, -pixelSize.y );
			float2 o10 = input.uv + float2(         0.0f, -pixelSize.y );
			float2 o20 = input.uv + float2(  pixelSize.x, -pixelSize.y );

			float2 o01 = input.uv + float2( -pixelSize.x, 0.0f         );
			float2 o21 = input.uv + float2(  pixelSize.x, 0.0f         );

			float2 o02 = input.uv + float2( -pixelSize.x,  pixelSize.y );
			float2 o12 = input.uv + float2(         0.0f,  pixelSize.y );
			float2 o22 = input.uv + float2(  pixelSize.x,  pixelSize.y );

			// Use of the sobel filter requires the eight samples
			// surrounding the current pixel:
			float h00 = g_HeightMap.Sample( SampleType, o00 ).r;
			float h10 = g_HeightMap.Sample( SampleType, o10 ).r;
			float h20 = g_HeightMap.Sample( SampleType, o20 ).r;

			float h01 = g_HeightMap.Sample( SampleType, o01 ).r;
			float h21 = g_HeightMap.Sample( SampleType, o21 ).r;

			float h02 = g_HeightMap.Sample( SampleType, o02 ).r;
			float h12 = g_HeightMap.Sample( SampleType, o12 ).r;
			float h22 = g_HeightMap.Sample( SampleType, o22 ).r;
			
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
			float Gz = 0.5f * sqrt( 1.0f - Gx * Gx - Gy * Gy );

			// Make sure the returned normal is of unit length
			normal = normalize( float3( 2.0f * Gx, 2.0f * Gy, Gz ) );
		}
		else
		{
			// Determine the offsets
			float2 o1 = float2( pixelSize.x, 0.0f        );
			float2 o2 = float2( 0.0f,        pixelSize.y );

			// Take three samples to determine two vectors that can be
			// use to generate the normal at this pixel
			float h0 = g_HeightMap.Sample( SampleType, input.uv ).r;
			float h1 = g_HeightMap.Sample( SampleType, input.uv + o1 ).r;
			float h2 = g_HeightMap.Sample( SampleType, input.uv + o2 ).r;
						
			float3 v01 = float3( o1, h1 - h0 );
			float3 v02 = float3( o2, h2 - h0 );

			float3 n = cross( v01, v02 );

			// Can be useful to scale the Z component to tweak the
			// amount bumps show up, less than 1.0 will make them
			// more apparent, greater than 1.0 will smooth them out
			n.z *= 0.5f;

			normal =  normalize( n );
		}
	}
	
	//=======================================================
	// Estimate normal and tangent using central differences.
	//float3 tangent 		= normalize(float3(2.0f * g_WorldCellSpace, input.rightY - input.leftY, 0.0f));
	//float3 bitangent   	= normalize(float3(0.0f, input.bottomY - input.topY, -2.0f * g_WorldCellSpace));
	//float3 normalW 		= cross(tangent, bitangent);
	
	//============================================ Calculate position of camera eye for lighting
	
	// The toEye vector is used in lighting.
	float3 toEye = g_EyePosW - input.positionW;
	
	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);
	
	// Normalize.
	toEye /= distToEye;
	
	float4 camWorld = float4(input.worldPos - float4(g_EyePosW, 1));
	float d = length(camWorld);
	
	//============================================ Texturing
	
	// Sample layers in texture array.
	float4 c0 = g_LayerMapArray.Sample( SampleType, float3(input.TiledTex, 0.0f) );
	float4 c1 = g_LayerMapArray.Sample( SampleType, float3(input.TiledTex, 1.0f) );
	float4 c2 = g_LayerMapArray.Sample( SampleType, float3(input.TiledTex, 2.0f) );
	float4 c3 = g_LayerMapArray.Sample( SampleType, float3(input.TiledTex, 3.0f) );
	//float4 c4 = g_LayerMapArray.Sample( SampleType, float3(input.TiledTex, 4.0f) ); 
	
	// Sample the blend map.
	float4 t = g_BlendMap.Sample( SampleType, input.uv );
    
    // Blend the layers on top of each other.
    float4 texColor = c0;
    texColor = lerp(texColor, c1, t.r);
    texColor = lerp(texColor, c2, t.g);
    texColor = lerp(texColor, c3, t.b);
    //texColor = lerp(texColor, c4, t.a);
	
	finalColor = texColor;
	
	//============================================ Shadow Mapping
	
	//if (g_useShadowMap == 1)
	//{
	//	finalColor = ComputeShadowColor(input.worldPos, finalColor);
	//}
	
	//============================================ Simple Fog Above Water

	float waterDiff 	= g_seaLevel - input.worldPos.y;
	float fogValue    	= g_fogThinning;
	float fogAlt      	= g_fogAltitude;
	float fogNearDist 	= g_fogNear;
	float fogFarDist  	= g_fogFar;
	float4 fogColor   	= g_fogColor;
	
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
	
	// Factor in normal mapping and terrain vertex normals as well in lighting of the pixel
	float lightIntensity = saturate(dot(normal, g_SunDir));
	
	if (lightIntensity > 0.0f)
	{
		float4 lightColor;
		lightColor = (g_AmbientColor + (g_DiffuseColor * lightIntensity) + (g_SpecularColor * lightIntensity));
		lightColor.a = 1.0f;
		lightColor = saturate(lightColor);
		finalColor *= lightColor;
	}
	
	//============================================ To Clip or Not to Clip
	
	if (g_useClippingPlane == 1)
	{
		clip((input.clip < 0.0) ? -1 : 1);
	}
	
	//============================================ Combine all Colors
	
	return lerp(finalColor, fogColor, l);
}