#include "ParticleMemory.h"

namespace Particle2D
{
	std::vector<Particle*> ParticleMemory::vParticlePool;
	std::vector<Particle*> ParticleMemory::vUnusedParticleList;

	int  ParticleMemory::nFreeIndex = 0;
	bool ParticleMemory::bInit = false;


	void ParticleMemory::initParticleMemory(int size)
	{
		if ( bInit ) return;
		bInit = true;
		Particle* particle = nullptr;
		for ( int i = 0; i < size; i++ ) {
			particle = new Particle;
			vParticlePool.push_back(particle);
			vUnusedParticleList.push_back(particle);
		}
	}

	void ParticleMemory::freeParticleMemory()
	{
		for ( auto& particle : vParticlePool ) {
			delete particle;
		}
		vParticlePool.clear();
		vUnusedParticleList.clear();
	}

	Particle* ParticleMemory::allocParticle()
	{
		if ( (nFreeIndex >= vParticlePool.size() - 1) ) {
			return nullptr;
		}
		else {
			return vUnusedParticleList[nFreeIndex++];
		//	iout = nFreeIndex;//debug
		}
	}

	void ParticleMemory::freeParticle(Particle* particle)
	{
		assert(nFreeIndex != 0);
		vUnusedParticleList[--nFreeIndex] = particle;
	//	iout = nFreeIndex;//debug
	}
}