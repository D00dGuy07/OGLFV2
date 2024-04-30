#include "Renderer/Framebuffer.h"

#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"
#include "Renderer/RenderBuffer.h"

#include "glad/glad.h"

#include <iostream>
#include <stdexcept>

Framebuffer* Framebuffer::m_BoundFramebuffer = nullptr;

Framebuffer::Framebuffer(const FramebufferSpec& spec)
	: m_RendererID(0), m_Spec(spec),
	m_ColorAttachments(), m_OtherAttachments()
{
	// Validate the spec
	{
		if (m_Spec.Width < 1 || m_Spec.Height < 1)
			throw std::invalid_argument("Width and Height of framebuffer must be greater than 0!");

		if (m_Spec.OtherAttachments.size() > 2)
			throw std::invalid_argument("Framebuffers can only support up to 2 non-color attachments!");

		if (m_Spec.ColorAttachments.size() < 1)
			throw std::invalid_argument("Framebuffers must have at least 1 color attachment!");

		bool observedTypes[] = { false, false, false };
		for (FBOAttachmentSpec attachment : m_Spec.OtherAttachments)
		{
			int32_t typeIndex = static_cast<int32_t>(attachment.AttachmentType);
			if (observedTypes[typeIndex] == true)
				throw std::invalid_argument("Framebuffers only support one of each type of non-color attachment!");

			switch (attachment.AttachmentType)
			{
			case FBOAttachmentType::Color:
				throw std::invalid_argument("Color attachment specs belong in the color attachments field!");
			case FBOAttachmentType::DepthStencil:
				if (observedTypes[0] == true || observedTypes[1] == true)
					throw std::invalid_argument("DepthStencil attachment is incompatible with Depth and Stencil attachment types!");
				break;
			case FBOAttachmentType::Depth:
				if (observedTypes[2] == true)
					throw std::invalid_argument("DepthStencil attachment is incompatible with Depth and Stencil attachment types!");
				else if (!(static_cast<uint32_t>(static_cast<int32_t>(attachment.Format) - 61) <= (61 - 60)))
					throw std::invalid_argument("Depth buffer attachments must use a depth format!");
				break;
			case FBOAttachmentType::Stencil:
				if (observedTypes[2] == true)
					throw std::invalid_argument("DepthStencil attachment is incompatible with Depth and Stencil attachment types!");
				else if (static_cast<int32_t>(attachment.Format) != 66)
					throw std::invalid_argument("Depth buffer attachments must use a depth format!");
				break;
			}
		}

		int32_t maxColorAttachments;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
		int32_t colorAttachmentCount = static_cast<int32_t>(m_Spec.ColorAttachments.size());
		if (colorAttachmentCount > maxColorAttachments) // Kinda suspicious if statement body
			throw std::invalid_argument(
				std::string("Framebuffer max color attachments is ")
				.append(std::to_string(maxColorAttachments))
				.append(" but the spec has ")
				.append(std::to_string(colorAttachmentCount))
				.append(" attachments!"));

		for (FBOAttachmentSpec attachmentSpec : m_Spec.ColorAttachments)
			if (static_cast<int32_t>(attachmentSpec.Format) > 60)
				throw std::invalid_argument("Color buffer attachments must use a color format not a depth/stencil format");
	}

	// Build framebuffer
	{
		Renderer::Submit([=]() {
			glGenFramebuffers(1, &m_RendererID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		});

		m_ColorAttachments.resize(m_Spec.ColorAttachments.size());
		m_ColorAttachments.clear();
		m_OtherAttachments.resize(m_Spec.OtherAttachments.size());
		m_OtherAttachments.clear();

		uint8_t attachmentNumber = 0;
		for (FBOAttachmentSpec attachmentSpec : m_Spec.ColorAttachments)
		{
			ImageBuffer* buffer = nullptr;
			uint32_t* id;

			switch (attachmentSpec.BufferType)
			{
			case ImageBufferType::Texture:
			{
				TextureSpec textureSpec = TextureSpec();
				textureSpec.Format = attachmentSpec.Format;
				buffer = new Texture(m_Spec.Width, m_Spec.Height, textureSpec);

				id = &buffer->m_RendererID;
				Renderer::Submit([=]() {
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentNumber, GL_TEXTURE_2D, *id, 0);
				});
				break;
			}
			case ImageBufferType::RenderBuffer:
				buffer = new RenderBuffer(m_Spec.Width, m_Spec.Height, attachmentSpec.Format);
				id = &buffer->m_RendererID;
				Renderer::Submit([=]() {
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentNumber, GL_RENDERBUFFER, *id);
				});
				break;
			};

			m_ColorAttachments.push_back({
				buffer,
				attachmentSpec.BufferType,
				attachmentSpec.AttachmentType
			});
			attachmentNumber++;
		}

		for (FBOAttachmentSpec attachmentSpec : m_Spec.OtherAttachments)
		{
			ImageBuffer* buffer = nullptr;
			int32_t glAttachmentPoint;
			uint32_t* id;

			switch (attachmentSpec.AttachmentType)
			{
			case FBOAttachmentType::Depth:
				glAttachmentPoint = GL_DEPTH_ATTACHMENT;
				break;
			case FBOAttachmentType::Stencil:
				glAttachmentPoint = GL_STENCIL_ATTACHMENT;
				break;
			case FBOAttachmentType::DepthStencil:
				glAttachmentPoint = GL_DEPTH_STENCIL_ATTACHMENT;
				break;
			}

			switch (attachmentSpec.BufferType)
			{
			case ImageBufferType::Texture:
			{
				TextureSpec textureSpec = TextureSpec();
				textureSpec.Format = attachmentSpec.Format;
				buffer = new Texture(m_Spec.Width, m_Spec.Height, textureSpec);
				id = &buffer->m_RendererID;
				Renderer::Submit([=]() {
					glFramebufferTexture2D(GL_FRAMEBUFFER, glAttachmentPoint, GL_TEXTURE_2D, *id, 0);
				});
				break;
			}
			case ImageBufferType::RenderBuffer:
				buffer = new RenderBuffer(m_Spec.Width, m_Spec.Height, attachmentSpec.Format);
				id = &buffer->m_RendererID;
				Renderer::Submit([=]() {
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, glAttachmentPoint, GL_RENDERBUFFER, *id);
				});
				break;
			};

			m_OtherAttachments.push_back({
				buffer,
				attachmentSpec.BufferType,
				attachmentSpec.AttachmentType
			});
		}
	}
}

Framebuffer::~Framebuffer()
{
	if (m_BoundFramebuffer == this)
		m_BoundFramebuffer = nullptr;

	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		glDeleteFramebuffers(1, &id);
	});

	for (FBOAttachment attachment : m_ColorAttachments)
		delete attachment.Buffer;

	for (FBOAttachment attachment : m_OtherAttachments)
		delete attachment.Buffer;
}

void Framebuffer::Bind()
{
	if (m_BoundFramebuffer == this)
		return;
	m_BoundFramebuffer = this;

	Renderer::Submit([=]() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	});
}

void Framebuffer::Unbind()
{
	if (m_BoundFramebuffer == nullptr)
		return;
	m_BoundFramebuffer = nullptr;

	Renderer::Submit([=]() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	});
}

void Framebuffer::Resize(int32_t width, int32_t height)
{
	if (width < 1 || height < 1)
		throw std::invalid_argument("Width and Height of framebuffer must be greater than 0!");

	for (FBOAttachment attachment : m_ColorAttachments)
		attachment.Buffer->Reallocate(width, height);
	for (FBOAttachment attachment : m_OtherAttachments)
		attachment.Buffer->Reallocate(width, height);
	m_Spec.Width = width;
	m_Spec.Height = height;
}