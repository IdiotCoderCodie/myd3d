#define MAX_SHADOWCASTING_LIGHTS 4

#define NUM_CONTROL_POINTS 16

SamplerState SampleTypeWrap : register(s0);

Texture2D heightMapTex	    : register(t0);

cbuffer MatrixBuffer
{
	matrix modelMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightPositionBuffer
{
	float4 lightPosition;
};

struct DomainOutputType
{
	float4 position : SV_POSITION;
    float3 normal   : NORMAL;
	float2 uv		: TEXCOORD0;
	float4 lightPos	: TEXCOORD1;
	// TODO: change/add other stuff
};

struct HullOutputType
{
	float3 position : POSITION; 
};


// Output patch constant data.
struct HullConstantOutputType
{
	float EdgeTessFactor[4]		: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor[2]	: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

float4 BernsteinBasis(float t)
{
	float invT = 1.0f - t;
	return float4(invT * invT * invT,
		3.0f * t * invT * invT,
		3.0f * t * t * invT,
		t * t * t);
}

float4 dBernsteinBasis(float t)
{
	float invT = 1.0f - t;
	return float4(-3 * invT * invT,
		3 * invT * invT - 6 * t * invT,
		6 * t * invT - 3 * t * t,
		3 * t * t);

}

float3 EvaluateBezier(const OutputPatch<HullOutputType, NUM_CONTROL_POINTS> bezPatch,
	float4 BasisU,
	float4 BasisV)
{
	float3 Value = float3(0.0, 0.0, 0.0);
	Value += BasisV.x * (bezPatch[0].position * BasisU.x + bezPatch[1].position * BasisU.y + bezPatch[2].position * BasisU.z + bezPatch[3].position * BasisU.w);
	Value += BasisV.y * (bezPatch[4].position * BasisU.x + bezPatch[5].position * BasisU.y + bezPatch[6].position * BasisU.z + bezPatch[7].position * BasisU.w);
	Value += BasisV.z * (bezPatch[8].position * BasisU.x + bezPatch[9].position * BasisU.y + bezPatch[10].position * BasisU.z + bezPatch[11].position * BasisU.w);
	Value += BasisV.w * (bezPatch[12].position * BasisU.x + bezPatch[13].position * BasisU.y + bezPatch[14].position * BasisU.z + bezPatch[15].position * BasisU.w);

	return Value;
}

[domain("quad")]
DomainOutputType main(HullConstantOutputType input, float2 UV : SV_DomainLocation,
			    const OutputPatch<HullOutputType, NUM_CONTROL_POINTS> patch )
{
	float4 BasisU = BernsteinBasis(UV.x);
	float4 BasisV = BernsteinBasis(UV.y);
	float4 dBasisU = dBernsteinBasis(UV.x);
	float4 dBasisV = dBernsteinBasis(UV.y);

	float3 WorldPos = EvaluateBezier(patch, BasisU, BasisV);
	float3 Tangent = EvaluateBezier(patch, dBasisU, BasisV);
	float3 BiTangent = EvaluateBezier(patch, BasisU, dBasisV);
	float3 Normal = normalize(cross(Tangent, BiTangent));

	DomainOutputType output;
	output.position = mul(float4(WorldPos, 1.0), modelMatrix);
	output.lightPos = lightPosition - output.position;
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.normal = Normal;
	output.uv = float2(0.0, 0.0);
	
	// TODO: Pass worldPos instead of lightPos to the pixel shader. Can calculate lightPos there.

	return output;

	//PixelInputType output;

	//

 //   // Interpolate normal.
 //   float3 edge1 = patch[1].position - patch[0].position;
 //   float3 edge2 = patch[2].position - patch[0].position;

	//// Calculate interpolated normal, used for displacing vertex.
 //   float3 thisNormal = patch[0].normal * domain.x 
 //                       + patch[1].normal * domain.y 
 //                       + patch[2].normal * domain.z;

	//thisNormal = normalize(thisNormal);
	//output.normal = thisNormal;

	//// Calculate interpolated UV coords.
	//float2 newUV = domain.x * patch[0].uv
	//	+ domain.y * patch[1].uv
	//	+ domain.z * patch[2].uv;

	//output.uv = newUV;

 //   float2 UV = newUV * 2 * 3.14159265359;

 //   float c = innerRadius;
 //   float a = tubeRadius;

 //   float x = (c + a * cos(UV.x)) * cos(UV.y);
 //   float y = (c + a * cos(UV.x)) * sin(UV.y);
 //   float z = a * sin(UV.x);

 //   float3 vertexPosition = float3(x, y, z);


 //   // calculate torus normal.
 //   float nx = c * cos(UV.y);
 //   float ny = c * sin(UV.y);
 //   float nz = 0.0;

 //   float3 torusNormal = normalize(float3(x - nx, y - ny, z - nz));
 //   
 //   output.normal = mul(float4(torusNormal, 0.0f), modelMatrix);
	////float heightMapVal = heightMapTex.Gather(SampleTypeWrap, heightUV).r;

	////// Calculate position of newly generated vertex, then displace.
	////float3 vertexPosition = domain.x * patch[0].position
	////	+ domain.y * patch[1].position
	////	+ domain.z * patch[2].position;

	////vertexPosition += thisNormal * heightMapVal * terrainHeight;

	////output.terrainHeight = heightMapVal;

	//output.position = mul(float4(vertexPosition, 1.0f), modelMatrix);
	//output.position = mul(output.position, viewMatrix);
	//output.position = mul(output.position, projectionMatrix);


	//// Test Calculating Normal - Works. Try in PS.
	///*float4 h;
	//float texelSize = terrainTexelSize;
	//h[0] = heightMapTex.Gather(SampleTypeWrap, heightUV + texelSize * float2(0, -1)).r * terrainHeight;
	//h[1] = heightMapTex.Gather(SampleTypeWrap, heightUV + texelSize * float2(-1, 0)).r * terrainHeight;
	//h[2] = heightMapTex.Gather(SampleTypeWrap, heightUV + texelSize * float2(1, 0)).r * terrainHeight;
	//h[3] = heightMapTex.Gather(SampleTypeWrap, heightUV + texelSize * float2(0, 1)).r * terrainHeight;

	//float3 n;
	//n.z = h[0] - h[3];
	//n.x = h[1] - h[2];
	//n.y = 0.8;

	//n = normalize(n);

	//output.normal = n;*/

	////output.color = float4(heightMapVal, heightMapVal, heightMapVal, 1.0f);

	//float4 worldPosition = mul(float4(vertexPosition, 1.0f), modelMatrix);
	//output.lightPos = normalize(lightPosition - worldPosition);


	//return output;
}
