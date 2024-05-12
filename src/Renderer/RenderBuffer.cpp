#include "oglfv2/Renderer/RenderBuffer.h"

#include "oglfv2/Renderer/Renderer.h"

#include "glad/glad.h"

RenderBuffer::RenderBuffer(int32_t width, int32_t height, InternalImageFormat format)
	: ImageBuffer(NULL, width, height), m_Format(format)
{
	int32_t sizedId = 0, baseId = 0;
	ImageBuffer::GetFormatData(m_Format, sizedId, baseId);
	Renderer::Submit([&, sizedId]() {
		glGenRenderbuffers(1, &m_RendererID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);

		glRenderbufferStorage(GL_RENDERBUFFER, sizedId, m_Width, m_Height);
	});
}

RenderBuffer::~RenderBuffer()
{
	if (m_BoundRenderBuffer == this)
		m_BoundRenderBuffer = nullptr;

	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		glDeleteRenderbuffers(1, &id);
	});
}

void RenderBuffer::Bind()
{
	if (m_BoundRenderBuffer == this)
		return;
	m_BoundRenderBuffer = this;

	Renderer::Submit([=]() {
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
	});
}

void RenderBuffer::Reallocate(int32_t width, int32_t height)
{
	m_Width = width;
	m_Height = height;

	int32_t sizedId = 0, baseId = 0;
	ImageBuffer::GetFormatData(m_Format, sizedId, baseId);

	Bind();
	Renderer::Submit([=]() {
		glRenderbufferStorage(GL_RENDERBUFFER, sizedId, m_Width, m_Height);
	});
}