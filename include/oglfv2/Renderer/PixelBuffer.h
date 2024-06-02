#pragma once

#include "oglfv2/Renderer/GLBuffer.h"
#include "oglfv2/Renderer/ImageBuffer.h"

class PixelBuffer
{
public:
	enum class Type
	{
		CPUtoGPU = GL_PIXEL_UNPACK_BUFFER,
		GPUtoCPU = GL_PIXEL_PACK_BUFFER
	};

	enum class Order
	{
		RED = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		BGR = GL_BGR,
		RGBA = GL_RGBA,
		BGRA = GL_BGRA
	};

	enum class Format
	{
		UINT8 = GL_UNSIGNED_BYTE,
		INT8 = GL_BYTE,
		UINT16 = GL_UNSIGNED_SHORT,
		INT16 = GL_SHORT,
		UINT32 = GL_UNSIGNED_INT,
		INT32 = GL_INT,
		FLOAT = GL_FLOAT
	};

	struct Spec
	{
		Type Type;
		Order Order;
		Format Format;

		int32_t Width;
		int32_t Height;
	};


	PixelBuffer(Spec spec);
	~PixelBuffer();

	void Bind();
	void Unbind();

	void* Map();
	void Unmap();

	// This function will fail when the buffer is mapped
	// CPU to GPU buffers are not resized until the next time they are mapped
	void Resize(int32_t width, int32_t height);

	inline Spec GetSpec() const { return m_Spec; }

private:
	Spec m_Spec;
	uint32_t m_RendererID;

	bool m_IsMapped;

	inline static PixelBuffer* m_BoundPackBuffer = nullptr;
	inline static PixelBuffer* m_BoundUnpackBuffer = nullptr;

	// These values should reflect the actual currently bound renderer ID while the above values might not.
	// This is necessary for resetting state when temporarily binding a buffer. glGetIntegerv is too slow for this
	inline static uint32_t m_BoundPackRendererID = 0;
	inline static uint32_t m_BoundUnpackRendererID = 0;
};