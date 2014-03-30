#define MAX_SHADOWCASTING_LIGHTS 4

cbuffer TessellationBuffer
{
    int distanceBased;
	float tessellationAmount;
	float2 padding;
    float4 eyePos;
    matrix modelMatrix;
};


// Input control point
struct HullInputType
{
	float4 position										: POSITION;
	float2 uv											: TEXCOORD0;
	float3 normal										: NORMAL;
	//float4 lightViewPosition[MAX_SHADOWCASTING_LIGHTS]	: TEXCOORD1;
	//float3 lightPos[MAX_SHADOWCASTING_LIGHTS]			: COLOR0;
};

// Output control point
struct HullOutputType
{
	float3 position : POSITION; 
	float3 normal   : NORMAL;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR;
};

// Output patch constant data.
struct HullConstantOutputType
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HullConstantOutputType CalcHSPatchConstants(InputPatch<HullInputType, NUM_CONTROL_POINTS> ip,
									   uint PatchID : SV_PrimitiveID )
{
	HullConstantOutputType Output;

    if(distanceBased)
    {
        for(int i = 0; i < 3; i++)
        {
            float4 worldPosition = ip[i].position;
            worldPosition = mul(ip[i].position, modelMatrix);
            float cameraDist = length(eyePos - worldPosition);
            Output.EdgeTessFactor[i] = 64.0f / cameraDist;
        }
    
        Output.InsideTessFactor = max(max(Output.EdgeTessFactor[0], Output.EdgeTessFactor[1]), Output.EdgeTessFactor[2]);
    }
    else
    {
        Output.EdgeTessFactor[0] = 
		Output.EdgeTessFactor[1] = 
		Output.EdgeTessFactor[2] = 
		Output.InsideTessFactor = tessellationAmount; // e.g. could calculate dynamic tessellation factors instead
    }
	// Insert code to compute Output here
	

	return Output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HullOutputType main( 
	InputPatch<HullInputType, NUM_CONTROL_POINTS> patch, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HullOutputType output;

    float4 worldPosition = patch[i].position;
    worldPosition = mul(patch[i].position, modelMatrix);

    float cameraDist = eyePos - worldPosition;

	// Insert code to compute Output here
	output.position = patch[i].position;
    output.normal = patch[i].normal;
	output.uv = patch[i].uv;
	output.color	= float4(patch[i].normal, 1.0f);

	return output;
}


[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HullOutputType main_fractional_even( 
	InputPatch<HullInputType, NUM_CONTROL_POINTS> patch, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HullOutputType output;

	// Insert code to compute Output here
	output.position = patch[i].position;
    output.normal = patch[i].normal;
	output.uv = patch[i].uv;
	output.color	= float4(patch[i].normal, 1.0f);

	return output;
}


[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HullOutputType main_fractional_odd( 
	InputPatch<HullInputType, NUM_CONTROL_POINTS> patch, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HullOutputType output;

	// Insert code to compute Output here
	output.position = patch[i].position;
    output.normal = patch[i].normal;
	output.uv = patch[i].uv;
	output.color	= float4(patch[i].normal, 1.0f);

	return output;
}


[domain("tri")]
[partitioning("pow2")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HullOutputType main_pow2( 
	InputPatch<HullInputType, NUM_CONTROL_POINTS> patch, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HullOutputType output;

	// Insert code to compute Output here
	output.position = patch[i].position;
    output.normal = patch[i].normal;
	output.uv = patch[i].uv;
	output.color	= float4(patch[i].normal, 1.0f);

	return output;
}