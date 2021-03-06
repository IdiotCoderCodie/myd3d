#define MAX_SHADOWCASTING_LIGHTS 4

SamplerState SampleTypeWrap : register(s0);

Texture2D heightMapTex	    : register(t0);

cbuffer MatrixBuffer
{
	matrix modelMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer EllipsoidBuffer
{
	float ellipA;
    float ellipB;
    float ellipC;
    float padding;
};

cbuffer LightPositionBuffer
{
	float4 lightPosition;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
    float3 normal   : NORMAL;
	float4 color	: COLOR;
	float2 uv		: TEXCOORD0;
	float4 lightPos	: TEXCOORD1;
	float  terrainHeight : TEXCOORD2;
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

	// Calculate interpolated normal, used for displacing vertex.
    float3 thisNormal = patch[0].normal * domain.x 
                        + patch[1].normal * domain.y 
                        + patch[2].normal * domain.z;

	thisNormal = normalize(thisNormal);
	output.normal = thisNormal;

	// Calculate interpolated UV coords.
	float2 heightUV = domain.x * patch[0].uv
		+ domain.y * patch[1].uv
		+ domain.z * patch[2].uv;

	output.uv = heightUV;

	float2 UV = heightUV * 2 * 3.14159265359;

    float a = ellipA;
    float b = ellipB;
    float c = ellipC;

    float x = a * cos(UV.y) * sin(UV.x);
    float y = b * sin(UV.y) * sin(UV.x);
    float z = c * cos(UV.x);

    float3 vertexPosition = float3(x, y, z);

    // Calculate ellipsoid normal.. TEST2
    float3 ellipsoidNormal = normalize(float3(x * b * c, y * a * c, z * a * b));
    
    output.normal = mul(float4(ellipsoidNormal, 0.0f), modelMatrix);
	//float heightMapVal = heightMapTex.Gather(SampleTypeWrap, heightUV).r;

	//// Calculate position of newly generated vertex, then displace.
	//float3 vertexPosition = domain.x * patch[0].position
	//	+ domain.y * patch[1].position
	//	+ domain.z * patch[2].position;

	//vertexPosition += thisNormal * heightMapVal * terrainHeight;

	//output.terrainHeight = heightMapVal;

	output.position = mul(float4(vertexPosition, 1.0f), modelMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);


	// Test Calculating Normal - Works. Try in PS.
	/*float4 h;
	float texelSize = terrainTexelSize;
	h[0] = heightMapTex.Gather(SampleTypeWrap, heightUV + texelSize * float2(0, -1)).r * terrainHeight;
	h[1] = heightMapTex.Gather(SampleTypeWrap, heightUV + texelSize * float2(-1, 0)).r * terrainHeight;
	h[2] = heightMapTex.Gather(SampleTypeWrap, heightUV + texelSize * float2(1, 0)).r * terrainHeight;
	h[3] = heightMapTex.Gather(SampleTypeWrap, heightUV + texelSize * float2(0, 1)).r * terrainHeight;

	float3 n;
	n.z = h[0] - h[3];
	n.x = h[1] - h[2];
	n.y = 0.8;

	n = normalize(n);

	output.normal = n;*/

	//output.color = float4(heightMapVal, heightMapVal, heightMapVal, 1.0f);

	float4 worldPosition = mul(float4(vertexPosition, 1.0f), modelMatrix);
	output.lightPos = normalize(lightPosition - worldPosition);


	return output;
}
