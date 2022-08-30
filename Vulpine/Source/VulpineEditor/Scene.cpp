#include "Scene.h"
#include <GraphicsEngine.h>
#include <Renderer.h>
#include <Camera.h>
#include <ModelAssetHandler.h>
#include "Input.h"
#include "Time.h"
#include "imgui.h"

void Scene::Init()
{
	myMainCamera = std::make_shared<Camera>(Camera());

	myMainCamera = std::make_shared<Camera>(Camera());
	myMainCamera->Init(90, { 1200, 800 }, 0.1f, 15000.f);
	myMainCamera->SetPosition({ 0,0,-200 });

	std::shared_ptr<Model> temp = ModelAssetHandler::GetModel(L"Cube");
	temp->SetPosition({ 0,0,0 });

	AddGameObject(temp);

	Renderer::SetCamera(myMainCamera);
}

void Scene::Update()
{
	ImGui::Begin("Gamer");

	float cubePos[] = { mySceneObjects[0]->GetTransform().GetPosition().x, mySceneObjects[0]->GetTransform().GetPosition().y, mySceneObjects[0]->GetTransform().GetPosition().z };
	float cubeRotation[] = { mySceneObjects[0]->GetTransform().GetRotation().x, mySceneObjects[0]->GetTransform().GetRotation().y, mySceneObjects[0]->GetTransform().GetRotation().z };

	ImGui::DragFloat3("Position", cubePos);
	ImGui::DragFloat3("Rotation", cubeRotation);

	mySceneObjects[0]->SetPosition({ cubePos[0], cubePos[1], cubePos[2] });
	mySceneObjects[0]->SetRotation({ cubeRotation[0], cubeRotation[1], cubeRotation[2] });

	ImGui::End();


	float speed = 20;
	float rotSpeeed = 0.2f;

	CommonUtilities::Vector3f dir;

	if (Input::KeyIsPressed(VK_SPACE))
	{
		dir.y = 1;
	}
	if (Input::KeyIsPressed(VK_SHIFT))
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
