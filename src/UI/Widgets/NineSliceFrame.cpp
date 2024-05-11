#include "oglfv2/UI/Widgets/NineSliceFrame.h"

#include "oglfv2/Renderer/Renderer.h"
#include "oglfv2/Renderer/UniformBuffer.h"

#include <array>

using namespace UI;

int32_t NineSliceFrame::BuildRenderMesh(Mesh& renderMesh, Shader& renderShader, glm::vec2 resolution)
{
	// Can't render the texture if there is none
	if (!TextureObject)
		return -1; // Discard this widget

	// Generate geometry info
	float vertices[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	uint32_t indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	renderMesh.SetVertices(vertices, 8 * sizeof(float));
	renderMesh.SetIndices(indices, 6);
	renderMesh.BufferLayout.Push<float>(2);

	// Set shader data
	std::shared_ptr<Texture> texture = TextureObject.Value();
	glm::vec2 textureRes = glm::vec2(texture->GetWidth(), texture->GetHeight());
	texture->Bind();

	renderShader.SetUniform1i("u_Texture", 0);
	renderShader.SetUniform1i("u_UseFetch", static_cast<int32_t>(ScaleMode == NineSliceScaleMode::Repeat));
	renderShader.SetUniform2f("u_TextureResolution", textureRes);

	// Calculate instancing info

	glm::vec2 totalSize = glm::vec2(m_ScreenRect.z - m_ScreenRect.x, m_ScreenRect.w - m_ScreenRect.y);
	glm::vec4 scaledBorder = BorderSizes * Scale.Value();
	glm::vec2 centerSize = totalSize - glm::vec2(scaledBorder.x + scaledBorder.z, scaledBorder.y + scaledBorder.w);

	glm::vec2 textureCenter = textureRes - glm::vec2(BorderSizes->x + BorderSizes->z, BorderSizes->y + BorderSizes->w);
	glm::vec2 centerRange = (ScaleMode == NineSliceScaleMode::Repeat) ? centerSize / textureCenter / Scale.Value() : glm::vec2(1.0f, 1.0f);

	std::array<glm::vec4, 27> instanceData = {
		// Top Row
		glm::vec4(m_ScreenRect.x, m_ScreenRect.y, scaledBorder.x, scaledBorder.y), // Screen position, width and height
		glm::vec4(0.0f, 0.0f, BorderSizes->x, BorderSizes->y), // Texture position, width and height
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // Sample range for repeating

		glm::vec4(m_ScreenRect.x + scaledBorder.x, m_ScreenRect.y, centerSize.x, scaledBorder.y),
		glm::vec4(BorderSizes->x, 0.0f, textureCenter.x, BorderSizes->y),
		glm::vec4(0.0f, centerRange.x, 0.0f, 1.0f),

		glm::vec4(m_ScreenRect.z - scaledBorder.z, m_ScreenRect.y, scaledBorder.z, scaledBorder.y),
		glm::vec4(textureRes.x - BorderSizes->z, 0.0f, BorderSizes->z, BorderSizes->y),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),

		// Middle Row
		glm::vec4(m_ScreenRect.x, m_ScreenRect.y + scaledBorder.y, scaledBorder.x, centerSize.y),
		glm::vec4(0.0f, BorderSizes->y, BorderSizes->x, textureCenter.y),
		glm::vec4(0.0f, 1.0f, 0.0f, centerRange.y),

		glm::vec4(m_ScreenRect.x + scaledBorder.x, m_ScreenRect.y + scaledBorder.y, centerSize),
		glm::vec4(BorderSizes->x, BorderSizes->y, textureCenter),
		glm::vec4(0.0f, centerRange.x, 0.0f, centerRange.y),

		glm::vec4(m_ScreenRect.z - scaledBorder.z, m_ScreenRect.y + scaledBorder.y, scaledBorder.z, centerSize.y),
		glm::vec4(textureRes.x - BorderSizes->z, BorderSizes->y, BorderSizes->z, textureCenter.y),
		glm::vec4(0.0f, 1.0f, 0.0f, centerRange.y),

		// Bottom Row
		glm::vec4(m_ScreenRect.x, m_ScreenRect.w - scaledBorder.w, scaledBorder.x, scaledBorder.w),
		glm::vec4(0.0f, textureRes.y - BorderSizes->w, BorderSizes->x, BorderSizes->w),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),

		glm::vec4(m_ScreenRect.x + scaledBorder.x, m_ScreenRect.w - scaledBorder.w, centerSize.x, scaledBorder.w),
		glm::vec4(BorderSizes->x, textureRes.y - BorderSizes->w, textureCenter.x, BorderSizes->w),
		glm::vec4(0.0f, centerRange.x, 0.0f, 1.0f),

		glm::vec4(m_ScreenRect.z - scaledBorder.z, m_ScreenRect.w - scaledBorder.w, scaledBorder.z, scaledBorder.w),
		glm::vec4(textureRes.x - BorderSizes->z, textureRes.y - BorderSizes->w, BorderSizes->z, BorderSizes->w),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
	};

	Renderer::CreateGarbage<UniformBuffer>(instanceData.data(), 27 * sizeof(glm::vec4))->BindIndexed();

	return 9; // Instanced render
}

void NineSliceFrame::OnSetRootReferences(Surface* rootSurface)
{
	SetPropertyRootReference(TextureObject, rootSurface);
	SetPropertyRootReference(BorderSizes, rootSurface);
	SetPropertyRootReference(Scale, rootSurface);
	SetPropertyRootReference(ScaleMode, rootSurface);
}