#pragma once

#include "UI/Widget.h"
#include "UI/Behaviors/IButton.h"
#include "UI/Widgets/Frame.h"

namespace UI
{
#pragma warning( push )
#pragma warning( disable: 4250 )
	class FrameButton : public IButton<Frame>
	{
	public:

		using Frame::GetShaderPath;

		virtual int32_t BuildRenderMesh(Mesh& renderMesh, Shader& renderShader, glm::vec2 resolution) override;

		WidgetProperty<glm::vec3> Color = WidgetProperty<glm::vec3>(nullptr, 255.0f);
		WidgetProperty<glm::vec3> HoverColor = WidgetProperty<glm::vec3>(nullptr, 200.0f);
		WidgetProperty<glm::vec3> ClickColor = WidgetProperty<glm::vec3>(nullptr, 150.0f);

		bool AutoColor = true;
	protected:

		virtual void OnSetRootReferences(Surface* rootSurface) override;
	};
#pragma warning( pop )
}