#include "Util/Helpers.h"

#include <sys/stat.h>

// Texture Atlas

TextureCoords TextureAtlasCoords(float x, float y, float width, float height)
{
	return {
		glm::vec2(((x * width))        , ((y * width) + width)),
		glm::vec2(((x * width) + width), ((y * width) + width)),
		glm::vec2(((x * width) + width), (y * width)),
		glm::vec2(((x * width))        , (y * width))
	};
}

// File helpers

namespace FileHelpers
{
	bool FileExists(const std::string& path)
	{
		struct stat buffer;
		return (stat(path.c_str(), &buffer) == 0);
	}
}