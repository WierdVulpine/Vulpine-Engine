#pragma once
#include "DirectionalLight.h"
#include "EnviromentLight.h"
#include "PointLight.h"
#include "SpotLight.hpp"
#include <vector>

class LightAssetHandler
{
	static inline std::vector<std::shared_ptr<Light>> myLights = {};
	static inline std::shared_ptr<DirectionalLight> myDirectionalLight;
	static inline std::shared_ptr<EnviromentLight> myEnviromentalLight;

public:

	static std::shared_ptr<DirectionalLight> CreateDirectionalLight(CommonUtilities::Vector3f aColor, float anIntensity, CommonUtilities::Vector3f aRotation, CommonUtilities::Vector3f aPosition);
	static std::shared_ptr<EnviromentLight> CreateEnvirometalLight(const std::wstring& aTextureName);
	static std::shared_ptr<PointLight> CreatePointLight(CommonUtilities::Vector3f aColor, float anIntensity, float aRange, CommonUtilities::Vector3f aPosition);
	static std::shared_ptr<SpotLight> CreateSpotLight(CommonUtilities::Vector3f aColor, float anIntensity, float aRange, float aAttenuation, float aInnerCone, float aOuterCone, CommonUtilities::Vector3f aDirection, CommonUtilities::Vector3f aPosition);

	static std::shared_ptr<DirectionalLight> GetDirectionalLight() { return myDirectionalLight; }
	static std::shared_ptr<EnviromentLight> GetEnviromentLight() { return myEnviromentalLight; }

	static std::vector<std::shared_ptr<Light>> GetLights() { return myLights; }
};

