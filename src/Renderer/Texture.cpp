#include "oglfv2/Renderer/Texture.h"

#include "oglfv2/Renderer/Renderer.h"

#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

Texture::Texture(const std::string& path, const TextureSpec& spec)
	: ImageBuffer(NULL, 0, 0), m_BPP(0), m_Spec(spec)
{
	// Load image data
	m_LocalData = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	Renderer::Submit([&, path]() {
		// Create texture object
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		// Set parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int32_t>(m_Spec.MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int32_t>(m_Spec.MagFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int32_t>(m_Spec.WrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int32_t>(m_Spec.WrapT));

		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, reinterpret_cast<float*>(&m_Spec.BorderColor));

		// Create and fill buffer
		int32_t sizedId = 0, baseId = 0;
		ImageBuffer::GetFormatData(m_Spec.Format, sizedId, baseId);
		glTexImage2D(GL_TEXTURE_2D, 0, sizedId, m_Width, m_Height, 0, baseId, GL_UNSIGNED_BYTE, m_LocalData);

		// Unbind texture and free local image data
		glBindTexture(GL_TEXTURE_2D, 0);
		if (m_LocalData)
			stbi_image_free(m_LocalData);
	});

	if (m_MaxTextureUnits == 0 || m_MaxImageUnits == 0)
		ReserveBindings();
}

Texture::Texture(const std::string& path)
	: Texture(path, TextureSpec()) {}

Texture::Texture(int32_t width, int32_t height, const TextureSpec& spec)
	: ImageBuffer(NULL, width, height), m_BPP(32), m_Spec(spec)
{
	Renderer::Submit([&]() {
		// Create texture object
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		// Set parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int32_t>(m_Spec.MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int32_t>(m_Spec.MagFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int32_t>(m_Spec.WrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int32_t>(m_Spec.WrapT));

		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, reinterpret_cast<float*>(&m_Spec.BorderColor));

		// Create and fill buffer
		int32_t sizedId = 0, baseId = 0;
		ImageBuffer::GetFormatData(m_Spec.Format, sizedId, baseId);
		glTexImage2D(GL_TEXTURE_2D, 0, sizedId, m_Width, m_Height, 0, baseId, GL_UNSIGNED_BYTE, NULL);

		// Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
	});

	if (m_MaxTextureUnits == 0 || m_MaxImageUnits == 0)
		ReserveBindings();
}

Texture::Texture(int32_t width, int32_t height)
	: Texture(width, height, TextureSpec()) {}

Texture::~Texture()
{
	FreeBinding(this);

	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		glDeleteTextures(1, &id);
	});
}

void Texture::Bind(uint32_t slot)
{
	if (slot > m_MaxTextureUnits - 1)
	{
		std::cout << "Tried to bind a texture to texture slot " << slot << " which is not a valid texture slot!";
		return;
	}

	if (m_BoundTextureUnits[slot] == this)
		return;
	m_BoundTextureUnits[slot] = this;

	Renderer::Submit([=]() {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	});
}

void Texture::BindImage(uint32_t slot)
{
	if (slot > m_MaxImageUnits - 1)
	{
		std::cout << "Tried to bind a texture to image slot " << slot << " which is not a valid image slot!";
		return;
	}

	if (m_BoundImageUnits[slot] == this)
		return;
	m_BoundImageUnits[slot] = this;
	
	Renderer::Submit([=]() {
		int32_t sizedId = 0, baseId = 0;
		ImageBuffer::GetFormatData(m_Spec.Format, sizedId, baseId);
		glBindImageTexture(slot, m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, sizedId);
	});
}

void Texture::Reallocate(int32_t width, int32_t height)
{
	m_Width = width;
	m_Height = height;

	int32_t sizedId = 0, baseId = 0;
	ImageBuffer::GetFormatData(m_Spec.Format, sizedId, baseId);

	Bind();
	Renderer::Submit([=]() {
		glTexImage2D(GL_TEXTURE_2D, 0, sizedId, m_Width, m_Height, 0, baseId, GL_UNSIGNED_BYTE, NULL);
	});
}

void Texture::ReserveBindings()
{
	// Get max bindings count
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, reinterpret_cast<int32_t*>(&m_MaxTextureUnits));
	glGetIntegerv(GL_MAX_IMAGE_UNITS, reinterpret_cast<int32_t*>(&m_MaxImageUnits));

	// Set initial values
	m_BoundTextureUnits.assign(m_MaxTextureUnits, nullptr);
	m_BoundImageUnits.assign(m_MaxImageUnits, nullptr);
}

void Texture::FreeBinding(Texture* texture)
{
	for (uint32_t i = 0; i < m_MaxTextureUnits; i++)
		if (m_BoundTextureUnits[i] == texture)
			m_BoundTextureUnits[i] = nullptr;
	for (uint32_t i = 0; i < m_MaxImageUnits; i++)
		if (m_BoundImageUnits[i] == texture)
			m_BoundImageUnits[i] = nullptr;
}

// Texture Atlas

TextureCoords TextureAtlasCoords(float x, float y, float width, float height)
{
	return {
		glm::vec2(((x * width))        , ((y * width) + width)),
		glm::vec2(((x * width) + width), ((y * width) + width)),
		glm::vec2(((x * width) + width), (y * width)),
		glm::vec2(((x * width))        , (y * width))
	};
}