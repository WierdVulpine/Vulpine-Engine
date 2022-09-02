#pragma once
#include "Light.h"
#include "Texture.h"

class EnviromentLight : public Light
{
	friend class LightAssetHandler;

	std::shared_ptr<Texture> myTexture;

public:

	void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;
};

