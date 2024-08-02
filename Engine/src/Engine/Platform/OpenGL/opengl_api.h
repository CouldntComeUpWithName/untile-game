#pragma once

#include <Engine/Graphics/graphics_api.h>

namespace utd
{
    class opengl_api : public graphics_api
    {
    public:
        void set_viewport(u32, u32, u32, u32) override;
        void set_line_width(float) override;
        void clear() override;
        void set_clear_color(const glm::vec4&) override;
        void draw_indexed(const vertex_array& vertex_array, u32 count) override;
        void draw_lines() override;
    };

} /* namespace utd */