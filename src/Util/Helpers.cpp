#include "oglfv2/Util/Helpers.h"

#include <sys/stat.h>

// File helpers

namespace FileHelpers
{
	bool FileExists(const std::string& path)
	{
		struct stat buffer;
		return (stat(path.c_str(), &buffer) == 0);
	}
}