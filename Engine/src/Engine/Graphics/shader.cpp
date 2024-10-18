#include "upch.h"
#include "shader.h"

#include <Engine/Core/Assert.h>

#include <Platform/OpenGL/gl_shader.h>
#include <Engine/Graphics/renderer.h>

std::uptr<utd::shader> utd::shader::create(const std::string& vertex, const std::string& fragment)
{
    std::uptr<utd::shader> shader = nullptr;

    switch (utd::renderer::API())
    {
    case utd::graphics_api::type::OPENGL: shader = std::make_unique<gl_shader>(); break;
    case utd::graphics_api::type::DX11:   UTD_ENGINE_ASSERT(false, "not supported yet"); break;
    case utd::graphics_api::type::DX12:   UTD_ENGINE_ASSERT(false, "not supported yet"); break;
    case utd::graphics_api::type::VULKAN: UTD_ENGINE_ASSERT(false, "not supported yet"); break;
    }

    UTD_ENGINE_ASSERT(utd::renderer::API() != utd::graphics_api::type::UNKNOWN);
    
    shader->source(vertex, fragment);
    return shader;
}

std::uptr<utd::shader> utd::shader::load(const std::string &vertex_path, const std::string &frag_path)
{
    std::uptr<utd::shader> sh = std::make_unique<utd::gl_shader>();
    sh->filepath(vertex_path, frag_path);
    return sh;
}
