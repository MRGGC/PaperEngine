#pragma once

#include <random>

namespace Paper
{
	class Random
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static float Float()
		{
			float r1 = (float) s_Distribution(s_RandomEngine);
			float r2 = (float) std::numeric_limits<uint64_t>::max();
			return r1 / r2;
		}

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

	std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;
}