
struct PixelInputType
{
	float4 position  : SV_POSITION;
    float3 normal    : NORMAL;
	float4 color	 : COLOR;
	// TODO: change/add other stuff
};

float4 main(PixelInputType input) : SV_TARGET
{
    return float4(input.normal, 1.0f);
	return input.color;
	//return float4(0.0f, 0.0f, 0.0f, 1.0f);
}