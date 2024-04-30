#pragma once

#include "Renderer/VertexBufferLayout.h"

class VertexArray;
class VertexBuffer;
class IndexBuffer;

class Mesh
{
public:
	~Mesh();

	void SetVertices(float* vertices, uint32_t size);
	void SetIndices(uint32_t* indices, uint32_t length);

	void Construct();

	uint32_t GetNumIndices() const;
	VertexArray* GetVertexArray() const { return m_VertexArray; }
	VertexBuffer* GetVertexBuffer() const { return m_VertexBuffer; }
	IndexBuffer* GetIndexBuffer() const { return m_IndexBuffer; }
	
	VertexBufferLayout BufferLayout = VertexBufferLayout();
private:
	VertexArray* m_VertexArray;
	VertexBuffer* m_VertexBuffer;
	IndexBuffer* m_IndexBuffer;

	friend class Renderer;
};