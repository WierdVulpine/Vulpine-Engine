#pragma once
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

using namespace Microsoft::WRL;

class Model
{
public:
	struct MeshData
	{
		UINT myNumberOfVertecies;
		UINT myNumberOfIndices;
		UINT myStride;
		UINT myOffset;
		ComPtr<ID3D11Buffer> myVertexBuffer;
		ComPtr<ID3D11Buffer> myIndexBuffer;
		ComPtr<ID3D11VertexShader> myVertexShader;
		ComPtr<ID3D11PixelShader> myPixelShader;
		ComPtr<ID3D11InputLayout> myInputLayout;
		UINT myPrimitiveTopology;
	};
private:

	MeshData myMeshData = {};
	std::wstring myName;

public:

	void Init(MeshData& someMeshData, const std::wstring& aPath)
	{
		myMeshData = someMeshData;
		myName = aPath;
	}

	const MeshData& GetMeshData() const { return myMeshData; }
	std::wstring const& GetName() const { return myName; }

};

