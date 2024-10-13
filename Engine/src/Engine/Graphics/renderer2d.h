#pragma once

#include <Engine/Core/Base.h>
#include <glm/glm.hpp>



namespace utd
{

    typedef struct renderer2d : utility
    {
        struct statistics
        {
            int draw_calls = 0;
            int quad_drawn_count = 0;
        };

        static void init();
        static void shutdown() noexcept;
        static void begin(const multi_camera& camera, const glm::mat4& transform = glm::mat4(1.f));
        static void begin(const editor_camera& editor_camera);
        static void begin(const camera& camera);
        static void end();
        
        static void draw(const rectangle& rect);
        
        static void draw(const transform& transform, const line& line);
        static void draw(const transform& transform, const circle& circle);
        static void draw(const transform& transform, const sprite& sprite);
        static void draw(const transform& transform, const render_string& rstring);
        static void draw(const sub_texture& subtexture, const transform& transform);
        
        static const statistics& stats();

    } renderer2D, renderer_2d;

} /* namespace utd */