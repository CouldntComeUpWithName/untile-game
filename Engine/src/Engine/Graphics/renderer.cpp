#include <upch.h>
#include "renderer.h"

#include <Engine/Platform/OpenGL/opengl_api.h>
#include <Engine/Core/assert.h>

std::uptr<utd::graphics_api> utd::renderer::s_graphics_api;
utd::graphics_api::type utd::renderer::s_api_type;

void utd::renderer::init(graphics_api::type _API)
{
    s_api_type = _API;

    switch(s_api_type)
    {
    case graphics_api::type::OPENGL:
        s_graphics_api = std::make_unique<utd::opengl_api>();
        return;
    case graphics_api::type::DX11:
        UTD_ENGINE_ASSERT(false, "Untiled Engine does NOT support DX11 yet!");
        return;
    case graphics_api::type::DX12:
        UTD_ENGINE_ASSERT(false, "Untiled Engine does NOT support DX12 yet!");
        return;
    case graphics_api::type::VULKAN:
        UTD_ENGINE_ASSERT(false, "Untiled Engine does NOT support VULKAN yet!");
        return;
    case graphics_api::type::UNKNOWN:
        UTD_ENGINE_ASSERT(false, "Unknown graphics API!");
        return;
    }
}

void utd::renderer::set_viewport(u32 x, u32 y, u32 width, u32 height)
{
    s_graphics_api->set_viewport(x, y, width, height);
}

void utd::renderer::set_clear_color(const glm::vec4& color)
{
    s_graphics_api->set_clear_color(color);
}

void utd::renderer::clear()
{
    s_graphics_api->clear();
}

void utd::renderer::draw_indexed(const utd::vertex_array& vertex_array, u32 count)
{
    s_graphics_api->draw_indexed(vertex_array, count);
}

void utd::renderer::draw_arrays(const vertex_array& vertex_array, u32 first, u32 count)
{
    s_graphics_api->draw_arrays(vertex_array, first, count);
}

void utd::renderer::depth_buffer(bool enabled)
{
    s_graphics_api->depth_buffer(enabled);
}