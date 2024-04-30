#include "UI/Widgets/TextLabel.h"

#include "Util/Helpers.h"

#include <sstream>

using namespace UI;

std::unordered_map<std::string, std::shared_ptr<FontData>> TextLabel::m_FontData = 
	std::unordered_map<std::string, std::shared_ptr<FontData>>();


FontData::FontData(const std::string& fontName)
	: IsValid(true)
{
	std::stringstream dataPath;
	dataPath << "res/Fonts/" << fontName << ".mfont";
	std::stringstream atlasPath;
	atlasPath << "res/Fonts/" << fontName << ".png";

	if (FileHelpers::FileExists(dataPath.str()))
		CharacterData = FontReader::ReadFont(dataPath.str());
	else
		IsValid = false;

	TextureSpec spec;
	spec.MagFilter = TextureMagFilter::Nearest;

	if (FileHelpers::FileExists(atlasPath.str()))
		FontAtlas = std::make_unique<Texture>(atlasPath.str(), spec);
	else
		IsValid = false;
}

bool TextLabel::LoadFont(const std::string& font)
{
	if (m_FontData.find(font) != m_FontData.end())
		return true;

	std::shared_ptr<FontData> data = std::make_shared<FontData>(font);
	if (data->IsValid)
		m_FontData[font] = data;
	return data->IsValid;
}

void UI::TextLabel::DrawCharacter(wchar_t character, glm::vec2 cursor, 
	std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& indices, 
	std::shared_ptr<FontData> fontData)
{
	int32_t startIndex = static_cast<int32_t>(vertices.size());

	RawCharacterData characterData = fontData->CharacterData[character];
	glm::vec4 characterBounds = glm::vec4(
		cursor,
		cursor + glm::vec2(characterData.Width, characterData.Height) * Scale.Value()
	);

	vertices.emplace_back(
		characterBounds.x, characterBounds.w,
		static_cast<float>(characterData.X),
		static_cast<float>(characterData.Y + characterData.Height)
	);
	vertices.emplace_back(
		characterBounds.z, characterBounds.w,
		static_cast<float>(characterData.X + characterData.Width),
		static_cast<float>(characterData.Y + characterData.Height)
	);
	vertices.emplace_back(
		characterBounds.z, characterBounds.y,
		static_cast<float>(characterData.X + characterData.Width),
		static_cast<float>(characterData.Y)
	);
	vertices.emplace_back(
		characterBounds.x, characterBounds.y,
		static_cast<float>(characterData.X),
		static_cast<float>(characterData.Y)
	);

	indices.emplace_back(startIndex,	 startIndex + 1, startIndex + 2);
	indices.emplace_back(startIndex + 2, startIndex + 3, startIndex);
}

std::vector<std::pair<int32_t, int32_t>> ParseWordData(const std::wstring& text)
{
	int32_t wordStartIndex = -1;

	auto wordData = std::vector<std::pair<int32_t, int32_t>>();
	for (int32_t i = 0; i < text.length(); i++)
	{
		if (wordStartIndex == -1)
		{
			if (text[i] != L' ')
				wordStartIndex = i;
		}
		else if (text[i] == L' ')
		{
			wordData.emplace_back(wordStartIndex, i);
			wordStartIndex = -1;
		}
	}
	if (wordStartIndex != -1)
		wordData.emplace_back(wordStartIndex, static_cast<int32_t>(text.length()));

	return wordData;
}

int32_t TextLabel::GetSubstrWidth(const std::wstring& text, int32_t start, int32_t end, std::shared_ptr<FontData> fontData)
{
	int32_t width = 0;
	for (int32_t i = start; i < end; i++)
	{
		if (text[i] == L' ')
			width += static_cast<int32_t>(3.0f * Scale);


		auto charData = fontData->CharacterData.find(text[i]);
		if (charData != fontData->CharacterData.end())
			width += static_cast<int32_t>(static_cast<float>((*charData).second.Width + 1) * Scale);
	}

	return width;
}

