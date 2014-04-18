Texture2D shaderTexture;
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
    return saturate(textureColor * float4(matColor, 1.0));
}