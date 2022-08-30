#pragma once
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include "SceneObject.h"
#include <vector>
#include "Material.h"

using namespace Microsoft::WRL;

class Model : public SceneObject 
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
		std::shared_ptr<Material> myMaterial;
	};
private:

	std::vector<MeshData> myMeshData = {};
	std::wstring myPath;

public:

	void Init(std::vector<MeshData>& someMeshData, const std::wstring& aPath)
	{
		myMeshData = someMeshData;
		myPath = aPath;
	}

	const MeshData& GetMeshData(unsigned int anIndex) const { return myMeshData[anIndex]; }
	std::wstring const& GetName() const { return myPath; }

	FORCEINLINE size_t GetNumMeshes() const { return myMeshData.size(); }

};

