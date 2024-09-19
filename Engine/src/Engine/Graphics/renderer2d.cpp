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

    struct circle
    {
        glm::vec3 position;
        glm::vec3 local_position;
        glm::vec4 color;
        glm::vec4 outline_color;
        glm::vec2 tex_coord;
        float tex_index;
        float tiling;
        float thickness = 0.5f;
        float fade = 0.02f;
    };

    struct line
    {

    };

}

struct render_data
{
    static constexpr utd::u32 TRIANGLE_INDEX_COUNT = 3U;
    
    static constexpr utd::u32 MAX_QUADS    = 12'000U;
    static constexpr utd::u32 QUAD_VERTICES = 4U;
    static constexpr utd::u32 QUAD_INDICES = TRIANGLE_INDEX_COUNT * 2U;
    
    static constexpr utd::u32 MAX_VERTICES = MAX_QUADS * QUAD_VERTICES;
    static constexpr utd::u32 MAX_INDICES  = MAX_QUADS * QUAD_INDICES;
    
    static constexpr utd::u32 MAX_TEXTURE_UNITS = 32U;

    // probably it'll end up not being constexpr, but it's good for now
    static constexpr glm::vec4 QUAD_VERTEX_POSITIONS[4]
    {
        { -0.5f, -0.5f, 0.0f, 1.0f },    { 0.5f, -0.5f, 0.0f, 1.0f },
        //           |                -1             |
        //           |                |              |
        //           *_______________0.5 ____________*
        //           |                |              |
        //-1_______-0.5_______________0_____________0.5_____________x 1
        //           |                |              |
        //           *______________-0.5_____________*                
        //           |                |              |                  
        //           |                y              |                   
        //           |                1              |                    
        { 0.5f,  0.5f, 0.0f, 1.0f },     { -0.5f,  0.5f, 0.0f, 1.0f }
    };

    static constexpr glm::vec2 TEXTURE_COORDS[render_data::QUAD_VERTICES]
    {
        {0.f, 0.f}, {1.f, 0.f},

        {1.f, 1.f}, {0.f, 1.f}
    };
    
    static inline std::uptr<utd::vertex_array> quad_vertex_array;
    static inline utd::ref_ptr<utd::vertex_buffer> quad_vertex_buffer;
    static inline std::uptr<utd::shader> quad_shader;
    static inline vertex_primitive::quad* quad_vertex_data_base;
    static inline vertex_primitive::quad* quad_vertex_data_iter;

    static inline std::uptr<utd::vertex_array> circle_vertex_array;
    static inline utd::ref_ptr<utd::vertex_buffer> circle_vertex_buffer;
    static inline std::uptr<utd::shader> circle_shader;
    static inline vertex_primitive::circle* circle_vertex_buffer_base;
    static inline vertex_primitive::circle* circle_vertex_buffer_iter;

    static inline std::uptr<utd::vertex_array> line_vertex_array;
    static inline std::uptr<utd::shader> line_shader;
    static inline vertex_primitive::line* line_vertex_buffer_base;
    static inline vertex_primitive::line* line_vertex_buffer_iter;
    
    static inline std::array<utd::ref_ptr<utd::texture>, MAX_TEXTURE_UNITS> texture_slots;
    static inline std::uptr<utd::texture> default_texture;
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

static utd::cstring glsl_circle_vertex
{
    R"(
    #version 450
    layout (location = 0) in vec3 a_WorldPosition;
    layout (location = 1) in vec3 a_LocalPosition;
    layout (location = 2) in vec4 a_Color;
    layout (location = 3) in vec4 a_OutlineColor;
    layout (location = 4) in vec2 a_TexCoord;
    layout(location = 5) in float a_TexIndex;
    layout(location = 6) in float a_TilingFactor;
    layout(location = 7) in float a_Thickness;
    layout(location = 8) in float a_Fade;


    layout(std140, binding = 0) uniform Camera
    {
	    mat4 u_ViewProjection;
    };

    layout (location = 0) out vec4 v_Color;
    layout (location = 1) out flat float v_TexIndex;
    layout(location = 2) out vec2 v_TexCoord;
    layout(location = 3) out float v_TilingFactor;
    layout(location = 4) out flat float v_Thickness;
    layout(location = 5) out float v_Fade;
    layout(location = 6) out vec3 v_LocalPosition;
    layout(location = 7) out vec4 v_OutlineColor;

    void main()
    {
	    v_Color    = a_Color;
	    v_TexCoord = a_TexCoord;
	    v_TexIndex = a_TexIndex;
	    v_TilingFactor = a_TilingFactor;
        v_Thickness = a_Thickness;
        v_Fade = a_Fade;
        v_LocalPosition = a_LocalPosition;
        v_OutlineColor = a_OutlineColor;

	    gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0f);
    }

    )"
};

