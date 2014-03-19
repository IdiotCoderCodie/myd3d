#define MAX_SHADOWCASTING_LIGHTS 4

struct VertexInputType
{
	float4 position	: POSITION;
	float2 uv		: TEXCOORD0;
	float3 normal	: NORMAL;
};


struct VertexOutputType
{
	float4 position										: POSITION;
	float2 uv											: TEXCOORD0;
	float3 normal										: NORMAL;
	/*float4 lightViewPosition[MAX_SHADOWCASTING_LIGHTS]	: TEXCOORD1;
	float3 lightPos[MAX_SHADOWCASTING_LIGHTS]			: COLOR0;*/
};


VertexOutputType main( VertexInputType input )
{
	VertexOutputType output;

	output.position = input.position;
	output.normal = input.normal;
	output.uv = input.uv;

	return output;
}
