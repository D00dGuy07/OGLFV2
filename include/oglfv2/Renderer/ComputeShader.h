#pragma once

#include "oglfv2/Renderer/ShaderBase.h"

class ComputeShader : public ShaderBase
{
public:
	ComputeShader(const std::string& filepath, bool binary);

	void Dispatch(uint32_t x, uint32_t y, uint32_t z);

	static glm::ivec3 GetMaxWorkGroupCount();
	static glm::ivec3 GetMaxWorkGroupSize();

private:
	std::string m_FilePath;

	std::string LoadShaderFile();
	uint32_t CreateShader(const std::string& shaderSource);

	inline static glm::ivec3 MaxWorkGroupCount = glm::ivec3(0, 0, 0);
	inline static glm::ivec3 MaxWorkGroupSize = glm::ivec3(0, 0, 0);
};