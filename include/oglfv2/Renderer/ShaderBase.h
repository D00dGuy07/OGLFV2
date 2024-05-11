#pragma once

#include "oglfv2/Renderer/Uniform.h"

#include "glm/glm.hpp"

#include <unordered_map>
#include <vector>
#include <fstream>

struct ShaderBinary
{
	int32_t Length;
	uint32_t Format;
	uint8_t* Data;

	// Must be called to prevent memory leak any time a valid
	// data pointer is stored
	void Free();

	void WriteFile(const std::string& filePath);
	void WriteStream(std::ostream& stream);
	static ShaderBinary LoadFile(const std::string& filePath);
	static ShaderBinary LoadStream(std::istream& stream);
};

// ShaderBase class contains functions shared by compute
// and render shaders such as those necessary for uniform usage
class ShaderBase
{
public:

	ShaderBase();
	virtual ~ShaderBase(); // Deriving destructor is unessecary unless for functionality

	// Free must be called on the return value of this function
	ShaderBinary GetBinary();

	// Misc
	void Bind();

	// Uniform Functions
	inline void SetUniform1f(const std::string& name, const float& value)
		{ SetUniform<float, Uniform1f>(name, value); }
	inline void SetUniform2f(const std::string& name, const glm::vec2& value)
		{ SetUniform<glm::vec2, Uniform2f>(name, value); }
	inline void SetUniform3f(const std::string& name, const glm::vec3& value)
		{ SetUniform<glm::vec3, Uniform3f>(name, value); }
	inline void SetUniform4f(const std::string& name, const glm::vec4& value)
		{ SetUniform<glm::vec4, Uniform4f>(name, value); }

	inline void SetUniform1i(const std::string& name, const int32_t& value)
		{ SetUniform<int32_t, Uniform1i>(name, value); }
	inline void SetUniform2i(const std::string& name, const glm::ivec2& value)
		{ SetUniform<glm::ivec2, Uniform2i>(name, value); }
	inline void SetUniform3i(const std::string& name, const glm::ivec3& value)
		{ SetUniform<glm::ivec3, Uniform3i>(name, value); }
	inline void SetUniform4i(const std::string& name, const glm::ivec4& value)
		{ SetUniform<glm::ivec4, Uniform4i>(name, value); }

	inline void SetUniform1ui(const std::string& name, const uint32_t& value)
		{ SetUniform<uint32_t, Uniform1ui>(name, value); }
	inline void SetUniform2ui(const std::string& name, const glm::uvec2& value)
		{ SetUniform<glm::uvec2, Uniform2ui>(name, value); }
	inline void SetUniform3ui(const std::string& name, const glm::uvec3& value)
		{ SetUniform<glm::uvec3, Uniform3ui>(name, value); }
	inline void SetUniform4ui(const std::string& name, const glm::uvec4& value)
		{ SetUniform<glm::uvec4, Uniform4ui>(name, value); }

	inline void SetUniform1fv(const std::string& name, const float* values, uint32_t count)
		{ SetUniformArray<float, Uniform1fv>(name, values, count); }
	inline void SetUniform2fv(const std::string& name, const glm::vec2* values, uint32_t count)
		{ SetUniformArray<glm::vec2, Uniform2fv>(name, values, count); }
	inline void SetUniform3fv(const std::string& name, const glm::vec3* values, uint32_t count)
		{ SetUniformArray<glm::vec3, Uniform3fv>(name, values, count); }
	inline void SetUniform4fv(const std::string& name, const glm::vec4* values, uint32_t count)
		{ SetUniformArray<glm::vec4, Uniform4fv>(name, values, count); }

	inline void SetUniform1iv(const std::string& name, const int32_t* values, uint32_t count)
		{ SetUniformArray<int32_t, Uniform1iv>(name, values, count); }
	inline void SetUniform2iv(const std::string& name, const glm::ivec2* values, uint32_t count)
		{ SetUniformArray<glm::ivec2, Uniform2iv>(name, values, count); }
	inline void SetUniform3iv(const std::string& name, const glm::ivec3* values, uint32_t count)
		{ SetUniformArray<glm::ivec3, Uniform3iv>(name, values, count); }
	inline void SetUniform4iv(const std::string& name, const glm::ivec4* values, uint32_t count)
		{ SetUniformArray<glm::ivec4, Uniform4iv>(name, values, count); }

