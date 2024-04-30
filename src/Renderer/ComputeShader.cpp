#include "Renderer/ComputeShader.h"

#include "Renderer/Renderer.h"

#include <fstream>
#include <sstream>
#include <iostream>

glm::ivec3 ComputeShader::MaxWorkGroupCount(0, 0, 0);
glm::ivec3 ComputeShader::MaxWorkGroupSize(0, 0, 0);

ComputeShader::ComputeShader(const std::string& filepath, bool binary)
	: m_FilePath(filepath)
{
	if (binary)
	{
		Renderer::Submit([&]() {
			// Process for loading binary is very abstracted :)
			m_RendererID = glCreateProgram();

			ShaderBinary binaryData = ShaderBinary::LoadFile(m_FilePath);
			LoadBinary(binaryData);
			binaryData.Free();
		});
	}
	else
	{
		Renderer::Submit([&]() {
			std::string source = std::move(LoadShaderFile());
			m_RendererID = CreateShader(source);
		});
	}
}

std::string ComputeShader::LoadShaderFile()
{
	std::ifstream file(m_FilePath);

	if (!file.good())
		std::cout << "[Compute Shader Compilation] Failed to open file " << m_FilePath << "\n";

	std::string line;
	std::stringstream stream;
	bool foundTag = false;
	while (getline(file, line))
	{
		// This first condition should in theory make it not check for
		// the tag if it has already found it
		if (!foundTag && line.find("#shader compute") != std::string::npos)
		{
			foundTag = true;
			continue; // Don't add the tag to the source
		}
		
		stream << line << '\n';
	}

	if (!foundTag)
		std::cout << "[Compute Shader Compilation] Could not verify that " << m_FilePath << " is a compute shader!\n";;

	return stream.str();
}

uint32_t ComputeShader::CreateShader(const std::string& shaderSource)
{
	uint32_t program = glCreateProgram();
	uint32_t cs = CompileShader(GL_COMPUTE_SHADER, shaderSource, m_FilePath + " (Compute Shader)");

	glAttachShader(program, cs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(cs);

	return program;
}

void ComputeShader::Dispatch(uint32_t x, uint32_t y, uint32_t z)
{
	glm::ivec3 maxCount = GetMaxWorkGroupCount();

	// Check if work group size is even possible
	if (x > static_cast<uint32_t>(maxCount.x) || y > static_cast<uint32_t>(maxCount.y) || z > static_cast<uint32_t>(maxCount.z))
	{
		std::cout << "Cannot dispatch compute shader " << m_FilePath << " with work group count of ("
			<< x << ", " << y << ", " << z << ") because the max work group count is ("
			<< maxCount.x << ", " << maxCount.y << ", " << maxCount.z << ")\n";
		return;
	}

	Bind();

	Renderer::Submit([=]() {
		glDispatchCompute(x, y, z);
	});
}

glm::ivec3 ComputeShader::GetMaxWorkGroupCount()
{
	if (MaxWorkGroupCount.x == 0 && MaxWorkGroupCount.y == 0 && MaxWorkGroupCount.z == 0)
	{
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &MaxWorkGroupCount.x);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &MaxWorkGroupCount.y);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &MaxWorkGroupCount.z);
	}

	return MaxWorkGroupCount;
}

glm::ivec3 ComputeShader::GetMaxWorkGroupSize()
{
	if (MaxWorkGroupSize.x == 0 && MaxWorkGroupSize.y == 0 && MaxWorkGroupSize.z == 0)
	{
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &MaxWorkGroupSize.x);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &MaxWorkGroupSize.y);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &MaxWorkGroupSize.z);
	}

	return MaxWorkGroupSize;
}