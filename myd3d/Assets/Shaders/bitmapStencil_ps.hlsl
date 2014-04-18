Texture2D shaderTexture : register(t0);
Texture2D stencilTexture : register(t1);
SamplerState SampleType;

cbuffer MaterialBuffer
{
    float3 matColor;
    float padding;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD0;
};


float4 ps_main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    textureColor = shaderTexture.Sample(SampleType, input.uv);
    float4 stencil = stencilTexture.Sample(SampleType, input.uv);
    if(stencil.r < 0.2)
    {
        discard;
        return float4(1.0, 0.0, 0.0, 1.0);
    }
    else
    {
        //return lerp(textureColor, float4(matColor, 1.0), 0.5);
        return saturate(textureColor * float4(matColor, 1.0));
    }
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}