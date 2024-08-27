#include <upch.h>
#include "renderer2d.h"

#include <Engine/Core/Assert.h>

#include <Engine/Graphics/renderer.h>
#include <Engine/Graphics/vertex_array.h>
#include <Engine/Graphics/vertex_buffer.h>
#include <Engine/Graphics/vertex_attrib.h>
#include <Engine/Graphics/camera.h>
#include <Engine/Graphics/uniform_buffer.h>

#include <Engine/Scene/components.h>

#include <Engine/Profiling/profile.h>

#include <glm/gtc/matrix_transform.hpp>

namespace vertex_primitive
{
    struct quad
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 tex_coord;
        float texture_index;
        float tiling;
    };

}

struct render_data
{
    static constexpr utd::u32 MAX_QUADS    = 12'000u;
    static constexpr utd::u32 MAX_VERTICES = MAX_QUADS * 4;
    static constexpr utd::u32 MAX_INDICES  = MAX_QUADS * 6; 
    static constexpr utd::u32 MAX_TEXTURE_UNITS = 32u;

    static inline utd::u32 current_quad_index_count = 0u;
    static inline std::uptr<utd::vertex_array> quad_vertex_array;
    static inline std::uptr<utd::shader> quad_shader;
    static inline std::uptr<utd::texture> default_texture;
    static inline vertex_primitive::quad* quad_vertex_buffer_data;
    static inline vertex_primitive::quad* quad_vertex_buffer_iter;

    static inline glm::vec4 quad_vertex_positions[4];

    static inline std::array<utd::ref_ptr<utd::texture>, MAX_TEXTURE_UNITS> texture_slots;
    static inline utd::u32 texture_slot_index = 1;
    
    static inline glm::mat4 camera_projection_data = glm::mat4(1.f);
    static inline std::uptr<utd::uniform_buffer> uniform_camera_buffer;

    //temp
   
};

static stats statistics;
stats get_stats() { return statistics; }

static utd::cstring glsl_quad_vertex
{
    R"(

    #version 450 core

    layout (location = 0) in vec3 a_Position;
    layout (location = 1) in vec4 a_Color;
    layout (location = 2) in vec2 a_TexCoord;
    layout(location = 3) in float a_TexIndex;
    layout(location = 4) in float a_TilingFactor;

    layout(std140, binding = 0) uniform Camera
    {
	    mat4 u_ViewProjection;
    };

    layout (location = 0) out vec4 v_Color;
    layout (location = 1) out flat float v_TexIndex;
    layout(location = 2) out vec2 v_TexCoord;
    layout(location = 3) out float v_TilingFactor;

    void main()
    {
	    v_Color    = a_Color;
	    v_TexCoord = a_TexCoord;
	    v_TexIndex = a_TexIndex;
	    v_TilingFactor = a_TilingFactor;

	    gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
    }

    )"
};

static utd::cstring glsl_quad_fragment
{
    R"(

    #version 450
    layout(location = 0) out vec4 o_Color;
    
    layout (location = 0) in vec4 v_Color;
    layout (location = 1) in flat float v_TexIndex;
    layout (location = 2) in vec2 v_TexCoord;
    layout(location = 3) in float v_TilingFactor;
    
    layout(binding = 0) uniform sampler2D u_Textures[32];
    
    void main()
    {
        int index = int(v_TexIndex);
        o_Color = texture(u_Textures[index], v_TexCoord * v_TilingFactor) * v_Color;
    }

    )"
};



void utd::renderer2d::init()
{
    UTD_PROFILE_FUNC();

    render_data::quad_vertex_array = std::make_unique<utd::vertex_array>();

    auto vertex_buffer = utd::vertex_buffer::create<vertex_primitive::quad>(render_data::MAX_VERTICES);
    vertex_buffer->set_layout
    (
        {
            { utd::shader::datatype::FLOAT3, "aPos"     },
            { utd::shader::datatype::FLOAT4, "aColor"        },
            { utd::shader::datatype::FLOAT2, "aTexCoord"     },
            { utd::shader::datatype::FLOAT,  "a_TexIndex"     },
            { utd::shader::datatype::FLOAT,  "a_TilingFactor" }, 
        }
    );

    render_data::quad_vertex_array->push_back(std::move(vertex_buffer));

    render_data::quad_vertex_buffer_data = new vertex_primitive::quad[render_data::MAX_VERTICES];
    
    utd::u32* quad_indices = new u32[render_data::MAX_INDICES];

    u32 offset = 0;
    for(auto i = 0; i < render_data::MAX_INDICES; i += 6)
    {
        quad_indices[i + 0] = offset + 0;
        quad_indices[i + 1] = offset + 1;
        quad_indices[i + 2] = offset + 2;

        quad_indices[i + 3] = offset + 2;
        quad_indices[i + 4] = offset + 3;
        quad_indices[i + 5] = offset + 0;

        offset += 4;
    }
    
    render_data::quad_vertex_array->make_index_buffer(quad_indices, render_data::MAX_INDICES);
    delete[] quad_indices;

    render_data::quad_shader = utd::shader::create(glsl_quad_vertex, glsl_quad_fragment);
    
    render_data::quad_vertex_positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    render_data::quad_vertex_positions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    render_data::quad_vertex_positions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    render_data::quad_vertex_positions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

    render_data::default_texture = utd::texture::create();
    u32 white_texture = 0xffffffff;
    render_data::default_texture->set_data(&white_texture, sizeof(u32));
    render_data::texture_slots[0] = render_data::default_texture;

    render_data::uniform_camera_buffer = std::make_unique<uniform_buffer>(static_cast<u32>(sizeof(glm::mat4)), 0u);
}

