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

cbuffer ParticleBuffer
{
    int     particleCount;
    float   particleSize;
    float2  padding;
};

cbuffer TimeBuffer
{
    float   timePassed;
    float3  padding4;
};

struct VertexOutputType
{
	float4 position										: POSITION;
	float2 uv											: TEXCOORD0;
	float3 normal										: NORMAL;
    uint   id : TEXCOORD1;
};

struct GeometryOutputType
{
	float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float4 color	: COLOR;  
    float2 uv       : TEXCOORD0;    
    float4 lightPos : TEXCOORD1;
};

[maxvertexcount(18)]
void main(
	triangle VertexOutputType input[3],
	inout TriangleStream< GeometryOutputType > output
)
{
    for(uint i = 0; i < 3; i++)
    {
        if(input[i].id > particleCount)
            return;
        
        float timeLoop = fmod(timePassed, 2.50f);//timePassed % 10.0f;

        float3 velocityDir = normalize(input[i].position.xyz);
         //   float4 centrePos = input[i].position;
        float4 centrePos = float4(0.0f, 0.0f, 0.0f, 1.0f);
        centrePos.xyz += (timeLoop) * velocityDir * 2.0f;


       // float zVal = 10.0f / input[i].id;
        float zVal = input[i].id / 50.0f;
        float xVal = sin(input[i].id / 100.0f) / 100.0f;
        float yVal = sin(input[i].id / 100.0f) / 100.0f;
        GeometryOutputType element;
        element.uv = input[i].uv;
        element.color = float4(1.0, 1.0, 1.0, 1.0);
        element.normal = mul(float4(input[i].normal, 1.0), modelMatrix);
        element.lightPos = normalize(lightPosition - mul(input[i].position, modelMatrix)); 
        //top left.
        //float4 centrePos = float4(0.0f, 0.0f, zVal, 1.0f);
        centrePos = mul(centrePos, modelMatrix);
        centrePos = mul(centrePos, viewMatrix);
        centrePos = mul(centrePos, projectionMatrix);
        element.position = centrePos + float4(-particleSize, +particleSize, 0.0, 0.0);
        element.uv = float2(1.0, 0.0);
        output.Append(element);

        // top right.
        element.position = centrePos + float4(particleSize, particleSize, 0.0, 0.0);
        element.uv = float2(0.0, 0.0);
        output.Append(element);

        // bot right
        element.position = centrePos + float4(particleSize, -particleSize, 0.0, 0.0);
        element.uv = float2(0.0, 1.0);
        output.Append(element);
        output.RestartStrip();


        // bot right
        element.position = centrePos + float4(particleSize, -particleSize, 0.0, 0.0);
        element.uv = float2(0.0, 1.0);
        output.Append(element);

        // bot left
        element.position = centrePos + float4(-particleSize, -particleSize, 0.0, 0.0);
        element.uv = float2(1.0, 1.0);
        output.Append(element);

        // top left.
        element.position = centrePos + float4(-particleSize, +particleSize, 0.0, 0.0);
        element.uv = float2(1.0, 0.0);
        output.Append(element);

        output.RestartStrip();

    }
  
}