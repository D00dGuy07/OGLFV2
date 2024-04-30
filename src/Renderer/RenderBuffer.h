#pragma once

#include <stdint.h>

#include "Renderer/ImageBuffer.h"

class RenderBuffer : public ImageBuffer
{
public:
	RenderBuffer(int32_t width, int32_t height, InternalImageFormat format);
	virtual ~RenderBuffer() override;

	void Bind();

	virtual void Reallocate(int32_t width, int32_t height) override;

private:
	InternalImageFormat m_Format;

	static RenderBuffer* m_BoundRenderBuffer;
};