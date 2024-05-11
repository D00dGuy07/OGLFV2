#include "oglfv2/UI/Widgets/FrameButton.h"

using namespace UI;

int32_t FrameButton::BuildRenderMesh(Mesh& renderMesh, Shader& renderShader, glm::vec2 resolution)
{
	Frame::Color = AutoColor ? (m_IsClicked ? ClickColor.Value() : (m_IsHovered ? HoverColor.Value() : Color.Value())) : Color.Value();
	return Frame::BuildRenderMesh(renderMesh, renderShader, resolution);
}

void FrameButton::OnSetRootReferences(Surface* rootSurface)
{
	SetPropertyRootReference(Color, rootSurface);
	SetPropertyRootReference(HoverColor, rootSurface);
	SetPropertyRootReference(ClickColor, rootSurface);
	Frame::OnSetRootReferences(rootSurface);
}