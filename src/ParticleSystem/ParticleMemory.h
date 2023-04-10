#pragma once
#include "Particle.h"

#include <vector>

namespace Particle2D
{
	class ParticleMemory
	{
	public:
		static void initParticleMemory(int size);
		static void freeParticleMemory();

		static Particle* allocParticle();
		static void freeParticle(Particle* particle);

	private:
		static std::vector<Particle*> vParticlePool;
		static std::vector<Particle*> vUnusedParticleList;

		static int	nFreeIndex;
		static bool bInit;
	};
}