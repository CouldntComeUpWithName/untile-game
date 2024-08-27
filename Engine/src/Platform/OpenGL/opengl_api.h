#pragma once

#include <Engine/Graphics/graphics_api.h>

namespace utd
{
    class opengl_api : public graphics_api
    {
    public:
        void init() override;
        void viewport(u32, u32, u32, u32) override;
        void line_width(float) override;
        void clear() override;
        void clear_color(const glm::vec4&) override;
        void draw_indexed(const vertex_array& vertex_array, u32 count) override;
        void draw_line(const vertex_array& vertex_array, u32 count) override;
        void draw_arrays(const vertex_array& vertex_array, u32 first, u32 count) override;

        void depth_buffer(bool enabled) override;
    };

} /* namespace utd */