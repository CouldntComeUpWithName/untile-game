#pragma once

#include "graphics_api.h"

namespace utd
{
    
    class renderer : utility
    {
    public:
        static void init(graphics_api::type _API = graphics_api::type::OPENGL);
        static void shutdown();

        static void set_viewport(u32, u32, u32, u32);
        static void set_line_width(float);
        static void set_clear_color(const glm::vec4&);
        static void clear();
        static void draw_indexed(const vertex_array& vertex_array, u32 count);
        static void draw_lines();

        static void draw_arrays(const vertex_array& vertex_array, u32 first, u32 count);
        static void depth_buffer(bool);

        static inline graphics_api::type API() { return s_api_type; };
       
    private:
        static graphics_api::type s_api_type;
        static std::uptr<graphics_api> s_graphics_api;
    };
    

} /* namespace utd */ 
