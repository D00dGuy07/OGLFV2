#pragma once

#include "glm/glm.hpp"
#include "glad/glad.h"

#include <stdint.h>
#include <vector>

// Uniform::Set function should not be called outside of a render submit, it has bare OpenGL calls!! 

// Base class of all uniform types
class Uniform
{
public:
	virtual ~Uniform() {}

	virtual void Set(int32_t location) = 0;
};

// Uniform templates

template<typename T, typename FType, FType func, uint32_t count>
class UniformSingle;

template<typename T, typename FType, FType func>
class UniformSingle<T, FType, func, 1> : public Uniform
{
public:
	UniformSingle(T value)
		: m_Value(value) {}
	virtual ~UniformSingle() override {}

	virtual void Set(int32_t location) override
	{
		(*func)(location, m_Value);
	}
private:
	T m_Value;
};

template<typename T, typename FType, FType func>
class UniformSingle<T, FType, func, 2> : public Uniform
{
public:
	UniformSingle(T value)
		: m_Value(value) {}
	virtual ~UniformSingle() override {}

	virtual void Set(int32_t location) override
	{
		(*func)(location, m_Value.x, m_Value.y);
	}
private:
	T m_Value;
};

template<typename T, typename FType, FType func>
class UniformSingle<T, FType, func, 3> : public Uniform
{
public:
	UniformSingle(T value)
		: m_Value(value) {}
	virtual ~UniformSingle() override {}

	virtual void Set(int32_t location) override
	{
		(*func)(location, m_Value.x, m_Value.y, m_Value.z);
	}
private:
	T m_Value;
};

template<typename T, typename FType, FType func>
class UniformSingle<T, FType, func, 4> : public Uniform
{
public:
	UniformSingle(T value)
		: m_Value(value) {}
	virtual ~UniformSingle() override {}

	virtual void Set(int32_t location) override
	{
		(*func)(location, m_Value.x, m_Value.y, m_Value.z, m_Value.w);
	}
private:
	T m_Value;
};

template<typename T, typename DestT, typename FType, FType func>
class UniformArray : public Uniform
{
public:
	UniformArray(const T* values, uint32_t count)
		: m_Values(reinterpret_cast<T*>(malloc(static_cast<size_t>(count) * sizeof(T)))), m_Count(count)
	{
		if (m_Values != NULL)
			memcpy(m_Values, values, static_cast<size_t>(count) * sizeof(T));
	}
	virtual ~UniformArray() override { free(m_Values); }

	virtual void Set(int32_t location) override
	{
		(*func)(location, static_cast<int32_t>(m_Count), reinterpret_cast<DestT*>(m_Values));
	}
private:
	T* m_Values;
	uint32_t m_Count;
};

template<typename T, typename FType, FType func>
class UniformMatrix : public Uniform
{
public:
	UniformMatrix(const T* values, uint32_t count)
		: m_Values(reinterpret_cast<T*>(malloc(static_cast<size_t>(count) * sizeof(T)))), m_Count(count)
	{
		if (m_Values != NULL)
			memcpy(m_Values, values, static_cast<size_t>(count) * sizeof(T));
	}
	virtual ~UniformMatrix() override { free(m_Values); }

	virtual void Set(int32_t location) override
	{
		(*func)(location, static_cast<int32_t>(m_Count), GL_FALSE, reinterpret_cast<float*>(m_Values));
	}
private:
	T* m_Values;
	uint32_t m_Count;
};

typedef UniformSingle<float, decltype(glUniform1f)*, &glUniform1f, 1> Uniform1f;
typedef UniformSingle<glm::vec2, decltype(glUniform2f)*, &glUniform2f, 2> Uniform2f;
typedef UniformSingle<glm::vec3, decltype(glUniform3f)*, &glUniform3f, 3> Uniform3f;
typedef UniformSingle<glm::vec4, decltype(glUniform4f)*, &glUniform4f, 4> Uniform4f;

typedef UniformSingle<int32_t, decltype(glUniform1i)*, &glUniform1i, 1> Uniform1i;
typedef UniformSingle<glm::ivec2, decltype(glUniform2i)*, &glUniform2i, 2> Uniform2i;
typedef UniformSingle<glm::ivec3, decltype(glUniform3i)*, &glUniform3i, 3> Uniform3i;
typedef UniformSingle<glm::ivec4, decltype(glUniform4i)*, &glUniform4i, 4> Uniform4i;

