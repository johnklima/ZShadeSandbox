//------------------------------------------------------------------
//Globals
//------------------------------------------------------------------
matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;
Texture2D shaderTexture;
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
};
//------------------------------------------------------------------
//Vertex Shader
//------------------------------------------------------------------
PixelInputType textureVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    input.position.w = 1.0f;
    
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = input.tex;
    
    return output;
}
//------------------------------------------------------------------
//Pixel Shader
//------------------------------------------------------------------
float4 texturePixelShader(PixelInputType input) : SV_Target
{
    float4 textureColor;
    
    //Sample the pixel color from the texture using the sampler at this texture
    //coordinate location
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    return textureColor;
}
//------------------------------------------------------------------
technique10 TextureTech
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, textureVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, texturePixelShader()));
        SetGeometryShader(NULL);
    }
}