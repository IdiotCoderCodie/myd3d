cbuffer MatrixBuffer
{
    matrix ModelMatrix;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
}

struct PixelInputType
{
    float4 position : POSITION;
};

float4 vp_main( PixelInputType input ) : SV_POSITION
{
	float4 output = mul(input.position, ViewMatrix);
    //output = mul(output, ViewMatrix);
    output = mul(output, ProjectionMatrix);

    return output;
}