static utd::cstring glsl_circle_fragment
{
    R"(
    #version 450
    layout(location = 0) out vec4 o_Color;
    
    layout (location = 0) in vec4 v_Color;
    layout (location = 1) in flat float v_TexIndex;
    layout(location = 2) in vec2 v_TexCoord;
    layout(location = 3) in float v_TilingFactor;
    layout(location = 4) in flat float v_Thickness;
    layout(location = 5) in float v_Fade;
    layout(location = 6) in vec3 v_LocalPosition;
    layout(location = 7) in vec4 v_OutlineColor;
    
    layout(binding = 0) uniform sampler2D u_Textures[32];
    
    void main()
    {
        int index = int(v_TexIndex);
        
        float distance = 1.0 - length(v_LocalPosition);
        float circle = smoothstep(v_Thickness + v_Fade, v_Thickness, distance);

        if (circle != 0.0)
	    {
            o_Color = v_OutlineColor;
        }
        else
        {
            o_Color = texture(u_Textures[index], v_TexCoord * v_TilingFactor) * v_Color;
        }
        circle = smoothstep(0.0, v_Fade, distance);
	    o_Color.a *= circle;
    }

    )"
};

static void _init_quad()
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
    render_data::quad_vertex_buffer = vertex_buffer;
    render_data::quad_vertex_array->push_back(std::move(vertex_buffer));

    render_data::quad_vertex_data_base = new vertex_primitive::quad[render_data::MAX_VERTICES];

    for (utd::u32 i = 0; i < render_data::MAX_VERTICES; i++)
    {
        auto index = i % render_data::QUAD_VERTICES;
        render_data::quad_vertex_data_base[i].tex_coord = render_data::TEXTURE_COORDS[index];
    }

    render_data::quad_shader = utd::shader::create(glsl_quad_vertex, glsl_quad_fragment);

    render_data::default_texture = utd::texture::create();
    utd::u32 white_texture = 0xffffffff;
    render_data::default_texture->set_data(&white_texture, sizeof(utd::u32));
    render_data::texture_slots[0] = render_data::default_texture;
}

static void _init_circle()
{
    render_data::circle_vertex_array = std::make_unique<utd::vertex_array>();
    auto vertex_buffer = utd::vertex_buffer::create<vertex_primitive::circle>(render_data::MAX_VERTICES);
    //vertex->layout<vec4, vec4, vec3, vec2, float, float>("color", "outline_color", "position", "tex_coord", "tiling", "")
    vertex_buffer->set_layout
    (
        {
            { utd::shader::datatype::FLOAT3, "a_WorldPosition" },
            { utd::shader::datatype::FLOAT3, "a_LocalPosition" },
            { utd::shader::datatype::FLOAT4,         "a_Color" },
            { utd::shader::datatype::FLOAT4,  "a_OutlineColor" },
            { utd::shader::datatype::FLOAT2,       "aTexCoord" },
            { utd::shader::datatype::FLOAT,       "a_TexIndex" },
            { utd::shader::datatype::FLOAT,   "a_TilingFactor" },
            { utd::shader::datatype::FLOAT,      "a_Thickness" },
            { utd::shader::datatype::FLOAT,           "a_Fade" }
        }
    );
    render_data::circle_vertex_buffer = vertex_buffer;
    render_data::circle_vertex_array->push_back(std::move(vertex_buffer));
    render_data::circle_vertex_buffer_base = new vertex_primitive::circle[render_data::MAX_VERTICES];
    
    for (utd::u32 i = 0; i < render_data::MAX_VERTICES; i++)
    {
        auto index = i % render_data::QUAD_VERTICES;
        render_data::circle_vertex_buffer_base[i].tex_coord = render_data::TEXTURE_COORDS[index];
    }
    
    render_data::circle_shader = utd::shader::create(glsl_circle_vertex, glsl_circle_fragment);
}

static void _set_index_buffers()
{
    utd::u32* quad_indices = new utd::u32[render_data::MAX_INDICES];

    for (utd::u32 i = 0, offset = 0; i < render_data::MAX_INDICES; i += 6, offset += 4)
    {
        quad_indices[i + 0] = offset + 0;
        quad_indices[i + 1] = offset + 1;
        quad_indices[i + 2] = offset + 2;

        quad_indices[i + 3] = offset + 2;
        quad_indices[i + 4] = offset + 3;
        quad_indices[i + 5] = offset + 0;
    }

    render_data::quad_vertex_array->make_index_buffer(quad_indices, render_data::MAX_INDICES);
    render_data::circle_vertex_array->make_index_buffer(quad_indices, render_data::MAX_INDICES);

    delete[] quad_indices;
}

void utd::renderer2d::init()
{
    _init_quad();
    _init_circle();
    
    _set_index_buffers();

    render_data::uniform_camera_buffer = std::make_unique<uniform_buffer>(static_cast<u32>(sizeof(glm::mat4)), 0u);
}

