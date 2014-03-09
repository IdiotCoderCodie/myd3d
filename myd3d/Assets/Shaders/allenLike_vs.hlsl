#define MAX_SHADOWCASTING_LIGHTS 4

cbuffer MatrixBuffer
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix       [MAX_SHADOWCASTING_LIGHTS];
    matrix lightProjectionMatrix [MAX_SHADOWCASTING_LIGHTS];
};

cbuffer LightPositionBuffer
{
    float4 lightPosition         [MAX_SHADOWCASTING_LIGHTS];
};


cbuffer TimeBuffer
{
    float time;
    float3 padding;
};


struct VertexInputType
{
    float4 position : POSITION;
    float2 uv       : TEXCOORD0;
    float3 normal   : NORMAL;
};


struct PixelInputType
{
    float4 position                                     : SV_POSITION;
    float2 uv                                           : TEXCOORD0;
    float3 normal                                       : NORMAL;
    float4 lightViewPosition[MAX_SHADOWCASTING_LIGHTS]  : TEXCOORD1;
    float3 lightPos[MAX_SHADOWCASTING_LIGHTS]           : COLOR0;
};


PixelInputType vp_main( VertexInputType input )
{
    PixelInputType output;

    input.position.w = 1.0f; // Ensure w is 1.0.

    // Multiply position by MVP.

    float4 newPos = input.position;

    //newPos += float4(input.normal, 0.0) * 5.0f;
    //newPos = newPos + float4(input.normal, 0.0f) * smoothstep(newPos.y, 0.0, 10.0) * 5.0f * sin(time);
 
    if(abs(newPos.z) > 5.0f && abs(newPos.x) < 5.0f && abs(newPos.y) < 2.0f)
    {
        newPos = newPos + float4(input.normal, 0.0) * 10.0f;
        newPos.y += abs(sin(time * abs(newPos.z) / 2.0f)) * 5.0f; //smoothstep(abs(newPos.z), 10.0, 15.0) * sin(time * 3.0f) * 5.0f;
        newPos.y += sin(time * 5.0) * 10.0f;
    }

    float xRot = 0.0f;
    if(newPos.x > 8.7)
    {
       // xRot = time * 5.0f;
        newPos.y = -newPos.y;
        newPos.z = -newPos.z;
        newPos.y -= 3.0f;
        newPos.x -= 3.0f;
        input.normal.y = -input.normal.y;
        input.normal.z = -input.normal.z;
        newPos = newPos + float4(input.normal, 0.0f) * 1.0f;
    }

    if(newPos.x < -11.5)
    {
        newPos.x -= 5.0f;
    }
   


    if(newPos.y < -4.0f && abs(newPos.x) < 8.0f)
    {
        newPos.y = -4.0f;
    }
    matrix xAxisRotation = {    1.0f,      0.0f,       0.0f,       0.0f,
                                0.0f, cos(xRot), -sin(xRot),       0.0f,
                                0.0f, sin(xRot),  cos(xRot),       0.0f,
                                0.0f,      0.0f,       0.0f,       1.0f };

    matrix newModelMatrix = modelMatrix;
    newModelMatrix = mul(modelMatrix, xAxisRotation);

   // newPos.x += smoothstep(newPos.y, 10.0, 15.0) * -10.0f;
    //newPos =  newPos + float4(input.normal, 1.0) * smoothstep(abs(newPos.x), 10.0, 15.0) * 10.0f; 

    newPos.x += sin(time * 5.0f) * 10.0f;
    newPos.y += cos(time * 5.0f) * 10.0f;
    float4 worldPosition = mul(newPos, newModelMatrix);  
    output.position = mul(worldPosition, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Get position from light's perspective.
    for(uint i = 0; i < MAX_SHADOWCASTING_LIGHTS; ++i)
    {
        output.lightViewPosition[i] = mul(worldPosition, lightViewMatrix[i]);
        output.lightViewPosition[i] = mul(output.lightViewPosition[i], lightProjectionMatrix[i]);

        output.lightPos[i] = normalize(lightPosition[i].xyz - worldPosition.xyz);
    }

    // Calculate normal against model matrix. 
    // NOTE: will not work if modelMatrix contains scalars.
    output.normal   = normalize( mul(input.normal,   (float3x3)modelMatrix) );
    
    output.uv = input.uv;

    return output;
}
