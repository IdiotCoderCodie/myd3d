#define MAX_SHADOWCASTING_LIGHTS 4

// Input control point
struct HullInputType
{
	float3 position	: POSITION;
};

// Output control point
struct HullOutputType
{
	float3 position : POSITION; 
};

// Output patch constant data.
struct HullConstantOutputType
{
	float Edges[4]		: SV_TessFactor;
	float Inside[2]	: SV_InsideTessFactor;
	// TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 16

// Patch Constant Function
HullConstantOutputType CalcHSPatchConstants(InputPatch<HullInputType, NUM_CONTROL_POINTS> ip,
									   uint PatchID : SV_PrimitiveID )
{
	HullConstantOutputType output;

	float tessAmount = 32.0f;

	output.Edges[0] = output.Edges[1] = output.Edges[2] = output.Edges[3] = tessAmount;
	output.Inside[0] = output.Inside[1] = tessAmount;

	return output;
	//HullConstantOutputType Output;

 //   if(distanceBased)
 //   {
 //       for(int i = 0; i < 3; i++)
 //       {
 //           float4 worldPosition = ip[i].position;
 //           worldPosition = mul(ip[i].position, modelMatrix);
 //           float cameraDist = length(eyePos - worldPosition);
 //           Output.EdgeTessFactor[i] = 64.0f / cameraDist;
 //       }
 //   
 //       Output.InsideTessFactor = max(max(Output.EdgeTessFactor[0], Output.EdgeTessFactor[1]), Output.EdgeTessFactor[2]);
 //   }
 //   else
 //   {
 //       Output.EdgeTessFactor[0] = 
	//	Output.EdgeTessFactor[1] = 
	//	Output.EdgeTessFactor[2] = 
	//	Output.InsideTessFactor = tessellationAmount; // e.g. could calculate dynamic tessellation factors instead
 //   }
	

	//return Output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(16)]
[patchconstantfunc("CalcHSPatchConstants")]
HullOutputType main( 
	InputPatch<HullInputType, NUM_CONTROL_POINTS> patch, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HullOutputType output;
	output.position = patch[i].position;
	return output;
 //   float4 worldPosition = patch[i].position;
 //   worldPosition = mul(patch[i].position, modelMatrix);

 //   float cameraDist = eyePos - worldPosition;

	//// Insert code to compute Output here
	//output.position = patch[i].position;
 //   output.normal = patch[i].normal;
	//output.uv = patch[i].uv;
	//output.color	= float4(patch[i].normal, 1.0f);

	//return output;
}


//[domain("tri")]
//[partitioning("fractional_even")]
//[outputtopology("triangle_cw")]
//[outputcontrolpoints(3)]
//[patchconstantfunc("CalcHSPatchConstants")]
//HullOutputType main_fractional_even( 
//	InputPatch<HullInputType, NUM_CONTROL_POINTS> patch, 
//	uint i : SV_OutputControlPointID,
//	uint PatchID : SV_PrimitiveID )
//{
//	HullOutputType output;
//
//	// Insert code to compute Output here
//	output.position = patch[i].position;
//    output.normal = patch[i].normal;
//	output.uv = patch[i].uv;
//	output.color	= float4(patch[i].normal, 1.0f);
//
//	return output;
//}
//
//
//[domain("tri")]
//[partitioning("fractional_odd")]
//[outputtopology("triangle_cw")]
//[outputcontrolpoints(3)]
//[patchconstantfunc("CalcHSPatchConstants")]
//HullOutputType main_fractional_odd( 
//	InputPatch<HullInputType, NUM_CONTROL_POINTS> patch, 
//	uint i : SV_OutputControlPointID,
//	uint PatchID : SV_PrimitiveID )
//{
//	HullOutputType output;
//
//	// Insert code to compute Output here
//	output.position = patch[i].position;
//    output.normal = patch[i].normal;
//	output.uv = patch[i].uv;
//	output.color	= float4(patch[i].normal, 1.0f);
//
//	return output;
//}
//
//
//[domain("tri")]
//[partitioning("pow2")]
//[outputtopology("triangle_cw")]
//[outputcontrolpoints(3)]
//[patchconstantfunc("CalcHSPatchConstants")]
//HullOutputType main_pow2( 
//	InputPatch<HullInputType, NUM_CONTROL_POINTS> patch, 
//	uint i : SV_OutputControlPointID,
//	uint PatchID : SV_PrimitiveID )
//{
//	HullOutputType output;
//
//	// Insert code to compute Output here
//	output.position = patch[i].position;
//    output.normal = patch[i].normal;
//	output.uv = patch[i].uv;
//	output.color	= float4(patch[i].normal, 1.0f);
//
//	return output;
//}