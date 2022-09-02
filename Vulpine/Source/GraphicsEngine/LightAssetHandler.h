#pragma once
#include "DirectionalLight.h"
#include "EnviromentLight.h"
#include <vector>

class LightAssetHandler
{
	static inline std::vector<std::shared_ptr<Light>> myLights = {};
	static inline std::shared_ptr<DirectionalLight> myDirectionalLight;
	static inline std::shared_ptr<EnviromentLight> myEnviromentalLight;

public:

	static std::shared_ptr<DirectionalLight> CreateDirectionalLight(CommonUtilities::Vector3f aColor, float anIntensity, CommonUtilities::Vector3f aRotation);
	static std::shared_ptr<EnviromentLight> CreateEnvirometalLight(const std::wstring& aTextureName);

	static std::shared_ptr<DirectionalLight> GetDirectionalLight() { return myDirectionalLight; }
	static std::shared_ptr<EnviromentLight> GetEnviromentLight() { return myEnviromentalLight; }
};