void utd::renderer2d::shutdown() noexcept
{
    delete[] render_data::circle_vertex_buffer_base;
    delete[] render_data::quad_vertex_data_base;
    delete[] render_data::line_vertex_buffer_base;
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

void utd::renderer2d::draw(const transform& transform, const circle& circle)
{
    UTD_PROFILE_FUNC();

    float index = 0.f;
    if (circle.texture)
    {
        for (u32 i = 1u; i < render_data::texture_slot_index; i++)
        {
            if (render_data::texture_slots[i].get() == circle.texture.get())
            {
                index = static_cast<float>(i);
                break;
            }
        }

        if (index == 0.f)
        {
            if (render_data::texture_slot_index == render_data::MAX_TEXTURE_UNITS)
            {
                restart_batch();
            }
            else
            {
                index = static_cast<float>(render_data::texture_slot_index);
                render_data::texture_slots[render_data::texture_slot_index] = circle.texture;
                render_data::texture_slot_index++;
            }

        }

    }

    for (u32 i = 0; i < 4; i++)
    {
        render_data::circle_vertex_buffer_iter->position = transform::get(transform) * render_data::QUAD_VERTEX_POSITIONS[i];
        render_data::circle_vertex_buffer_iter->local_position = render_data::QUAD_VERTEX_POSITIONS[i] * 2.f;
        render_data::circle_vertex_buffer_iter->color = circle.color;
        render_data::circle_vertex_buffer_iter->outline_color = circle.outline_color;
        render_data::circle_vertex_buffer_iter->tiling = circle.tiling_count;
        render_data::circle_vertex_buffer_iter->tex_index = index;
        render_data::circle_vertex_buffer_iter->thickness = circle.outline_thickness;
        render_data::circle_vertex_buffer_iter->fade = circle.fade;

        render_data::circle_vertex_buffer_iter++;
    }

    statistics.quad_drawn_count++;

}

void utd::renderer2d::draw(const transform &transform, const sprite &sprite)
{
    UTD_PROFILE_FUNC();

    float index = 0.f;
    if(sprite.texture)
    {
        for(u32 i = 1u; i < render_data::texture_slot_index; i++)
        {
            if(render_data::texture_slots[i].get() == sprite.texture.get())
            {
                index = static_cast<float>(i);
                break;
            }
            if (render_data::texture_slots[i]->get_id() != -1)
            {
                index = 0.f;
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
    
    for (u32 i = 0; i < render_data::QUAD_VERTICES; i++)
    {
        render_data::quad_vertex_data_iter->position = transform::get(transform) * render_data::QUAD_VERTEX_POSITIONS[i];
        render_data::quad_vertex_data_iter->color = sprite.color;
        render_data::quad_vertex_data_iter->tiling = sprite.tiling_count;
        render_data::quad_vertex_data_iter->texture_index = index;

        render_data::quad_vertex_data_iter++;
    }

    statistics.quad_drawn_count++;

}

void utd::renderer2d::start_batch()
{
    statistics.quad_drawn_count = 0; statistics.draw_calls = 0;

    render_data::quad_vertex_data_iter = render_data::quad_vertex_data_base;
    render_data::circle_vertex_buffer_iter = render_data::circle_vertex_buffer_base;
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
    size_t quad_vertex_data_offset = std::distance(render_data::quad_vertex_data_base, render_data::quad_vertex_data_iter);
    if(quad_vertex_data_offset)
    {
        auto range_size = quad_vertex_data_offset * sizeof(vertex_primitive::quad);
        //(*render_data::quad_vertex_array)[0].set_data(render_data::quad_vertex_data_base, static_cast<u32>(range_size));
        
        render_data::quad_vertex_buffer->set_data(render_data::quad_vertex_data_base, static_cast<u32>(range_size));

        render_data::quad_shader->bind();
        for (u32 i = 0; i < render_data::texture_slot_index; i++)
        {
            render_data::texture_slots[i]->bind(i);
        }

        renderer::command::draw_indexed(render_data::quad_vertex_array, quad_vertex_data_offset * render_data::QUAD_INDICES);
        statistics.draw_calls++;
    }
    
    size_t circle_vertex_buffer_offset = std::distance(render_data::circle_vertex_buffer_base, render_data::circle_vertex_buffer_iter);
    if (circle_vertex_buffer_offset)
    {
        auto data_size = circle_vertex_buffer_offset * sizeof(vertex_primitive::circle);
        //(*render_data::circle_vertex_array)[0].set_data();
        render_data::circle_vertex_buffer->set_data(render_data::circle_vertex_buffer_base, static_cast<u32>(data_size));

        for (u32 i = 0; i < render_data::texture_slot_index; i++)
        {
            render_data::texture_slots[i]->bind(i);
        }

        render_data::circle_shader->bind();
        renderer::command::draw_indexed(render_data::circle_vertex_array, circle_vertex_buffer_offset * render_data::QUAD_INDICES);
        statistics.draw_calls++;
    }

}
