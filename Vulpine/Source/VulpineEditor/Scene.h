#pragma once
#include <memory>
#include <vector>
#include "SceneObject.h"

class Camera;

class Scene
{
public:
	template<typename T>
	void AddGameObject(std::shared_ptr<T> aSceneObject)
	{
		mySceneObjects.push_back(aSceneObject);
	}

	void SetCamera(std::shared_ptr<Camera> aCamera) { myMainCamera = aCamera; }

private:
	std::vector<std::shared_ptr<SceneObject>> mySceneObjects;

	std::shared_ptr<Camera> myMainCamera;
};

