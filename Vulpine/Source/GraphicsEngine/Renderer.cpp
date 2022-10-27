#include "Renderer.h"
#include "Model.h"
#include "ForwardRenderer.h"
#include "LightAssetHandler.h"
#include "RenderStateManager.h"
#include "DefferedRenderer.h"
#include "ShadowRenderer.h"

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

void Renderer::Render(ForwardRenderer& aRenderer, DefferedRenderer& aDefferedRenderer, ShadowRenderer& aShadowRenderer)
{
	float clear[] = { 1,1,1,1 };
	std::shared_ptr<Light> myDirLight = LightAssetHandler::GetDirectionalLight();

	DX11::Context->ClearRenderTargetView(aDefferedRenderer.GetGBuffer()->myVRTVs.Get(), &clear[0]);
	aDefferedRenderer.ClearGBuffer();
	myDirLight->ClearShadowMap();
	myDirLight->SetShadowMapAsDepth(*aDefferedRenderer.GetGBuffer());

	DX11::SetViewPort(2048.f, 2048.f);

	aShadowRenderer.Render(myDirLight, myStorage.myModels);

	DX11::SetViewPort(static_cast<float>(DX11::ClientRect.right - DX11::ClientRect.left),
		static_cast<float>(DX11::ClientRect.bottom - DX11::ClientRect.top));

	RenderStateManager::SetBlendState(RenderStateManager::BlendState::Opaque);
	RenderStateManager::SetDepthStencilState(RenderStateManager::DepthStencilState::ReadWrite);
	aDefferedRenderer.GenerateGBuffer(myCameraPtr, myStorage.myModels);
	aDefferedRenderer.Render(myCameraPtr, LightAssetHandler::GetDirectionalLight(), LightAssetHandler::GetEnviromentLight());

	RenderStateManager::SetBlendState(RenderStateManager::BlendState::Additive);
	RenderStateManager::SetDepthStencilState(RenderStateManager::DepthStencilState::ReadOnly);
	aRenderer.RenderParticles(myCameraPtr, myStorage.mySystems);
	myStorage.myModels.clear();
}
