
struct PixelInputType
{
	float4 position  : SV_POSITION;
	float4 color	 : COLOR;
	// TODO: change/add other stuff
};

float4 main(PixelInputType input) : SV_TARGET
{
	return input.color;
	//return float4(0.0f, 0.0f, 0.0f, 1.0f);
}