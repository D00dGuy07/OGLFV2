#pragma once

#include "UI/Widget.h"
#include "UI/Behaviors/IButton.h"
#include "UI/Widgets/NineSliceFrame.h"

namespace UI
{
#pragma warning( push )
#pragma warning( disable: 4250 )
	class NineSliceButton : public IButton<NineSliceFrame>
	{
	public:

		using NineSliceFrame::GetShaderPath;
		using NineSliceFrame::BuildRenderMesh;

		using NineSliceFrame::TextureObject;
		using NineSliceFrame::BorderSizes;
		using NineSliceFrame::Scale;
		using NineSliceFrame::ScaleMode;

	protected:

		using NineSliceFrame::OnSetRootReferences;
	};
#pragma warning( pop )
}