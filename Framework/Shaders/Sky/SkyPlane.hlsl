//------------------------------------------------------------------
//Globals
//------------------------------------------------------------------
cbuffer cbPerFrame : register(b0)
{
	float g_Translation;
	float g_Scale;
	float g_Brightness;
	float padding;
};

cbuffer cbPerObject : register(b1)
{
    matrix gWorld;
    matrix gView;
    matrix gProj;
};

Texture2D g_CloudTexture : register(t0);
Texture2D g_PerturbTexture : register(t1);

//------------------------------------------------------------------
//Sample States
//------------------------------------------------------------------
SamplerState SampleType : register(s0);
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = WRAP;
//    AddressV = WRAP;
//};
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
};
//------------------------------------------------------------------
//Vertex Shader
//------------------------------------------------------------------
PixelInputType skyPlaneVS(VertexInputType input)
{
    PixelInputType output;
    
	input.position.w = 1.0f;
    
    output.position = mul(input.position, gWorld);
    output.position = mul(output.position, gView);
    output.position = mul(output.position, gProj);

    output.tex = input.tex;
    
	float2 moveVector = float2(1, 0);
	// Translate the texture coordinate sampling location by the translation value
	output.tex += g_Translation * moveVector;
	
    return output;
}
//------------------------------------------------------------------
//Pixel Shader
//------------------------------------------------------------------
float4 skyPlanePS(PixelInputType input) : SV_Target
{
	float4 perturbValue;
	float4 cloudColor;
	
	// Sample the texture value from the perturb texture using the translated texture coordinates
	perturbValue = g_PerturbTexture.Sample(SampleType, input.tex);
	
	// Multiply the perturb value by the perturb scale
	perturbValue = perturbValue * g_Scale;
	
	// Add the texture coordinates as well as the translation value to get the perturbed texture coordinate sampling location
	perturbValue.xy = perturbValue.xy + input.tex.xy + g_Translation;
	
	// Now sample the color from the cloud texture using the perturbed sampling coordinates
	cloudColor = g_CloudTexture.Sample(SampleType, perturbValue.xy);
	
	// Reduce the color cloud by the brightness value
	cloudColor = cloudColor * g_Brightness;
	
	return cloudColor;
	
	//float4(0.34f, 0.2f, 0.1f, 1.0f);
	
    //float4 textureColor;
    //Sample the pixel color from the texture using the sampler at this texture
    //coordinate location
    //textureColor = g_CloudTexture.Sample(SampleType, input.tex);
	//return textureColor;
}

float4 skyPlaneWireframePS(PixelInputType input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}
