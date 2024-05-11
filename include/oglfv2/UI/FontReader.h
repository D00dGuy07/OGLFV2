#pragma once

#include <unordered_map>
#include <string>

namespace UI
{
	struct RawCharacterData
	{
		int32_t X;
		int32_t Y;
		int32_t Width;
		int32_t Height;
	};

	class FontReader
	{
	public:
		static std::unordered_map<wchar_t, RawCharacterData> ReadFont(const std::string& path);
	};
}