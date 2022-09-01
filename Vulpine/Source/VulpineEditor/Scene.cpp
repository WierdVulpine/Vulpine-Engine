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

	AddGameObject(temp);
	AddGameObject(temp1);
	AddGameObject(temp2);

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

	static bool ShowSettings = false;

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

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (ShowSettings)
	{
		ImGui::SetNextWindowSize({ 400,400 });

		bool windowVisible = ImGui::Begin("Settings");

		if (!windowVisible)
		{
			ImGui::End();
			return;
		}

		float ClearColor[] = {myEnginePtr->GetClearColor().x, myEnginePtr->GetClearColor().y, myEnginePtr->GetClearColor().z, 1};

		ImGui::ColorPicker4("Clear Color", ClearColor);

		myEnginePtr->GetClearColor() = { ClearColor[0], ClearColor[1], ClearColor[2], ClearColor[3] };

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
