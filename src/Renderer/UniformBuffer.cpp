#include "oglfv2/Renderer/UniformBuffer.h"

#include "oglfv2/Renderer/Renderer.h"

UniformBuffer::UniformBuffer(const void* data, size_t size)
	: GLBuffer()
{
	SetContents(data, size, Target::Uniform, Usage::DynamicDraw);

	if (m_MaxIndexedTargets == 0)
		ReserveBindings();
}

UniformBuffer::~UniformBuffer()
{
	if (m_BoundUniformBuffer == this)
		m_BoundUniformBuffer = nullptr;
	FreeBinding(this);

	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		glDeleteBuffers(1, &id);
	});
}

void UniformBuffer::Bind()
{
	if (m_Target == GLBuffer::Target::Uniform && m_BoundUniformBuffer == nullptr)
		return;
	m_BoundUniformBuffer = this;

	Renderer::Submit([=]() {
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	});
}

void UniformBuffer::BindIndexed(uint32_t index)
{
	if (m_MaxIndexedTargets == 0)
		ReserveBindings();

	if (index > m_MaxIndexedTargets - 1)
	{
		std::cout << "Tried to bind a uniform buffer to buffer index " << index << " which is not a valid indexed uniform buffer index!";
		return;
	}

	if (m_Target == GLBuffer::Target::Uniform && m_BoundIndexedBuffers[index] == this)
		return;
	m_BoundIndexedBuffers[index] = this;

	Bind();
	Renderer::Submit([=]() {
		glBindBufferBase(GL_UNIFORM_BUFFER, index, m_RendererID);
	});
}

void UniformBuffer::ReserveBindings()
{
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, reinterpret_cast<int32_t*>(&m_MaxIndexedTargets));
	m_BoundIndexedBuffers.assign(m_MaxIndexedTargets, nullptr);
}

void UniformBuffer::FreeBinding(UniformBuffer* uniformBuffer)
{
	for (uint32_t i = 0; i < m_MaxIndexedTargets; i++)
		if (m_BoundIndexedBuffers[i] == uniformBuffer)
			m_BoundIndexedBuffers[i] = nullptr;
}