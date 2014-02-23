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
    float4 worldPosition = mul(input.position, modelMatrix);  
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
