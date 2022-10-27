#pragma once
#include "GBuffer.h"
#include "DirectionalLight.h"
#include "EnviromentLight.h"

constexpr UINT MAX_DEFFERED_LIGHTS = 64;

class DefferedRenderer
{
	struct FrameBufferData
	{
		Matrix4x4f View;
		Matrix4x4f Projection;
		CommonUtilities::Vector3f CamTranslation;
		float Padding;
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

	struct SceneLightBuffer
	{
		Light::LightBufferData DirectionalLight;
		Light::LightBufferData Lights[MAX_DEFFERED_LIGHTS];
		unsigned int NumLights;
		CommonUtilities::Vector3f Padding;
	} mySceneLightBufferData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> myFrameBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myObjectBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myMaterialBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myLightBuffer;

	ComPtr<ID3D11PixelShader> myGBufferPixelShader;
	ComPtr<ID3D11VertexShader> myFullscreenShader;
	ComPtr<ID3D11PixelShader> myEnvironmentShader;
	ComPtr<ID3D11InputLayout> myGBufferLayout;
	std::unique_ptr<GBuffer> myGBuffer;

public:
	bool Initialize();

	void GenerateGBuffer(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<ModelInstance>>& aModelIist);

	void Render(const std::shared_ptr<Camera>& aCamera, const std::shared_ptr<DirectionalLight>& aDirectionalLight,
		const std::shared_ptr<EnviromentLight>& aEnviromentLight);

	const std::unique_ptr<GBuffer>& GetGBuffer() const { return myGBuffer; }

	void ClearGBuffer();
};

