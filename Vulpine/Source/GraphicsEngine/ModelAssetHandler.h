#pragma once
#include <string>
#include <unordered_map>
#include <memory>

class Model;

class ModelAssetHandler
{
private:
	static std::unordered_map<std::wstring, std::shared_ptr<Model>> myModelRegistry;

	static bool InitUnitCube();

public:

	static bool Initialize();

	static std::shared_ptr<Model> GetModel(const std::wstring& someFilePath);
};

