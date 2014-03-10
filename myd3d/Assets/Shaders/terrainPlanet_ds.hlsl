#define MAX_SHADOWCASTING_LIGHTS 4

cbuffer MatrixBuffer
{
	matrix modelMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct PixelInputType
{
	float4 position  : SV_POSITION;
	float4 color	 : COLOR;
	// TODO: change/add other stuff
};

struct HullOutputType
{
	float3 position : POSITION;
	float4 color	: COLOR;
};

// Output patch constant data.
struct HullConstantOutputType
{
	float EdgeTessFactor[3]	: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor	: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
PixelInputType main( HullConstantOutputType input, float3 domain : SV_DomainLocation,
			    const OutputPatch<HullOutputType, NUM_CONTROL_POINTS> patch )
{
	PixelInputType output;

	float3 vertexPosition = domain.x * patch[0].position
							+ domain.y * patch[1].position
							+ domain.z * patch[2].position;

	output.position = mul(float4(vertexPosition, 1.0f), modelMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.color = patch[0].color;

	return output;
}
