#include "oglfv2/Renderer/PixelBuffer.h"

#include "oglfv2/Renderer/Renderer.h"

#include <iostream>

size_t GetDataSize(const PixelBuffer::Spec& spec)
{
	size_t bpp = 0;

	// Figure out the size of the data type
	switch (spec.Format)
	{
	default:
	case PixelBuffer::Format::UINT8:
	case PixelBuffer::Format::INT8:
		bpp = 1;
		break;
	case PixelBuffer::Format::UINT16:
	case PixelBuffer::Format::INT16:
		bpp = 2;
		break;
	case PixelBuffer::Format::UINT32:
	case PixelBuffer::Format::INT32:
	case PixelBuffer::Format::FLOAT:
		bpp = 4;
		break;
	}

	// Figure out how many components each pixel has
	switch (spec.Order)
	{
	case PixelBuffer::Order::RED:
		break;
	case PixelBuffer::Order::RG:
		bpp *= 2;
		break;
	case PixelBuffer::Order::RGB:
	case PixelBuffer::Order::BGR:
		bpp *= 3;
		break;
	default:
	case PixelBuffer::Order::RGBA:
	case PixelBuffer::Order::BGRA:
		bpp *= 4;
		break;
	}

	// Calculate buffer size
	return spec.Width * spec.Height * bpp;
}

PixelBuffer::PixelBuffer(Spec spec)
	: m_Spec(spec), m_RendererID(), m_IsMapped(false)
{
	glGenBuffers(1, &m_RendererID);
	std::cout << "Created Pixel Buffer Object: " << m_RendererID << '\n';

	// This function will ensure that a GPU to CPU PBO has an allocated buffer.
	// It may be a little overkill for that, but it should work
	Resize(spec.Width, spec.Height);
}

PixelBuffer::~PixelBuffer()
{
	PixelBuffer** bindPoint = m_Spec.Type == Type::CPUtoGPU ? &m_BoundUnpackBuffer : &m_BoundPackBuffer;
	if (*bindPoint == this)
		*bindPoint = nullptr;

	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		glDeleteBuffers(1, &id);

		// Reset the bind cache if this is bound
		uint32_t* bindCache = (m_Spec.Type == Type::CPUtoGPU ? &m_BoundUnpackRendererID : &m_BoundPackRendererID);
		if (*bindCache == id)
			*bindCache = 0;
	});
}

void PixelBuffer::Bind()
{
	// Am I evil for using this pointer magic? Maybe. But I like how this looks better than an if statement
	// Maybe that means I'm a bad programmer, but I don't really care... Now I'm having second thoughts.
	PixelBuffer** bindPoint = m_Spec.Type == Type::CPUtoGPU ? &m_BoundUnpackBuffer : &m_BoundPackBuffer;
	if (*bindPoint == this)
		return;
	*bindPoint = this;

	Renderer::Submit([=]() {
		glBindBuffer(static_cast<uint32_t>(m_Spec.Type), m_RendererID);

		uint32_t* bindCache = (m_Spec.Type == Type::CPUtoGPU ? &m_BoundUnpackRendererID : &m_BoundPackRendererID);
		*bindCache = m_RendererID;
	});
}

void PixelBuffer::Unbind()
{
	PixelBuffer** bindPoint = m_Spec.Type == Type::CPUtoGPU ? &m_BoundUnpackBuffer : &m_BoundPackBuffer;
	if (*bindPoint == nullptr)
		return;
	*bindPoint = nullptr;

	Renderer::Submit([=]() {
		glBindBuffer(static_cast<uint32_t>(m_Spec.Type), 0);

		uint32_t* bindCache = (m_Spec.Type == Type::CPUtoGPU ? &m_BoundUnpackRendererID : &m_BoundPackRendererID);
		*bindCache = 0;
	});
}

void* PixelBuffer::Map()
{
	// Bind the buffer to be mapped
	bool shouldBind = (m_Spec.Type == Type::CPUtoGPU ? m_BoundUnpackRendererID : m_BoundPackRendererID) != m_RendererID;
	if (shouldBind)
		glBindBuffer(static_cast<uint32_t>(m_Spec.Type), m_RendererID);

	if (m_Spec.Type == Type::CPUtoGPU)
		glBufferData(GL_PIXEL_UNPACK_BUFFER, GetDataSize(m_Spec), nullptr, GL_STREAM_DRAW);
	// If there are problems with reading from the write only memory then this should be changed from GL_WRITE_ONLY to GL_READ_WRITE
	void* mappedPtr = glMapBuffer(static_cast<uint32_t>(m_Spec.Type), m_Spec.Type == Type::CPUtoGPU ? GL_WRITE_ONLY : GL_READ_ONLY);

	// Bind the previously bound buffer
	if (shouldBind)
		glBindBuffer(static_cast<uint32_t>(m_Spec.Type), m_Spec.Type == Type::CPUtoGPU ? m_BoundUnpackRendererID : m_BoundPackRendererID);

	return mappedPtr;
}

void PixelBuffer::Unmap()
{
	// Bind the buffer to be unmapped
	bool shouldBind = (m_Spec.Type == Type::CPUtoGPU ? m_BoundUnpackRendererID : m_BoundPackRendererID) != m_RendererID;
	if (shouldBind)
		glBindBuffer(static_cast<uint32_t>(m_Spec.Type), m_RendererID);

	glUnmapBuffer(static_cast<uint32_t>(m_Spec.Type));

	// Bind the previously bound buffer
	if (shouldBind)
		glBindBuffer(static_cast<uint32_t>(m_Spec.Type), m_Spec.Type == Type::CPUtoGPU ? m_BoundUnpackRendererID : m_BoundPackRendererID);
}

void PixelBuffer::Resize(int32_t width, int32_t height)
{
	if (m_IsMapped)
		throw std::exception("Can't resize a mapped pixel buffer (This should be a warning not an exception but I haven't ported the logging system)");

	// Bind the buffer to be resized
	bool shouldBind = (m_Spec.Type == Type::CPUtoGPU ? m_BoundUnpackRendererID : m_BoundPackRendererID) != m_RendererID;
	if (shouldBind)
		glBindBuffer(static_cast<uint32_t>(m_Spec.Type), m_RendererID);

	m_Spec.Width = width;
	m_Spec.Height = height;
	if (m_Spec.Type == Type::GPUtoCPU)
		glBufferData(static_cast<uint32_t>(m_Spec.Type), GetDataSize(m_Spec), nullptr, GL_STREAM_READ);

	// Bind the previously bound buffer
	if (shouldBind)
		glBindBuffer(static_cast<uint32_t>(m_Spec.Type), m_Spec.Type == Type::CPUtoGPU ? m_BoundUnpackRendererID : m_BoundPackRendererID);
}