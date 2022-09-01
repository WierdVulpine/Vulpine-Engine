#include "CommonStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput result;

	const float3x3 TBN = float3x3
	(
		normalize(input.myTangent),
		normalize(input.myBinormal),
		normalize(input.myNormal)
	);

	float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV).agr;
	normalMap.z = 0;

	normalMap = 2.0f * normalMap - 1;

	normalMap.z = sqrt(1 - saturate(normalMap.x + normalMap.x + normalMap.y + normalMap.y));

	normalMap = normalize(normalMap);

	const float3 pixelNormal = normalize(mul(normalMap, TBN));

	float3 debugNormal = pixelNormal;
	const float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;
	if (signedLength < 0)
	{
		debugNormal = float3(1 - abs(debugNormal));
	}

	result.myColor.rgb = debugNormal;
	result.myColor.a = 1;

	return result;
}