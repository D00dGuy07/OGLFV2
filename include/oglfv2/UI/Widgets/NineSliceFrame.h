#pragma once

#include "oglfv2/UI/Widget.h"

#include "oglfv2/Renderer/Texture.h"

#include <memory>

namespace UI
{
	enum class NineSliceScaleMode
	{
		Stretch,
		Repeat
	};

	class NineSliceFrame : virtual public Widget
	{
	public:
		virtual const char* GetShaderPath() const override { return "res/WidgetShaders/NineSliceFrame.shader"; };
		virtual int32_t BuildRenderMesh(Mesh& renderMesh, Shader& renderShader, glm::vec2 resolution) override;

		WidgetProperty<std::shared_ptr<Texture>> TextureObject = WidgetProperty<std::shared_ptr<Texture>>(nullptr, nullptr);
		WidgetProperty<glm::vec4> BorderSizes = WidgetProperty<glm::vec4>(nullptr, 0.0f, 0.0f, 0.0f, 0.0f);
		WidgetProperty<float> Scale = WidgetProperty<float>(nullptr, 1.0f);
		WidgetProperty<NineSliceScaleMode> ScaleMode = WidgetProperty<NineSliceScaleMode>(nullptr, NineSliceScaleMode::Stretch);

	protected:

		virtual void OnSetRootReferences(Surface* rootSurface) override;
	};
}