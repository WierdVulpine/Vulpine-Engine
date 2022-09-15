#pragma once
#include "SceneObject.h"
#include <vector>
#include "ParticleEmitter.h"

struct ParticleSystemTemplate
{
	std::wstring Path;
	std::vector<std::wstring> EmitterFields;
};

struct ParticleSystemEmitter
{
	ParticleEmitter Emitter;
	EmitterSettingsData Settings;
	CommonUtilities::Transform Transform;
};

class ParticleSystem : public SceneObject
{
	friend class ParticleAssetHandler;
	std::vector<ParticleEmitter> myEmitters;

public:

	void Update(float someDeltaTime);

	FORCEINLINE const std::vector<ParticleEmitter>& GetEmitters() const { return myEmitters; }
};

