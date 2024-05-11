#include "oglfv2/Renderer/Mesh.h"

#include <iterator>

#include "oglfv2/Renderer/VertexArray.h"
#include "oglfv2/Renderer/VertexBuffer.h"
#include "oglfv2/Renderer/IndexBuffer.h"

Mesh::~Mesh()
{
	delete m_VertexArray;
	delete m_VertexBuffer;
	delete m_IndexBuffer;
}

void Mesh::SetVertices(float* vertices, uint32_t size)
{
	delete m_VertexArray;
	delete m_VertexBuffer;

	m_VertexBuffer = new VertexBuffer(vertices, size);
}

void Mesh::SetIndices(uint32_t* indices, uint32_t length)
{
	delete m_IndexBuffer;

	m_IndexBuffer = new IndexBuffer(indices, length);
}

void Mesh::Construct()
{
	if (m_VertexBuffer == NULL || m_VertexArray != NULL)
		return;
	
	m_VertexArray = new VertexArray();
	m_VertexArray->AddBuffer(*m_VertexBuffer, BufferLayout);
}

uint32_t Mesh::GetNumIndices() const { return static_cast<uint32_t>(m_IndexBuffer->GetCount()); }