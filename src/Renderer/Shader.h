#pragma once

#include "Renderer/ShaderBase.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader : public ShaderBase
{
public:
	Shader(const std::string& filepath, bool binary);
	Shader(const std::string& vertex, const std::string& fragment);

private:
	std::string m_FilePath;

	static ShaderProgramSource ParseShader(const std::string& filePath);
	uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};