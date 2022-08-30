#pragma once
#include <string>
#include <unordered_map>
#include <memory>

class Model;
class ModelInstance;
class Material;

class ModelAssetHandler
{
private:
	static inline std::unordered_map<std::wstring, std::shared_ptr<Model>> myModelRegistry;
	static inline std::unordered_map<std::wstring, std::shared_ptr<Material>> myMaterialRegistry;

	static bool InitUnitCube();

	static bool LoadModel(const std::wstring& someFilePath);

public:

	static bool Initialize();

	static std::shared_ptr<Model> GetModel(const std::wstring& someFilePath);
	static std::shared_ptr<ModelInstance> GetModelInstance(const std::wstring& aModelName);

};

