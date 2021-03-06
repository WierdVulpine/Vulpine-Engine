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
    if (!aCamera)
    {
        return;
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

    for (const std::shared_ptr<Model> model : aModelList)
    {
        auto meshData = model->GetMeshData();

        myObjectBufferData.World = model->GetTransform().GetMatrix();
        ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

        result = DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
        if (FAILED(result))
        {
            // DX
        }
        //dadawdadawdasw
        //DAb

        memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
        DX11::Context->Unmap(myObjectBuffer.Get(), 0);

        DX11::Context->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
        DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        DX11::Context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));
        DX11::Context->IASetInputLayout(meshData.myInputLayout.Get());

        DX11::Context->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
        DX11::Context->PSSetShader(meshData.myPixelShader.Get(), nullptr, 0);

        DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
        DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

        DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);

    }

}
