#pragma once

#include "glm/glm.hpp"

#include "Util/IValueObserver.h"

#include <string>

// Texture Atlas

struct TextureCoords
{
	glm::vec2 v0;
	glm::vec2 v1;
	glm::vec2 v2;
	glm::vec2 v3;
};

// (x, y) of (0, 0) gives bottom left corner. Width and Height are of the individual sprites
TextureCoords TextureAtlasCoords(float x, float y, float width, float height);

// File helpers

namespace FileHelpers
{
	// Uses posix stat function because it's fast I guess
	bool FileExists(const std::string& path);
}