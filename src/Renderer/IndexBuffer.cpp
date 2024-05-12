#include "oglfv2/Renderer/IndexBuffer.h"

#include "oglfv2/Renderer/Renderer.h"

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
	: GLBuffer()
{
	SetContents(data, static_cast<size_t>(count * sizeof(uint32_t)),
		Target::ElementArray, Usage::StaticDraw);
}

IndexBuffer::~IndexBuffer()
{
	if (m_BoundIndexBuffer == this)
		m_BoundIndexBuffer = nullptr;

	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		glDeleteBuffers(1, &id);
	});
}

void IndexBuffer::Bind()
{
	if (m_BoundIndexBuffer == this)
		return;
	m_BoundIndexBuffer = this;

	Renderer::Submit([=]() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	});
}
