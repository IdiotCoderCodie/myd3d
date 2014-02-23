Texture2D particleTexture : register(t0);
SamplerState SampleTypeWrap;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD0;
    float4 color    : COLOR;
};

float4 ps_main(PixelInputType input) : SV_TARGET
{
	float4 textureColor = particleTexture.Sample(SampleTypeWrap, input.uv);

    float4 finalColor = textureColor * input.color;

    return finalColor;
}
