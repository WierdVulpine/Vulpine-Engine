#pragma once
#include <memory>
#include <vector>
#include "SceneObject.h"
#include "ModelInstance.h"

class Camera;
class GraphicsEngine;

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	template<typename T>
	void AddGameObject(std::shared_ptr<T> aSceneObject)
	{
		mySceneObjects.push_back(aSceneObject);
	}

	void SetCamera(std::shared_ptr<Camera> aCamera) { myMainCamera = aCamera; }

	void Init();

	void Update();

	void Render();

private:
	std::vector<std::shared_ptr<ModelInstance>> mySceneObjects;

	std::shared_ptr<Camera> myMainCamera;
};