	inline void SetUniform1uiv(const std::string& name, const uint32_t* values, uint32_t count)
		{ SetUniformArray<uint32_t, Uniform1uiv>(name, values, count); }
	inline void SetUniform2uiv(const std::string& name, const glm::uvec2* values, uint32_t count)
		{ SetUniformArray<glm::uvec2, Uniform2uiv>(name, values, count); }
	inline void SetUniform3uiv(const std::string& name, const glm::uvec3* values, uint32_t count)
		{ SetUniformArray<glm::uvec3, Uniform3uiv>(name, values, count); }
	inline void SetUniform4uiv(const std::string& name, const glm::uvec4* values, uint32_t count)
		{ SetUniformArray<glm::uvec4, Uniform4uiv>(name, values, count); }

	// OpenGL only has uniform functions for arrays of matrices so I only made
	// matrix uniform classes for arrays. These inline functions convert a single
	// matrix into an array and call the relevant function for convenience
	inline void SetUniformMatrix2fv(const std::string& name, const glm::mat2* values, uint32_t count)
		{ SetUniformArray<glm::mat2, UniformMatrix2fv>(name, values, count); }
	inline void SetUniformMatrix3fv(const std::string& name, const glm::mat3* values, uint32_t count)
		{ SetUniformArray<glm::mat3, UniformMatrix3fv>(name, values, count); }
	inline void SetUniformMatrix4fv(const std::string& name, const glm::mat4* values, uint32_t count)
		{ SetUniformArray<glm::mat4, UniformMatrix4fv>(name, values, count); }

	inline void SetUniformMatrix2x3fv(const std::string& name, const glm::mat2x3* values, uint32_t count)
		{ SetUniformArray<glm::mat2x3, UniformMatrix2x3fv>(name, values, count); }
	inline void SetUniformMatrix3x2fv(const std::string& name, const glm::mat3x2* values, uint32_t count)
		{ SetUniformArray<glm::mat3x2, UniformMatrix3x2fv>(name, values, count); }
	inline void SetUniformMatrix2x4fv(const std::string& name, const glm::mat2x4* values, uint32_t count)
		{ SetUniformArray<glm::mat2x4, UniformMatrix2x4fv>(name, values, count); }
	inline void SetUniformMatrix4x2fv(const std::string& name, const glm::mat4x2* values, uint32_t count)
		{ SetUniformArray<glm::mat4x2, UniformMatrix4x2fv>(name, values, count); }
	inline void SetUniformMatrix3x4fv(const std::string& name, const glm::mat3x4* values, uint32_t count)
		{ SetUniformArray<glm::mat3x4, UniformMatrix3x4fv>(name, values, count); }
	inline void SetUniformMatrix4x3fv(const std::string& name, const glm::mat4x3* values, uint32_t count)
		{ SetUniformArray<glm::mat4x3, UniformMatrix4x3fv>(name, values, count); }

protected:
	// A lot of the functions in here don't technically need to be protected and it might
	// the shader api more flexible if it isn't but for now it makes it a little cleaner

	uint32_t m_RendererID = 0;
	// Having BoundRendererID here assumes that all shaders use same binding points
	// and methods which is currently true for all shader types that I know of
	static ShaderBase* m_BoundShader;

	// Uniform caching
	std::unordered_map<std::string, int> m_UniformLocationCache;
	std::unordered_map<std::string, Uniform*> m_UniformCache;
	bool m_CachedUniforms = false;

	int GetUniformLocation(const std::string& name);
	void SetUniforms();

	// Shader compilation
	static uint32_t CompileShader(uint32_t type, const std::string& source, const std::string& identifier);
	static uint32_t CheckErrors(uint32_t shaderHandle, const std::string& identifier);

