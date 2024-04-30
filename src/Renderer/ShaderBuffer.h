#pragma once

#include "Renderer/GLBuffer.h"

#include <vector>

class ShaderBuffer : public GLBuffer
{
public:
	ShaderBuffer(const void* data, size_t size);
	virtual ~ShaderBuffer();

	virtual void Bind() override;
	void BindIndexed(uint32_t index = 0);
private:
	static ShaderBuffer* m_BoundShaderBuffer;

	static std::vector<ShaderBuffer*> m_BoundIndexedBuffers;
	static uint32_t m_MaxIndexedTargets;

	static void ReserveBindings();
	void FreeBinding(ShaderBuffer* shaderBuffer);
};