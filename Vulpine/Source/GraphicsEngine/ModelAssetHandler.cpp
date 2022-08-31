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

CommonUtilities::Matrix4x4f ConvertMatrix(TGA::Matrix& aTGAMAtrix)
{
	CommonUtilities::Matrix4x4f result;

	result(1, 1) = aTGAMAtrix.Data[0];
	result(1, 2) = aTGAMAtrix.Data[1];
	result(1, 3) = aTGAMAtrix.Data[2];
	result(1, 4) = aTGAMAtrix.Data[3];

	result(2, 1) = aTGAMAtrix.Data[4];
	result(2, 2) = aTGAMAtrix.Data[5];
	result(2, 3) = aTGAMAtrix.Data[6];
	result(2, 4) = aTGAMAtrix.Data[7];

	result(3, 1) = aTGAMAtrix.Data[8];
	result(3, 2) = aTGAMAtrix.Data[9];
	result(3, 3) = aTGAMAtrix.Data[10];
	result(3, 4) = aTGAMAtrix.Data[11];

	result(4, 1) = aTGAMAtrix.Data[12];
	result(4, 2) = aTGAMAtrix.Data[13];
	result(4, 3) = aTGAMAtrix.Data[14];
	result(4, 4) = aTGAMAtrix.Data[15];

	return result;
}

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

	std::wstring wideMatName = L"Default_M.dds";

	std::shared_ptr<Material> meshMaterial;
	if (myMaterialRegistry.find(wideMatName) != myMaterialRegistry.end())
	{
		meshMaterial = myMaterialRegistry[wideMatName];
	}
	else
	{
		meshMaterial = std::make_shared<Material>();
		meshMaterial->Init(wideMatName, { GetRand(), GetRand(), GetRand() });

		myMaterialRegistry.insert({ wideMatName, meshMaterial });
	}

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

		{"BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11InputLayout* inputLayout;
	result = DX11::Device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
	if (FAILED(result))
	{
		return false;
	}

	Model::MeshData modelData = {};
	modelData.myMaterial = meshMaterial;
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

		Model::Skeleton mdlSkeleton;
		const bool hasSkeleton = tgaModel.Skeleton.GetRoot();

		if (hasSkeleton)
		{
			mdlSkeleton.Bones.resize(tgaModel.Skeleton.Bones.size());
			mdlSkeleton.BoneName.resize(tgaModel.Skeleton.Bones.size());

			for (size_t i = 0; i < tgaModel.Skeleton.Bones.size(); i++)
			{
				mdlSkeleton.Bones[i].Parent = tgaModel.Skeleton.Bones[i].Parent;
				mdlSkeleton.Bones[i].Name = tgaModel.Skeleton.Bones[i].Name;
				mdlSkeleton.Bones[i].Children = tgaModel.Skeleton.Bones[i].Children;
				mdlSkeleton.BoneName[i] = tgaModel.Skeleton.Bones[i].Name;

				mdlSkeleton.Bones[i].BindPoseInverse(1, 1) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[0];
				mdlSkeleton.Bones[i].BindPoseInverse(1, 2) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[1];
				mdlSkeleton.Bones[i].BindPoseInverse(1, 3) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[2];
				mdlSkeleton.Bones[i].BindPoseInverse(1, 4) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[3];

				mdlSkeleton.Bones[i].BindPoseInverse(2, 1) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[4];
				mdlSkeleton.Bones[i].BindPoseInverse(2, 2) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[5];
				mdlSkeleton.Bones[i].BindPoseInverse(2, 3) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[6];
				mdlSkeleton.Bones[i].BindPoseInverse(2, 4) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[7];

				mdlSkeleton.Bones[i].BindPoseInverse(3, 1) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[8];
				mdlSkeleton.Bones[i].BindPoseInverse(3, 2) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[9];
				mdlSkeleton.Bones[i].BindPoseInverse(3, 3) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[10];
				mdlSkeleton.Bones[i].BindPoseInverse(3, 4) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[11];

				mdlSkeleton.Bones[i].BindPoseInverse(4, 1) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[12];
				mdlSkeleton.Bones[i].BindPoseInverse(4, 2) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[13];
				mdlSkeleton.Bones[i].BindPoseInverse(4, 3) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[14];
				mdlSkeleton.Bones[i].BindPoseInverse(4, 4) = tgaModel.Skeleton.Bones[i].BindPoseInverse.Data[15];
			}
		}

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

				{"BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
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
		if (hasSkeleton)
		{
			model->Init(mdlMeshData, someFilePath, mdlSkeleton);
		}
		else
		{
			model->Init(mdlMeshData, someFilePath);
		}
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
	if (myModelRegistry.find(someFilePath) == myModelRegistry.end())
	{
		LoadModel(someFilePath);
	}
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

bool ModelAssetHandler::LoadAnimation(const std::wstring& aModelName, const std::wstring& someFilePath)
{
	const std::string ansiFileName = std::string(someFilePath.begin(), someFilePath.end());

	std::shared_ptr<Model> model = GetModel(aModelName);
	TGA::FBXAnimation tgaAnimation;

	if (TGA::FBXImporter::LoadAnimation(ansiFileName, model->GetSkeleton()->BoneName, tgaAnimation))
	{
		Model::Animation result;

		result.Length = tgaAnimation.Length;
		result.Duration = tgaAnimation.Duration;
		result.FramesPerSecond = tgaAnimation.FramesPerSecond;
		result.Name = std::wstring(tgaAnimation.Name.begin(), tgaAnimation.Name.end());
		result.Frames.resize(tgaAnimation.Frames.size());

		for (size_t i = 0; i < result.Frames.size(); i++) 
		{
			for (size_t j = 0; j < tgaAnimation.Frames[i].LocalTransforms.size(); j++)
			{
				result.Frames[i].LocalTransforms.push_back(ConvertMatrix(tgaAnimation.Frames[i].LocalTransforms[j]));
			}
		}

		model->AddAnimation(result);
		return true;
	}

	return false;
}
