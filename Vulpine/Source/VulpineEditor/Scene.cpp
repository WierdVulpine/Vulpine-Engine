#include "Scene.h"
#include <GraphicsEngine.h>
#include <Renderer.h>
#include <Camera.h>
#include <ModelAssetHandler.h>
#include "Input.h"
#include "Time.h"
#include "imgui.h"
#include "ModelInstance.h"

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

	Renderer::SetCamera(myMainCamera);
}

void Scene::Update()
{
	for (size_t i = 0; i < mySceneObjects.size(); i++)
	{
		mySceneObjects[i]->Update(Time::GetDeltaTime());
	}

	ImGui::SetNextWindowSize({ 350, 200 });
	ImGui::Begin("Editor");

	static int currentObject = 0;
	if (mySceneObjects.size() > 1)
	{
		ImGui::SliderInt("Current Object ", &currentObject, 0, mySceneObjects.size() - 1);
	}

	float cubePos[] = { mySceneObjects[currentObject]->GetTransform().GetPosition().x, mySceneObjects[currentObject]->GetTransform().GetPosition().y, mySceneObjects[currentObject]->GetTransform().GetPosition().z };
	float cubeRotation[] = { mySceneObjects[currentObject]->GetTransform().GetRotation().x, mySceneObjects[currentObject]->GetTransform().GetRotation().y, mySceneObjects[currentObject]->GetTransform().GetRotation().z };
	float cubeScale[] = { mySceneObjects[currentObject]->GetTransform().GetScale().x, mySceneObjects[currentObject]->GetTransform().GetScale().y, mySceneObjects[currentObject]->GetTransform().GetScale().z };

	ImGui::DragFloat3("Position", cubePos);
	ImGui::DragFloat3("Rotation", cubeRotation);
	ImGui::DragFloat3("Scale", cubeScale, 0.01f);

	mySceneObjects[currentObject]->SetPosition({ cubePos[0], cubePos[1], cubePos[2] });
	mySceneObjects[currentObject]->SetRotation({ cubeRotation[0], cubeRotation[1], cubeRotation[2] });
	mySceneObjects[currentObject]->SetScale({ cubeScale[0], cubeScale[1], cubeScale[2] });

	static float speed = 80;
	static float rotSpeeed = 5.2f;

	ImGui::Text("Camera:");
	ImGui::DragFloat("Move Speed", &speed, 0.5f);
	ImGui::DragFloat("Rotate Speed", &rotSpeeed, 0.5f);

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
