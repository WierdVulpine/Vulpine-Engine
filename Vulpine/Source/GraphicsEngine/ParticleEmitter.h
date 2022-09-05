#pragma once
#include <Vector4.hpp>
#include <Vector3.hpp>
#include <string>
#include <vector>
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>

using namespace Microsoft::WRL;

struct EmitterSettingsData
{
	float SpawnRate = 1.0f;
	float SpawnAngle = 0.0f;

	float LifeTime;

	CommonUtilities::Vector3f StartVelocity;
	CommonUtilities::Vector3f EndVelocity;

	float GravityScale;

	float StartSize;
	float EndSize;

	CommonUtilities::Vector4f StartColor;
	CommonUtilities::Vector4f EndColor;
};

struct ParticleEmitterTemplate
{
	std::wstring Path;
	EmitterSettingsData EmitterSettings;
};

struct ParticleVertex
{
	CommonUtilities::Vector4f Position = { 0,0,0,0 };
	CommonUtilities::Vector4f Color = { 0,0,0,0 };
	CommonUtilities::Vector3f Velocity = { 0,0,0 };
	CommonUtilities::Vector3f Scale = { 1,1,1 };
	float LifeTime = 0;
};

class ParticleEmitter
{
	friend class ParticleAssetHandler;

	EmitterSettingsData myEmitterSettings;

	std::vector<ParticleVertex> myParticles;

	ComPtr<ID3D11Buffer> myVertexBuffer;
	UINT myStride;
	UINT myOffset;
	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11GeometryShader> myGeometryShader;
	ComPtr<ID3D11PixelShader> myPixelShader;
	UINT myPrimitiveTopology;
	ComPtr<ID3D11InputLayout> myInputLayout;

};

