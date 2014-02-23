cbuffer MatrixBuffer
{
    matrix projectionMatrix;
};


struct VertexInputType
{
    float4 position : POSITION;
    float2 uv       : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD0;
};

PixelInputType vp_main(VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.0f;

   /* output.position = mul(input.position,  worldMatrix);
    output.position = mul(output.position, viewMatrix);*/
    output.position = mul(input.position, projectionMatrix);

    output.uv = input.uv;

	return output;
}