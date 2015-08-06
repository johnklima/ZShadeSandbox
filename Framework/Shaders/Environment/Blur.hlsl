//==============================================================================
// Blur Shader
//
//==============================================================================
// History
//
// -Created on 7/2/2014 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbConstants : register(b0)
{
	float g_ScreenWidth;
	float g_ScreenHeight;
};

cbuffer cbPerObject : register(b1)
{
	float4x4 g_World;
	float4x4 g_View;
	float4x4 g_Proj;
};

//======================================================================================================

//
// Textures and Samplers
//

Texture2D    g_blurHorizontalTexture : register(t0);
Texture2D    g_blurVerticalTexture   : register(t1);

SamplerState blurSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	MaxAnisotropy = 1;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

//======================================================================================================

//
// Vertex Shader
//

struct VertexInput
{
	float4 position    : POSITION;
	float2 tex_uv      : TEXCOORD0;
};

struct PixelInput
{
	float4 position    : SV_POSITION;
	float2 tex_uv      : TEXCOORD0;
	float2 texCoord1   : TEXCOORD1;
	float2 texCoord2   : TEXCOORD2;
	float2 texCoord3   : TEXCOORD3;
	float2 texCoord4   : TEXCOORD4;
	float2 texCoord5   : TEXCOORD5;
	float2 texCoord6   : TEXCOORD6;
	float2 texCoord7   : TEXCOORD7;
	float2 texCoord8   : TEXCOORD8;
	float2 texCoord9   : TEXCOORD9;
};

PixelInput HorizontalBlurVS(VertexInput input)
{
	PixelInput output;

	input.position.w = 1.0f;

	output.position = mul(input.position, g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Proj);

	output.tex_uv = input.tex_uv;

	//Determine the float point size of a texel for a screen with this width
	float texelSize = 1.0f / g_ScreenWidth;

	//Create the UV coordinates for the pixel and its four horizontal neighbors on either side
	output.texCoord1 = input.tex_uv + float2(texelSize * -4.0f, 0.0f);
	output.texCoord2 = input.tex_uv + float2(texelSize * -3.0f, 0.0f);
	output.texCoord3 = input.tex_uv + float2(texelSize * -2.0f, 0.0f);
	output.texCoord4 = input.tex_uv + float2(texelSize * -1.0f, 0.0f);
	output.texCoord5 = input.tex_uv + float2(texelSize *  0.0f, 0.0f);
	output.texCoord6 = input.tex_uv + float2(texelSize *  1.0f, 0.0f);
	output.texCoord7 = input.tex_uv + float2(texelSize *  2.0f, 0.0f);
	output.texCoord8 = input.tex_uv + float2(texelSize *  3.0f, 0.0f);
	output.texCoord9 = input.tex_uv + float2(texelSize *  4.0f, 0.0f);

	return output;
}

PixelInput VerticalBlurVS(VertexInput input)
{
	PixelInput output;

	input.position.w = 1.0f;

	output.position = mul(input.position, g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Proj);

	output.tex_uv = input.tex_uv;

	//Determine the float point size of a texel for a screen with this height
	float texelSize = 1.0f / g_ScreenHeight;

	//Create the UV coordinates for the pixel and its four vertical neighbors on either side
	output.texCoord1 = input.tex_uv + float2(0.0f, texelSize * -4.0f);
	output.texCoord2 = input.tex_uv + float2(0.0f, texelSize * -3.0f);
	output.texCoord3 = input.tex_uv + float2(0.0f, texelSize * -2.0f);
	output.texCoord4 = input.tex_uv + float2(0.0f, texelSize * -1.0f);
	output.texCoord5 = input.tex_uv + float2(0.0f, texelSize *  0.0f);
	output.texCoord6 = input.tex_uv + float2(0.0f, texelSize *  1.0f);
	output.texCoord7 = input.tex_uv + float2(0.0f, texelSize *  2.0f);
	output.texCoord8 = input.tex_uv + float2(0.0f, texelSize *  3.0f);
	output.texCoord9 = input.tex_uv + float2(0.0f, texelSize *  4.0f);

	return output;
}

//======================================================================================================

//
// Pixel Shader
//

float4 HorizontalBlurPS(PixelInput input) : SV_Target
{
	//Create the weights that each neighbor pixel will contribute to the blur
	float weight0 = 1.0f;
	float weight1 = 0.9f;
	float weight2 = 0.55f;
	float weight3 = 0.18f;
	float weight4 = 0.1f;

	//Create the normalized value to average the weights out a bit
	float normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	//Normalize the weights
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	//Initialize the color to black
	float4 color = float4(0,0,0,0);

	//Add the nine horizontal pixels to the color by the specific weight of each
	color += g_blurHorizontalTexture.Sample( blurSampler, input.texCoord1 ) * weight4;
	color += g_blurHorizontalTexture.Sample( blurSampler, input.texCoord2 ) * weight3;
	color += g_blurHorizontalTexture.Sample( blurSampler, input.texCoord3 ) * weight2;
	color += g_blurHorizontalTexture.Sample( blurSampler, input.texCoord4 ) * weight1;
	color += g_blurHorizontalTexture.Sample( blurSampler, input.texCoord5 ) * weight0;
	color += g_blurHorizontalTexture.Sample( blurSampler, input.texCoord6 ) * weight1;
	color += g_blurHorizontalTexture.Sample( blurSampler, input.texCoord7 ) * weight2;
	color += g_blurHorizontalTexture.Sample( blurSampler, input.texCoord8 ) * weight3;
	color += g_blurHorizontalTexture.Sample( blurSampler, input.texCoord9 ) * weight4;

	//Set the alpha channel to one
	color.a = 1.0f;

	return color;
}

float4 VerticalBlurPS(PixelInput input) : SV_Target
{
	//Create the weights that each neighbor pixel will contribute to the blur
	float weight0 = 1.0f;
	float weight1 = 0.9f;
	float weight2 = 0.55f;
	float weight3 = 0.18f;
	float weight4 = 0.1f;

	//Create the normalized value to average the weights out a bit
	float normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	//Normalize the weights
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	//Initialize the color to black
	float4 color = float4(0,0,0,0);

	//Add the nine horizontal pixels to the color by the specific weight of each
	color += g_blurVerticalTexture.Sample( blurSampler, input.texCoord1 ) * weight4;
	color += g_blurVerticalTexture.Sample( blurSampler, input.texCoord2 ) * weight3;
	color += g_blurVerticalTexture.Sample( blurSampler, input.texCoord3 ) * weight2;
	color += g_blurVerticalTexture.Sample( blurSampler, input.texCoord4 ) * weight1;
	color += g_blurVerticalTexture.Sample( blurSampler, input.texCoord5 ) * weight0;
	color += g_blurVerticalTexture.Sample( blurSampler, input.texCoord6 ) * weight1;
	color += g_blurVerticalTexture.Sample( blurSampler, input.texCoord7 ) * weight2;
	color += g_blurVerticalTexture.Sample( blurSampler, input.texCoord8 ) * weight3;
	color += g_blurVerticalTexture.Sample( blurSampler, input.texCoord9 ) * weight4;

	//Set the alpha channel to one
	color.a = 1.0f;

	return color;
}