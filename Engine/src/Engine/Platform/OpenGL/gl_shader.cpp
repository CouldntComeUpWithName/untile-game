#include "upch.h"
#include "gl_shader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Core/Log.h>

void utd::gl_shader::bind()
{
	glUseProgram(get_id());
}

void utd::gl_shader::unbind()
{
	glUseProgram(false);
}

void utd::gl_shader::set_int(const std::string_view name, int value)
{
	glUniform1i(glGetUniformLocation(get_id(), name.data()), value);
}

void utd::gl_shader::set_array(const std::string_view name, int* values, u32 count)
{
	glUniform1iv(glGetUniformLocation(get_id(), name.data()), count, values);
}

void utd::gl_shader::set_float(const std::string_view name, float value)
{
	glUniform1f(glGetUniformLocation(get_id(), name.data()), value);
}

void utd::gl_shader::set_float2(const std::string_view name, const glm::vec2& value)
{
	glUniform2f(glGetUniformLocation(get_id(), name.data()), value.x, value.y);
}

void utd::gl_shader::set_float3(const std::string_view name, const glm::vec3& value)
{
	glUniform3f(glGetUniformLocation(get_id(), name.data()), value.x, value.y, value.z);
}

void utd::gl_shader::set_float4(const std::string_view name, const glm::vec4& value)
{
	glUniform4f(glGetUniformLocation(get_id(), name.data()), value.x, value.y, value.z, value.w);
}

void utd::gl_shader::set_mat4(const std::string_view name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(get_id(), name.data()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void utd::gl_shader::source(const std::string& source_glsl)
{

}

void utd::gl_shader::filepath(const std::string& filepath_glsl)
{
}

void utd::gl_shader::source(const std::string& vert_source, const std::string& frag_source)
{
	u32 vertex, fragment;
	int success;

	const char* vert = vert_source.c_str();
	const char* frag = frag_source.c_str();

	vertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex, 1, &vert, nullptr);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		i8 info[512];
		glGetShaderInfoLog(vertex, 512, nullptr, info);
		UTD_ENGINE_ERROR("Vertex shader compilation failed: {0}", info);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment, 1, &frag, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		i8 info[512];
		glGetShaderInfoLog(fragment, 512, nullptr, info);
		UTD_ENGINE_ERROR("Fragment shader compilation failed: {0}", info);
	}

	m_id = glCreateProgram();

	glAttachShader(m_id, vertex);
	glAttachShader(m_id, fragment);

	glLinkProgram(m_id);

	glGetProgramiv(m_id, GL_LINK_STATUS, &success);

	if (!success)
	{
		i8 info[512];
		glGetProgramInfoLog(m_id, 512, nullptr, info);
		UTD_ENGINE_ERROR("Shader linkage failed: {0}", info);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

std::string read_file(const std::string& filepath, std::ios_base::openmode mode)
{
	std::ifstream file;
	file.open(filepath, mode);
	std::string source;

	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		auto size = file.tellg();
		file.seekg(0, std::ios::beg);
		source.resize(size, ' ');
		
		file.read(&source[0], size);
		
		return source;
	}

	return "";
}

std::string read_file(const std::string& filepath, std::ifstream& file_stream, std::ios_base::openmode mode)
{
	std::ifstream file;
	file.open(filepath, mode);
	std::string source;

	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		auto size = file.tellg();

		source.reserve(size);
		
		file.seekg(0);
		file.read(&source[0], size);
		file.close();
		
		return std::move(source);
	}

	return "";
}

std::string read_file1(const std::string& filepath)
{
	std::ifstream vShaderFile;
	std::stringstream sstream;
	vShaderFile.open(filepath);

	sstream << vShaderFile.rdbuf();
	
	return sstream.str();
}

void utd::gl_shader::filepath(const std::string& vert_path, const std::string& frag_path)
{
	std::string vert_source = read_file(vert_path, std::ios::in | std::ios::binary);
	std::string frag_source = read_file(frag_path, std::ios::in | std::ios::binary);

	/*std::string vert_source = read_file1(vert_path);
	std::string frag_source = read_file1(frag_path);*/

	u32 vertex, fragment;
	int success;

	auto* vert = vert_source.c_str();
	auto* frag = frag_source.c_str();

	vertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex, 1, &vert, nullptr);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char info[512];
		glGetShaderInfoLog(vertex, 512, nullptr, info);
		UTD_ENGINE_ERROR("Vertex shader compilation failed: {0}", info);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment, 1, &frag, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char info[512];
		glGetShaderInfoLog(fragment, 512, nullptr, info);
		UTD_ENGINE_ERROR("Fragment shader compilation failed: {0}", info);
	}

	m_id = glCreateProgram();

	glAttachShader(m_id, vertex);
	glAttachShader(m_id, fragment);

	glLinkProgram(m_id);

	glGetProgramiv(m_id, GL_LINK_STATUS, &success);

	if (!success)
	{
		i8 info[512];
		glGetProgramInfoLog(m_id, 512, nullptr, info);
		UTD_ENGINE_ERROR("Shader linkage failed: {0}", info);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
