#include "CommonStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput result;

	const float3x3 TBN = float3x3(
		normalize(input.myTangent),
		normalize(input.myBinormal),
		normalize(input.myNormal));

	float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV).agr;
	normalMap.z = 0;

	normalMap = 2.0f * normalMap - 1;
	normalMap.z = sqrt(1 - saturate(normalMap.x + normalMap.x + normalMap.y + normalMap.y));
	normalMap = normalize(normalMap);

	const float3 pixelNormal = normalize(mul(normalMap, TBN));

	//Default light rendering
	float3 albedo = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
	const float3 L = -1 * normalize(LB_Direction);
	const float3 N = pixelNormal;
	const float LdotN = saturate(dot(L, N));
	const float3 C = LB_Color;
	const float Ilight = LB_Intensity;

	const float3 Ipixel = LdotN * C * Ilight;

	const float3 diffuse = albedo * Ipixel;

	const float3 environment = enviromentTexture.SampleLevel(defaultSampler, input.myNormal, 5).rgb;
	const float3 ambient = albedo * environment;

	result.myColor.rgb = saturate(diffuse + ambient);
	result.myColor.a = 1;

	return result;
}