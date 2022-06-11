#include "Renderer.h"
#include "Model.h"
#include "ForwardRenderer.h"

void Renderer::SetCamera(std::shared_ptr<Camera> aCamera)
{
	myCameraPtr = aCamera;
}

void Renderer::AddModel(const std::shared_ptr<Model>& aModel)
{
	myStorage.myModels.push_back(aModel);
}

void Renderer::Render(ForwardRenderer& aRenderer)
{
	aRenderer.Render(myCameraPtr, myStorage.myModels);
	myStorage.myModels.clear();
}
