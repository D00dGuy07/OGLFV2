#pragma once

#include "UI/Widget.h"

namespace UI
{
	class Frame : virtual public Widget
	{
	public:
		virtual const char* GetShaderPath() const override { return "res/WidgetShaders/Frame.shader"; };
		virtual int32_t BuildRenderMesh(Mesh& renderMesh, Shader& renderShader, glm::vec2 resolution) override;

		WidgetProperty<glm::vec3> Color = WidgetProperty<glm::vec3>(nullptr, 255.0f);
	protected:

		virtual void OnSetRootReferences(Surface* rootSurface) override;
	};
}