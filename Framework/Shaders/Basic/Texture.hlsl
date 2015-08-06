//------------------------------------------------------------------
//Globals
//------------------------------------------------------------------
cbuffer cbPerFrame : register(b0)
{
	float4 gClipPlane;
	float2 padding;
	int	   g_FlipTextureH;
	int	   g_FlipTextureV;
};

cbuffer cbPerObject : register(b1)
{
    matrix gWorld;
    matrix gView;
    matrix gProj;
};

Texture2D shaderTexture : register(t0);

//------------------------------------------------------------------
//Sample States
//------------------------------------------------------------------
SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};
//------------------------------------------------------------------
//Typedefs
//------------------------------------------------------------------
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};
//------------------------------------------------------------------
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float clip : TEXCOORD1;
};
//------------------------------------------------------------------
//Vertex Shader
//------------------------------------------------------------------
PixelInputType textureVertexShader(VertexInputType input)
{
    PixelInputType output;
    
	input.position.w = 1.0f;
    
    output.position = mul(input.position, gWorld);
    output.position = mul(output.position, gView);
    output.position = mul(output.position, gProj);

    output.tex = input.tex;
    
	output.clip = dot(output.position, gClipPlane);

    return output;
}
//------------------------------------------------------------------
//Pixel Shader
//------------------------------------------------------------------
float4 texturePixelShader(PixelInputType input) : SV_Target
{
    float4 textureColor;
    
	// 1 - u: flips the texture horizontally
	if (g_FlipTextureH == 1)
		input.tex.x = 1 - input.tex.x;
	
	// 1 - v: flips the texture vertically
	if (g_FlipTextureV == 1)
		input.tex.y = 1 - input.tex.y;
	
    //Sample the pixel color from the texture using the sampler at this texture
    //coordinate location
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
	clip( input.clip < 0.0 ? -1 : 1 );

	return textureColor;
}

float4 TextureWireframePS(PixelInputType input) : SV_Target
{
	clip( input.clip < 0.0 ? -1 : 1 );
	
    return float4(0.9f, 0.9f, 0.9f, 1);
}
