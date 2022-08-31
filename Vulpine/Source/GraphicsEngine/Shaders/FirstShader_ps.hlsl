#include "CommonStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput result;

	// UV TEst
	/*result.myColor = float4(input.myUV.r, input.myUV.g, 0, 1);*/

	result.myColor.rgb = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
	result.myColor.a = 1;

	return result;
}