#include "ParticleEmitter.h"
#include "TextureAssetHandler.h"
#include "DX11.h"
#include <fstream>

void ParticleEmitter::InitParticle(size_t aParticleIndex)
{
    myParticles[aParticleIndex].Color = myEmitterSettings.StartColor;
    myParticles[aParticleIndex].LifeTime = 0;
    myParticles[aParticleIndex].Position = { 0, 0 ,0 ,1 };
    myParticles[aParticleIndex].Scale = { myEmitterSettings.StartSize, myEmitterSettings.StartSize, myEmitterSettings.StartSize };
    myParticles[aParticleIndex].Velocity = myEmitterSettings.StartVelocity;
}

bool ParticleEmitter::Init(const ParticleEmitterTemplate& aTamlate)
{
    myEmitterSettings = aTamlate.EmitterSettings;

    myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

    const size_t maxNumberOfParticles = static_cast<size_t>(ceilf(myEmitterSettings.SpawnRate * myEmitterSettings.LifeTime));

    myParticles.resize(maxNumberOfParticles);

    for (size_t i = 0; i < myParticles.size(); i++)
    {
        InitParticle(i);
    }

    HRESULT result;

    D3D11_BUFFER_DESC vertexBufferDesc{};
    vertexBufferDesc.ByteWidth = static_cast<UINT>(myParticles.size()) * static_cast<UINT>(sizeof(ParticleVertex));
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
    vertexSubresourceData.pSysMem = &myParticles[0];

    result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, myVertexBuffer.GetAddressOf());

    // Pixel Shader
    std::ifstream psFile;
    psFile.open("Shaders/Particle_ps.cso", std::ios::binary);
    std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
    result = DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, &myPixelShader);
    if (FAILED(result))
    {
        return false;
    }
    psFile.close();

    // Vertex Shader
    std::ifstream vsFile;
    vsFile.open("Shaders/Particle_vs.cso", std::ios::binary);
    std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
    result = DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &myVertexShader);
    if (FAILED(result))
    {
        return false;
    }
    psFile.close();

    // Geometry Shader
    std::ifstream gsFile;
    gsFile.open("Shaders/Particle_gs.cso", std::ios::binary);
    std::string gsData = { std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>() };
    result = DX11::Device->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, &myGeometryShader);
    if (FAILED(result))
    {
        return false;
    }
    gsFile.close();

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"SCALE",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"LIFETIME", 0, DXGI_FORMAT_R32_FLOAT,          0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    result = DX11::Device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &myInputLayout);
    if (FAILED(result))
    {
        return false;
    }

    if (!TextureAssetHandler::LoadTexture(L"ParticleStar.dds"))
    {
        return false;
    }

    myTexture = TextureAssetHandler::GetTexture(L"ParticleStar.dds");

    return false;
}

void ParticleEmitter::Update(float someDeltaTime)
{
    for (size_t i = 0; i < myParticles.size(); i++)
    {
        ParticleVertex& particle = myParticles[i];
        particle.LifeTime += someDeltaTime;

        if (particle.LifeTime >= myEmitterSettings.LifeTime)
        {
            InitParticle(i);
        }
    }
}

void ParticleEmitter::SetAsResource() const
{
    HRESULT result = S_FALSE;

    D3D11_MAPPED_SUBRESOURCE bufferData;
    ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
    result = DX11::Context->Map(
        myVertexBuffer.Get(),
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &bufferData
    );

    memcpy_s(
        bufferData.pData,
        sizeof(ParticleVertex) * myParticles.size(),
        &myParticles[0],
        sizeof(ParticleVertex) * myParticles.size()
    );

    DX11::Context->Unmap(myVertexBuffer.Get(), 0);

    DX11::Context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(myPrimitiveTopology));
    DX11::Context->IASetInputLayout(myInputLayout.Get());
    DX11::Context->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &myStride, &myOffset);

    DX11::Context->VSSetShader(myVertexShader.Get(), nullptr, 0);
    DX11::Context->GSSetShader(myGeometryShader.Get(), nullptr, 0);
    DX11::Context->PSSetShader(myPixelShader.Get(), nullptr, 0);

    if (myTexture)
    {
        myTexture->SetAsResource(0);
    }

}

void ParticleEmitter::Draw() const
{
    DX11::Context->Draw(static_cast<UINT>(myParticles.size()), 0);
}
