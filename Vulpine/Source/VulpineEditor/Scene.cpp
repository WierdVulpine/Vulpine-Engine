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

namespace fs = std::filesystem;

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

	std::shared_ptr<ModelInstance> temp = ModelAssetHandler::GetModelInstance(L"gremlin_sk.fbx");
	ModelAssetHandler::LoadAnimation(L"gremlin_sk.fbx", L"gremlin@run.fbx");
	temp->SetAnimation(L"gremlin@run.fbx");
	temp->SetPosition({ 0,0,0 });

	std::shared_ptr<ModelInstance> temp1 = ModelAssetHandler::GetModelInstance(L"SM_Particle_Chest.fbx");
	temp1->SetPosition({ -100,0,0 });
	std::shared_ptr<ModelInstance> temp2 = ModelAssetHandler::GetModelInstance(L"Cube");
	temp2->SetPosition({ 100,0,0 });

	ParticleAssetHandler::LoadEmitterTemplate(L"TestEmitter.em");
	myTestSystem = ParticleAssetHandler::GetParticleSystem(L"TestSystem");
	Renderer::AddSystem(myTestSystem);

	AddGameObject(temp);
	AddGameObject(temp1);
	AddGameObject(temp2);

	LightAssetHandler::CreateDirectionalLight({ 1,1,1 }, 1, { 0,0,-1 });
	LightAssetHandler::CreateEnvirometalLight(L"skansen_cubemap.dds");

	LoadSettings();
	LoadScene();

	Renderer::SetCamera(myMainCamera);
}

void Scene::Update()
{
	for (size_t i = 0; i < mySceneObjects.size(); i++)
	{
		mySceneObjects[i]->Update(Time::GetDeltaTime());
	}

	myTestSystem->Update(Time::GetDeltaTime());

	static bool ShowSettings = false;
	static bool ShowPresetEditor = false;
	static bool ShowEmitterEditor = false;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Save", "These Do nothing yet :)");
			ImGui::MenuItem("Load", "These Do nothing yet :)");

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Settings")) { ShowSettings = !ShowSettings; }
			if (ImGui::MenuItem("Preset Editor")) { ShowPresetEditor = !ShowPresetEditor; }
			//if (ImGui::MenuItem("Emitter Editor")) { ShowEmitterEditor = !ShowEmitterEditor; }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (ShowEmitterEditor)
	{
		ImGui::Begin("Emitter Editor");

		std::vector<std::string> emitterFiles;

		std::string path = "Emitters/";
		for (const auto& entry : fs::directory_iterator(path))
		{
			emitterFiles.push_back(entry.path().filename().string());
		}
		static std::string selectedFirstItem = emitterFiles[0];
		static int selectedItemIndex = 0;
		if (ImGui::BeginCombo("Emitter Name", selectedFirstItem.c_str()))
		{
			for (size_t i = 0; i < emitterFiles.size(); i++)
			{
				bool isSelected = (selectedFirstItem == emitterFiles[i]);
				if (ImGui::Selectable(emitterFiles[i].c_str(), isSelected))
				{
					selectedFirstItem = emitterFiles[i];
					selectedItemIndex;
					//PresetOne = LoadPreset(selectedFirstItem);
				}
			}

			ImGui::EndCombo();
		}


		ImGui::End();
	}

	if (ShowPresetEditor)
	{
		ImGui::SetNextWindowSize({ 400,400 });

		ImGui::Begin("Preset Editor");

		static char curValue[256] = "";

		static float colorPresetTest[] = { 0,0,0,0 };

		ImGui::InputText("FileName", curValue, sizeof(curValue));
		ImGui::ColorEdit4("Color:", colorPresetTest);
		if (ImGui::Button("Save Preset"))
		{
			SavePreset(curValue, colorPresetTest);
		}

		static char removeValue[256] = "";

		ImGui::InputText("File remove name:", removeValue, sizeof(removeValue));
		if (ImGui::Button("Remove File"))
		{
			std::string removePrefix = "Presets/";
			removePrefix += removeValue;
			if (remove(removePrefix.c_str()))
			{

			}
		}

		ImGui::End();
	}

	if (ShowSettings)
	{
		ImGui::SetNextWindowSize({ 400,400 });

		bool windowVisible = ImGui::Begin("Settings");

		static bool usePreset = false;

		static ImVec4 PresetOne = { 0,0,0,0 };
		static ImVec4 PresetTwo = { 1,1,1,1 };

		std::vector<std::string> presetFiles;

		std::string path = "Presets/";
		for (const auto &entry : fs::directory_iterator(path))
		{
			presetFiles.push_back(entry.path().filename().string());
		}

		if (!windowVisible)
		{
			ImGui::End();
			return;
		}

		float ClearColor[] = {myEnginePtr->GetClearColor().x, myEnginePtr->GetClearColor().y, myEnginePtr->GetClearColor().z, 1};

		static float ColorLerp = { 0 };

		if (presetFiles.size() >= 2)
		{
			ImGui::Checkbox("Use Presets", &usePreset);
		}
		else
		{
			ImGui::Text("There needs to be atleast 2 presets to use presets");
		}

		if (!usePreset)
		{
			ImGui::ColorPicker4("Clear Color", ClearColor);
			myEnginePtr->GetClearColor() = { ClearColor[0], ClearColor[1], ClearColor[2], ClearColor[3] };
		}
		else
		{
			ImGui::ColorButton("First Preset", PresetOne);
			ImGui::SameLine();
			if (myFirstPresetIndex >= presetFiles.size())
			{
				myFirstPresetIndex = presetFiles.size() - 1;
			}
			static std::string selectedFirstItem = presetFiles[myFirstPresetIndex];
			PresetOne = LoadPreset(selectedFirstItem);
			if (ImGui::BeginCombo("First Preset#", selectedFirstItem.c_str()))
			{
				for (size_t i = 0; i < presetFiles.size(); i++)
				{
					bool isSelected = (selectedFirstItem == presetFiles[i]);
					if (ImGui::Selectable(presetFiles[i].c_str(), isSelected))
					{
						selectedFirstItem = presetFiles[i];
						myFirstPresetIndex = i;
					}
				}

				ImGui::EndCombo();
			}

			ImGui::ColorButton("Second Preset", PresetTwo);
			ImGui::SameLine();

			if (mySecondPresetIndex >= presetFiles.size())
			{
				mySecondPresetIndex = presetFiles.size() - 1;
			}

			static std::string selectedSecondItem = presetFiles[mySecondPresetIndex];
			PresetTwo = LoadPreset(selectedSecondItem);
			if (ImGui::BeginCombo("Second Preset#", selectedSecondItem.c_str()))
			{
				for (size_t i = 0; i < presetFiles.size(); i++)
				{
					bool isSelected = (selectedSecondItem == presetFiles[i]);
					if (ImGui::Selectable(presetFiles[i].c_str(), isSelected))
					{
						selectedSecondItem = presetFiles[i];
						mySecondPresetIndex = i;
					}
				}

				ImGui::EndCombo();
			}

			ImGui::SliderFloat("Color Lerp", &myLerpValue, 0, 1);
			ImGui::Text("Lerped Color:");
			ImVec4 lerpedColor = ColorLerper(PresetOne, PresetTwo, myLerpValue);
			ImGui::ColorButton("LerpedColor", lerpedColor, 0, { 50, 50 });
			myEnginePtr->GetClearColor() = { lerpedColor.x, lerpedColor.y, lerpedColor.z, lerpedColor.w };
		}

		if (ImGui::Button("Save"))
		{
			SaveSettings();
		}

		ImGui::End();
	}


	ImGui::SetNextWindowSize({ 350, 400 });
	ImGui::Begin("Editor");

	for (size_t i = 0; i < mySceneObjects.size(); i++)
	{

		std::wstring ObjName = mySceneObjects[i]->GetName();
		std::string strName = std::string(ObjName.begin(), ObjName.end());
		if (ImGui::TreeNode(strName.c_str()))
		{
			float cubePos[] = { mySceneObjects[i]->GetTransform().GetPosition().x, mySceneObjects[i]->GetTransform().GetPosition().y, mySceneObjects[i]->GetTransform().GetPosition().z };
			float cubeRotation[] = { mySceneObjects[i]->GetTransform().GetRotation().x, mySceneObjects[i]->GetTransform().GetRotation().y, mySceneObjects[i]->GetTransform().GetRotation().z };
			float cubeScale[] = { mySceneObjects[i]->GetTransform().GetScale().x, mySceneObjects[i]->GetTransform().GetScale().y, mySceneObjects[i]->GetTransform().GetScale().z };

			ImGui::DragFloat3("Position", cubePos);
			ImGui::DragFloat3("Rotation", cubeRotation);
			ImGui::DragFloat3("Scale", cubeScale, 0.01f);

			mySceneObjects[i]->SetPosition({ cubePos[0], cubePos[1], cubePos[2] });
			mySceneObjects[i]->SetRotation({ cubeRotation[0], cubeRotation[1], cubeRotation[2] });
			mySceneObjects[i]->SetScale({ cubeScale[0], cubeScale[1], cubeScale[2] });


			ImGui::TreePop();
		}
	}

	static float speed = 80;
	static float rotSpeeed = 5.2f;

	ImGui::Text("Camera:");
	ImGui::DragFloat("Move Speed", &speed, 0.5f);
	ImGui::DragFloat("Rotate Speed", &rotSpeeed, 0.5f);

	if (ImGui::Button("Save Scene"))
	{
		SaveScene();
	}

	float DirLightInt = LightAssetHandler::GetDirectionalLight()->myLightBufferData.Intensity;

	ImGui::DragFloat("DirLightInt", &DirLightInt);

	LightAssetHandler::GetDirectionalLight()->myLightBufferData.Intensity = DirLightInt;

	ImGui::End();

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

