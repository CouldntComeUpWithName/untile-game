#pragma once

#include "graphics_api.h"
#include <Engine/Utils/memory.h>

namespace utd
{
    class renderer : utility
    {
    public:
        static void init(graphics_api::type _API = graphics_api::type::OPENGL);
        static void shutdown();
        
        class command : utility
        {
        public:
            static void viewport(u32, u32, u32, u32);
            static void line_width(float);
            static void clear_color(const glm::vec4&);
            static void clear();
            static void draw_indexed(const std::uptr<utd::vertex_array>& vertex_array, u32 count);
            static void draw_indexed(const vertex_array& vertex_array, u32 count);
            
            static void draw_line(const vertex_array& vertex_array, u32 count);
            static void depth_buffer(bool);
            static void draw_arrays(const vertex_array& vertex_array, u32 first, u32 count);
        };

        static inline graphics_api::type API() { return s_api_type; };
        static void submit(const vertex_array& vertex_array, const shader& shader /* TODO: to be expanded and implemented */);
    private:
        static graphics_api::type s_api_type;
        static std::uptr<graphics_api> s_graphics_api;
    };

    using render_command = renderer::command;

} /* namespace utd */ 
