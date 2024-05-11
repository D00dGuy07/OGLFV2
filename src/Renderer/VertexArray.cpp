#include "oglfv2/Renderer/VertexArray.h"

#include "oglfv2/Renderer/VertexBufferLayout.h"
#include "oglfv2/Renderer/Renderer.h"

#include <iostream>

VertexArray* VertexArray::m_BoundVertexArray = nullptr;

VertexArray::VertexArray()
	: m_RendererID(NULL)
{
	Renderer::Submit([&]() {
		glGenVertexArrays(1, &m_RendererID);
	});
	
}

VertexArray::~VertexArray()
{
	if (m_BoundVertexArray == this)
		m_BoundVertexArray = nullptr;

	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		glDeleteVertexArrays(1, &id);
	});
}

void VertexArray::AddBuffer(VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	uint64_t offset = 0;
	for (uint32_t i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		uint32_t stride = layout.GetStride();
		Renderer::Submit([=]() {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type,
				element.normalized, stride, reinterpret_cast<const void*>(offset));
		});
		offset += static_cast<uint64_t>(element.count * VertexBufferElement::GetSizeOfType(element.type));
	}
}

void VertexArray::Bind()
{
	if (m_BoundVertexArray == this)
		return;
	m_BoundVertexArray = this;

	Renderer::Submit([=]() {
		glBindVertexArray(m_RendererID);
	});
}