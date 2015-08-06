//==============================================================================
// Deferred Shading Common
//
//==============================================================================
// History
//
// -Created on 5/22/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

#define MSAA_SAMPLE_COUNT 4

cbuffer cbDeferredConfigBuffer : register(b1)
{
	int		g_UseDiscontinuity;
	int		g_UseAdaptiveShading;
	int		g_UseSeprateEdgePass;
	int		g_ShowComplexPixels;
	int		g_UseOrenNayar;
	int3	padding;
};

//======================================================================================================

//
// Samplers and textures
//

Texture2DMS<float4, MSAA_SAMPLE_COUNT> texGBufferMS1 : register(t0);
Texture2DMS<float4, MSAA_SAMPLE_COUNT> texGBufferMS2 : register(t1);
Texture2D texGBuffer2								 : register(t2);

SamplerState pointSampler							 : register(s0);

//======================================================================================================

//
// Structs
//

struct VQPixelInput
{
	float4 position		: SV_POSITION;
	float4 projectedPos : TEXCOORD0;
	float2 uv			: TEXCOORD1;
};

//======================================================================================================

//
// Vertex Shader for a fullscreen quad
// The Quad is computed here
//

VQPixelInput QuadVS(uint id : SV_VertexID)
{
	VQPixelInput output;
	
	output.uv = float2((id << 1) & 2, id & 2);
	output.position = float4(output.uv * float2(2, -2) + float2(-1, 1), 0, 1);
	output.projectedPos = output.position;
	
	return output;
}

//======================================================================================================

//
// Pixel Shader
//

float4 ComplexPS(VQPixelInput input) : SV_Target
{
	if (g_UseDiscontinuity == 1)
	{
		float4 gBuffer1 = texGBufferMS1.Load(input.position.xy, 0);
		float4 gBuffer2 = texGBufferMS2.Load(input.position.xy, 0);
		
		float3 normal = gBuffer1.xyz;
		float depth = gBuffer1.w;
		float3 albedo = gBuffer2.xyz;
		
		[unroll]
		for (int i = 1; i < MSAA_SAMPLE_COUNT; i++)
		{
			float4 nextGBuffer1 = texGBufferMS1.Load(input.position.xy, i);
			float4 nextGBuffer2 = texGBufferMS2.Load(input.position.xy, i);
			
			float3 nextNormal = nextGBuffer1.xyz;
			float nextDepth = nextGBuffer1.w;
			float3 nextAlbedo = nextGBuffer2.xyz;
			
			[branch]
			if (abs(depth - nextDepth) > 0.1f
			 || abs(dot(abs(normal - nextNormal), float3(1, 1, 1))) > 0.1f
			 || abs(dot(albedo - nextAlbedo, float3(1, 1, 1))) > 0.1f)
			{
				clip( -0.5f );
				return 0;
			}
		}
	}
	else if (g_UseDiscontinuity == 0)
	{
		if (texGBuffer2.Sample(pointSampler, input.uv).a)
		{
			clip( -0.5f );
			return 0;
		}
	}
	
	return 0;
}