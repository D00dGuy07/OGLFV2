#include "oglfv2/Renderer/ImageBuffer.h"

#include "glad/glad.h"

struct ImageFormatData
{
	int32_t SizedFormat;
	int32_t BaseFormat;
};

ImageFormatData FormatDataLookup[] = {
	{ GL_R8, GL_RED },
	{ GL_R8_SNORM, GL_RED },
	{ GL_R16, GL_RED },
	{ GL_R16_SNORM, GL_RED },
	{ GL_RG8, GL_RG },
	{ GL_RG8_SNORM, GL_RG },
	{ GL_RG16, GL_RG },
	{ GL_RG16_SNORM, GL_RG },
	{ GL_R3_G3_B2, GL_RGB },
	{ GL_RGB4, GL_RGB },
	{ GL_RGB5, GL_RGB },
	{ GL_RGB8, GL_RGB },
	{ GL_RGB8_SNORM, GL_RGB },
	{ GL_RGB10, GL_RGB },
	{ GL_RGB12, GL_RGB },
	{ GL_RGB16_SNORM, GL_RGB },
	{ GL_RGBA2, GL_RGB },
	{ GL_RGBA4, GL_RGB },
	{ GL_RGB5_A1, GL_RGBA },
	{ GL_RGBA8, GL_RGBA },
	{ GL_RGBA8_SNORM, GL_RGBA },
	{ GL_RGB10_A2, GL_RGBA },
	{ GL_RGB10_A2UI, GL_RGBA },
	{ GL_RGBA12, GL_RGBA },
	{ GL_RGBA16, GL_RGBA },
	{ GL_SRGB8, GL_RGB },
	{ GL_SRGB8_ALPHA8, GL_RGBA },
	{ GL_R16F, GL_RED },
	{ GL_RG16F, GL_RG },
	{ GL_RGB16F, GL_RGB },
	{ GL_RGBA16F, GL_RGBA },
	{ GL_R32F, GL_RED },
	{ GL_RG32F, GL_RG },
	{ GL_RGB32F, GL_RGB },
	{ GL_RGBA32F, GL_RGBA },
	{ GL_R11F_G11F_B10F, GL_RGB },
	{ GL_RGB9_E5, GL_RGB },
	{ GL_R8I, GL_RED },
	{ GL_R8UI, GL_RED },
	{ GL_R16I, GL_RED },
	{ GL_R16UI, GL_RED },
	{ GL_R32I, GL_RED },
	{ GL_R32UI, GL_RED },
	{ GL_RG8I, GL_RG },
	{ GL_RG8UI, GL_RG },
	{ GL_RG16I, GL_RG },
	{ GL_RG16UI, GL_RG },
	{ GL_RG32I, GL_RG },
	{ GL_RG32UI, GL_RG },
	{ GL_RGB8I, GL_RGB },
	{ GL_RGB8UI, GL_RGB },
	{ GL_RGB16I, GL_RGB },
	{ GL_RGB16UI, GL_RGB },
	{ GL_RGB32I, GL_RGB },
	{ GL_RGB32UI, GL_RGB },
	{ GL_RGBA8I, GL_RGBA },
	{ GL_RGBA8UI, GL_RGBA },
	{ GL_RGBA16I, GL_RGBA },
	{ GL_RGBA16UI, GL_RGBA },
	{ GL_RGBA32I, GL_RGBA },
	{ GL_RGBA32UI, GL_RGBA },
	{ GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT },
	{ GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT },
	{ GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT },
	{ GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL },
	{ GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL },
	{ GL_STENCIL_INDEX8, GL_STENCIL }
};

void ImageBuffer::GetFormatData(const InternalImageFormat& format, int32_t& sizedFormat, int32_t& baseFormat)
{
	ImageFormatData data = FormatDataLookup[static_cast<int32_t>(format)];
	sizedFormat = data.SizedFormat;
	baseFormat = data.BaseFormat;
}