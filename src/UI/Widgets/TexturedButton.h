#pragma once

#include "UI/Widget.h"
#include "UI/Behaviors/IButton.h"
#include "UI/Widgets/TexturedFrame.h"

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