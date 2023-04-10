#include "Random.h"

namespace Particle2D
{

	std::mt19937 &RandomHelper::getEngine()
	{
		static std::random_device seed_gen;
		static std::mt19937 engine(seed_gen());
		return engine;
	}

}