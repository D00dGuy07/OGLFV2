#pragma once

#include "Renderer/GLBuffer.h"

#include <vector>

class UniformBuffer : public GLBuffer
{
public:
	UniformBuffer(const void* data, size_t size);
	virtual ~UniformBuffer();

	virtual void Bind() override;
	void BindIndexed(uint32_t index = 0);

	inline void UpdateContents(const void* data, int32_t offset, size_t size) { GLBuffer::UpdateContents(data, offset, size); }
private:
	static UniformBuffer* m_BoundUniformBuffer;

	static std::vector<UniformBuffer*> m_BoundIndexedBuffers;
	static uint32_t m_MaxIndexedTargets;

	static void ReserveBindings();
	void FreeBinding(UniformBuffer* shaderBuffer);
};