void utd::renderer2d::begin(const multi_camera &camera, const glm::mat4 &transform)
{
    UTD_PROFILE_FUNC();
    statistics.quad_drawn_count = 0; statistics.draw_calls = 0;

    render_data::camera_projection_data = camera.projection();
    render_data::camera_projection_data *= transform;
    render_data::uniform_camera_buffer->set_data(&render_data::camera_projection_data, sizeof(render_data::camera_projection_data));
    
    start_batch();
}

void utd::renderer2d::begin(const editor_camera& editor_camera)
{
    render_data::camera_projection_data = editor_camera.view_projection();
    render_data::uniform_camera_buffer->set_data(&render_data::camera_projection_data, sizeof(render_data::camera_projection_data));
    
    start_batch();
}

void utd::renderer2d::end()
{
    flush();
}

// void utd::renderer2d::draw(const triangle &rect)
// {

// }

void utd::renderer2d::draw(const transform &transform, const sprite &sprite)
{
    UTD_PROFILE_FUNC();

    constexpr size_t VERTICES = 4;
    constexpr glm::vec2 TEXTURE_COORDS[VERTICES]
    {
        {0.f, 0.f}, {1.f, 0.f}, 
        
        {1.f, 1.f}, {0.f, 1.f}
    };

    float index = 0.f;
    if(sprite.texture)
    {
        for(u32 i = 1u; i < render_data::texture_slot_index; i++)
        {
            if(render_data::texture_slots[i]->get_id() == sprite.texture->get_id())
            {
                index = static_cast<float>(i);
                break;
            }
        }

        if(index == 0.f)
        {
            if (render_data::texture_slot_index == render_data::MAX_TEXTURE_UNITS)
            {
                restart_batch();
            }
            else
            {
                index = static_cast<float>(render_data::texture_slot_index);
                render_data::texture_slots[render_data::texture_slot_index] = sprite.texture;
                render_data::texture_slot_index++;
            }

        }
        
    }
    
    for (u32 i = 0; i < VERTICES; i++)
    {
        render_data::quad_vertex_buffer_iter->position = transform::get(transform) * render_data::quad_vertex_positions[i];
        render_data::quad_vertex_buffer_iter->color = sprite.color;
        render_data::quad_vertex_buffer_iter->tex_coord = TEXTURE_COORDS[i];
        render_data::quad_vertex_buffer_iter->texture_index = index;
        render_data::quad_vertex_buffer_iter->tiling = sprite.tiling_count;

        render_data::quad_vertex_buffer_iter++;
    }
    render_data::current_quad_index_count += 6;

    statistics.quad_drawn_count++;

}

void utd::renderer2d::start_batch()
{
    
    statistics.quad_drawn_count = 0; statistics.draw_calls = 0;

    render_data::current_quad_index_count = 0;
    render_data::quad_vertex_buffer_iter = render_data::quad_vertex_buffer_data;

    render_data::texture_slot_index = 1;
}

void utd::renderer2d::restart_batch()
{
    UTD_PROFILE_FUNC();

    flush();
    start_batch();
}

void utd::renderer2d::flush()
{
    UTD_PROFILE_FUNC();
    if(render_data::current_quad_index_count)
    {
        auto data_size = std::distance(render_data::quad_vertex_buffer_data, render_data::quad_vertex_buffer_iter) * sizeof(vertex_primitive::quad);
        (*render_data::quad_vertex_array)[0].set_data(render_data::quad_vertex_buffer_data, static_cast<u32>(data_size));

        render_data::quad_shader->bind();
        for (u32 i = 0; i < render_data::texture_slot_index; i++)
        {
            render_data::texture_slots[i]->bind(i);
        }
        
        renderer::command::draw_indexed(render_data::quad_vertex_array, render_data::current_quad_index_count);
        statistics.draw_calls++;
    }
}
