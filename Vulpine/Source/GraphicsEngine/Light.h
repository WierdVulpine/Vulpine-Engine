#pragma once
#include <string>
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "SceneObject.h"
#include <Vector3.hpp>

using namespace Microsoft::WRL;

class Light : SceneObject
{
	friend class LightAssetHandler;

public:
	struct LightBufferData
	{
		CommonUtilities::Vector3f Color;
		float Intensity;
		CommonUtilities::Vector3f Direction;
		float Padding;
	};

public:
	LightBufferData myLightBufferData;

	virtual ~Light() override = default;
	virtual void Init(CommonUtilities::Vector3f aColor, float aIntensity);

	virtual void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) = 0;

	FORCEINLINE CommonUtilities::Vector4f GetColor() const { return { myLightBufferData.Color.x, myLightBufferData.Color.y,
		                                                              myLightBufferData.Color.z, 1 }; }
	FORCEINLINE float GetIntensity() const { return myLightBufferData.Intensity; }
};

