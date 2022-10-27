#pragma once
#include <memory>
#include <vector>
#include "SceneObject.h"
#include "ModelInstance.h"
#include "ParticleAssetHandler.h"
#include "EditorLayer.h"

class Camera;
class GraphicsEngine;

class Scene
{
	friend class EditorLayer;

public:
	Scene(std::shared_ptr<GraphicsEngine> aEnginePtr) : myEnginePtr(aEnginePtr) {}
	~Scene() = default;

	template<typename T>
	void AddGameObject(std::shared_ptr<T> aSceneObject)
	{
		int index = mySceneObjects.size();
		mySceneObjects.push_back(aSceneObject);
		mySceneObjects[index]->myIndex = index;
	}

	bool HasParent(int aObjectIndex, int aParentIndex)
	{
		if (mySceneObjects[aObjectIndex]->myParentIndex == -1)
		{
			return false;
		}
		else if (mySceneObjects[aObjectIndex]->myParentIndex == aParentIndex)
		{
			return true;
		}
		else
		{
			return HasParent(mySceneObjects[aObjectIndex]->myParentIndex, aParentIndex);
		}
	}

	void SetCamera(std::shared_ptr<Camera> aCamera) { myMainCamera = aCamera; }

	void Init();

	void Update();

	void Render();

	void LoadSettings();
	void LoadScene(std::string aPath);

	void SaveSettings();
	void SaveScene(std::string aPath);

	void SetChild(int aObjectIndex, int aParentIndex);

private:
	std::vector<std::shared_ptr<ModelInstance>> mySceneObjects;

	std::shared_ptr<Camera> myMainCamera;

	std::shared_ptr<GraphicsEngine> myEnginePtr;

	std::shared_ptr<ParticleSystem> myTestSystem;

	float myLerpValue;

	float myFirstPresetIndex;
	float mySecondPresetIndex;

};

