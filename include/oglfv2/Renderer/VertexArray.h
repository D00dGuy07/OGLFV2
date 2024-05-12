#pragma once

#include "oglfv2/Renderer/VertexBuffer.h"

#include <cstdint>

class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind();
private:
	uint32_t m_RendererID;

	inline static VertexArray* m_BoundVertexArray = nullptr;
};