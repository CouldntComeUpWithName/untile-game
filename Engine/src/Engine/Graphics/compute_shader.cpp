#include<upch.h>
#include "compute_shader.h"
#include <glad/glad.h>
#include<Platform/Common/iosystem.h>
#include<Engine/Core/buffer.h>
void checkCompileErrors(GLuint shader, std::string type);

static inline std::string read_file(const std::string& filepath, std::ios_base::openmode mode)
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

void utd::compute_shader::load(const std::filesystem::path &file)
{
    auto source = read_file(file.string(), std::ios_base::in | std::ios_base::binary);
    const char* csource = source.c_str();

    unsigned int compute;

    compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &csource, nullptr);
    glCompileShader(compute);

    checkCompileErrors(compute, "Compute");

    m_id = glCreateProgram();
    glAttachShader(m_id, compute);
    glLinkProgram(m_id);
   
    checkCompileErrors(m_id, "PROGRAM");
    
    glDeleteShader(compute);

}

void utd::compute_shader::bind() const
{
    glUseProgram(m_id);
}

void utd::compute_shader::dispatch(u32 x, u32 y, u32 z)
{
    glDispatchCompute(x, y, z);
}

void checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
