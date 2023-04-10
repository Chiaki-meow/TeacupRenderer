#pragma once
#include "Common.h"

#include <string>

namespace Particle2D
{
	class PathHelper
	{
	public:
		PathHelper();

		static std::string fullPath(const std::string& filename);

	private:
		static PathHelper pathHelper;

		std::string sAppPath;
		std::string sAssertName;
		std::string sAssertPath;
	};
}