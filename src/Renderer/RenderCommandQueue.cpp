#include "oglfv2/Renderer/RenderCommandQueue.h"

#include "glad/glad.h"

#include <cstring>

#define BUFFER_SIZE_MB 1 

RenderCommandQueue::RenderCommandQueue()
{
	m_CommandBuffer = new uint8_t[BUFFER_SIZE_MB * 1024 * 1024];
	m_CommandLocation = m_CommandBuffer;
	memset(m_CommandBuffer, 0, BUFFER_SIZE_MB * 1024 * 1024);
}

RenderCommandQueue::~RenderCommandQueue()
{
	delete[] m_CommandBuffer;
}

void* RenderCommandQueue::Allocate(RenderCommand fn, uint32_t size)
{
	*(RenderCommand*)m_CommandLocation = fn;
	m_CommandLocation += sizeof(RenderCommand);

	*(uint32_t*)m_CommandLocation = size;
	m_CommandLocation += sizeof(uint32_t);

	void* memory = m_CommandLocation;
	m_CommandLocation += size;

	m_CommandCount++;
	return memory;
}

void RenderCommandQueue::Execute()
{
	uint8_t* buffer = m_CommandBuffer;

	for (uint32_t i = 0; i < m_CommandCount; i++)
	{
		RenderCommand function = *(RenderCommand*)buffer;
		buffer += sizeof(RenderCommand);

		uint32_t size = *(uint32_t*)buffer;
		buffer += sizeof(uint32_t);
		function(buffer);
		buffer += size;
	}

	m_CommandLocation = m_CommandBuffer;
	m_CommandCount = 0;
}