void Scene::LoadScene()
{
	FILE* fp;
	fopen_s(&fp, "Scenes/OnlyScene.json", "rb");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	fclose(fp);

	for (size_t i = 0; i < d["SceneObjects"].GetArray().Size(); i++)
	{
		rapidjson::Value& currentObj = d["SceneObjects"][i];

		mySceneObjects[i]->SetPosition({ currentObj["Position"][0].GetFloat(),currentObj["Position"][1].GetFloat(), currentObj["Position"][2].GetFloat() });
		mySceneObjects[i]->SetRotation({ currentObj["Rotation"][0].GetFloat(),currentObj["Rotation"][1].GetFloat(), currentObj["Rotation"][2].GetFloat() });
		mySceneObjects[i]->SetScale({ currentObj["Scale"][0].GetFloat(),currentObj["Scale"][1].GetFloat(), currentObj["Scale"][2].GetFloat() });
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

void Scene::SaveScene()
{
	FILE* fp;
	fopen_s(&fp, "Scenes/OnlyScene.json", "w");

	char writeBuffer[65536];
	rapidjson::FileWriteStream is(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Document d;
	d.SetObject();

	rapidjson::Value SceneObjects(rapidjson::kArrayType);

	for (size_t i = 0; i < mySceneObjects.size(); i++)
	{
		rapidjson::Value objValue(rapidjson::kObjectType);

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

		SceneObjects.PushBack(objValue, d.GetAllocator());
	}

	d.AddMember("SceneObjects", SceneObjects, d.GetAllocator());

	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(is);
	d.Accept(writer);
	fclose(fp);
}
