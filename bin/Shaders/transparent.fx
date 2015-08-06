//------------------------------------------------------------------
//Globals
//------------------------------------------------------------------
matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;
Texture2D shaderTexture;
float blendAmount;
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
//Blending States
//------------------------------------------------------------------
BlendState AlphaBlendingOn
{
    BlendEnable[0] = TRUE;
    DestBlend = INV_SRC_ALPHA;
    SrcBlend = SRC_ALPHA;
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
PixelInputType transparentVertexShader(VertexInputType input)
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
float4 transparentPixelShader(PixelInputType input) : SV_Target
{
    float4 textureColor;
    
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    if (textureColor.r == 255 && textureColor.g == 0 && textureColor.b == 255)
    {
        //Set the alpha value of this pixel to the blending amount to
        //create the alpha blending effect.
        textureColor.a = blendAmount;
    }
    
    return textureColor;
}
//------------------------------------------------------------------
technique10 TransparentTech
{
    pass p0
    {
        SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_4_0, transparentVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, transparentPixelShader()));
        SetGeometryShader(NULL);
    }
}