#pragma once

#include "Renderer/GLBuffer.h"

class IndexBuffer : public GLBuffer
{
public:
	IndexBuffer(const uint32_t* data, uint32_t count);
	virtual ~IndexBuffer();

	virtual void Bind() override;

	inline size_t GetCount() const { return m_Size / sizeof(uint32_t); }
private:
	static IndexBuffer* m_BoundIndexBuffer;
};