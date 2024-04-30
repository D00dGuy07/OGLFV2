#include "UI/Widgets/TexturedFrame.h"

#include <iostream>

using namespace UI;

int32_t TexturedFrame::BuildRenderMesh(Mesh& renderMesh, Shader& renderShader, glm::vec2 resolution)
{
	// Can't render the texture if there is none
	if (!TextureObject)
		return -1; // Discard this widget

	// Calculate geometry info
	glm::vec2 sizeExact = m_AbsoluteSize * resolution;

	float textureWidth = static_cast<float>(TextureObject.Value()->GetWidth());
	float textureHeight = static_cast<float>(TextureObject.Value()->GetHeight());

	switch (ScaleType)
	{
	case TextureScaleType::Repeat:
	{
		float vertices[] = {
			m_ScreenRect.x, m_ScreenRect.w, 0.0f,						sizeExact.y / textureHeight,
			m_ScreenRect.z, m_ScreenRect.w, sizeExact.x / textureWidth, sizeExact.y / textureHeight,
			m_ScreenRect.z, m_ScreenRect.y, sizeExact.x / textureWidth, 0.0f,
			m_ScreenRect.x, m_ScreenRect.y, 0.0f,						0.0f
		};
		renderMesh.SetVertices(vertices, 16 * sizeof(float));

		break;
	}
	case TextureScaleType::Stretch:
	{
		float vertices[] = {
			m_ScreenRect.x, m_ScreenRect.w, 0.0f, 1.0f,
			m_ScreenRect.z, m_ScreenRect.w, 1.0f, 1.0f,
			m_ScreenRect.z, m_ScreenRect.y, 1.0f, 0.0f,
			m_ScreenRect.x, m_ScreenRect.y, 0.0f, 0.0f
		};
		renderMesh.SetVertices(vertices, 16 * sizeof(float));

		break;
	}
	case TextureScaleType::Fit:
	{
		if (sizeExact.y / sizeExact.x > textureHeight / textureWidth)
		{
			float padding = (sizeExact.y - (sizeExact.x * (textureHeight / textureWidth))) / sizeExact.y;
			float timesFit = 1.0f / (1.0f - padding);
			float adjustAmount = (timesFit - 1.0f) / 2.0f;

			float vertices[] = {
				m_ScreenRect.x, m_ScreenRect.w, 0.0f, 1.0f + adjustAmount,
				m_ScreenRect.z, m_ScreenRect.w, 1.0f, 1.0f + adjustAmount,
				m_ScreenRect.z, m_ScreenRect.y, 1.0f, -adjustAmount,
				m_ScreenRect.x, m_ScreenRect.y, 0.0f, -adjustAmount
			};
			renderMesh.SetVertices(vertices, 16 * sizeof(float));
		}
		else
		{
			float padding = (sizeExact.x - (sizeExact.y * (textureWidth / textureHeight))) / sizeExact.x;
			float timesFit = 1.0f / (1.0f - padding);
			float adjustAmount = (timesFit - 1.0f) / 2.0f;

			float vertices[] = {
				m_ScreenRect.x, m_ScreenRect.w, -adjustAmount,		 1.0f,
				m_ScreenRect.z, m_ScreenRect.w, 1.0f + adjustAmount, 1.0f,
				m_ScreenRect.z, m_ScreenRect.y, 1.0f + adjustAmount, 0.0f,
				m_ScreenRect.x, m_ScreenRect.y, -adjustAmount,		 0.0f
			};
			renderMesh.SetVertices(vertices, 16 * sizeof(float));
		}

		break;
	}
	default:
		return -1; // Discard this widget
	}

	uint32_t indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	renderMesh.SetIndices(indices, 6);

	renderMesh.BufferLayout.Push<float>(2);
	renderMesh.BufferLayout.Push<float>(2);

	// Setup other state
	TextureObject.Value()->Bind();
	renderShader.SetUniform1i("u_Texture", 0);

	return 0; // Render not instanced
}

void TexturedFrame::OnSetRootReferences(Surface* rootSurface)
{
	SetPropertyRootReference(TextureObject, rootSurface);
	SetPropertyRootReference(ScaleType, rootSurface);
}