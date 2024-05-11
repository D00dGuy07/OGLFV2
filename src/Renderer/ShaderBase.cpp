#include "oglfv2/Renderer/ShaderBase.h"

#include "oglfv2/Renderer/Renderer.h"

#include "glad/glad.h"

#include <iostream>
#include <fstream>

ShaderBase* ShaderBase::m_BoundShader = nullptr;

ShaderBase::ShaderBase()
	: m_UniformLocationCache() {}

ShaderBase::~ShaderBase()
{
	if (m_BoundShader == this)
		m_BoundShader = nullptr;

	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		glDeleteProgram(id);
	});
}

// Shader Binary

void ShaderBinary::Free()
{
	free(Data);
	Data = NULL;
}

void ShaderBinary::WriteFile(const std::string& filePath)
{
	if (!Data)
		return;

	std::ofstream file(filePath, std::ios::trunc | std::ios::binary);

	// Minimal error checking
	if (!file.good())
		return;

	WriteStream(file);

	file.close();
}

void ShaderBinary::WriteStream(std::ostream& stream)
{
	// I'm not sure if reordering for little/big endian is important here
	stream.write(reinterpret_cast<char*>(&Length), sizeof(int32_t));
	stream.write(reinterpret_cast<char*>(&Format), sizeof(uint32_t));
	stream.write(reinterpret_cast<char*>(Data), Length);
}

ShaderBinary ShaderBinary::LoadFile(const std::string& filePath)
{
	std::ifstream file(filePath, std::ios::binary);

	// Will return NULL data if it can't open the file
	if (!file.good())
		return { 0, 0, NULL };

	ShaderBinary binary = LoadStream(file);

	file.close();
	return binary;
}

ShaderBinary ShaderBinary::LoadStream(std::istream& stream)
{
	ShaderBinary binary = { 0, 0, NULL };
	stream.read(reinterpret_cast<char*>(&binary.Length), sizeof(int32_t));
	stream.read(reinterpret_cast<char*>(&binary.Format), sizeof(uint32_t));

	binary.Data = static_cast<uint8_t*>(malloc(static_cast<size_t>(binary.Length)));
	stream.read(reinterpret_cast<char*>(binary.Data), binary.Length);

	return binary;
}

ShaderBinary ShaderBase::GetBinary()
{
	int32_t binaryLength = 0;
	glGetProgramiv(m_RendererID, GL_PROGRAM_BINARY_LENGTH, &binaryLength);

	// Could probably put binaryLength in here but im filling it with glGetProgramBinary anyway
	ShaderBinary binaryStruct = { 0, 0, static_cast<uint8_t*>(malloc(binaryLength)) };
	glGetProgramBinary(m_RendererID, binaryLength, &binaryStruct.Length, &binaryStruct.Format, binaryStruct.Data);

	return binaryStruct;
}

void ShaderBase::LoadBinary(ShaderBinary binary)
{
	glProgramBinary(m_RendererID, binary.Format, binary.Data, binary.Length);
	glValidateProgram(m_RendererID);
}

// Shader compilations

uint32_t ShaderBase::CompileShader(uint32_t type, const std::string& source, const std::string& identifier)
{
	uint32_t shader = glCreateShader(type);
	const char* src = source.c_str(); // glShaderSource takes a pointer to a pointer
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	return CheckErrors(shader, identifier);
}

uint32_t ShaderBase::CheckErrors(uint32_t shaderHandle, const std::string& identifier)
{
	int32_t result = 0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		int32_t length = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
		char* message = static_cast<char*>(_malloca(length));
		glGetShaderInfoLog(shaderHandle, length, &length, message);

		// Could query GL_SHADER_TYPE for more error info
		std::cout << "[Shader Compilation] Compilation of " << identifier << " failed: \n\t" << message << '\n';
		_freea(message);

		glDeleteShader(shaderHandle);
		return 0;
	}

	return shaderHandle;
}

// Misc

void ShaderBase::Bind()
{
	if (m_BoundShader == this)
		return;
	m_BoundShader = this;

	Renderer::Submit([=]() {
		glUseProgram(m_RendererID);
	});

	if (m_CachedUniforms)
		SetUniforms();
}

// Uniform Setting

int ShaderBase::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}

void ShaderBase::SetUniforms()
{
	// Submits a render function that will set any uniforms that are cached at the
	// time of this render function being called in the render queue, not the uniforms
	// cached when SetUniforms is called
	Renderer::Submit([=]() {
		if (!m_CachedUniforms)
			return;

		for (auto& uPair : m_UniformCache)
		{
			uPair.second->Set(GetUniformLocation(uPair.first));
			delete uPair.second;
		}
		m_UniformCache.clear();

		m_CachedUniforms = false;
	});
}