glm::vec2 TextLabel::GetLineStart(int32_t lineNumber, int32_t lineWidth, int32_t totalHeight, glm::vec4 screenRect)
{
	float x = 0.0f, y = 0.0f;

	switch (HorizontalAlignment.Value())
	{
	case TextAlignX::Left:
		x = screenRect.x;
		break;
	case TextAlignX::Center:
		x = screenRect.x + (screenRect.z - screenRect.x - static_cast<float>(lineWidth) + 1.0f) / 2.0f;
		break;
	case TextAlignX::Right:
		x = screenRect.z - static_cast<float>(lineWidth);
		break;
	default:
		break;
	}

	switch (VerticalAlignment.Value())
	{
	case TextAlignY::Top:
		y = screenRect.y + lineNumber * 9 * Scale.Value();
		break;
	case TextAlignY::Center:
		y = (screenRect.y + screenRect.w) / 2.0f - static_cast<float>(totalHeight) / 2.0f + lineNumber * 9 * Scale.Value();
		break;
	case TextAlignY::Bottom:
		y = screenRect.w - static_cast<float>(totalHeight) + lineNumber * 9 * Scale.Value();
		break;
	default:
		break;
	}

	return glm::vec2(x, y);
}

std::vector<TextLabel::LineInfo> TextLabel::ParseLineData(
	const std::wstring& text,
	int32_t boundsWidth,
	std::shared_ptr<FontData> fontData)
{
	auto wordData = ParseWordData(text);

	auto lineData = std::vector<LineInfo>();

	int32_t lineStart = 0;
	int32_t lineEnd = 0;
	int32_t lineWidth = 0;
	for (auto& [start, end] : wordData)
	{
		if (lineWidth == 0)
		{
			lineEnd = end;
			lineWidth = GetSubstrWidth(text, lineStart, lineEnd, fontData);
			continue;
		}

		int32_t segmentWidth = GetSubstrWidth(text, lineEnd, end, fontData);
		if (lineWidth + segmentWidth <= boundsWidth)
		{
			lineEnd = end;
			lineWidth += segmentWidth;
		}
		else
		{
			lineData.push_back({ lineStart, lineEnd, lineWidth });
			lineStart = start;
			lineEnd = end;
			lineWidth = GetSubstrWidth(text, lineStart, lineEnd, fontData);
		}
	}
	lineData.push_back({ lineStart, lineEnd, lineWidth });

	return lineData;
}

int32_t TextLabel::BuildRenderMesh(Mesh& renderMesh, Shader& renderShader, glm::vec2 resolution)
{
	if (!LoadFont(Font))
		return -1; // Discard this widget

	std::shared_ptr<FontData> fontData = m_FontData[Font];

	int32_t boundsWidth = static_cast<int32_t>(m_ScreenRect.z - m_ScreenRect.x + 1.0f);

	std::vector<glm::vec4> vertices = std::vector<glm::vec4>();
	std::vector<glm::uvec3> indices = std::vector<glm::uvec3>();

	glm::vec2 cursor = glm::vec2(0, m_ScreenRect.y);
	auto lineData = ParseLineData(Text.Value(), boundsWidth, fontData);
	int32_t totalHeight = static_cast<int32_t>(lineData.size()) * 9 * static_cast<int32_t>(Scale);

	for (int32_t i = 0; i < lineData.size(); i++)
	{
		auto& line = lineData[i];
		glm::vec2 cursor = GetLineStart(i, line.PixelWidth, totalHeight, m_ScreenRect);

		for (int32_t i = line.Start; i < line.End; i++)
		{
			wchar_t character = Text.Value()[i];

			if (character == L' ')
				cursor.x += 3.0f * Scale;

			if (fontData->CharacterData.find(character) != fontData->CharacterData.end())
			{
				DrawCharacter(character, cursor, vertices, indices, fontData);
				cursor.x += (fontData->CharacterData[character].Width + 1.0f) * Scale;
			}
		}
	}

	renderMesh.SetVertices(reinterpret_cast<float*>(vertices.data()), static_cast<uint32_t>(vertices.size()) * 4 * sizeof(float));
	renderMesh.SetIndices(reinterpret_cast<uint32_t*>(indices.data()), static_cast<uint32_t>(indices.size()) * 3);
	renderMesh.BufferLayout.Push<float>(2);
	renderMesh.BufferLayout.Push<float>(2);

	fontData->FontAtlas->Bind();
	renderShader.SetUniform1i("u_TextureAtlas", 0);
	renderShader.SetUniform3f("u_Color", Color / 255.0f);

	return 0; // Render not instanced
}

void TextLabel::OnSetRootReferences(Surface* rootSurface)
{
	SetPropertyRootReference(Color, rootSurface);
	SetPropertyRootReference(Scale, rootSurface);
	SetPropertyRootReference(Text, rootSurface);
	SetPropertyRootReference(Font, rootSurface);
	SetPropertyRootReference(HorizontalAlignment, rootSurface);
	SetPropertyRootReference(VerticalAlignment, rootSurface);
}