#pragma once

#include "Renderer/GLBuffer.h"

class VertexBuffer : public GLBuffer
{
public:
	VertexBuffer(const void* data, size_t size);
	virtual ~VertexBuffer();

	virtual void Bind() override;
private:
	static VertexBuffer* m_BoundVertexBuffer;
};