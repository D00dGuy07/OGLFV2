#pragma once
#include <cstdint>

// This whole class is copied from the Hazel game engine
class RenderCommandQueue
{
public:
	typedef void(*RenderCommand)(void*);

	RenderCommandQueue();
	~RenderCommandQueue();

	void* Allocate(RenderCommand fn, uint32_t size);
	void Execute();

private:
	uint8_t* m_CommandBuffer;
	uint8_t* m_CommandLocation;
	uint32_t m_CommandCount = 0;
};