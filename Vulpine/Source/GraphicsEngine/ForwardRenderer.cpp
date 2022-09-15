#include "ForwardRenderer.h"
#include "DX11.h"
#include "Camera.h"
#include "ModelInstance.h"

bool ForwardRenderer::Initialize()
{
    HRESULT result = S_FALSE;

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

    bufferDescription.ByteWidth = sizeof(MaterialBufferData);
    result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myMaterialBuffer.GetAddressOf());
    if (FAILED(result))
    {
        return false;
    }

    bufferDescription.ByteWidth = sizeof(Light::LightBufferData);
    result = DX11::Device->CreateBuffer(&bufferDescription, nullptr, myLightBuffer.GetAddressOf());
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void ForwardRenderer::RenderModels(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<ModelInstance>>& aModelList, const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::shared_ptr<EnviromentLight>& anEnviromentalLight)
{
    if (!aCamera)
    {
        return;
    }

    if (aDirectionalLight)
    {
        aDirectionalLight->SetAsResource(myLightBuffer);
    }

    if (anEnviromentalLight)
    {
        anEnviromentalLight->SetAsResource(nullptr);
    }

    HRESULT result = S_FALSE;
    D3D11_MAPPED_SUBRESOURCE bufferData;

    myFrameBufferData.View = Matrix4x4f::GetFastInverse(aCamera->GetTransform().GetMatrix());
    myFrameBufferData.Projection = aCamera->GetProjection();

    ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
    result = DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
    if (FAILED(result))
    {
        // XD
    }

    memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
    DX11::Context->Unmap(myFrameBuffer.Get(), 0);
    DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
    DX11::Context->GSSetShader(nullptr, nullptr, 0);

    for (const std::shared_ptr<ModelInstance> model : aModelList)
    {
        for (size_t i = 0; i < model->GetNumMeshes(); i++)
        {
            auto meshData = model->GetMeshData(i);

            myObjectBufferData.World = model->GetTransform().GetMatrix();
            myObjectBufferData.hasBones = model->HasBones();

            if (myObjectBufferData.hasBones)
            {
                memcpy_s(&myObjectBufferData.BoneData[0], sizeof(Matrix4x4f) * 128, model->myBoneTransforms, sizeof(Matrix4x4f) * 128);
            }

            ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

            result = DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
            if (FAILED(result))
            {
                // DX
            }

            memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
            DX11::Context->Unmap(myObjectBuffer.Get(), 0);


            ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

            result = DX11::Context->Map(myMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
            if (FAILED(result))
            {
                // UwU!!!
            }

            memcpy(bufferData.pData, &myMaterialBufferData, sizeof(MaterialBufferData));
            DX11::Context->Unmap(myMaterialBuffer.Get(), 0);

            if (meshData.myMaterial)
            {
                meshData.myMaterial->SetAsResource(myMaterialBuffer);
            }
            

            DX11::Context->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
            DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

            DX11::Context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));
            DX11::Context->IASetInputLayout(meshData.myInputLayout.Get());

            DX11::Context->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
            DX11::Context->PSSetShader(meshData.myPixelShader.Get(), nullptr, 0);

            DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
            DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
            DX11::Context->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());

            DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
        }
    }

}

void ForwardRenderer::RenderParticles(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<ParticleSystem>>& aParticleSystemList)
{


    HRESULT result = S_FALSE;
    D3D11_MAPPED_SUBRESOURCE bufferData;

    myFrameBufferData.View = Matrix4x4f::GetFastInverse(aCamera->GetTransform().GetMatrix());
    myFrameBufferData.Projection = aCamera->GetProjection();

    ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
    result = DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
    if (FAILED(result))
    {
        // XD
    }

    memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));

    DX11::Context->Unmap(myFrameBuffer.Get(), 0);
    DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
    DX11::Context->GSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
    DX11::Context->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

    for (const std::shared_ptr<ParticleSystem>& system : aParticleSystemList)
    {
        myObjectBufferData.World = system->GetTransform().GetMatrix();
        myObjectBufferData.hasBones = false;

        result = DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);

        if (FAILED(result))
        {

        }

        memcpy_s(bufferData.pData, sizeof(ObjectBufferData), &myObjectBufferData, sizeof(ObjectBufferData));

        DX11::Context->Unmap(myObjectBuffer.Get(), 0);

        DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
        DX11::Context->GSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
        DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

        for (const ParticleEmitter& emitter : system->GetEmitters())
        {
            emitter.SetAsResource();
            emitter.Draw();
        }
    }
}
