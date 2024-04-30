#pragma once

#include "VertexBuffer.h"

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

	static VertexArray* m_BoundVertexArray;
};