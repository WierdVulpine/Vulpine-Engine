#include "Renderer.h"
#include "Model.h"
#include "ForwardRenderer.h"
#include "LightAssetHandler.h"

void Renderer::SetCamera(std::shared_ptr<Camera> aCamera)
{
	myCameraPtr = aCamera;
}

void Renderer::AddModel(const std::shared_ptr<ModelInstance>& aModel)
{
	myStorage.myModels.push_back(aModel);
}

void Renderer::Render(ForwardRenderer& aRenderer)
{
	aRenderer.Render(myCameraPtr, myStorage.myModels, LightAssetHandler::GetDirectionalLight(), LightAssetHandler::GetEnviromentLight());
	myStorage.myModels.clear();
}
