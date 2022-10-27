#include "Scene.h"
#include <GraphicsEngine.h>
#include <Renderer.h>
#include <Camera.h>
#include <ModelAssetHandler.h>
#include "Input.h"
#include "Time.h"
#include "imgui.h"
#include "ModelInstance.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "filereadstream.h"
#include "filewritestream.h"
#include "LightAssetHandler.h"
#include <filesystem>
#include <UtilityFunctions.h>

namespace fs = std::filesystem;

CommonUtilities::Vector3f GetRandomColor()
{
	CommonUtilities::Vector3f result;

	result.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	result.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	result.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	return result;
}

ImVec4 ColorLerper(ImVec4 aFirst, ImVec4 aSecond, float aA)
{
	ImVec4 result;

	result.x = { aFirst.x + aA * (aSecond.x - aFirst.x) };
	result.y = { aFirst.y + aA * (aSecond.y - aFirst.y) };
	result.z = { aFirst.z + aA * (aSecond.z - aFirst.z) };
	result.w = { aFirst.w + aA * (aSecond.w - aFirst.w) };

	return result;
}

void SavePreset(std::string aName, float aColor[4])
{
	std::string fileName = "Presets/" + aName + ".cp";
	FILE* fp;
	fopen_s(&fp, fileName.c_str(), "w");

	char writeBuffer[65536];
	rapidjson::FileWriteStream is(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Document d;
	d.SetObject();

	d.AddMember("ColorR", aColor[0], d.GetAllocator());
	d.AddMember("ColorG", aColor[1], d.GetAllocator());
	d.AddMember("ColorB", aColor[2], d.GetAllocator());
	d.AddMember("ColorA", aColor[3], d.GetAllocator());

	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(is);
	d.Accept(writer);
	fclose(fp);

}

ImVec4 LoadPreset(std::string aPath)
{
	ImVec4 result;

	std::string fileName = "Presets/" + aPath;
	FILE* fp;
	fopen_s(&fp, fileName.c_str(), "rb");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	fclose(fp);

	result =   { d["ColorR"].GetFloat(),
								  d["ColorG"].GetFloat(),
								  d["ColorB"].GetFloat(),
								  d["ColorA"].GetFloat() };

	return result;
}

void Scene::Init()
{
	myMainCamera = std::make_shared<Camera>(Camera());

	myMainCamera = std::make_shared<Camera>(Camera());
	myMainCamera->Init(90, { 1200, 800 }, 0.1f, 15000.f);
	myMainCamera->SetPosition({ 0,0,-200 });

	std::shared_ptr<ModelInstance> ground = ModelAssetHandler::GetModelInstance(L"Cube");
	ground->SetScale(10, 0.1f, 10);
	ground->SetPosition(0, -100, 200);
	ground->SetRotation(-90, 0, 0);
	ground->myRealName = "Ground";
	mySceneObjects.push_back(ground);
	
	std::shared_ptr<ModelInstance> cube = ModelAssetHandler::GetModelInstance(L"SM_Particle_Chest.fbx");
	cube->SetScale(1, 1, 1);
	cube->SetPosition(-150, -100, 0);
	cube->SetRotation(0, 0, 0);
	cube->myRealName = "chest";
	mySceneObjects.push_back(cube);

	std::shared_ptr<ModelInstance> gnobber = ModelAssetHandler::GetModelInstance(L"gremlin_sk.fbx");
	ModelAssetHandler::LoadAnimation(L"gremlin_sk.fbx", L"gremlin@run.fbx");
	gnobber->SetAnimation(L"gremlin@run.fbx");
	gnobber->myRealName = "Goblin";
	gnobber->SetPosition(0, -95, 130);
	gnobber->SetRotation(0, 180, 0);

	mySceneObjects.push_back(gnobber);

	LightAssetHandler::CreateDirectionalLight({ 1,1,1 }, 1, { 0,1,0 }, {0,0,-10000});
	LightAssetHandler::CreateEnvirometalLight(L"skansen_cubemap.dds");

	/*CommonUtilities::Vector3f startPos = { -500, 0, 0 };

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			CommonUtilities::Vector3f newPos = { (float)i * 500.f, 0, (float)j * 500.f };
			newPos += startPos;
			LightAssetHandler::CreatePointLight(GetRandomColor(), 50000.f, 500, newPos);
		}
	}*/


	LoadSettings();

	Renderer::SetCamera(myMainCamera);
}

void Scene::Update()
{
	for (size_t i = 0; i < mySceneObjects.size(); i++)
	{
		mySceneObjects[i]->Update(Time::GetDeltaTime());
	}

	mySceneObjects[1]->GetTransform().AddRotation({ 0,45 * Time::GetDeltaTime(),0 });

	static float speed = 80;
	static float rotSpeeed = 5.2f;

	CommonUtilities::Vector3f dir;

	if (Input::KeyIsPressed(VK_SPACE))
	{
		dir.y = 1;
	}
	if (Input::KeyIsPressed(VK_CONTROL))
	{
		dir.y = -1;
	}
	if (Input::KeyIsPressed(W_KEY))
	{
		myMainCamera->GetTransform().AddPosition(myMainCamera->GetTransform().GetForward() * speed * Time::GetDeltaTime());
	}
	if (Input::KeyIsPressed(S_KEY))
	{
		myMainCamera->GetTransform().AddPosition(myMainCamera->GetTransform().GetForward() * -speed * Time::GetDeltaTime());
	}
	if (Input::KeyIsPressed(D_KEY))
	{
		myMainCamera->GetTransform().AddPosition(myMainCamera->GetTransform().GetRight() * speed * Time::GetDeltaTime());
	}
	if (Input::KeyIsPressed(A_KEY))
	{
		myMainCamera->GetTransform().AddPosition(myMainCamera->GetTransform().GetRight() * -speed * Time::GetDeltaTime());
	}

	Vector3f rotDir;

	rotDir.y = Input::GetMouseDelta().x;
	rotDir.x = Input::GetMouseDelta().y;

	if (Input::MouseButtonPressed(2))
	{
		myMainCamera->GetTransform().AddRotation(rotDir * rotSpeeed * Time::GetDeltaTime());
	}

	myMainCamera->GetTransform().AddPosition(dir * speed * Time::GetDeltaTime());
}

void Scene::Render()
{
	for (size_t i = 0; i < mySceneObjects.size(); i++)
	{
		Renderer::AddModel(mySceneObjects[i]);
	}
}

void Scene::LoadSettings()
{
	FILE* fp;
	fopen_s(&fp,"Settings/Settings.json", "rb");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	fclose(fp);

	Vector4f loadedClearColor = { d["ClearColorR"].GetFloat(), 
		                          d["ClearColorG"].GetFloat(), 
		                          d["ClearColorB"].GetFloat(), 
		                          d["ClearColorA"].GetFloat() };

	myLerpValue = d["LerpValue"].GetFloat();
	myFirstPresetIndex = static_cast<int>(d["FirstIndex"].GetFloat());
	mySecondPresetIndex = static_cast<int>(d["SecondIndex"].GetFloat());

	myEnginePtr->GetClearColor() = loadedClearColor;


}

void Scene::LoadScene(std::string aPath)
{
	FILE* fp;
	fopen_s(&fp, aPath.c_str(), "rb");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	fclose(fp);

	for (size_t i = 0; i < d["SceneObjects"].GetArray().Size(); i++)
	{
		rapidjson::Value& currentObj = d["SceneObjects"][i];

		std::string modelPath = currentObj["Path"].GetString();
		std::wstring wModelPath = std::wstring(modelPath.begin(), modelPath.end());

		std::shared_ptr<ModelInstance> tempObj = ModelAssetHandler::GetModelInstance(wModelPath);
		
		CommonUtilities::Vector3f position =
		{
			currentObj["Position"][0].GetFloat(), currentObj["Position"][1].GetFloat(),currentObj["Position"][2].GetFloat()
		};
		CommonUtilities::Vector3f rotation =
		{
			currentObj["Rotation"][0].GetFloat(), currentObj["Rotation"][1].GetFloat(),currentObj["Rotation"][2].GetFloat()
		};
		CommonUtilities::Vector3f scale =
		{
			currentObj["Scale"][0].GetFloat(), currentObj["Scale"][1].GetFloat(),currentObj["Scale"][2].GetFloat()
		};
		
		tempObj->SetPosition(position);
		tempObj->SetRotation(rotation);
		tempObj->SetScale(scale);

		for (size_t i = 0; i < currentObj["Children"].Size(); i++)
		{
			tempObj->AddChild(static_cast<int>(currentObj["Children"][i].GetInt()));
		}

		tempObj->myIndex = currentObj["Index"].GetInt();
		tempObj->myRealName = currentObj["Name"].GetString();
		tempObj->myParentIndex = currentObj["Parent"].GetInt();

		mySceneObjects.push_back(tempObj);
	}

	for (size_t i = 0; i < mySceneObjects.size(); i++)
	{
		for (size_t j = 0; j < mySceneObjects[i]->myChildren.size(); j++)
		{
			mySceneObjects[mySceneObjects[i]->myChildren[j]]->SetParent(i);
		}
	}

}

void Scene::SaveSettings()
{
	FILE* fp;
	fopen_s(&fp, "Settings/Settings.json", "w");

	char writeBuffer[65536];
	rapidjson::FileWriteStream is(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Document d;
	d.SetObject();

	d.AddMember("ClearColorR", myEnginePtr->GetClearColor().x, d.GetAllocator());
	d.AddMember("ClearColorG", myEnginePtr->GetClearColor().y, d.GetAllocator());
	d.AddMember("ClearColorB", myEnginePtr->GetClearColor().z, d.GetAllocator());
	d.AddMember("ClearColorA", myEnginePtr->GetClearColor().w, d.GetAllocator());
	d.AddMember("LerpValue", myLerpValue, d.GetAllocator());
	d.AddMember("FirstIndex", myFirstPresetIndex, d.GetAllocator());
	d.AddMember("SecondIndex", mySecondPresetIndex, d.GetAllocator());

	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(is);
	d.Accept(writer);
	fclose(fp);
}

void Scene::SaveScene(std::string aPath)
{
	FILE* fp;
	fopen_s(&fp, aPath.c_str(), "w");

	char writeBuffer[65536];
	rapidjson::FileWriteStream is(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Document d;
	d.SetObject();

	rapidjson::Value SceneObjects(rapidjson::kArrayType);

	for (size_t i = 0; i < mySceneObjects.size(); i++)
	{
		rapidjson::Value objValue(rapidjson::kObjectType);

		objValue.AddMember("Index", mySceneObjects[i]->myIndex, d.GetAllocator());

		rapidjson::Value positionValues(rapidjson::kArrayType);
		positionValues.PushBack(mySceneObjects[i]->GetTransform().GetPosition().x, d.GetAllocator());
		positionValues.PushBack(mySceneObjects[i]->GetTransform().GetPosition().y, d.GetAllocator());
		positionValues.PushBack(mySceneObjects[i]->GetTransform().GetPosition().z, d.GetAllocator());
		objValue.AddMember("Position", positionValues, d.GetAllocator());

		rapidjson::Value rotationValues(rapidjson::kArrayType);
		rotationValues.PushBack(mySceneObjects[i]->GetTransform().GetRotation().x, d.GetAllocator());
		rotationValues.PushBack(mySceneObjects[i]->GetTransform().GetRotation().y, d.GetAllocator());
		rotationValues.PushBack(mySceneObjects[i]->GetTransform().GetRotation().z, d.GetAllocator());
		objValue.AddMember("Rotation", rotationValues, d.GetAllocator());

		rapidjson::Value scaleValues(rapidjson::kArrayType);
		scaleValues.PushBack(mySceneObjects[i]->GetTransform().GetScale().x, d.GetAllocator());
		scaleValues.PushBack(mySceneObjects[i]->GetTransform().GetScale().y, d.GetAllocator());
		scaleValues.PushBack(mySceneObjects[i]->GetTransform().GetScale().z, d.GetAllocator());
		objValue.AddMember("Scale", scaleValues, d.GetAllocator());

		rapidjson::Value neighbourValues(rapidjson::kArrayType);
		for (size_t j = 0; j < mySceneObjects[i]->myChildren.size(); j++)
		{
			neighbourValues.PushBack(mySceneObjects[i]->myChildren[j], d.GetAllocator());
		}
		objValue.AddMember("Children", neighbourValues, d.GetAllocator());

		objValue.AddMember("Parent", mySceneObjects[i]->myParentIndex, d.GetAllocator());

		rapidjson::Value Name(rapidjson::kStringType);
		Name.SetString((rapidjson::GenericStringRef<char>)mySceneObjects[i]->myRealName.c_str());
		objValue.AddMember("Name", Name, d.GetAllocator());

		std::string gamerTag = mySceneObjects[i]->GetModelPath();
		Name.SetString(gamerTag.c_str(), d.GetAllocator());
		objValue.AddMember("Path", Name, d.GetAllocator());

		SceneObjects.PushBack(objValue, d.GetAllocator());
	}

	d.AddMember("SceneObjects", SceneObjects, d.GetAllocator());

	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(is);
	d.Accept(writer);
	fclose(fp);
}

void Scene::SetChild(int aObjectIndex, int aParentIndex)
{
	mySceneObjects[aParentIndex]->AddChild(aObjectIndex);
	mySceneObjects[aObjectIndex]->SetParent(aParentIndex);
}
