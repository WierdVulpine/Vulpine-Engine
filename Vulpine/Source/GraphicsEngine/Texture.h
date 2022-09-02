#pragma once
#include <string>
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>

using namespace Microsoft::WRL;


class Texture
{
	friend class TextureAssetHandler;

protected:

	std::wstring myName;

	ComPtr<ID3D11Resource> myTexture;

	ComPtr<ID3D11ShaderResourceView> mySRV;

public:
	virtual ~Texture();

	void SetAsResource(unsigned int aSlot);
};
