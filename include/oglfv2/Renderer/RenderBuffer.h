#pragma once

#include <stdint.h>

#include "oglfv2/Renderer/ImageBuffer.h"

class RenderBuffer : public ImageBuffer
{
public:
	RenderBuffer(int32_t width, int32_t height, InternalImageFormat format);
	virtual ~RenderBuffer() override;

	void Bind();

	virtual void Reallocate(int32_t width, int32_t height) override;

private:
	InternalImageFormat m_Format;

	inline static RenderBuffer* m_BoundRenderBuffer = nullptr;
};