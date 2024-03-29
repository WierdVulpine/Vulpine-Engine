#pragma once
#include "Texture.h"
#include <unordered_map>
#include <memory>
#include "GBuffer.h"
#include "Light.h"

class TextureAssetHandler
{
	typedef std::unordered_map<std::wstring, std::shared_ptr<Texture>> TextureMap;

	inline static TextureMap myRegistry{};

public:

	static std::shared_ptr<Texture> GetTexture(const std::wstring& aName);

	static bool LoadTexture(const std::wstring& aFileName);

	static std::unique_ptr<GBuffer> CreateGBuffer(RECT aWindowSize);
	static std::unique_ptr<Light::DepthStencil> CreateDepthStencil(const std::wstring& aName, size_t aWidth, size_t aHeight);
};

