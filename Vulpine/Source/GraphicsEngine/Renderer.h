#pragma once
#include <vector>
#include <memory>
#include "ModelInstance.h"

class ForwardRenderer;
class Model;
class Camera;

struct Storage
{
	std::vector<std::shared_ptr<ModelInstance>> myModels;
};

class Renderer
{
private:
	static inline Storage myStorage;
	static inline std::shared_ptr<Camera> myCameraPtr;

public:
	static void SetCamera(std::shared_ptr<Camera> aCamera);
	static void AddModel(const std::shared_ptr<ModelInstance>& aModel);

	static void Render(ForwardRenderer& aRenderer);

};

