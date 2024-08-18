#include <upch.h>
#include "renderer.h"

#include <Engine/Core/assert.h>
#include <Engine/Graphics/renderer2d.h>

std::uptr<utd::graphics_api> utd::renderer::s_graphics_api;
utd::graphics_api::type utd::renderer::s_api_type;

void utd::renderer::init(graphics_api::type _API)
{
    s_api_type = _API;
    
    s_graphics_api = graphics_api::create(s_api_type);
    s_graphics_api->init();

    utd::renderer2d::init();
}

void utd::renderer::command::viewport(u32 x, u32 y, u32 width, u32 height)
{
    s_graphics_api->viewport(x, y, width, height);
}

void utd::renderer::command::clear_color(const glm::vec4& color)
{
    s_graphics_api->clear_color(color);
}

void utd::renderer::command::clear()
{
    s_graphics_api->clear();
}

void utd::renderer::command::draw_indexed(const utd::vertex_array& vertex_array, u32 count)
{
    s_graphics_api->draw_indexed(vertex_array, count);
}

void utd::renderer::command::draw_indexed(const utd::ref_ptr<utd::vertex_array> vertex_array, u32 count)
{
    s_graphics_api->draw_indexed(vertex_array.ref(), count);
}

void utd::renderer::command::draw_arrays(const vertex_array& vertex_array, u32 first, u32 count)
{
    s_graphics_api->draw_arrays(vertex_array, first, count);
}

void utd::renderer::command::depth_buffer(bool enabled)
{
    s_graphics_api->depth_buffer(enabled);
}