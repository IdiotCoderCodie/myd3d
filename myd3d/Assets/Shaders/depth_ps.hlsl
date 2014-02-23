
struct PixelInputType
{
    // SV_POSITION semantic offsets position by 0.5, so use a second vector which is not modified
    // so we can perform depth calculations.
    float4 position : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};


float4 ps_main(PixelInputType input) : SV_TARGET
{
	float depthValue = input.depthPosition.z / input.depthPosition.w;

    float4 color = float4(depthValue, depthValue, depthValue, 1.0f);

    return color;
}