#pragma once

#include <stdint.h>

enum class ImageBufferType
{
	Texture,
	RenderBuffer
};

enum class InternalImageFormat
{
	R8,
	R8_SNorm,
	R16,
	R16_SNorm,
	RG8,
	RG8_SNorm,
	RG16,
	RG16_SNorm,
	R3_G3_B2,
	RGB4,
	RGB5,
	RGB8,
	RGB8_SNorm,
	RGB10,
	RGB12,
	RGB16_SNorm,
	RGBA2,
	RGBA4,
	RGB5_A1,
	RGBA8,
	RGBA8_SNorm,
	RGB10_A2,
	RGB10_A2UI,
	RGBA12,
	RGBA16,
	SRGB8,
	SRGB8_Alpha8,
	R16F,
	RG16F,
	RGB16F,
	RGBA16F,
	R32F,
	RG32F,
	RGB32F,
	RGBA32F,
	R11F_G11F_B10F,
	RGB9_E5,
	R8I,
	R8UI,
	R16I,
	R16UI,
	R32I,
	R32UI,
	RG8I,
	RG8UI,
	RG16I,
	RG16UI,
	RG32I,
	RG32UI,
	RGB8I,
	RGB8UI,
	RGB16I,
	RGB16UI,
	RGB32I,
	RGB32UI,
	RGBA8I,
	RGBA8UI,
	RGBA16I,
	RGBA16UI,
	RGBA32I,
	RGBA32UI,
	Depth_Component16,
	Depth_Component24,
	Depth_Component32F,
	Depth24_Stencil8,
	Depth32F_Stencil8,
	Stencil_Index8
};

class ImageBuffer
{
public:
	ImageBuffer(uint32_t rendererID, int32_t width, int32_t height)
		: m_RendererID(rendererID), m_Width(width), m_Height(height) {}

	virtual ~ImageBuffer() {}

	// Clears data and reallocates buffer to specified size
	virtual void Reallocate(int32_t width, int32_t height) = 0;

	inline int32_t GetWidth() const { return m_Width; }
	inline int32_t GetHeight() const { return m_Height; }

	static void GetFormatData(const InternalImageFormat& format, int32_t& sizedId, int32_t& baseId);

protected:
	uint32_t m_RendererID;
	int32_t m_Width, m_Height;

	friend class Framebuffer;
};