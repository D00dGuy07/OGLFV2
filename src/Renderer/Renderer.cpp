#include "oglfv2/Renderer/Renderer.h"

#include "oglfv2/Renderer/VertexArray.h"
#include "oglfv2/Renderer/IndexBuffer.h"
#include "oglfv2/Renderer/Framebuffer.h"
#include "oglfv2/Renderer/PixelBuffer.h"

void Renderer::UseBlending(bool enable, BlendFunction source, BlendFunction destination)
{
	Submit([=]() {
		if (enable)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		glBlendFunc(static_cast<GLenum>(source), static_cast<GLenum>(destination));
	});
}

void Renderer::UseDepthFunction(bool enable, DepthFunction function)
{
	Submit([=]() {
		if (enable)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		glDepthFunc(static_cast<GLenum>(function));
	});
}

void Renderer::SetCullFace(bool enable, CullMode mode)
{
	Submit([=]() {
		if (enable)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);

		glCullFace(static_cast<GLenum>(mode));
	});
}

void Renderer::SetPolygonMode(PolygonMode mode)
{
	Submit([=]() {
		glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(mode));
	});
}

void Renderer::SetDepthRange(float near, float far)
{
	Submit([=]() {
		glDepthRange(near, far);
	});
}

void Renderer::SetDepthRange(double near, double far)
{
	Submit([=]() {
		glDepthRange(near, far);
	});
}

void Renderer::SetClearColor(glm::vec4 color)
{
	Submit([=]() {
		glClearColor(color.r, color.g, color.b, color.a);
	});
}

void Renderer::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	Submit([=]() {
		glViewport(x, y, width, height);
	});
}

void Renderer::SubmitMesh(Mesh& mesh, Shader& shader)
{
	if (mesh.m_VertexBuffer == NULL || mesh.m_IndexBuffer == NULL)
		return;

	if (mesh.m_VertexArray == NULL)
		mesh.Construct();

	shader.Bind();
	mesh.m_VertexArray->Bind();
	mesh.m_IndexBuffer->Bind();

	uint32_t indicesCount = mesh.GetNumIndices();
	Submit([indicesCount]() mutable {
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
	});
}

void Renderer::SubmitMeshInstanced(Mesh& mesh, Shader& shader, uint32_t instanceCount)
{
	if (mesh.m_VertexBuffer == NULL || mesh.m_IndexBuffer == NULL)
		return;

	if (mesh.m_VertexArray == NULL)
		mesh.Construct();

	shader.Bind();
	mesh.m_VertexArray->Bind();
	mesh.m_IndexBuffer->Bind();

	uint32_t indicesCount = mesh.GetNumIndices();
	Submit([indicesCount, instanceCount]() mutable {
		glDrawElementsInstanced(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr, instanceCount);
	});
}

void Renderer::Clear()
{
	Submit([]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});
}

void Renderer::DrawFrame()
{
	m_CommandQueue.Execute();
	m_GarbageHeap.CleanupGarbage();
}

void Renderer::PackPBO(PixelBuffer& pixelBuffer)
{
	PixelBuffer::Spec pboSpec = pixelBuffer.GetSpec();
	if (pboSpec.Type == PixelBuffer::Type::CPUtoGPU)
		throw std::exception("Can't pack to a CPU to GPU pixel buffer");

	pixelBuffer.Bind();

	Submit([=]() {
		glReadBuffer(GL_FRONT);

		glReadPixels(0, 0, pboSpec.Width, pboSpec.Height, static_cast<uint32_t>(pboSpec.Order), static_cast<uint32_t>(pboSpec.Format), nullptr);
	});
}