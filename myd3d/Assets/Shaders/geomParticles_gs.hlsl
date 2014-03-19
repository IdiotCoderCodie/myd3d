cbuffer MatrixBuffer
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

cbuffer LightPositionBuffer
{
	float4 lightPosition;
};

struct VertexOutputType
{
	float4 position										: POSITION;
	float2 uv											: TEXCOORD0;
	float3 normal										: NORMAL;
};

struct GeometryOutputType
{
	float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float4 color	: COLOR;  
    float2 uv       : TEXCOORD0;    
    float4 lightPos : TEXCOORD1;
};

[maxvertexcount(6)]
void main(
	triangle VertexOutputType input[3],
	inout TriangleStream< GeometryOutputType > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GeometryOutputType element;
		element.position = input[i].position;
        element.position = mul(element.position, modelMatrix);
        element.position = mul(element.position, viewMatrix);
        element.position = mul(element.position, projectionMatrix);
        element.uv       = input[i].uv;
        element.color    = float4(1.0, 1.0, 1.0, 1.0);
        element.normal   = input[i].normal;
        element.normal   = mul(float4(input[i].normal, 0.0), modelMatrix);
        element.lightPos = normalize(lightPosition - mul(input[i].position, modelMatrix));
		output.Append(element);
	}
    output.RestartStrip();

    // Clone, move right +5
    for (uint i = 0; i < 3; i++)
	{
		GeometryOutputType element;
		element.position = input[i].position;
        element.position.x += 5.0;
        element.position = mul(element.position, modelMatrix);
        element.position = mul(element.position, viewMatrix);
        element.position = mul(element.position, projectionMatrix);
        element.uv = input[i].uv;
        element.color    = float4(1.0, 1.0, 1.0, 1.0);
        element.normal = input[i].normal;
        element.normal = mul(float4(input[i].normal, 0.0), modelMatrix);
        element.lightPos = normalize(lightPosition - mul(input[i].position, modelMatrix));
		output.Append(element);
	}
}