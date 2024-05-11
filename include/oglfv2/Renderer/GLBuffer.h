#pragma once

#include <stdint.h>

#include <glad/glad.h>

class GLBuffer
{
public:
	GLBuffer();
	virtual ~GLBuffer() {}

	virtual void Bind() = 0;

	inline size_t GetSize() const { return m_Size; }
protected:
	uint32_t m_RendererID;
	size_t m_Size = 0;

	enum class Target
	{
		ArrayBuffer = GL_ARRAY_BUFFER,
		AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,
		CopyRead = GL_COPY_READ_BUFFER,
		CopyWrite = GL_COPY_WRITE_BUFFER,
		DispatchInderect = GL_DISPATCH_INDIRECT_BUFFER,
		DrawInderect = GL_DRAW_INDIRECT_BUFFER,
		ElementArray = GL_ELEMENT_ARRAY_BUFFER,
		PixelPack = GL_PIXEL_PACK_BUFFER,
		PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
		ShaderStorage = GL_SHADER_STORAGE_BUFFER,
		Texture = GL_TEXTURE_BUFFER,
		TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
		Uniform = GL_UNIFORM_BUFFER,
	};

	enum class Usage
	{
		StaticDraw = GL_STATIC_DRAW,
		StaticRead = GL_STATIC_READ,
		StaticCopy = GL_STATIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicRead = GL_DYNAMIC_READ,
		DynamicCopy = GL_DYNAMIC_COPY,
		StreamDraw = GL_STREAM_DRAW,
		StreamRead = GL_STREAM_READ,
		StreamCopy = GL_STREAM_COPY
	};

	Target m_Target = static_cast<Target>(0);

	void SetContents(const void* data, size_t size, Target target, Usage usage);
	void UpdateContents(const void* data, int32_t offset, size_t size);
};