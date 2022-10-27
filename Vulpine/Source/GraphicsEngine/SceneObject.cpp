#include "SceneObject.h"

void SceneObject::AddChild(int aIndexToAdd)
{
	for (size_t i = 0; i < myChildren.size(); i++)
	{
		if (myChildren[i] == aIndexToAdd)
		{
			return;
		}
	}
	myChildren.push_back(aIndexToAdd);
}

void SceneObject::RemoveChild(int aIndexToRemove)
{
	for (size_t i = 0; i < myChildren.size(); i++)
	{
		if (myChildren[i] == aIndexToRemove)
		{
			myChildren.erase(myChildren.begin() + i);
			return;
		}
	}
}

void SceneObject::SetParent(int aParentIndex)
{
	myParentIndex = aParentIndex;
}

void SceneObject::RemoveParent()
{
	myParentIndex = -1;
}
