#pragma once
#include <memory>
#include <vector>
#include "SceneObject.h"
#include "ModelInstance.h"
#include "ParticleAssetHandler.h"

class Camera;
class GraphicsEngine;

class Scene
{
public:
	Scene(std::shared_ptr<GraphicsEngine> aEnginePtr) : myEnginePtr(aEnginePtr) {}
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

	std::shared_ptr<GraphicsEngine> myEnginePtr;

	std::shared_ptr<ParticleSystem> myTestSystem;

	float myLerpValue;

	float myFirstPresetIndex;
	float mySecondPresetIndex;

	void LoadSettings();
	void LoadScene();

	void SaveSettings();
	void SaveScene();


};

