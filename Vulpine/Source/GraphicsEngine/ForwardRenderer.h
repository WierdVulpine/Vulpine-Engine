#pragma once
#include "Matrix4x4.hpp"
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <memory>
#include <vector>

class Camera;
class ModelInstance;

using namespace CommonUtilities;

class ForwardRenderer
{
	struct FrameBufferData
	{
		Matrix4x4f View;
		Matrix4x4f Projection;
	} myFrameBufferData;

	struct ObjectBufferData
	{
		Matrix4x4f World;
		bool hasBones;
		Vector3f paddington;
		Matrix4x4f BoneData[128];
	} myObjectBufferData;

	struct MaterialBufferData
	{
		Vector3f Albedo;
		float padding;
	} myMaterialBufferData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> myFrameBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myObjectBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myMaterialBuffer;

public:

	bool Initialize();

	void Render(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<ModelInstance>>& aModelList);

};

