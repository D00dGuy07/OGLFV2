#include "oglfv2/Renderer/VertexBuffer.h"

#include "oglfv2/Renderer/Renderer.h"

#include <iostream>

VertexBuffer* VertexBuffer::m_BoundVertexBuffer = nullptr;

VertexBuffer::VertexBuffer(const void* data, size_t size)
	: GLBuffer()
{
	SetContents(data, size, Target::ArrayBuffer, Usage::StaticDraw);
}

VertexBuffer::~VertexBuffer()
{
	if (m_BoundVertexBuffer == this)
		m_BoundVertexBuffer = nullptr;

	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		glDeleteBuffers(1, &id);
	});
}

void VertexBuffer::Bind()
{
	if (m_BoundVertexBuffer == this)
		return;
	m_BoundVertexBuffer = this;

	Renderer::Submit([=]() {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	});
}