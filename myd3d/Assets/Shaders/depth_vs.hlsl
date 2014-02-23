cbuffer MatrixBuffer
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    // SV_POSITION semantic offsets position by 0.5, so use a second vector which is not modified
    // so we can perform depth calculations.
    float4 position : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};


PixelInputType vp_main( VertexInputType input )
{
    PixelInputType output;

    output.position = mul(input.position, modelMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.depthPosition = output.position;

	return output;
}