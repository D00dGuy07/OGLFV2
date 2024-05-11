#pragma once

#include "oglfv2/UI/Widget.h"

#include "oglfv2/Renderer/Texture.h"

#include <memory>

namespace UI
{
	enum class TextureScaleType
	{
		Repeat,
		Stretch,
		Fit
	};

	class TexturedFrame : virtual public Widget
	{
	public:

		virtual const char* GetShaderPath() const override { return "res/WidgetShaders/TexturedFrame.shader"; };
		virtual int32_t BuildRenderMesh(Mesh& renderMesh, Shader& renderShader, glm::vec2 resolution) override;

		WidgetProperty<std::shared_ptr<Texture>> TextureObject = WidgetProperty<std::shared_ptr<Texture>>(nullptr, nullptr);
		WidgetProperty<TextureScaleType> ScaleType = WidgetProperty<TextureScaleType>(nullptr, TextureScaleType::Stretch);

	protected:

		virtual void OnSetRootReferences(Surface* rootSurface) override;
	};
}