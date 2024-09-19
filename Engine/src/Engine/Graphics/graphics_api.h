#pragma once

#include <glm/vec4.hpp>

#include <Engine/Core/Base.h>

namespace utd
{
    class vertex_array;
    class shader;

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
        virtual void init() = 0;
        
        virtual void viewport(u32, u32, u32, u32) = 0;
        virtual void line_width(float) = 0;
        virtual void clear() = 0;
        virtual void clear_color(const glm::vec4&) = 0;
        virtual void draw_indexed(const vertex_array& vertex_array, u32 count) = 0;
        virtual void draw_line(const vertex_array& vertex_array, u32 count)    = 0;
        virtual void draw_arrays(const vertex_array& vertex_array, u32 first, u32 count) = 0;
        
        virtual void depth_buffer(bool enabled) = 0;

    public:
        static std::uptr<graphics_api> create(graphics_api::type _API);
    };
    
} /* namespace utd */