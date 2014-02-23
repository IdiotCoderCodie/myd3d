cbuffer MatrixBuffer
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 uv       : TEXCOORD0;
    float4 color    : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD0;
    float4 color    : COLOR;
};


PixelInputType vp_main(VertexInputType input)
{
	PixelInputType output;
    
    input.position.w = 1.0f;

    output.position = mul(input.position, modelMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.uv = input.uv;

    output.color = input.color;

    return output;
}
