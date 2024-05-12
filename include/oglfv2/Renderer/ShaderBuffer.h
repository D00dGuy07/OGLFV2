#pragma once

#include "oglfv2/Renderer/GLBuffer.h"

#include <vector>

class ShaderBuffer : public GLBuffer
{
public:
	ShaderBuffer(const void* data, size_t size);
	virtual ~ShaderBuffer();

	virtual void Bind() override;
	void BindIndexed(uint32_t index = 0);
private:
	inline static ShaderBuffer* m_BoundShaderBuffer = nullptr;

	inline static std::vector<ShaderBuffer*> m_BoundIndexedBuffers = std::vector<ShaderBuffer*>();
	inline static uint32_t m_MaxIndexedTargets = 0U;

	static void ReserveBindings();
	void FreeBinding(ShaderBuffer* shaderBuffer);
};