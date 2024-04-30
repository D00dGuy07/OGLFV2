#pragma once

#include <string>
#include <vector>

#include "Renderer/ImageBuffer.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

enum class TextureMinFilter
{
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
	NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
	LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
	NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
	LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
};

enum class TextureMagFilter
{
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR
};

enum class TextureWrapMode
{
	Repeat = GL_REPEAT,
	MirroredRepeat = GL_MIRRORED_REPEAT,
	ClampToEdge = GL_CLAMP_TO_EDGE,
	ClampToBorder = GL_CLAMP_TO_BORDER
};

struct TextureSpec
{
	InternalImageFormat Format = InternalImageFormat::RGBA8;

	TextureMinFilter MinFilter = TextureMinFilter::Linear;
	TextureMagFilter MagFilter = TextureMagFilter::Linear;

	TextureWrapMode WrapS = TextureWrapMode::Repeat;
	TextureWrapMode WrapT = TextureWrapMode::Repeat;

	glm::vec4 BorderColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
};

class Texture : public ImageBuffer
{
public:
	Texture(const std::string& path, const TextureSpec& spec);
	Texture(const std::string& path);
	Texture(int32_t width, int32_t height, const TextureSpec& spec);
	Texture(int32_t width, int32_t height);

	virtual ~Texture() override;

	void Bind(uint32_t slot = 0);
	void BindImage(uint32_t slot = 0);

	virtual void Reallocate(int32_t width, int32_t height) override;
	
private:
	int32_t m_BPP;
	uint8_t* m_LocalData = nullptr;
	TextureSpec m_Spec;

	static std::vector<Texture*> m_BoundTextureUnits;
	static uint32_t m_MaxTextureUnits;

	static std::vector<Texture*> m_BoundImageUnits;
	static uint32_t m_MaxImageUnits;

	static void ReserveBindings();
	static void FreeBinding(Texture* texture);
};