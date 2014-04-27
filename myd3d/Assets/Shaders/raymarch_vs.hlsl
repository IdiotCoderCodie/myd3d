struct VertexInput
{
    float4 Position : POSITION;
	float2 uv		: TEXCOORD0;
	float3 normal	: NORMAL;
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float2 Tex       : TEXCOORD0;
};


VertexOutput main( VertexInput input )
{
	VertexOutput Output;

    Output.Tex = sign(input.Position.xy);
    Output.Position =  float4(Output.Tex, 0.0f, 1.0f);

    return Output;
}