	// Misc
	void LoadBinary(ShaderBinary binary);

	// Uniform Templates

	template<typename T, typename uniformType>
	void SetUniform(const std::string& name, const T& value);

	template<typename T, typename uniformType>
	void SetUniformArray(const std::string& name, const T* value, uint32_t count);
};

extern template void ShaderBase::SetUniform<float, Uniform1f>(const std::string& name, const float& value);
extern template void ShaderBase::SetUniform<glm::vec2, Uniform2f>(const std::string& name, const glm::vec2& value);
extern template void ShaderBase::SetUniform<glm::vec3, Uniform3f>(const std::string& name, const glm::vec3& value);
extern template void ShaderBase::SetUniform<glm::vec4, Uniform4f>(const std::string& name, const glm::vec4& value);

extern template void ShaderBase::SetUniform<int32_t, Uniform1i>(const std::string& name, const int32_t& value);
extern template void ShaderBase::SetUniform<glm::ivec2, Uniform2i>(const std::string& name, const glm::ivec2& value);
extern template void ShaderBase::SetUniform<glm::ivec3, Uniform3i>(const std::string& name, const glm::ivec3& value);
extern template void ShaderBase::SetUniform<glm::ivec4, Uniform4i>(const std::string& name, const glm::ivec4& value);

extern template void ShaderBase::SetUniform<uint32_t, Uniform1ui>(const std::string& name, const uint32_t& value);
extern template void ShaderBase::SetUniform<glm::uvec2, Uniform2ui>(const std::string& name, const glm::uvec2& value);
extern template void ShaderBase::SetUniform<glm::uvec3, Uniform3ui>(const std::string& name, const glm::uvec3& value);
extern template void ShaderBase::SetUniform<glm::uvec4, Uniform4ui>(const std::string& name, const glm::uvec4& value);

extern template void ShaderBase::SetUniformArray<float, Uniform1fv>(const std::string& name, const float* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::vec2, Uniform2fv>(const std::string& name, const glm::vec2* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::vec3, Uniform3fv>(const std::string& name, const glm::vec3* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::vec4, Uniform4fv>(const std::string& name, const glm::vec4* value, uint32_t count);

extern template void ShaderBase::SetUniformArray<int32_t, Uniform1iv>(const std::string& name, const int32_t* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::ivec2, Uniform2iv>(const std::string& name, const glm::ivec2* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::ivec3, Uniform3iv>(const std::string& name, const glm::ivec3* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::ivec4, Uniform4iv>(const std::string& name, const glm::ivec4* value, uint32_t count);

extern template void ShaderBase::SetUniformArray<uint32_t, Uniform1uiv>(const std::string& name, const uint32_t* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::uvec2, Uniform2uiv>(const std::string& name, const glm::uvec2* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::uvec3, Uniform3uiv>(const std::string& name, const glm::uvec3* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::uvec4, Uniform4uiv>(const std::string& name, const glm::uvec4* value, uint32_t count);

extern template void ShaderBase::SetUniformArray<glm::mat2, UniformMatrix2fv>(const std::string& name, const glm::mat2* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::mat3, UniformMatrix3fv>(const std::string& name, const glm::mat3* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::mat4, UniformMatrix4fv>(const std::string& name, const glm::mat4* value, uint32_t count);

extern template void ShaderBase::SetUniformArray<glm::mat2x3, UniformMatrix2x3fv>(const std::string& name, const glm::mat2x3* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::mat3x2, UniformMatrix3x2fv>(const std::string& name, const glm::mat3x2* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::mat2x4, UniformMatrix2x4fv>(const std::string& name, const glm::mat2x4* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::mat4x2, UniformMatrix4x2fv>(const std::string& name, const glm::mat4x2* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::mat3x4, UniformMatrix3x4fv>(const std::string& name, const glm::mat3x4* value, uint32_t count);
extern template void ShaderBase::SetUniformArray<glm::mat4x3, UniformMatrix4x3fv>(const std::string& name, const glm::mat4x3* value, uint32_t count);