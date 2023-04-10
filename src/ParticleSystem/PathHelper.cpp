#include "PathHelper.h"

//#include <Windows.h>

namespace Particle2D
{
	PathHelper PathHelper::pathHelper;
	PathHelper *pPathHelper;

	PathHelper::PathHelper()
	{
		pPathHelper = this;

		std::string sFullpath;
		sFullpath.resize(1024);

//		GetModuleFileNameA(nullptr, &sFullpath[0], sFullpath.size());

		sFullpath[sFullpath.size() - 1] = '\0';
		sFullpath = sFullpath.substr(0, sFullpath.find_last_of('\\'));
		sFullpath = sFullpath.substr(0, sFullpath.find_last_of('\\'));

		char* ch = &sFullpath[0];
		while ( (*ch) != '\0' ) {
			if ( (*ch) == '\\' ) {
				*ch = '/';
			}
			ch++;
		}

		sAppPath = sFullpath;
		sAssertName = "Assert";
		sAssertPath = sAppPath + '/' + sAssertName;
	}

	std::string PathHelper::fullPath(const std::string& filename)
	{
		return pPathHelper->sAssertPath + "/" + filename;
	}
}