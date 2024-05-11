#pragma once

#include "oglfv2/UI/Widget.h"
#include "oglfv2/UI/Behaviors/IButton.h"
#include "oglfv2/UI/Widgets/TexturedFrame.h"

namespace UI
{
#pragma warning( push )
#pragma warning( disable: 4250 )
	class TexturedButton : public IButton<TexturedFrame>
	{
	public:

		using TexturedFrame::GetShaderPath;
		using TexturedFrame::BuildRenderMesh;

		using TexturedFrame::TextureObject;
		using TexturedFrame::ScaleType;

	protected:

		using TexturedFrame::OnSetRootReferences;
	};
#pragma warning( pop )
}