template<typename T, typename uniformType>
void ShaderBase::SetUniform(const std::string& name, const T& value)
{
	if (m_BoundShader == this)
	{
		Renderer::Submit([=]() {
			uniformType(value).Set(GetUniformLocation(name));
		});
		return;
	}

	delete m_UniformCache[name];
	m_UniformCache[name] = new uniformType(value);
	m_CachedUniforms = true;
}

template<typename T, typename uniformType>
void ShaderBase::SetUniformArray(const std::string& name, const T* value, uint32_t count)
{
	if (m_BoundShader == this)
	{
		Renderer::Submit([=]() {
			uniformType(value, count).Set(GetUniformLocation(name));
		});
		return;
	}

	delete m_UniformCache[name];
	m_UniformCache[name] = new uniformType(value, count);
	m_CachedUniforms = true;
}

template void ShaderBase::SetUniform<float, Uniform1f>(const std::string& name, const float& value);
template void ShaderBase::SetUniform<glm::vec2, Uniform2f>(const std::string& name, const glm::vec2& value);
template void ShaderBase::SetUniform<glm::vec3, Uniform3f>(const std::string& name, const glm::vec3& value);
template void ShaderBase::SetUniform<glm::vec4, Uniform4f>(const std::string& name, const glm::vec4& value);

template void ShaderBase::SetUniform<int32_t, Uniform1i>(const std::string& name, const int32_t& value);
template void ShaderBase::SetUniform<glm::ivec2, Uniform2i>(const std::string& name, const glm::ivec2& value);
template void ShaderBase::SetUniform<glm::ivec3, Uniform3i>(const std::string& name, const glm::ivec3& value);
template void ShaderBase::SetUniform<glm::ivec4, Uniform4i>(const std::string& name, const glm::ivec4& value);

template void ShaderBase::SetUniform<uint32_t, Uniform1ui>(const std::string& name, const uint32_t& value);
template void ShaderBase::SetUniform<glm::uvec2, Uniform2ui>(const std::string& name, const glm::uvec2& value);
template void ShaderBase::SetUniform<glm::uvec3, Uniform3ui>(const std::string& name, const glm::uvec3& value);
template void ShaderBase::SetUniform<glm::uvec4, Uniform4ui>(const std::string& name, const glm::uvec4& value);

template void ShaderBase::SetUniformArray<float, Uniform1fv>(const std::string& name, const float* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::vec2, Uniform2fv>(const std::string& name, const glm::vec2* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::vec3, Uniform3fv>(const std::string& name, const glm::vec3* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::vec4, Uniform4fv>(const std::string& name, const glm::vec4* value, uint32_t count);

template void ShaderBase::SetUniformArray<int32_t, Uniform1iv>(const std::string& name, const int32_t* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::ivec2, Uniform2iv>(const std::string& name, const glm::ivec2* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::ivec3, Uniform3iv>(const std::string& name, const glm::ivec3* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::ivec4, Uniform4iv>(const std::string& name, const glm::ivec4* value, uint32_t count);

template void ShaderBase::SetUniformArray<uint32_t, Uniform1uiv>(const std::string& name, const uint32_t* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::uvec2, Uniform2uiv>(const std::string& name, const glm::uvec2* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::uvec3, Uniform3uiv>(const std::string& name, const glm::uvec3* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::uvec4, Uniform4uiv>(const std::string& name, const glm::uvec4* value, uint32_t count);

template void ShaderBase::SetUniformArray<glm::mat2, UniformMatrix2fv>(const std::string& name, const glm::mat2* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::mat3, UniformMatrix3fv>(const std::string& name, const glm::mat3* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::mat4, UniformMatrix4fv>(const std::string& name, const glm::mat4* value, uint32_t count);

template void ShaderBase::SetUniformArray<glm::mat2x3, UniformMatrix2x3fv>(const std::string& name, const glm::mat2x3* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::mat3x2, UniformMatrix3x2fv>(const std::string& name, const glm::mat3x2* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::mat2x4, UniformMatrix2x4fv>(const std::string& name, const glm::mat2x4* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::mat4x2, UniformMatrix4x2fv>(const std::string& name, const glm::mat4x2* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::mat3x4, UniformMatrix3x4fv>(const std::string& name, const glm::mat3x4* value, uint32_t count);
template void ShaderBase::SetUniformArray<glm::mat4x3, UniformMatrix4x3fv>(const std::string& name, const glm::mat4x3* value, uint32_t count);