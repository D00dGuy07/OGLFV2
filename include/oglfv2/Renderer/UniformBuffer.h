#pragma once

#include "oglfv2/Renderer/GLBuffer.h"

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
	inline static UniformBuffer* m_BoundUniformBuffer = nullptr;

	inline static std::vector<UniformBuffer*> m_BoundIndexedBuffers = std::vector<UniformBuffer*>();
	inline static uint32_t m_MaxIndexedTargets = 0U;

	static void ReserveBindings();
	void FreeBinding(UniformBuffer* shaderBuffer);
};