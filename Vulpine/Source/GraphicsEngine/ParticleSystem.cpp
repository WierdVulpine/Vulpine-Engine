#include "ParticleSystem.h"

void ParticleSystem::Update(float someDeltaTime)
{
	for (ParticleEmitter& emitter : myEmitters)
	{
		emitter.Update(someDeltaTime);
	}
}
