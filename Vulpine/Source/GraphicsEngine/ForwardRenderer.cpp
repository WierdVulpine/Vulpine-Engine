#include "ForwardRenderer.h"
#include "DX11.h"
#include "Camera.h"
#include "Model.h"

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

    return true;
}

void ForwardRenderer::Render(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<Model>>& aModelList)
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

    for (const std::shared_ptr<Model> model : aModelList)
    {
        auto meshData = model->GetMeshData();

        myObjectBufferData.World = {};
        ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

        result = DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
        if (FAILED(result))
        {
            // DX
        }

        //DAb

        memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
        DX11::Context->Unmap(myObjectBuffer.Get(), 0);

        DX11::Context->IAGetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);

    }

}
