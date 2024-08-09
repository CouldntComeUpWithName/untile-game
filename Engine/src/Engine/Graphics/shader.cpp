#include "upch.h"
#include "shader.h"

#include <Engine/Platform/OpenGL/gl_shader.h>

std::uptr<utd::shader> utd::shader::create(const std::string& vertex, const std::string& fragment)
{
    std::uptr<utd::shader> sh = std::make_unique<utd::gl_shader>();
    sh->source(vertex, fragment);
    return sh;
    
    /* switch (renderer::API::get())
    {
        case renderer::API::OPENGL:
        {
            std::uptr<shader> shader = std::make_unique<gl_shader>();
            shader->source(vertex, fragment);
            return shader;
        }
    case renderer::API::DX11:
    case renderer::API::VULKAN:
    }*/
}

std::uptr<utd::shader> utd::shader::load(const std::string &vertex_path, const std::string &frag_path)
{
    std::uptr<utd::shader> sh = std::make_unique<utd::gl_shader>();
    sh->filepath(vertex_path, frag_path);
    return sh;
}
