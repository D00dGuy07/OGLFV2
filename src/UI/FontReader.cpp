#include "oglfv2/UI/FontReader.h"

#include <fstream>
#include <ios>

#include <vector>
#include <array>

using namespace UI;

bool getBit(uint8_t byte, int loc)
{
	return static_cast<bool>((byte >> loc) & 0b00000001);
}

std::array<bool, 8> getBits(uint8_t byte)
{
	std::array<bool, 8> bits = {
		getBit(byte, 0),
		getBit(byte, 1),
		getBit(byte, 2),
		getBit(byte, 3),
		getBit(byte, 4),
		getBit(byte, 5),
		getBit(byte, 6),
		getBit(byte, 7)
	};
	return bits;
}

uint8_t getFirstByteMask(int numBytes)
{
	switch (numBytes)
	{
	case 1:
		return 0b11111111;
	case 2:
		return 0b00011111;
	case 3:
		return 0b00001111;
	case 4:
		return 0b00000111;
	default:
		return 0b11111111;
		break;
	}
}

// I would imagine that this is the worst UTF-8 implementation ever
uint32_t readUTF8(char*& pointer)
{
	// Get number of bytes
	int numBytes = 0;
	{
		std::array<bool, 8> firstByte = getBits(*pointer);

		for (int i = 7; i >= 0; i--)
		{
			if (firstByte[i] == false)
				break;
			else
				numBytes++;
		}

		if (numBytes == 0)
			numBytes = 1;
	}

	// Get bytes
	std::vector<uint8_t> bytes = std::vector<uint8_t>();
	for (int i = 0; i < numBytes; i++)
	{
		bytes.emplace_back(*(uint8_t*)pointer);
		pointer++;
	}

	// Verify bytes

	if (numBytes > 1)
	{
		for (int i = 1; i < numBytes; i++)
		{
			std::array<bool, 8> bits = getBits(bytes[i]);

			if (!(bits[7] == true && bits[6] == false))
				return NULL;
		}
	}
	
	// Decode character
	int32_t character = 0;

	int32_t firstByte = bytes[0] & getFirstByteMask(numBytes);
	character |= firstByte << (6 * (numBytes - 1));

	if (numBytes > 1)
	{
		for (int i = 1; i < numBytes; i++)
		{
			int32_t byte = bytes[i] & 0b00111111;
			character |= byte << (6 * ((numBytes - 1) - i));
		}
	}

	return character;
}

std::unordered_map<wchar_t, RawCharacterData> FontReader::ReadFont(const std::string& path)
{
	auto characterData = std::unordered_map<wchar_t, RawCharacterData>();

	std::ifstream file(path, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);
	if (file.good() && file.read(buffer.data(), size))
	{
		char* pointer = buffer.data();

		int8_t id = *pointer;
		pointer += sizeof(int8_t);

		if (id == 69)
		{
			int count = *pointer;
			pointer += sizeof(int32_t);

			for (int i = 0; i < count; i++)
			{
				wchar_t character = (wchar_t)readUTF8(pointer);

				RawCharacterData charData = RawCharacterData();
				charData.X = *pointer;
				pointer += sizeof(int32_t);
				charData.Y = *pointer;
				pointer += sizeof(int32_t);
				charData.Width = *pointer;
				pointer += sizeof(int32_t);
				charData.Height = *pointer;
				pointer += sizeof(int32_t);

				characterData[character] = charData;
			}
		}
	}

	file.close();

	return characterData;
}