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
	int     effectId;
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

float random(float2 p)
{
    // We need irrationals for pseudo randomness.
  // Most (all?) known transcendental numbers will (generally) work.
  const float2 r = float2(
    23.1406926327792690,  // e^pi (Gelfond's constant)
     2.6651441426902251); // 2^sqrt(2) (Gelfond–Schneider constant)
  return frac( cos( fmod( 123456789., 1e-7 + 256. * dot(p,r) ) ) );  
}


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

		float4 centrePos;
		float3 particleColor;

		switch (effectId)
		{
		case 0:
            {
			int randFacSeed = input[i].id % 20;

			float randomFactor = ((randFacSeed / 20.0f) * 2.0) - 1.0f;

			float timeLoop = fmod(timePassed, 5.0f);//timePassed % 10.0f;
            if(timeLoop > 2.5f)
                return;

			float3 velocityDir = normalize(input[i].position.xyz) * randomFactor;
			velocityDir.x += 0.1 * randomFactor;
			velocityDir.y += 0.1 * randomFactor;
			velocityDir.z += 0.1 * randomFactor;

			float3 yellowCol = float3(1.0, 1.0, 0.0);
			float3 redCol = float3(1.0, 0.0, 0.0);

			/*  float particleColorStep = smoothstep(randomFactor, -1.0, 1.0);
			float3 particleColor = lerp(yellowCol, redCol, particleColorStep);
			*/
			//   float4 centrePos = input[i].position;
			float4 origin = float4(0.0f, 0.0f, 0.0f, 1.0f);
			centrePos = origin;
			centrePos.xyz += (timeLoop)* velocityDir * 2.0f;
			//centrePos.y -= (timeLoop) * (timeLoop) * (timeLoop);

			float particleDist = distance(origin, centrePos);
			float maxDist = 5.0f;
			particleColor = lerp(yellowCol, redCol, particleDist / maxDist);

			break;
            }
		case 1:
            {
            
            float3 startPos = float3(0.0, 0.0, 0.0);
            float3 velocityDir = (input[i].position - startPos);
            velocityDir.y = 1.0f + velocityDir.y * 0.4f; //abs(velocityDir.y);
            velocityDir.y += (random(input[i].position.xy) * 2.0 - 1.0) * 0.2;
            
            velocityDir.y = abs(velocityDir.y);
            velocityDir.x = (random(input[i].position.xy + float2(2.1, 4.3)) * 2.0 - 1.0) * 0.3f;
            velocityDir.z = (random(input[i].position.xy + float2(3.1, 1.3)) * 2.0 - 1.0) * 0.3f;

            float rand = fmod(input[i].id, 500.0f);
            //velocityDir.x = (rand / 500.0) * 2.0 - 1.0;
            //velocityDir.x = velocityDir.x * 0.1f;
           // velocityDir.x = 0.1 * velocityDir.x * input[i].id;

            float timeLoop = fmod(timePassed, 5.0);

            float3 startColor = float3(0.0, 0.0, 0.0);
            float3 endColor = float3(0.5, 0.5, 0.5);

            float4 origin = float4(0.0, 0.0, 0.0, 1.0);
            centrePos = origin;
              
            centrePos.xyz += timeLoop * velocityDir * 2.0f;// * 2.0f;
            float particleDist = distance(origin, centrePos);
            centrePos.x += cos(particleDist * 10.0) * 0.02f;
            centrePos.z += cos(particleDist * 10.0) * 0.02f;
            //centrePos.z = 0.0f;

          
            float maxDist = 5.0f;
            particleColor = lerp(startColor, endColor, particleDist / maxDist);
			break;
            }
		}
   
        
        
       // float zVal = 10.0f / input[i].id;
        float zVal = input[i].id / 50.0f;
        float xVal = sin(input[i].id / 100.0f) / 100.0f;
        float yVal = sin(input[i].id / 100.0f) / 100.0f;
        GeometryOutputType element;
        element.uv = input[i].uv;
        element.color = float4(particleColor, 1.0);
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