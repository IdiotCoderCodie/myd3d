#define MAX_SHADOWCASTING_LIGHTS 4
#define BIAS 0.0001

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap  : register(s1);

Texture2D modelTexture      : register(t0);
Texture2D normalTexture		: register(t1);
Texture2D shadowMapTextures[MAX_SHADOWCASTING_LIGHTS] : register(t3); 


struct light
{
    bool   enabled;
    float4 position;
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float  spotCutoff;
    float3 spotDirection;
    float  spotExponent;
    float3 attenuation;
    float3 padding;
};
StructuredBuffer<light> LightBuffer : register(t2);


cbuffer CameraBuffer
{
    float3 cameraPosition;
    float  padding;
};


struct PixelInputType
{
    float4 position           : SV_POSITION;
    float2 uv                 : TEXCOORD0;
    float3 normal             : NORMAL;
    float3 tangent			  : TANGENT;
    float3 binormal           : BINORMAL;
    float4 lightViewPosition[MAX_SHADOWCASTING_LIGHTS]  : TEXCOORD1;
    float3 lightPos[MAX_SHADOWCASTING_LIGHTS]           : COLOR0;
};


void accumulateLights(StructuredBuffer<light> lights, float3 pos, float3 norm, float3 eye, 
                      float shininess, inout float4 ambient, inout float4 diffuse,
                      inout float4 specular, PixelInputType input)
{
    uint numLights = 0;
    uint dummy = 0;
    lights.GetDimensions(numLights, dummy);
    LightBuffer.GetDimensions(numLights, dummy);

    for(uint index = 0; index < MAX_SHADOWCASTING_LIGHTS; ++index)
    {
        if(lights[index].enabled)
        {
            // Light is active.
            // Get the light vector.
            float3 lightVec = normalize(input.lightPos[index]);
            
            float NdotL = max(dot(norm, lightVec), 0.0);
                  
            if(index < MAX_SHADOWCASTING_LIGHTS)
            {
                float2 projectTexCoord;
                // Calculate projected texture coords.
                projectTexCoord.x =  input.lightViewPosition[index].x 
                                        / input.lightViewPosition[index].w / 2.0f + 0.5f;
                projectTexCoord.y = -input.lightViewPosition[index].y 
                                        / input.lightViewPosition[index].w / 2.0f + 0.5f;
                
                // Check if projected coords are within 0 to 1. If so, pixel is in view of light.
                if( (saturate(projectTexCoord.x) == projectTexCoord.x) &&
                    (saturate(projectTexCoord.y) == projectTexCoord.y) )
                {
                    // Sample the shadow map at projected tex coordinate.
                    float depthValue;
                    depthValue = shadowMapTextures[index].Sample(SampleTypeClamp, projectTexCoord).r;

                    // Calculate the depth of the light.
                    float lightDepthValue = input.lightViewPosition[index].z 
                                            / input.lightViewPosition[index].w;

                    lightDepthValue = lightDepthValue - BIAS;

                    if(lightDepthValue < depthValue)
                    {
                        // Pixel in view of light!

                        // Calculate spot attenuation.
                        float3 spotDir = normalize(lights[index].position - lights[index].spotDirection);
                        float cosCurAngle = dot(-lightVec, normalize(lights[index].spotDirection));
                        float cosOuterAngle = saturate(cos(LightBuffer[index].spotCutoff));
                        float cosInnerAngle = saturate(cos((1.0 - (lights[index].spotExponent / 128.0)) *
                                                                   lights[index].spotCutoff));

                        float attenuation = saturate((cosCurAngle - cosOuterAngle) / 
                                                (cosInnerAngle - cosOuterAngle));  

                       // attenuation - 1.0f;
                        


                        if(NdotL > 0.0)
                        {
                            float3 H         = normalize(lightVec - normalize(pos - eye));
                            float  NdotH     = max(dot(norm, H), 0.0);
                            float  exponent  = max(128.0 / max(0.0, min(128.0, shininess)), 0.0);
                            float4 litResult = lit(NdotL, NdotH, exponent);

                            // Light the pixel.
                            //ambient  += (lights[index].ambient  * litResult.x);
                            diffuse  += (attenuation * lights[index].diffuse  * litResult.y);
                            specular += (attenuation * lights[index].specular * litResult.z);
                        }
                    }
                }               
            }
            ambient += lights[index].ambient;
        }
        
    }
}

float4 ps_main(PixelInputType input) : SV_TARGET
{
    float4 color = float4(0.0, 0.0, 0.0, 1.0);
    float lightIntensity = 0.0f;

    // Sample the normal texture, put int range -1.0 to +1.0.
    float4 bumpMap    = (normalTexture.Sample(SampleTypeWrap, input.uv) * 2.0f) - 1.0f;

	float3 bumpNormal = 
            normalize(input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal); 

    float4 ambient  = float4(0.0, 0.0, 0.0, 1.0);
    float4 diffuse  = float4(0.0, 0.0, 0.0, 1.0);
    float4 specular = float4(0.0, 0.0, 0.0, 1.0);

    accumulateLights(LightBuffer, input.position, bumpNormal, cameraPosition, 0.0, 
                     ambient, diffuse, specular, input);

    color += ambient;
    color += diffuse;
    //color += specular;

    color = saturate(color);

    // Sample the pixel color from the texture.
    float4 textureColor = modelTexture.Sample(SampleTypeWrap, input.uv);

    //Combine final colors.
    color = color * textureColor;

    return color;
}
