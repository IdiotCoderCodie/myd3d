SamplerState SampleTypeWrap  : register(s0);

Texture2D grassTexture      : register(t0);
Texture2D rockTexture		: register(t1);
Texture2D snowTexture	    : register(t2);

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float  padding;
};

struct light
{
	int   enabled;
	int    shadows;
	float2 padding;
	float4 position;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float  spotCutoff;
	float3 spotDirection;
	float  spotExponent;
	float3 attenuation;
	
};
StructuredBuffer<light> LightBuffer : register(t3);

struct PixelInputType
{
	float4 position	: SV_POSITION;
    float3 normal	: NORMAL;
	float4 color	: COLOR;
	float2 uv		: TEXCOORD0;
	float4 lightPos	: TEXCOORD1;
	float  terrainHeight : TEXCOORD2;
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
	float4 ambient = float4(0.0, 0.0, 0.0, 1.0);
	float4 diffuse = float4(0.0, 0.0, 0.0, 1.0);
	float4 specular = float4(0.0, 0.0, 0.0, 1.0);

	float4 grassColor = grassTexture.Sample(SampleTypeWrap, input.uv);
	float4 rockColor = rockTexture.Sample(SampleTypeWrap, input.uv * 5.0);
	float4 snowColor = snowTexture.Sample(SampleTypeWrap, input.uv * 5.0);

	float blendGrassRock = smoothstep(0.2, 0.65, input.terrainHeight);
	float blendRockSnow = smoothstep(0.7, 0.85, input.terrainHeight);

	//float4 terrainColor = (float4)0.0;
	//if (input.terrainHeight > 0.7)
	//{
	//	terrainColor = lerp(rockColor, snowColor, blendRockSnow);
	//}
	//else
	//{
	//	terrainColor = lerp(grassColor, rockColor, blendGrassRock);
	//}

	float4 terrainColor = (float4)0.0;

	terrainColor = lerp(grassColor, rockColor, blendGrassRock);
	float flatness = dot(normalize(input.normal), float3(0.0, 0.2, 0.0)); // 1.0 = not steep
	terrainColor = lerp(terrainColor, grassColor, flatness);

	terrainColor = lerp(terrainColor, snowColor, blendRockSnow);   

	accumulateLights(LightBuffer, input.position, input.normal, cameraPosition, 64.0,
						ambient, diffuse, specular, input);

	float4 lightColor = ambient;
	lightColor += diffuse;
	//lightColor += specular;
	
	float4 finalColor = terrainColor * lightColor;

		finalColor = saturate(finalColor);

	return finalColor;
}