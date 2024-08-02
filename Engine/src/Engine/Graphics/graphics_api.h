#pragma once

#include <glm/vec4.hpp>

#include <Engine/Core/Base.h>

namespace utd
{
    class vertex_array;

    class graphics_api
    {
    public:
        enum class type
        {
            UNKNOWN   = 0,
            OPENGL    = 1,
            DX11      = 2,
            DX12      = 3,
            VULKAN    = 4
        };
    public:
        virtual void set_viewport(u32, u32, u32, u32) = 0;
        virtual void set_line_width(float) = 0;
        virtual void clear() = 0;
        virtual void set_clear_color(const glm::vec4&) = 0;
        virtual void draw_indexed(const vertex_array& vertex_array, u32 count) = 0;
        virtual void draw_lines() = 0;
    };
    
} /* namespace utd */