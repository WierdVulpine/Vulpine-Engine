#pragma once
#include <string>
#include <Vector3.hpp>
#include "Texture.h"
#include <memory>
#include <array>

namespace MaterialTextureChannel
{
	enum
	{
		Albedo,
		Normal,
		COUNT
	};
}

class Material
{
	std::wstring myName;
	CommonUtilities::Vector3f myAlbedo;

public:

	struct MaterialData
	{
		CommonUtilities::Vector3f Albedo;
		float padding;
	};

private:

	MaterialData myMaterialData;

	std::array<std::shared_ptr<Texture>, MaterialTextureChannel::COUNT> myTextures;

public:

	Material() = default;
	virtual ~Material() = default;

	void Init(std::wstring aName, const CommonUtilities::Vector3f& anAlbedo);

	const std::wstring& GetName() const { return myName; }
	const CommonUtilities::Vector3f& GetAlbedo() const { return myAlbedo; }

	void SetAlbedoTexture(std::shared_ptr<Texture> aTexture);
	void SetNormalTexture(std::shared_ptr<Texture> aTexture);

	void SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer) const;

};

