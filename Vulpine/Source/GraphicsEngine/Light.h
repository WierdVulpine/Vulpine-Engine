#pragma once
#include <string>
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "SceneObject.h"
#include <Vector3.hpp>
#include <Vector2.hpp>
#include "GBuffer.h"

using namespace Microsoft::WRL;

class Light : public SceneObject
{
	friend class LightAssetHandler;
	friend class Scene;

public:

	struct DepthStencil
	{
		ComPtr<ID3D11ShaderResourceView> mySRV;
		ComPtr<ID3D11DepthStencilView> myDSV;
		ComPtr<ID3D11Texture2D> myTexture;
		std::wstring myName;
		D3D11_VIEWPORT myViewPort;
	};

	struct LightBufferData
	{
		Matrix4x4f LightView;
		Matrix4x4f LightProjection;

		CommonUtilities::Vector3f Color;
		float Intensity;

		CommonUtilities::Vector3f Direction;
		float Range;

		CommonUtilities::Vector3f Position;
		float Attenuation;

		float SpotInnerRadius;
		float SpotOuterRadius;
		unsigned LightType;
		bool CastShadows;

		float NearPlane;
		float FarPlane;
		CommonUtilities::Vector2f Padding;
	};

public:
	LightBufferData myLightBufferData;

	std::unique_ptr<DepthStencil> myShadowMap;

	virtual ~Light() override = default;
	virtual void Init(CommonUtilities::Vector3f aColor, float aIntensity);

	virtual void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) = 0;

	void ClearShadowMap();
	void SetShadowMapAsDepth(const GBuffer& aGbuffer);
	void SetShadowMapAsResource(int aSlot);

	FORCEINLINE CommonUtilities::Vector4f GetColor() const { return { myLightBufferData.Color.x, myLightBufferData.Color.y,
		                                                              myLightBufferData.Color.z, 1 }; }
	FORCEINLINE float GetIntensity() const { return myLightBufferData.Intensity; }

	FORCEINLINE LightBufferData& GetLightBufferData() { return myLightBufferData; }

	FORCEINLINE DepthStencil* GetShadowMap() const { return myShadowMap.get(); }
};

