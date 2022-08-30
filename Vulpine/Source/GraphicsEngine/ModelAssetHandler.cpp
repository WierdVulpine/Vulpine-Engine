#include "ModelAssetHandler.h"
#include "Model.h"
#include "DX11.h"
#include "Vertex.h"
#include <fstream>
#include "FBXImporter.h"
#include "ModelInstance.h"
#include "Material.h"
#include <random>

float GetRand()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

//std::unordered_map<std::wstring, std::shared_ptr<Model>> ModelAssetHandler::myModelRegistry;

bool ModelAssetHandler::InitUnitCube()
{
	std::vector<Vertex> mdlVerticies =
	{
		{
			50.0f, 50.0f, 50.0f,
			1 ,0, 0, 1
		},
		{
			50.0f, 50.0f, -50.0f,
			0 , 1, 0, 1
		},
		{
			50.0f, -50.0f, 50.0f,
			0 , 0, 1, 1
		},
		{
			-50.0f, 50.0f, 50.0f,
			1 , 0, 1, 1
		},
		{
			50.0f, -50.0f, -50.0f,
			0 , 1, 1, 1
		},
		{
			-50.0f, 50.0f, -50.0f,
			1 , 1, 0, 1
		},
		{
			-50.0f, -50.0f, 50.0f,
			1 , 1, 1, 1
		},
		{
			-50.0f, -50.0f, -50.0f,
			0 , 0, 0, 1
		}
	};

	HRESULT result;

	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(mdlVerticies.size()) * static_cast<UINT>(sizeof(Vertex));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
	vertexSubResourceData.pSysMem = &mdlVerticies[0];

	ID3D11Buffer* vertexBuffer;
	result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	std::vector<unsigned int> mdlIndecies =
	{
		0,2,4, // Right Side
		4,1,0,

		3,0,1, // Top Side
		1,5,3,

		3,6,2, // Back Side
		2,0,3,

		3,5,7, // Left Side
		7,6,3,

		5,1,4, // Front Side
		4,7,5,

		7,4,2, // Bottom Side
		2,6,7,
	};

	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.ByteWidth = static_cast<UINT>(mdlIndecies.size()) * static_cast<UINT>(sizeof(unsigned int));
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubresourceData{};
	indexSubresourceData.pSysMem = &mdlIndecies[0];

	ID3D11Buffer* indexBuffer;
	result = DX11::Device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Shader Stuff

	std::ifstream vsFile;
	vsFile.open("Shaders/FirstShader_vs.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	result = DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	vsFile.close();

	std::ifstream psFile;
	psFile.open("Shaders/FirstShader_ps.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	result = DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	if (FAILED(result))
	{
		return false;
	}
	psFile.close();

	// No Shader Stuff

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3D11InputLayout* inputLayout;
	result = DX11::Device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
	if (FAILED(result))
	{
		return false;
	}

	Model::MeshData modelData = {};
	modelData.myNumberOfVertecies = static_cast<UINT>(mdlVerticies.size());
	modelData.myNumberOfIndices = static_cast<UINT>(mdlIndecies.size());
	modelData.myStride = sizeof(Vertex);
	modelData.myOffset = 0;
	modelData.myVertexBuffer = vertexBuffer;
	modelData.myIndexBuffer = indexBuffer;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = inputLayout;

	std::shared_ptr<Model> mdl = std::make_shared<Model>();
	std::vector<Model::MeshData> mdlData;
	mdlData.push_back(modelData);
	mdl->Init(mdlData, L"Cube");
	myModelRegistry.insert({ L"Cube", mdl });

	return true;
}

bool ModelAssetHandler::LoadModel(const std::wstring& someFilePath)
{
	const std::string ansiFileName = std::string(someFilePath.begin(), someFilePath.end());

	TGA::FBXModel tgaModel;
	if (TGA::FBXImporter::LoadModel(ansiFileName, tgaModel))
	{
		std::vector<Model::MeshData> mdlMeshData;
		mdlMeshData.resize(tgaModel.Meshes.size());

		for (size_t i = 0; i < tgaModel.Meshes.size(); i++)
		{
			TGA::FBXModel::FBXMesh& mesh = tgaModel.Meshes[i];

			std::wstring wideMatName = someFilePath;
			wideMatName.pop_back();
			wideMatName.pop_back();
			wideMatName.pop_back();
			wideMatName.pop_back();

			wideMatName = wideMatName + L"_M.dds";

			std::shared_ptr<Material> meshMaterial;
			if (myMaterialRegistry.find(wideMatName) != myMaterialRegistry.end())
			{
				meshMaterial = myMaterialRegistry[wideMatName];
			}
			else
			{
				meshMaterial = std::make_shared<Material>();
				meshMaterial->Init(wideMatName, { GetRand(), GetRand(), GetRand ()});

				myMaterialRegistry.insert({ wideMatName, meshMaterial });
			}


			std::vector<Vertex> mdlVertecies;
			mdlVertecies.resize(mesh.Vertices.size());

			for (size_t v = 0; v < mesh.Vertices.size(); v++)
			{
				mdlVertecies[v] = mesh.Vertices[v];
				mdlVertecies[v].VertexColors[0].x = GetRand();
				mdlVertecies[v].VertexColors[0].y = GetRand();
				mdlVertecies[v].VertexColors[0].z = GetRand();
			}

			std::vector<unsigned int> mdlIndecies = mesh.Indices;

			HRESULT result;

			D3D11_BUFFER_DESC vertexBufferDesc{};
			vertexBufferDesc.ByteWidth = static_cast<UINT>(mdlVertecies.size()) * static_cast<UINT>(sizeof(Vertex));
			vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
			vertexSubResourceData.pSysMem = &mdlVertecies[0];

			ID3D11Buffer* vertexBuffer;
			result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &vertexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			D3D11_BUFFER_DESC indexBufferDesc{};
			indexBufferDesc.ByteWidth = static_cast<UINT>(mdlIndecies.size()) * static_cast<UINT>(sizeof(unsigned int));
			indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA indexSubresourceData{};
			indexSubresourceData.pSysMem = &mdlIndecies[0];

			ID3D11Buffer* indexBuffer;
			result = DX11::Device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			std::ifstream vsFile;
			vsFile.open("Shaders/FirstShader_vs.cso", std::ios::binary);
			std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
			ID3D11VertexShader* vertexShader;
			result = DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
			if (FAILED(result))
			{
				return false;
			}
			vsFile.close();

			std::ifstream psFile;
			psFile.open("Shaders/FirstShader_ps.cso", std::ios::binary);
			std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
			ID3D11PixelShader* pixelShader;
			result = DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
			if (FAILED(result))
			{
				return false;
			}
			psFile.close();

			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			ID3D11InputLayout* inputLayout;
			result = DX11::Device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
			if (FAILED(result))
			{
				return false;
			}

			Model::MeshData modelData = {};
			modelData.myMaterial = meshMaterial;
			modelData.myNumberOfVertecies = static_cast<UINT>(mdlVertecies.size());
			modelData.myNumberOfIndices = static_cast<UINT>(mdlIndecies.size());
			modelData.myStride = sizeof(Vertex);
			modelData.myOffset = 0;
			modelData.myVertexBuffer = vertexBuffer;
			modelData.myIndexBuffer = indexBuffer;
			modelData.myVertexShader = vertexShader;
			modelData.myPixelShader = pixelShader;
			modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			modelData.myInputLayout = inputLayout;
			mdlMeshData[i] = modelData;
		}

		std::shared_ptr<Model> model = std::make_shared<Model>();
		model->Init(mdlMeshData, someFilePath);
		myModelRegistry.insert({ someFilePath, model });
		return true;
	}

	return false;
}

bool ModelAssetHandler::Initialize()
{
	InitUnitCube();

	return false;
}

std::shared_ptr<Model> ModelAssetHandler::GetModel(const std::wstring& someFilePath)
{
	return myModelRegistry[someFilePath];
}

std::shared_ptr<ModelInstance> ModelAssetHandler::GetModelInstance(const std::wstring& aModelName)
{
	std::shared_ptr<ModelInstance> temp = std::make_shared<ModelInstance>();
	if (myModelRegistry.find(aModelName) == myModelRegistry.end())
	{
		LoadModel(aModelName);
	}
	temp->Init(GetModel(aModelName));
	return temp;
}
