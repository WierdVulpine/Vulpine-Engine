#pragma once
#include "ParticleSystem.h"
#include <unordered_map>

class ParticleAssetHandler
{
	typedef std::unordered_map<std::wstring, ParticleEmitterTemplate> EmitterTemplateMap;
	typedef std::unordered_map<std::wstring, ParticleSystem> SystemTemplateMap;

	inline static EmitterTemplateMap myEmitterTemplates{};
	inline static SystemTemplateMap mySystemTemplates{};

public:

	//static bool LoadSystemTemplate(const std::wstring& someFilePath);
	static bool LoadEmitterTemplate(const std::wstring& someFilePath);
	static std::shared_ptr<ParticleSystem> GetParticleSystem(const std::wstring& aSystemName);
};

