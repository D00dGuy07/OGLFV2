#include "Renderer/ShaderBuffer.h"

#include "Renderer/Renderer.h"

#include <iostream>

ShaderBuffer* ShaderBuffer::m_BoundShaderBuffer = nullptr;

std::vector<ShaderBuffer*> ShaderBuffer::m_BoundIndexedBuffers = std::vector<ShaderBuffer*>();
uint32_t ShaderBuffer::m_MaxIndexedTargets = 0U;

ShaderBuffer::ShaderBuffer(const void* data, size_t size)
	: GLBuffer()
{
	SetContents(data, size, Target::ShaderStorage, Usage::StreamDraw);

	if (m_MaxIndexedTargets == 0)
		ReserveBindings();
}

ShaderBuffer::~ShaderBuffer()
{
	if (m_BoundShaderBuffer == this)
		m_BoundShaderBuffer = nullptr;
	FreeBinding(this);

	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		glDeleteBuffers(1, &id);
	});
}

void ShaderBuffer::Bind()
{
	if (m_Target == GLBuffer::Target::ShaderStorage && m_BoundShaderBuffer == nullptr)
		return;
	m_BoundShaderBuffer = this;

	Renderer::Submit([=]() {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
	});
}

void ShaderBuffer::BindIndexed(uint32_t index)
{
	if (m_MaxIndexedTargets == 0)
		ReserveBindings();

	if (index > m_MaxIndexedTargets - 1)
	{
		std::cout << "Tried to bind a buffer to buffer index " << index << " which is not a valid indexed buffer index!";
		return;
	}

	if (m_Target == GLBuffer::Target::ShaderStorage && m_BoundIndexedBuffers[index] == this)
		return;
	m_BoundIndexedBuffers[index] = this;

	Bind();
	Renderer::Submit([=]() {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_RendererID);
	});
}

void ShaderBuffer::ReserveBindings()
{
	glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, reinterpret_cast<int32_t*>(&m_MaxIndexedTargets));
	m_BoundIndexedBuffers.assign(m_MaxIndexedTargets, nullptr);
}

void ShaderBuffer::FreeBinding(ShaderBuffer* shaderBuffer)
{
	for (uint32_t i = 0; i < m_MaxIndexedTargets; i++)
		if (m_BoundIndexedBuffers[i] == shaderBuffer)
			m_BoundIndexedBuffers[i] = nullptr;
}