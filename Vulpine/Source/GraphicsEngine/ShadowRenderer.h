#pragma once
#include <Matrix4x4.hpp>
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Light.h"
#include "ModelInstance.h"

class ShadowRenderer
{
	struct FrameBufferData
	{
		Matrix4x4f View;
		Matrix4x4f Projection;
		CommonUtilities::Vector3f CamTranslation;
		float FarPlane;
		float NearPlane;
		CommonUtilities::Vector3f paddington;
	} myFrameBufferData;

	struct ObjectBufferData
	{
		Matrix4x4f World;
		bool hasBones;
		CommonUtilities::Vector3f paddington;
		Matrix4x4f BoneData[128];
	} myObjectBufferData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> myFrameBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myObjectBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myMaterialBuffer;

public:
	bool Initialize();

	void Render(const std::shared_ptr<Light>& aLight, const std::vector<std::shared_ptr<ModelInstance>>& aModelList);
};

