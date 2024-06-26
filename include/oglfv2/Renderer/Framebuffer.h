#pragma once

#include <vector>

#include "oglfv2/Renderer/ImageBuffer.h"

// Numbered like this to index search while verifying spec
enum class FBOAttachmentType
{
	Depth = 0,
	Stencil = 1,
	DepthStencil = 2,
	Color = 3
};

struct FBOAttachmentSpec
{
	ImageBufferType BufferType;
	FBOAttachmentType AttachmentType;
	InternalImageFormat Format;
};

struct FramebufferSpec
{
	uint32_t Width;
	uint32_t Height;

	std::vector<FBOAttachmentSpec> ColorAttachments;
	std::vector<FBOAttachmentSpec> OtherAttachments;
};

struct FBOAttachment
{
	ImageBuffer* Buffer;
	ImageBufferType BufferType;
	FBOAttachmentType AttachmentType;
};

class PixelBuffer;

class Framebuffer
{
public:
	Framebuffer(const FramebufferSpec& spec);
	~Framebuffer();

	void Bind();
	void Unbind();

	void Resize(int32_t width, int32_t height);

	void PackPBO(PixelBuffer& pixelBuffer, uint32_t index);

	inline int32_t GetWidth() const { return m_Spec.Width; }
	inline int32_t GetHeight() const { return m_Spec.Height; }

	inline FBOAttachment GetColorAttachment(uint32_t index) const { return m_ColorAttachments[index]; }
	inline FBOAttachment GetOtherAttachment(uint32_t index) const { return m_OtherAttachments[index]; }

private:
	uint32_t m_RendererID;
	FramebufferSpec m_Spec;

	std::vector<FBOAttachment> m_ColorAttachments;
	std::vector<FBOAttachment> m_OtherAttachments;

	inline static Framebuffer* m_BoundFramebuffer = nullptr;

	friend class Renderer;
};