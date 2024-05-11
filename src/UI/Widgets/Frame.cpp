#include "oglfv2/UI/Widgets/Frame.h"

using namespace UI;

int32_t Frame::BuildRenderMesh(Mesh& renderMesh, Shader& renderShader, glm::vec2 resolution)
{
	// Generate geometry info
	float vertices[] = {
		m_ScreenRect.x, m_ScreenRect.w,
		m_ScreenRect.z, m_ScreenRect.w,
		m_ScreenRect.z, m_ScreenRect.y,
		m_ScreenRect.x, m_ScreenRect.y
	};

	uint32_t indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	renderMesh.SetVertices(vertices, 8 * sizeof(float));
	renderMesh.SetIndices(indices, 6);
	renderMesh.BufferLayout.Push<float>(2);

	// Deal with other info for rendering
	renderShader.SetUniform3f("u_Color", Color / 255.0f);

	return 0; // Not instanced
}

void Frame::OnSetRootReferences(Surface* rootSurface)
{
	SetPropertyRootReference(Color, rootSurface);
}