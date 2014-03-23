#define MAX_SHADOWCASTING_LIGHTS 4

struct VertexInputType
{
	float3 position	: POSITION;
};


struct VertexOutputType
{
	float3 position										: POSITION;
};


VertexOutputType main( VertexInputType input )
{
	VertexOutputType output;

	output.position = input.position;

	return output;
}