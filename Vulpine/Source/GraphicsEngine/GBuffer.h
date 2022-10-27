#pragma once
#include <array>
#include "DX11.h"
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Camera.h"
#include "ModelInstance.h"

class GBuffer
{
	friend class TextureAssetHandler;

public:
	enum GBufferTexture
	{
		GB_ALBEDO,
		GB_NORMAL,
		GB_MATERIAL,
		GB_VERTEXLORMAL,
		GB_POSITION,
		GB_AMBIENTOCCLUSION,
		GB_COUNT
	};

private: 
	std::array<ComPtr<ID3D11RenderTargetView>, GBufferTexture::GB_COUNT> myRTVs;
	std::array<ComPtr<ID3D11ShaderResourceView>, GBufferTexture::GB_COUNT> mySRVs;


public:
	inline static ComPtr<ID3D11RenderTargetView> myVRTVs;
	inline static ComPtr<ID3D11ShaderResourceView> myVSRVs;

	void SetAsTarget() const;
	void ClearTarget() const;
	void SetAsResource(unsigned int aStartSlot) const;
	void ClearResource(unsigned int aStartSlot) const;
	void Clear() const;
};

