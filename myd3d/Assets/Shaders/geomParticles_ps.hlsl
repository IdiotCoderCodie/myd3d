cbuffer CameraBuffer
{
	float3 cameraPosition;
	float  padding;
};

struct light
{
	int   enabled;
	int    shadows;
	float4 position;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float  spotCutoff;
	float3 spotDirection;
	float  spotExponent;
	float3 attenuation;
	float2 padding;
};
StructuredBuffer<light> LightBuffer : register(t2);

struct PixelInputType
{
	float4 position	: SV_POSITION;
    float3 normal	: NORMAL;
	float4 color	: COLOR;
	float2 uv		: TEXCOORD0;
	float4 lightPos	: TEXCOORD1;
	// TODO: change/add other stuff
};


void accumulateLights(StructuredBuffer<light> lights, float3 pos, float3 norm, float3 eye,
	float shininess, inout float4 ambient, inout float4 diffuse,
	inout float4 specular, PixelInputType input)
{
	uint numLights = 0;
	uint dummy = 0;
	lights.GetDimensions(numLights, dummy);
	
	for (uint index = 0; index < 1; ++index)
	{	
		if (lights[index].enabled)
		{
			float3 lightVec = normalize(input.lightPos);

			float NdotL = max(dot(norm, lightVec), 0.0);

			float3 H		 = normalize(lightVec - normalize(pos - eye));
			float  NdotH	 = max(dot(norm, H), 0.0);
			float  exponent	 = max(128.0 / max(0.0, min(128.0, shininess)), 0.0);
			float4 litResult = lit(NdotL, NdotH, exponent);

			ambient += (lights[index].ambient * litResult.x);
			diffuse  += (lights[index].diffuse * litResult.y);
			specular += (lights[index].specular * litResult.z);
			
		}
	}
}


float4 main(PixelInputType input) : SV_TARGET
{
 /*   return float4(input.normal, 1.0f);
    return float4(0.0, 1.0, 0.0, 1.0);*/

	float4 ambient = float4(0.0, 0.0, 0.0, 1.0);
	float4 diffuse = float4(0.0, 0.0, 0.0, 1.0);
	float4 specular = float4(0.0, 0.0, 0.0, 1.0);

	static float4 grassColor = float4(0.2, 0.6, 0.2, 1.0f);
	static float4 snowColor = float4(0.95, 0.95, 0.95, 1.0f);

	//float terrainBlend = smoothstep(0.0, 1.0, input.terrainHeight);
	//float4 terrainColor = lerp(grassColor, snowColor, terrainBlend);

    

	accumulateLights(LightBuffer, input.position, input.normal, cameraPosition, 1.0,
						ambient, diffuse, specular, input);

	float4 lightColor = ambient;
	lightColor += diffuse;
	lightColor += specular;

	float4 finalColor = lightColor;
	return finalColor;
	//float4 finalColor = terrainColor * lightColor;

	//	finalColor = saturate(finalColor);

	//return finalColor;
	
	//finalColor += specular;



    //return float4(input.normal, 1.0f);
	//return input.color;
	//return float4(0.0f, 0.0f, 0.0f, 1.0f);
}