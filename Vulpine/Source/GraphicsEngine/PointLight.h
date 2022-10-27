#pragma once
#include "Light.h"

class PointLight : public Light
{
	friend class LightAssetHandler;

public:

	virtual ~PointLight() = default;

	void SetRange(float aRange) { myLightBufferData.Range = aRange; }
	void SetPosition(CommonUtilities::Vector3f aPosition) { myPosition = aPosition; myLightBufferData.Position = aPosition; }

	void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;

	FORCEINLINE float GetRange() const { return myLightBufferData.Range; }
	FORCEINLINE CommonUtilities::Vector3f GetPosition() const { return myPosition; }

private:
	CommonUtilities::Vector3f myPosition;
};