typedef UniformSingle<uint32_t, decltype(glUniform1ui)*, &glUniform1ui, 1> Uniform1ui;
typedef UniformSingle<glm::uvec2, decltype(glUniform2ui)*, &glUniform2ui, 2> Uniform2ui;
typedef UniformSingle<glm::uvec3, decltype(glUniform3ui)*, &glUniform3ui, 3> Uniform3ui;
typedef UniformSingle<glm::uvec4, decltype(glUniform4ui)*, &glUniform4ui, 4> Uniform4ui;

typedef UniformArray<float, float, decltype(glUniform1fv)*, &glUniform1fv> Uniform1fv;
typedef UniformArray<glm::vec2, float, decltype(glUniform2fv)*, &glUniform2fv> Uniform2fv;
typedef UniformArray<glm::vec3, float, decltype(glUniform3fv)*, &glUniform3fv> Uniform3fv;
typedef UniformArray<glm::vec4, float, decltype(glUniform4fv)*, &glUniform4fv> Uniform4fv;

typedef UniformArray<int32_t, int32_t, decltype(glUniform1iv)*, &glUniform1iv> Uniform1iv;
typedef UniformArray<glm::ivec2, int32_t, decltype(glUniform2iv)*, &glUniform2iv> Uniform2iv;
typedef UniformArray<glm::ivec3, int32_t, decltype(glUniform3iv)*, &glUniform3iv> Uniform3iv;
typedef UniformArray<glm::ivec4, int32_t, decltype(glUniform4iv)*, &glUniform4iv> Uniform4iv;

typedef UniformArray<uint32_t, uint32_t, decltype(glUniform1uiv)*, &glUniform1uiv> Uniform1uiv;
typedef UniformArray<glm::uvec2, uint32_t, decltype(glUniform2uiv)*, &glUniform2uiv> Uniform2uiv;
typedef UniformArray<glm::uvec3, uint32_t, decltype(glUniform3uiv)*, &glUniform3uiv> Uniform3uiv;
typedef UniformArray<glm::uvec4, uint32_t, decltype(glUniform4uiv)*, &glUniform4uiv> Uniform4uiv;

typedef UniformMatrix<glm::mat2, decltype(glUniformMatrix2fv)*, &glUniformMatrix2fv> UniformMatrix2fv;
typedef UniformMatrix<glm::mat3, decltype(glUniformMatrix3fv)*, &glUniformMatrix3fv> UniformMatrix3fv;
typedef UniformMatrix<glm::mat4, decltype(glUniformMatrix4fv)*, &glUniformMatrix4fv> UniformMatrix4fv;

typedef UniformMatrix<glm::mat2x3, decltype(glUniformMatrix2x3fv)*, &glUniformMatrix2x3fv> UniformMatrix2x3fv;
typedef UniformMatrix<glm::mat3x2, decltype(glUniformMatrix3x2fv)*, &glUniformMatrix3x2fv> UniformMatrix3x2fv;
typedef UniformMatrix<glm::mat2x4, decltype(glUniformMatrix2x4fv)*, &glUniformMatrix2x4fv> UniformMatrix2x4fv;
typedef UniformMatrix<glm::mat4x2, decltype(glUniformMatrix4x2fv)*, &glUniformMatrix4x2fv> UniformMatrix4x2fv;
typedef UniformMatrix<glm::mat3x4, decltype(glUniformMatrix3x4fv)*, &glUniformMatrix3x4fv> UniformMatrix3x4fv;
typedef UniformMatrix<glm::mat4x3, decltype(glUniformMatrix4x3fv)*, &glUniformMatrix4x3fv> UniformMatrix4x3fv;

extern template Uniform1f;
extern template Uniform2f;
extern template Uniform3f;
extern template Uniform4f;

extern template Uniform1i;
extern template Uniform2i;
extern template Uniform3i;
extern template Uniform4i;

extern template Uniform1ui;
extern template Uniform2ui;
extern template Uniform3ui;
extern template Uniform4ui;

extern template Uniform1fv;
extern template Uniform2fv;
extern template Uniform3fv;
extern template Uniform4fv;

extern template Uniform1iv;
extern template Uniform2iv;
extern template Uniform3iv;
extern template Uniform4iv;

extern template Uniform1uiv;
extern template Uniform2uiv;
extern template Uniform3uiv;
extern template Uniform4uiv;

extern template UniformMatrix2fv;
extern template UniformMatrix3fv;
extern template UniformMatrix4fv;

extern template UniformMatrix2x3fv;
extern template UniformMatrix3x2fv;
extern template UniformMatrix2x4fv;
extern template UniformMatrix4x2fv;
extern template UniformMatrix3x4fv;
extern template UniformMatrix4x3fv;