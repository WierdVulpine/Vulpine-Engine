#pragma once
#include <memory>
#include <vector>
#include <stack>
#include <Vector3.hpp>
#include <string>
#include "GraphicsEngine.h"

class Scene;

struct EditCommand
{
	int IndexToChange;
	std::string name;

	EditCommand(const std::string& aName, const int& aIndex)
	{
		IndexToChange = aIndex;
		name = aName;
	}
};

class EditorLayer
{
public:
	EditorLayer() = default;
	~EditorLayer() = default;

	bool Init(std::shared_ptr<GraphicsEngine> aEngine);

	bool Update();

	std::string myCurrentContentDirectory{".\\"};
private:
	void DrawEntity(int aIndexToDraw);

	std::shared_ptr<Scene> myActiveScene;
	std::vector<int> mySelectedItems;
	int mySelectedItem{ -1 };

	std::stack<EditCommand> myCommandStack;
	std::stack<EditCommand> myCommandntStack;
	std::shared_ptr<GraphicsEngine> myEngine;

	EditCommand* myYMoment{ nullptr };

	std::string myDragDropStuff;
	std::string myCurrentScenePath{ "" };
};

