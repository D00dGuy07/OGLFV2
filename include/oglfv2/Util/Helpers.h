#pragma once

#include <string>


// File helpers

namespace FileHelpers
{
	// Uses posix stat function because it's fast I guess
	bool FileExists(const std::string& path);
}