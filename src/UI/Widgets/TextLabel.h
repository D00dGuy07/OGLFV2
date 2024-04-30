#pragma once

#include "UI/Widget.h"
#include "UI/FontReader.h"

#include <unordered_map>

namespace UI
{
	class FontData
	{
	public:
		FontData(const std::string& fontName);

		std::unordered_map<wchar_t, RawCharacterData> CharacterData;
		std::unique_ptr<Texture> FontAtlas;
		bool IsValid;
	};

	enum class TextAlignX
	{
		Left,
		Center,
		Right
	};

	enum class TextAlignY
	{
		Top,
		Center,
		Bottom
	};

	class TextLabel : virtual public Widget
	{
	public:
		virtual const char* GetShaderPath() const override { return "res/WidgetShaders/TextLabel.shader"; };
		virtual int32_t BuildRenderMesh(Mesh& renderMesh, Shader& renderShader, glm::vec2 resolution) override;

		WidgetProperty<glm::vec3> Color = WidgetProperty<glm::vec3>(nullptr, 255.0f);
		WidgetProperty<float> Scale = WidgetProperty<float>(nullptr, 10.0f);
		WidgetProperty<std::wstring> Text = WidgetProperty<std::wstring>(nullptr, L"Default Text");
		WidgetProperty<std::string> Font = WidgetProperty<std::string>(nullptr, "default-font");
		WidgetProperty<TextAlignX> HorizontalAlignment = WidgetProperty<TextAlignX>(nullptr, TextAlignX::Center);
		WidgetProperty<TextAlignY> VerticalAlignment = WidgetProperty<TextAlignY>(nullptr, TextAlignY::Center);

		inline void SetText(const std::string& text) { Text = std::wstring(text.begin(), text.end()); }

	protected:

		virtual void OnSetRootReferences(Surface* rootSurface) override;

		static bool LoadFont(const std::string& font);

		void DrawCharacter(wchar_t character, glm::vec2 cursor,
			std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& indices, 
			std::shared_ptr<FontData> fontData);

		int32_t GetSubstrWidth(const std::wstring& text, int32_t start, int32_t end, std::shared_ptr<FontData> fontData);
		glm::vec2 GetLineStart(int32_t lineNumber, int32_t lineWidth, int32_t totalHeight, glm::vec4 screenRect);

		struct LineInfo { int32_t Start, End, PixelWidth; };
		std::vector<LineInfo> ParseLineData(
			const std::wstring& text,
			int32_t boundsWidth,
			std::shared_ptr<FontData> fontData);

		static std::unordered_map<std::string, std::shared_ptr<FontData>> m_FontData;
	};
}