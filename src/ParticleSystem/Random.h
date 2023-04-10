#pragma once
#include "Common.h"

#include <random>
#include <cstdlib>

namespace Particle2D
{

	class RandomHelper
	{
	public:
		template<typename T>
		static inline T random_real(T min, T max)
		{
			std::uniform_real_distribution<T> dist(min, max);
			auto &mt = RandomHelper::getEngine();
			return dist(mt);
		}

		template<typename T>
		static inline T random_int(T min, T max)
		{
			std::uniform_int_distribution<T> dist(min, max);
			auto &mt = RandomHelper::getEngine();
			return dist(mt);
		}
	private:
		static std::mt19937 &getEngine();
	};


	template<typename T>
	inline T random(T min, T max)
	{
		return RandomHelper::random_int<T>(min, max);
	}

	template<>
	inline float random(float min, float max)
	{
		return RandomHelper::random_real(min, max);
	}

	template<>
	inline long double random(long double min, long double max)
	{
		return RandomHelper::random_real(min, max);
	}

	template<>
	inline double random(double min, double max)
	{
		return RandomHelper::random_real(min, max);
	}

	/* 返回 0 到 RAND_MAX 随机浮点数 */
	inline int random()
	{
		return random(0, RAND_MAX);
	}

	/* 返回 -1 到 1 随机浮点数 */
	inline float rand_minus1_1()
	{
		return ((std::rand() / ( float ) RAND_MAX) * 2) - 1;
	}

	/* 返回 0 到 1 随机浮点数 */
	inline float rand_0_1()
	{
		return std::rand() / ( float ) RAND_MAX;
	}

	#define RANDOM_MINUS1_1() rand_minus1_1()

	#define RANDOM_0_1() rand_0_1()

	#define MAX(a, b) ((a) > (b) ? (a) : (b))
	#define MIN(a, b) ((a) < (b) ? (a) : (b))
	#define CLAMPF(a, mi, ma) (MIN(MAX(a, mi), ma))

}

