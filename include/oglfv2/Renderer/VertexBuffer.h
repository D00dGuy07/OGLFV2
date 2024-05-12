#pragma once

#include "oglfv2/Renderer/GLBuffer.h"

class VertexBuffer : public GLBuffer
{
public:
	VertexBuffer(const void* data, size_t size);
	virtual ~VertexBuffer();

	virtual void Bind() override;
private:
	inline static VertexBuffer* m_BoundVertexBuffer = nullptr;
};