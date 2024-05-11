#include "oglfv2/Renderer/GLHelpers.h"

#include "oglfv2/Renderer/Renderer.h"

// Memory Helper

void GLMemoryHelper::Barrier(GLMemoryHelper::BarrierType type)
{
	Renderer::Submit([=]() {
		glMemoryBarrier(static_cast<uint32_t>(type));
	});
}

GLMemoryHelper::BarrierType operator||(const GLMemoryHelper::BarrierType& first, const GLMemoryHelper::BarrierType& second)
{
	return static_cast<GLMemoryHelper::BarrierType>(static_cast<uint32_t>(first) || static_cast<uint32_t>(second));
}