#pragma once
#include <vector>
#include <memory>
#include "ModelInstance.h"
#include "ParticleSystem.h"

class ForwardRenderer;
class Model;
class Camera;
class DefferedRenderer;
class ShadowRenderer;

struct Storage
{
	std::vector<std::shared_ptr<ModelInstance>> myModels;
	std::vector<std::shared_ptr<ParticleSystem>> mySystems;
};

class Renderer
{
private:
	static inline Storage myStorage;
	static inline std::shared_ptr<Camera> myCameraPtr;

public:
	static void SetCamera(std::shared_ptr<Camera> aCamera);
	static void AddModel(const std::shared_ptr<ModelInstance>& aModel);
	static void AddSystem(const std::shared_ptr<ParticleSystem>& aSystem);

	static void Render(ForwardRenderer& aRenderer, DefferedRenderer& aDefferedRenderer, ShadowRenderer& aShadowRenderer);

};

