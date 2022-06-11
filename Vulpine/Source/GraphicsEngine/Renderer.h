#pragma once
#include <vector>
#include <memory>

class ForwardRenderer;
class Model;
class Camera;

struct Storage
{
	std::vector<std::shared_ptr<Model>> myModels;
};

class Renderer
{
private:
	static inline Storage myStorage;
	static inline std::shared_ptr<Camera> myCameraPtr;

public:
	static void SetCamera(std::shared_ptr<Camera> aCamera);
	static void AddModel(const std::shared_ptr<Model>& aModel);

	static void Render(ForwardRenderer& aRenderer);

};

