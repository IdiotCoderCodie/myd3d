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

[maxvertexcount(18)]
void main(
	triangle VertexOutputType input[3],
	inout TriangleStream< GeometryOutputType > output
)
{

    //float explode = 2.0f;
    //float3 edgeA = input[1].position.xyz - input[0].position.xyz;
    //float3 edgeB = input[2].position.xyz - input[0].position.xyz;
    //float3 triNormal = normalize(cross(edgeA, edgeB));
    //float3 scaling = triNormal * explode;

    //// Calc centre of triangle.
    //float3 centrePos = (input[0].position.xyz + input[1].position.xyz + input[2].position.xyz) / 3.0;
    //float2 centreUV = (input[0].uv + input[1].uv + input[2].uv) / 3.0;
    //centrePos += triNormal * explode;
    //float3 centreLightPos = normalize(lightPosition - mul(float4(centrePos, 1.0), modelMatrix));
    /*for(uint i = 0; i < 3; i++)
    {
        GeometryOutputType element;
        element.position = mul((input[i].position + float4(scaling, 1.0)), modelMatrix);
        element.lightPos = normalize(lightPosition - element.position);
        element.position = mul(element.position, viewMatrix);
        element.position = mul(element.position, projectionMatrix);
        element.uv       = input[i].uv;
        element.color    = float4(1.0, 1.0, 1.0, 1.0);
        element.normal = input[i].normal;
        element.normal = mul(float4(element.normal, 1.0), modelMatrix);
        output.Append(element);
    }

    for(uint i = 0; i < 3; i++)
    {
        GeometryOutputType element;
        element.position = mul((input[i].position + float4(scaling, 1.0)), modelMatrix);
        element.lightPos = normalize(lightPosition - element.position);
        element.position = mul(element.position, viewMatrix);
        element.position = mul(element.position, projectionMatrix);
        element.uv       = input[i].uv;
        element.color    = float4(1.0, 1.0, 1.0, 1.0);
        element.normal = input[i].normal;
        element.normal = mul(float4(element.normal, 1.0), modelMatrix);
        output.Append(element);
    }*/

    for(uint i = 0; i < 3; i++)
    {
        GeometryOutputType element;
        element.uv = input[i].uv;
        element.color = float4(1.0, 1.0, 1.0, 1.0);
        element.normal = mul(float4(input[i].normal, 1.0), modelMatrix);
        element.lightPos = normalize(lightPosition - mul(input[i].position, modelMatrix)); 
        //top left.
        float4 centrePos = mul(input[i].position, modelMatrix);
        centrePos = mul(centrePos, viewMatrix);
        centrePos = mul(centrePos, projectionMatrix);
        element.position = centrePos + float4(-0.05, +0.05, 0.0, 0.0);
        output.Append(element);

        // top right.
        element.position = centrePos + float4(0.05, 0.05, 0.0, 0.0);
        output.Append(element);

        // bot right
        element.position = centrePos + float4(0.05, -0.05, 0.0, 0.0);
        output.Append(element);
        output.RestartStrip();


        // bot right
        element.position = centrePos + float4(0.05, -0.05, 0.0, 0.0);
        output.Append(element);

        // bot left
        element.position = centrePos + float4(-0.05, -0.05, 0.0, 0.0);
        output.Append(element);

        // top left.
        element.position = centrePos + float4(-0.05, +0.05, 0.0, 0.0);
        output.Append(element);
        output.RestartStrip();

    }
    

    //GeometryOutputType element;
    //// third vertex.
    //float3 V = normalize(input[1].position.xyz - input[0].position.xyz);
    //element.position = input[2].position + (0.2)*float4(V, 0.0);
    //element.position = mul(element.position, modelMatrix);
    //element.lightPos = normalize(lightPosition - element.position);
    //element.position = mul(element.position, viewMatrix);
    //element.position = mul(element.position, projectionMatrix);
    //element.uv = input[1].uv - input[0].uv + input[2].uv;
    //element.color = float4(1.0, 1.0, 1.0, 1.0);
    //element.normal = input[2].normal;

    //output.Append(element);

   // output.RestartStrip();

	//for (uint i = 0; i < 3; i++)
	//{
	//	GeometryOutputType element;
	//	element.position = input[i].position;
 //       element.position = mul(element.position, modelMatrix);
 //       element.position = mul(element.position, viewMatrix);
 //       element.position = mul(element.position, projectionMatrix);
 //       element.uv       = input[i].uv;
 //       element.color    = float4(1.0, 1.0, 1.0, 1.0);
 //       element.normal   = input[i].normal;
 //       element.normal   = mul(float4(input[i].normal, 0.0), modelMatrix);
 //       element.lightPos = normalize(lightPosition - mul(input[i].position, modelMatrix));
	//	output.Append(element);
	//}
 //   output.RestartStrip();

 //   // Clone, move right +5
 //   for (uint i = 0; i < 3; i++)
	//{
	//	GeometryOutputType element;
	//	element.position = input[i].position;
 //       element.position.x += 5.0;
 //       element.position = mul(element.position, modelMatrix);
 //       element.lightPos = normalize(lightPosition - element.position);
 //       element.position = mul(element.position, viewMatrix);
 //       element.position = mul(element.position, projectionMatrix);
 //       element.uv = input[i].uv;
 //       element.color    = float4(1.0, 1.0, 1.0, 1.0);
 //       element.normal = input[i].normal;
 //       element.normal = mul(float4(input[i].normal, 0.0), modelMatrix);
 //       
	//	output.Append(element);
	//}
}