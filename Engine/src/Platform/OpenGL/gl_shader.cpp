#include "upch.h"
#include "gl_shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Core/Log.h>
#include <Engine/Core/Assert.h>
#include <Engine/Profiling/Profile.h>
#include <Platform/Common/iosystem.h>
#include <Engine/Core/Buffer.h>

utd::gl_shader::gl_shader(const std::filesystem::path &vertex, const std::filesystem::path &fragment)
{
    _filepath(vertex, fragment);
}

void utd::gl_shader::bind() const
{
    UTD_PROFILE_FUNC();
    glUseProgram(get_id());
}

void utd::gl_shader::unbind() const
{
    UTD_PROFILE_FUNC();
    glUseProgram(false);
}

void utd::gl_shader::integer(const std::string_view name, int value) const
{
	UTD_PROFILE_FUNC();

	auto location = glGetUniformLocation(get_id(), name.data());

	UTD_ENGINE_ASSERT(location != -1, "The uniform location doesn\'t exist!");

	glUniform1i(location, value);
}

void utd::gl_shader::array(const std::string_view name, int* values, u32 count) const
{
	UTD_PROFILE_FUNC();
	
	auto location = glGetUniformLocation(get_id(), name.data());

	UTD_ENGINE_ASSERT(location != -1, "The uniform location doesn\'t exist!");

	glUniform1iv(location, count, values);
}

void utd::gl_shader::real(const std::string_view name, float value) const
{
	UTD_PROFILE_FUNC();

	auto location = glGetUniformLocation(get_id(), name.data());

	UTD_ENGINE_ASSERT(location != -1, "The uniform location doesn\'t exist!");


	glUniform1f(location, value);
}

void utd::gl_shader::vec2(const std::string_view name, const glm::vec2& value) const
{
	UTD_PROFILE_FUNC();

	auto location = glGetUniformLocation(get_id(), name.data());

	UTD_ENGINE_ASSERT(location != -1, "The uniform location doesn\'t exist!");


	glUniform2f(location, value.x, value.y);
}

void utd::gl_shader::vec3(const std::string_view name, const glm::vec3& value) const
{
	UTD_PROFILE_FUNC();

	auto location = glGetUniformLocation(get_id(), name.data());

	UTD_ENGINE_ASSERT(location != -1, "The uniform location doesn\'t exist!");


	glUniform3f(location, value.x, value.y, value.z);
}

void utd::gl_shader::vec4(const std::string_view name, const glm::vec4& value) const
{
	UTD_PROFILE_FUNC();

	auto location = glGetUniformLocation(get_id(), name.data());

	UTD_ENGINE_ASSERT(location != -1, "The uniform location doesn\'t exist!");


	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void utd::gl_shader::mat4(const std::string_view name, const glm::mat4& matrix) const
{
	UTD_PROFILE_FUNC();
	
	auto location = glGetUniformLocation(get_id(), name.data());
	
	UTD_ENGINE_ASSERT(location != -1, "The uniform location doesn\'t exist!");
	
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
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

	cstring vert = vert_source.c_str();
	cstring frag = frag_source.c_str();

	vertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex, 1, &vert, nullptr);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char info[512];
		glGetShaderInfoLog(vertex, 512, nullptr, info);
		UTD_ERROR("Vertex shader compilation failed: {0}", info);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment, 1, &frag, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char info[512];
		glGetShaderInfoLog(fragment, 512, nullptr, info);
		UTD_ERROR("Fragment shader compilation failed: {0}", info);
	}

	m_id = glCreateProgram();

	glAttachShader(m_id, vertex);
	glAttachShader(m_id, fragment);

	glLinkProgram(m_id);

	glGetProgramiv(m_id, GL_LINK_STATUS, &success);

	if (!success)
	{
		char info[512];
		glGetProgramInfoLog(m_id, 512, nullptr, info);
		UTD_ERROR("Shader linkage failed: {0}", info);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

std::string read_file(const std::string& filepath, std::ios_base::openmode mode)
{
	UTD_PROFILE_FUNC(utd::profile::color::deepgreen);

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

void utd::gl_shader::filepath(const std::string& vert_path, const std::string& frag_path)
{
	UTD_PROFILE_FUNC(profile::color::deeprose);
	_filepath(vert_path, frag_path);
}

void utd::gl_shader::_filepath(const std::filesystem::path &vert_path, const std::filesystem::path &frag_path)
{
	
	auto vert_source = read_file(vert_path.string(), std::ios::in | std::ios::binary);
	auto frag_source = read_file(frag_path.string(), std::ios::in | std::ios::binary);

	u32 vertex, fragment;
	int success;
	cstring vert = vert_source.c_str();
	const char* frag = frag_source.c_str();

	vertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex, 1, &vert, nullptr);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char info[512];
		glGetShaderInfoLog(vertex, 512, nullptr, info);
		UTD_ERROR("Vertex shader compilation failed: {0}", info);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment, 1, &frag, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char info[512];
		glGetShaderInfoLog(fragment, 512, nullptr, info);
		UTD_ERROR("Fragment shader compilation failed: {0}", info);
	}

	m_id = glCreateProgram();

	glAttachShader(m_id, vertex);
	glAttachShader(m_id, fragment);

	glLinkProgram(m_id);

	glGetProgramiv(m_id, GL_LINK_STATUS, &success);

	if (!success)
	{
		char info[512];
		glGetProgramInfoLog(m_id, 512, nullptr, info);
		UTD_ERROR("Shader linkage failed: {0}", info);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
