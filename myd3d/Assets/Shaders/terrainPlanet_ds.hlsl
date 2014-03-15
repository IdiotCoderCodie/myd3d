#define MAX_SHADOWCASTING_LIGHTS 4

SamplerState SampleTypeWrap : register(s0);

Texture2D heightMapTex	    : register(t0);

cbuffer MatrixBuffer
{
	matrix modelMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct PixelInputType
{
	float4 position  : SV_POSITION;
    float3 normal    : NORMAL;
	float4 color	 : COLOR;
	// TODO: change/add other stuff
};

struct HullOutputType
{
	float3 position : POSITION; 
	float3 normal   : NORMAL;
	float2 uv		: TEXCOORD0;
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

	

    // Interpolate normal.
    float3 edge1 = patch[1].position - patch[0].position;
    float3 edge2 = patch[2].position - patch[0].position;

    float3 thisNormal = patch[0].normal * domain.x 
                        + patch[1].normal * domain.y 
                        + patch[2].normal * domain.z;

	thisNormal = normalize(thisNormal);
	output.normal = thisNormal;

	float2 heightUV = domain.x * patch[0].uv
		+ domain.y * patch[1].uv
		+ domain.z * patch[2].uv;

	float heightMapVal = heightMapTex.Gather(SampleTypeWrap, heightUV).r;

	float3 vertexPosition = domain.x * patch[0].position
		+ domain.y * patch[1].position
		+ domain.z * patch[2].position;

	vertexPosition += thisNormal * heightMapVal;

	output.position = mul(float4(vertexPosition, 1.0f), modelMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.color = float4(heightMapVal, heightMapVal, heightMapVal, 1.0f);

	return output;
}
