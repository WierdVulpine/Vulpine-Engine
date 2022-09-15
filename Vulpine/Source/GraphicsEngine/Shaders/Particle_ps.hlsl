#include "CommonStructs.hlsli"

ParticlePixelOutput main(ParticleGeometryToPixel input)
{
	ParticlePixelOutput result;

	float4 textureColor = albedoTexture.Sample(defaultSampler, input.UV);
	result.Color.rgba = textureColor.rgba * input.Color.rgba;

	return result;
}