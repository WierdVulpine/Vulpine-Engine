#pragma once
#include <wrl.h>
#include "Vector4.hpp"

using namespace Microsoft::WRL;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11SamplerState;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

class DX11
{
public:
	DX11();
	~DX11();

	bool Initialize(HWND aWindowHandle, bool anIsDebugDeviceEnabled);
	void BeginFrame(CommonUtilities::Vector4<float> aClearColor = { 1,0,0,1 });
	void EndFrame();

	static ComPtr<ID3D11Device> Device;
	static ComPtr<ID3D11DeviceContext> Context;
	static ComPtr<IDXGISwapChain> SwapChain;

	static ComPtr<ID3D11SamplerState> SampleStateDefault;
	static ComPtr<ID3D11SamplerState> SampleStateWrap;

	static ComPtr<ID3D11RenderTargetView> BackBuffer;
	static ComPtr<ID3D11DepthStencilView> DepthBuffer;
};

