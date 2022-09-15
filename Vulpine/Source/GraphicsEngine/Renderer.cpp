#include "Renderer.h"
#include "Model.h"
#include "ForwardRenderer.h"
#include "LightAssetHandler.h"
#include "RenderStateManager.h"

void Renderer::SetCamera(std::shared_ptr<Camera> aCamera)
{
	myCameraPtr = aCamera;
}

void Renderer::AddModel(const std::shared_ptr<ModelInstance>& aModel)
{
	myStorage.myModels.push_back(aModel);
}

void Renderer::AddSystem(const std::shared_ptr<ParticleSystem>& aSystem)
{
	myStorage.mySystems.push_back(aSystem);
}

void Renderer::Render(ForwardRenderer& aRenderer)
{
	RenderStateManager::SetDepthStencilState(RenderStateManager::DepthStencilState::ReadWrite);
	RenderStateManager::SetBlendState(RenderStateManager::BlendState::Opaque);
	aRenderer.RenderModels(myCameraPtr, myStorage.myModels, LightAssetHandler::GetDirectionalLight(), LightAssetHandler::GetEnviromentLight());

	RenderStateManager::SetDepthStencilState(RenderStateManager::DepthStencilState::ReadOnly);
	RenderStateManager::SetBlendState(RenderStateManager::BlendState::Additive);
	aRenderer.RenderParticles(myCameraPtr, myStorage.mySystems);
	myStorage.myModels.clear();
}
