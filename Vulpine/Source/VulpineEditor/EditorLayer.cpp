#include "EditorLayer.h"
#include "Scene.h"
#include "imgui.h"
#include <stack>
#include "Input.h"
#include <filesystem>
#include "TextureAssetHandler.h"
#include "ImGui/misc/cpp/imgui_stdlib.h"
#include <Matrix4x4.hpp>
#include "ImGuizmo.h"
#include <Windows.h>
#include "ModelAssetHandler.h"
#include "LightAssetHandler.h"

#pragma warning(disable : 4996)

namespace fs = std::filesystem;

bool EditorLayer::Init(std::shared_ptr<GraphicsEngine> aEngine)
{
	myEngine = aEngine;
	myActiveScene = std::make_shared<Scene>(Scene(myEngine));
	myActiveScene->Init();
	mySelectedItem = -1;
	return false;
}

bool EditorLayer::Update()
{
	if (myActiveScene)
	{
		myActiveScene->Update();
	}

	{
		static bool SaveIsOpen{ false };
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save Scene"))
			{
				SaveIsOpen = true;
			}

			if (ImGui::MenuItem("Load Scene"))
			{
				OPENFILENAMEA ofn;
				char szFile[260] = { 0 };

				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = myEngine->GetWindowHandle();
				ofn.lpstrFile = szFile;
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = "Scene (.scene)\0*.scene\0";
				ofn.nFilterIndex = 1;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
				if (GetOpenFileNameA(&ofn) == TRUE)
				{
					std::filesystem::path fileName(ofn.lpstrFile);
					std::wstring name = fileName.filename();

					myActiveScene = std::make_shared<Scene>(Scene(myEngine));
					myActiveScene->Init();
					myActiveScene->LoadScene(fileName.string());

					name = name.erase(name.size() - 5);
					mySelectedItem = -1;
				}
			}

			if (ImGui::MenuItem("New Scene"))
			{
				myActiveScene = std::make_shared<Scene>(Scene(myEngine));
				myActiveScene->Init();
				mySelectedItem = -1;
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();

		if (SaveIsOpen)
		{
			ImGui::OpenPopup("Scene");
			if (ImGui::BeginPopup("Scene"))
			{
				char curValue[256];
				static char prevValue[256];
				memset(curValue, 0, sizeof(curValue));
				memcpy(curValue, prevValue, sizeof(prevValue));

				if (ImGui::InputText("Scene Name", curValue, sizeof(curValue), ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
				{
					SaveIsOpen = false;
					myCurrentScenePath = "Scenes/";
					myCurrentScenePath += curValue;
					myCurrentScenePath += ".Scene";
					myActiveScene->SaveScene(myCurrentScenePath);
					memcpy(prevValue, curValue, sizeof(curValue));
				}

				ImGui::EndPopup();
			}
		}

	}

	{
		static bool pOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;



		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		 // because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}



		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpacePanel() will render our background
		 // and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;



		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		 // This is because we want to keep our DockSpacePanel() active. If a DockSpacePanel() is inactive,
		 // all active windows docked into it will lose their parent and become undocked.
		 // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		 // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpacePanel Demo", &pOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();



		if (opt_fullscreen)
			ImGui::PopStyleVar(2);



		// Submit the DockSpacePanel
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
	}

	if (Input::KeyIsPressed(VK_CONTROL))
	{
		if (Input::GetKeyDown('Z'))
		{
			if (myCommandStack.size())
			{
				EditCommand temp = myCommandStack.top();
				myCommandStack.pop();

				EditCommand temp1(myActiveScene->mySceneObjects[temp.IndexToChange]->myRealName, temp.IndexToChange);
				myCommandntStack.push(temp1);

				myActiveScene->mySceneObjects[temp.IndexToChange]->myRealName = temp.name;
			}
		}
		if (Input::GetKeyDown('Y'))
		{
			if (myCommandntStack.size())
			{
				EditCommand temp = myCommandntStack.top();
				myCommandntStack.pop();

				EditCommand temp1(myActiveScene->mySceneObjects[temp.IndexToChange]->myRealName, temp.IndexToChange);
				myCommandStack.push(temp1);

				myActiveScene->mySceneObjects[temp.IndexToChange]->myRealName = temp.name;
			}
		}
	}

	// Hierarcy
	{
		ImGui::Begin("Hierarcy");

		if (!Input::KeyIsPressed('M'))
		{
			mySelectedItems.clear();
		}

		if (myActiveScene)
		{
			if (ImGui::TreeNode("Scene"))
			{
				std::vector<std::shared_ptr<ModelInstance>> tempSceneObjects = myActiveScene->mySceneObjects;

				for (size_t i = 0; i < tempSceneObjects.size(); i++)
				{
					if (tempSceneObjects[i]->myParentIndex == -1)
					{
						DrawEntity(i);
					}
				}
				ImGui::TreePop();
			}
		}
		else
		{
			ImGui::Text("No Active Scene");
		}

		ImGui::End();
	}

	// Inspector
	{
		ImGui::Begin("Inspector");

		if (mySelectedItem != -1)
		{
			std::shared_ptr<ModelInstance> SelectedItem = myActiveScene->mySceneObjects[mySelectedItem];

			float pos[] = { SelectedItem->GetTransform().GetPosition().x, SelectedItem->GetTransform().GetPosition().y, SelectedItem->GetTransform().GetPosition().z };
			float Rotation[] = { SelectedItem->GetTransform().GetRotation().x, SelectedItem->GetTransform().GetRotation().y, SelectedItem->GetTransform().GetRotation().z };
			float Scale[] = { SelectedItem->GetTransform().GetScale().x, SelectedItem->GetTransform().GetScale().y, SelectedItem->GetTransform().GetScale().z };

			std::string name = SelectedItem->GetNameStr();

			ImGui::Text("Name:"); ImGui::SameLine();
			if (ImGui::InputText("Havbba", &name, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
			{
				EditCommand temp(SelectedItem->myRealName, mySelectedItem);
				SelectedItem->myRealName = name;
				myCommandStack.push(temp);
			}

			ImGui::Text("Transform");

			ImGui::DragFloat3("Position", pos, 0.5f);
			ImGui::DragFloat3("Rotation", Rotation, 0.5f);
			ImGui::DragFloat3("Scale", Scale, 0.5f);

			SelectedItem->SetPosition({ pos[0], pos[1], pos[2] });
			SelectedItem->SetRotation({ Rotation[0], Rotation[1], Rotation[2] });
			SelectedItem->SetScale({ Scale[0], Scale[1], Scale[2] });

			ImGui::Spacing();
			ImGui::Text("Model Textures");

			std::vector<std::pair<std::string, std::string>> textureFiles;
			std::vector<std::pair<std::string, std::string>> normalFiles;
			std::vector<std::pair<std::string, std::string>> materialFiles;

			std::string path = "Textures/";
			for (const auto& entry : fs::directory_iterator(path))
			{
				std::string entryFulllName = entry.path().string();
				std::string entryName = entry.path().filename().string();
				std::string cutName = entryFulllName.substr(entryFulllName.size() - 6, entryFulllName.size() - 1);
				if (cutName == "_C.dds")
				{
					std::pair<std::string, std::string> tempPair;
					tempPair.first = entryFulllName;
					tempPair.second = entryName;
					textureFiles.push_back(tempPair);
				}
				if (cutName == "_N.dds")
				{
					std::pair<std::string, std::string> tempPair;
					tempPair.first = entryFulllName;
					tempPair.second = entryName;
					normalFiles.push_back(tempPair);
				}
				if (cutName == "_M.dds")
				{
					std::pair<std::string, std::string> tempPair;
					tempPair.first = entryFulllName;
					tempPair.second = entryName;
					materialFiles.push_back(tempPair);
				}
			}

			if (SelectedItem->GetMaterial()->myTextures[0])
			{
				std::string selectedFirstItem = std::string(SelectedItem->GetMaterial()->myTextures[0]->myName.begin(), SelectedItem->GetMaterial()->myTextures[0]->myName.end());

				if (ImGui::BeginCombo("Texture", selectedFirstItem.c_str()))
				{
					for (size_t i = 0; i < textureFiles.size(); i++)
					{
						bool isSelected = (selectedFirstItem == textureFiles[i].second);
						if (ImGui::Selectable(textureFiles[i].second.c_str(), isSelected))
						{
							selectedFirstItem = textureFiles[i].second;
							std::wstring tempLoader = std::wstring(textureFiles[i].first.begin(), textureFiles[i].first.end());
							if (TextureAssetHandler::LoadTexture(tempLoader))
							{
								std::shared_ptr<Material> tempMat = SelectedItem->GetMaterial();
								tempMat->SetAlbedoTexture(TextureAssetHandler::GetTexture(tempLoader));
								SelectedItem->SetMaterial(tempMat);
							}
						}
					}

					ImGui::EndCombo();
				}
			}

		}
		else
		{
			ImGui::Text("No Object Selected");
		}

		ImGui::End();
	}

	////
	//{
	//	ImGui::Begin("Lights");
	//	ImGui::Text("Directional Light");

	//	ImGui::End();
	//}

	// Content browser
	{
		ImGui::Begin("Content Browser");

		if (ImGui::Button("Return To Root"))
		{
			myCurrentContentDirectory = ".\\";
		}
		ImGui::SameLine();
		std::string dirName = "Current Directory: " + myCurrentContentDirectory;
		ImGui::Text(dirName.c_str());


		std::vector<std::pair<std::string, std::string>> paths;

		std::string path = myCurrentContentDirectory;
		for (const auto& entry : fs::directory_iterator(path))
		{
			std::string entryFulllName = entry.path().string();
			std::string entryName = entry.path().filename().string();

			std::pair<std::string, std::string> temp;
			temp.first = entryName;
			temp.second = entryFulllName;

			if (entryName.find('.') == std::string::npos)
			{
				paths.push_back(temp);
				continue;
			}

			std::string ending = entryName.substr(entryName.find_last_of('.') + 1);

			if (ending == "fbx" || ending == "dds")
			{
				paths.push_back(temp);
			}
		}

		for (size_t i = 0; i < paths.size(); i++)
		{
			const char* tempName = paths[i].first.c_str();
			ImGui::Selectable(tempName);

			if (ImGui::BeginDragDropSource())
			{
				myDragDropStuff = paths[i].second;
				ImGui::SetDragDropPayload("G", nullptr, 0);
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				if (paths[i].first.find('.') == std::string::npos)
				{
					myCurrentContentDirectory += "\\";
					myCurrentContentDirectory += paths[i].first;
				}
			}

		}

		ImGui::End();
	}

	// ViewPort
	{
		ImGui::Begin("ViewPort");
		Vector2f windowWidth(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		ImGui::Image((void*)GBuffer::myVSRVs.Get(), { windowWidth.x, windowWidth.y });
		ImGuiViewport viewport;

		if (ImGui::BeginDragDropTarget() && ImGui::IsMouseReleased(0))
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("browser");

			if (myDragDropStuff.size())
			{
				int endPos = myDragDropStuff.find_last_of('.') + 1;
				std::string ending = myDragDropStuff.substr(endPos);
				std::wstring wStringPath = std::wstring(myDragDropStuff.begin(), myDragDropStuff.end());

				if (ending == "dds")
				{
					if (mySelectedItem != -1)
					{
						std::string texType = myDragDropStuff.substr(myDragDropStuff.find_last_of('_') + 1, 1);

						std::shared_ptr<Material> tempMat = myActiveScene->mySceneObjects[mySelectedItem]->GetMaterial();
						if (texType == "C")
						{
							if (TextureAssetHandler::LoadTexture(wStringPath))
							{
								tempMat->SetAlbedoTexture(TextureAssetHandler::GetTexture(wStringPath));
							}
						}
						else if (texType == "N")
						{
							if (TextureAssetHandler::LoadTexture(wStringPath))
							{
								tempMat->SetNormalTexture(TextureAssetHandler::GetTexture(wStringPath));
							}
						}
						else if (texType == "M")
						{
							if (TextureAssetHandler::LoadTexture(wStringPath))
							{
								tempMat->SetMaterialTexture(TextureAssetHandler::GetTexture(wStringPath));
							}
						}
						myActiveScene->mySceneObjects[mySelectedItem]->SetMaterial(tempMat);
					}

				}
				else if (ending == "fbx")
				{
					std::shared_ptr<ModelInstance> newModel = ModelAssetHandler::GetModelInstance(wStringPath);
					newModel->SetPosition(0, 0, 0);

					myActiveScene->AddGameObject(newModel);
				}
			}

		}
		ImGui::End();
	}
	ImGui::End();

	if (myActiveScene)
	{
		myActiveScene->Render();
	}

	return true;
}

void EditorLayer::DrawEntity(int aIndexToDraw)
{
	std::vector<std::shared_ptr<ModelInstance>> myStuff = myActiveScene->mySceneObjects;
	if (aIndexToDraw >= 0 && aIndexToDraw < myStuff.size() && myStuff[aIndexToDraw])
	{
		bool Node;
		//ImGui::PushID(aIndexToDraw);
		if (aIndexToDraw == mySelectedItem)
		{
			Node = ImGui::TreeNodeEx((void*)aIndexToDraw, 1, myStuff[aIndexToDraw]->myRealName.c_str());
		}
		else
		{
			Node = ImGui::TreeNodeEx((void*)aIndexToDraw, 0, myStuff[aIndexToDraw]->myRealName.c_str());
		}
		//ImGui::PopID();
		if (Node)
		{
			if (ImGui::IsItemClicked())
			{
				if (Input::KeyIsPressed('M'))
				{
					mySelectedItems.push_back(myStuff[aIndexToDraw]->myIndex);
				}
				else if (Input::KeyIsPressed('C'))
				{
					if (myStuff[aIndexToDraw]->myParentIndex != -1)
					{
						myStuff[myStuff[aIndexToDraw]->myParentIndex]->RemoveChild(aIndexToDraw);
						myStuff[aIndexToDraw]->myParentIndex = myStuff[myStuff[aIndexToDraw]->myParentIndex]->myParentIndex;
						if (myStuff[aIndexToDraw]->myParentIndex != -1)
						{
							myStuff[myStuff[aIndexToDraw]->myParentIndex]->AddChild(aIndexToDraw);
						}
					}
				}
				else
				{
					mySelectedItem = aIndexToDraw;
				}
			}

			if (ImGui::BeginDragDropSource())
			{
				mySelectedItems.push_back(myStuff[aIndexToDraw]->myIndex);
				ImGui::SetDragDropPayload("index", &mySelectedItems, sizeof(std::vector<int>));
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("index");

				if (payload != nullptr)
				{
					const std::vector<int> droppedIndexes = *(std::vector<int>*)payload->Data;

					for (size_t i = 0; i < droppedIndexes.size(); i++)
					{
						if (!myActiveScene->HasParent(aIndexToDraw, droppedIndexes[i]))
						{
							if (myStuff[droppedIndexes[i]]->myParentIndex != -1)
							{
								myStuff[myStuff[droppedIndexes[i]]->myParentIndex]->RemoveChild(droppedIndexes[i]);
							}
							myActiveScene->SetChild(droppedIndexes[i], aIndexToDraw);
						}
					}
				}
			}

			for (size_t j = 0; j < myStuff[aIndexToDraw]->myChildren.size(); j++)
			{
				DrawEntity(myStuff[aIndexToDraw]->myChildren[j]);
			}

			ImGui::TreePop();
		}
	}
}
