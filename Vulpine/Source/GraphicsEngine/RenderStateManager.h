#pragma once
#include <stack>
#include <unordered_map>
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>

using namespace Microsoft::WRL;

class RenderStateManager
{
public:
    enum class BlendState
    {
        Opaque,
        AlphaBlend,
        Additive,
        TextBlend
    };

    enum class DepthStencilState
    {
        None,
        ReadOnly,
        ReadWrite
    };

    RenderStateManager() = default;
    static bool Initialize();
    static void SetBlendState(BlendState aState);
    static void SetDepthStencilState(DepthStencilState aState);

private:
    static std::unordered_map<BlendState, ComPtr<ID3D11BlendState>> myBlendStates;
    static std::unordered_map<DepthStencilState, ComPtr<ID3D11DepthStencilState>> myDepthStencilStates;
};

