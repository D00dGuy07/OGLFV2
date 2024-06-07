#pragma once

#include <utility>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "oglfv2/Renderer/RenderCommandQueue.h"
#include "oglfv2/Renderer/Mesh.h"
#include "oglfv2/Renderer/Shader.h"

#include "Arrowhead/GarbageHeap.h"

class PixelBuffer;

class Renderer
{
public:
	enum class BlendFunction
	{
		Zero = GL_ZERO,
		One = GL_ONE,
		SrcColor = GL_SRC_COLOR,
		OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
		DstColor = GL_DST_COLOR,
		OneMinusDstCOlor = GL_ONE_MINUS_DST_COLOR,
		SrcAlpha = GL_SRC_ALPHA,
		OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
		DstAlpha = GL_DST_ALPHA,
		OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA,
		ConstantColor = GL_CONSTANT_COLOR,
		OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
		ConstantAlpha = GL_CONSTANT_ALPHA,
		OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA
	};

	enum class DepthFunction
	{
		Never = GL_NEVER,
		Less = GL_LESS,
		Equal = GL_EQUAL,
		LEqual = GL_LEQUAL,
		Greater = GL_GREATER,
		NotEqual = GL_NOTEQUAL,
		GEqual = GL_GEQUAL,
		Always = GL_ALWAYS
	};

	enum class CullMode
	{
		Front = GL_FRONT,
		Back = GL_BACK,
		Both = GL_FRONT_AND_BACK
	};

	enum class PolygonMode
	{
		Point = GL_POINT,
		Line = GL_LINE,
		Fill = GL_FILL
	};

	// This function is copied from the Hazel game engine
	template<typename FuncT>
	static void Submit(FuncT&& func)
	{
		auto renderCmd = [](void* ptr) {
			auto pFunc = (FuncT*)ptr;
			(*pFunc)();
			pFunc->~FuncT();
		};
		auto storageBuffer = m_CommandQueue.Allocate(renderCmd, sizeof(func));
		new (storageBuffer) FuncT(std::forward<FuncT>(func));
	}

	static void UseBlending(bool enable, BlendFunction source = BlendFunction::One, BlendFunction destination = BlendFunction::Zero);
	static void UseDepthFunction(bool enable, DepthFunction function = DepthFunction::Less);
	static void SetCullFace(bool enable, CullMode mode = CullMode::Back);

	static void SetPolygonMode(PolygonMode mode);

	static void SetDepthRange(float near, float far);
	static void SetDepthRange(double near, double far);

	static void SetClearColor(glm::vec4 color);

	static void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);
	static inline void SetViewport(int32_t width, int32_t height) { SetViewport(0, 0, width, height); }

	static void SubmitMesh(Mesh& mesh, Shader& shader);
	static void SubmitMeshInstanced(Mesh& mesh, Shader& shader, uint32_t instanceCount);

	static void Clear();
	static void DrawFrame();

	static void PackPBO(PixelBuffer& pixelBuffer);

	template<typename T, typename... Args>
	static T* CreateGarbage(Args&&... args)
	{
		return new(m_GarbageHeap.Allocate<T>()) T(std::forward<Args>(args)...);
	}

private:
	inline static RenderCommandQueue m_CommandQueue = RenderCommandQueue();
	inline static arwh::GarbageHeap m_GarbageHeap = arwh::GarbageHeap(1024 * 512);
};
