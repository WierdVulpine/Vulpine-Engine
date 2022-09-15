#pragma once
#include "Matrix4x4.hpp"
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <memory>
#include <vector>
#include "EnviromentLight.h"
#include "DirectionalLight.h"
#include "ParticleSystem.h"

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
	Microsoft::WRL::ComPtr<ID3D11Buffer> myLightBuffer;

public:

	bool Initialize();

	void RenderModels(
		const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<ModelInstance>>& aModelList,
		const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::shared_ptr<EnviromentLight>& anEnviromentalLight
		);

	void RenderParticles(
		const std::shared_ptr<Camera>& aCamera,
		const std::vector<std::shared_ptr<ParticleSystem>>& aParticleSystemList
	);

};

