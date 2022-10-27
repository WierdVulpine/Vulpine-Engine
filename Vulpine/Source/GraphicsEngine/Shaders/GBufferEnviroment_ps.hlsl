#include "PBRFunctions.hlsli"

//Texture2D albedoTexture : register(t0);
//Texture2D normalTexture : register(t1);
//Texture2D materialTexture : register(t2);
Texture2D vertexNormalTexture : register(t3);
Texture2D worldPositionTexture : register(t4);
Texture2D ambientOcclusionTexture : register(t5);
TextureCube environmentTexture : register(t10);
Texture2D dirLightShadowMap : register(t20);

DefferedPixelOutput main(DefferedVertexToPixel input)
{
	DefferedPixelOutput result;

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.UV);
	if (albedo.a == 0)
	{
		discard;
		result.Color = float4(0, 0, 0, 0);
		return result;
	}

	float4 material = materialTexture.Sample(defaultSampler, input.UV);

	const float metalness = material.r;
	const float roughness = material.g;
	const float emissive = material.b;
	const float emissiveStr = material.a;

	const float3 normal = normalTexture.Sample(defaultSampler, input.UV).rgb;
	const float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, input.UV).rgb;
	const float4 worldPosition = worldPositionTexture.Sample(defaultSampler, input.UV);
	const float ambientOcclusion = ambientOcclusionTexture.Sample(defaultSampler, input.UV).r;
	const float3 toEye = normalize(FB_CamTranslation.xyz - vertexNormal.xyz);

	const float3 specularColor = lerp((float3)0.04f, albedo, metalness);
	const float3 diffuseColor = lerp((float3)0.00f, albedo, 1 - metalness);

	const float3 ambientLighting = EvaluateAmbience(
		defaultSampler,
		environmentTexture, normal, vertexNormal,
		toEye, roughness, ambientOcclusion, diffuseColor, specularColor);

	float3 directLighting = EvaluateDirectionalLight(
		diffuseColor, specularColor, normal, roughness,
		LB_DirectionalLight.Color, LB_DirectionalLight.Intensity, -LB_DirectionalLight.Direction, toEye);

	if (LB_DirectionalLight.CastShadows)
	{
		const float4 worldToLightView = mul(LB_DirectionalLight.View, worldPosition);
		const float4 lightViewToLightProj = mul(LB_DirectionalLight.Projection, worldToLightView);

		float2 projectedTexCoord;
		projectedTexCoord.x = lightViewToLightProj.x / lightViewToLightProj.w / 2.0f + 0.5f;
		projectedTexCoord.y = -lightViewToLightProj.y / lightViewToLightProj.w / 2.0f + 0.5f;

		if (saturate(projectedTexCoord.x) == projectedTexCoord.x && saturate(projectedTexCoord.y) == projectedTexCoord.y)
		{
			const float shadowBias = 0.0005f;
			const float shadow = 0.01f;
			const float viewDepth = (lightViewToLightProj.z / lightViewToLightProj.w) - shadowBias;
			const float lightDepth = dirLightShadowMap.Sample(pointClampSampler, projectedTexCoord).r;

			if (lightDepth < viewDepth)
			{
				directLighting *= shadow;
			}
		}

	}


	float3 pointLight = 0;
	float3 spotLight = 0;

	for (unsigned int l = 0; l < LB_NumLights; l++)
	{
		const LightData Light = LB_Lights[l];

		switch (Light.LightType)
		{
		default:
			break;
		case 0:
			break;
		case 1:
		{
			pointLight += EvaluatePointLight(diffuseColor, specularColor, normal, material.g, Light.Color,
				Light.Intensity, Light.Range, Light.Position, toEye, worldPosition);
			break;
		}
		case 2:
		{
			spotLight += EvaluateSpotLight(diffuseColor, specularColor, normal, material.g, Light.Color,
				Light.Intensity, Light.Range, Light.Position, Light.Direction, Light.SpotOuterRadius, Light.SpotInnerRadius, toEye, worldPosition);
			break;
		}
		case 3:
			break;
		}
	}

	result.Color.rgb = LinearToGamma(directLighting + ambientLighting + pointLight + spotLight);
	result.Color.a = 1;

	return result;
}