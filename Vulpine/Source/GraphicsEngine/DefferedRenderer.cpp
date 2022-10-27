#include "DefferedRenderer.h"
#include "TextureAssetHandler.h"
#include <fstream>
#include "RenderStateManager.h"
#include "LightAssetHandler.h"

bool DefferedRenderer::Initialize()
{
	HRESULT result;

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(FrameBufferData);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myFrameBuffer.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(ObjectBufferData);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myObjectBuffer.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(Material::MaterialData);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myMaterialBuffer.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}
	
	bufferDescription.ByteWidth = sizeof(SceneLightBuffer);
	result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myLightBuffer.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	myGBuffer = TextureAssetHandler::CreateGBuffer(DX11::ClientRect);

	std::ifstream psFile;
	psFile.open("Shaders/GBuffer_ps.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	result = DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	psFile.close();

	if (FAILED(result))
	{
		return false;
	}
	myGBufferPixelShader = pixelShader;
	
	std::ifstream vsFile;
	vsFile.open("Shaders/GBuffer_vs.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	result = DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	vsFile.close();

	if (FAILED(result))
	{
		return false;
	}
	myFullscreenShader = vertexShader;
	
	psFile.open("Shaders/GBufferEnviroment_ps.cso", std::ios::binary);
	psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* epixelShader;
	result = DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, &epixelShader);
	psFile.close();

	if (FAILED(result))
	{
		return false;
	}
	myEnvironmentShader = epixelShader;

	return true;
}

void DefferedRenderer::GenerateGBuffer(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<ModelInstance>>& aModelIist)
{
	myGBuffer->Clear();

	myGBuffer->SetAsTarget();

	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (FAILED(result))
	{
		// XD
	}

	memcpy_s(bufferData.pData, sizeof(FrameBufferData), &myFrameBufferData, sizeof(FrameBufferData));

	DX11::Context->Unmap(myFrameBuffer.Get(), 0);
	DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Context->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	for (const std::shared_ptr<ModelInstance>& model : aModelIist)
	{
		for (size_t i = 0; i < model->GetNumMeshes(); i++)
		{
			const Model::MeshData& meshData = model->GetMeshData(i);

			myObjectBufferData.World = model->GetTransform().GetMatrix();
			myObjectBufferData.hasBones = model->HasBones();

			if (myObjectBufferData.hasBones)
			{
				memcpy_s(&myObjectBufferData.BoneData[0], sizeof(Matrix4x4f) * 128, model->myBoneTransforms, sizeof(Matrix4x4f) * 128);
			}

			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

			result = DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);

			memcpy_s(bufferData.pData, sizeof(ObjectBufferData), &myObjectBufferData, sizeof(ObjectBufferData));

			DX11::Context->Unmap(myObjectBuffer.Get(), 0);

			DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

			DX11::Context->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
			DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DX11::Context->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));
			DX11::Context->IASetInputLayout(meshData.myInputLayout.Get());

			if (meshData.myMaterial)
			{
				meshData.myMaterial->SetAsResource(myMaterialBuffer);
			}

			DX11::Context->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());
			DX11::Context->GSSetShader(nullptr, nullptr, 0);
			DX11::Context->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
			DX11::Context->PSSetShader(myGBufferPixelShader.Get(), nullptr, 0);

			DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
		}
	}
}

void DefferedRenderer::Render(const std::shared_ptr<Camera>& aCamera, const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::shared_ptr<EnviromentLight>& aEnviromentLight)
{
	if (!aCamera)
	{
		return;
	}
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myFrameBufferData.View = Matrix4x4f::GetFastInverse(aCamera->GetTransform().GetMatrix());
	myFrameBufferData.CamTranslation = aCamera->GetTransform().GetPosition();
	myFrameBufferData.Projection = aCamera->GetProjection();

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);

	if (FAILED(result))
	{
		// XD
	}

	memcpy_s(bufferData.pData, sizeof(FrameBufferData), &myFrameBufferData, sizeof(FrameBufferData));

	DX11::Context->Unmap(myFrameBuffer.Get(), 0);
	DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::Context->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	ZeroMemory(&mySceneLightBufferData, sizeof(SceneLightBuffer));
	if (aDirectionalLight)
	{
		mySceneLightBufferData.DirectionalLight = aDirectionalLight->GetLightBufferData();
	}

	if (aEnviromentLight)
	{
		aEnviromentLight->SetAsResource(myLightBuffer);
	}

	mySceneLightBufferData.NumLights = 0;
	std::vector<std::shared_ptr<Light>> lights = LightAssetHandler::GetLights();
	ZeroMemory(mySceneLightBufferData.Lights, sizeof(Light::LightBufferData) * MAX_DEFFERED_LIGHTS);
	for (size_t i = 0; i < lights.size() && i < MAX_DEFFERED_LIGHTS; i++)
	{
		mySceneLightBufferData.Lights[i] = lights[i]->GetLightBufferData();
		mySceneLightBufferData.Lights[i].Position = lights[i]->GetLightBufferData().Position;
		mySceneLightBufferData.NumLights++;
	}

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::Context->Map(myLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);

	if (FAILED(result))
	{

	}

	memcpy_s(bufferData.pData, sizeof(SceneLightBuffer), &mySceneLightBufferData, sizeof(SceneLightBuffer));
	DX11::Context->Unmap(myLightBuffer.Get(), 0);
	DX11::Context->PSSetConstantBuffers(3, 1, myLightBuffer.GetAddressOf());

	DX11::Context->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	DX11::Context->IASetInputLayout(nullptr);
	DX11::Context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	DX11::Context->GSSetShader(nullptr, nullptr, 0);
	DX11::Context->VSSetShader(myFullscreenShader.Get(), nullptr, 0);
	DX11::Context->PSSetShader(myEnvironmentShader.Get(), nullptr, 0);

#ifdef _DEBUG
	DX11::Context->OMSetRenderTargets(1, GBuffer::myVRTVs.GetAddressOf(), DX11::DepthBuffer.Get()); // _DEBUG
#endif
#ifndef _DEBUG
	DX11::Context->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), DX11::DepthBuffer.Get());
#endif // !_


	myGBuffer->SetAsResource(0);
	aDirectionalLight->SetShadowMapAsResource(20);
	RenderStateManager::SetDepthStencilState(RenderStateManager::DepthStencilState::None);
	DX11::Context->Draw(3, 0);
}

void DefferedRenderer::ClearGBuffer()
{
	myGBuffer->Clear();
}
