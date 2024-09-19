#pragma once

#include <Engine/Core/Base.h>
#include <glm/glm.hpp>

struct stats
{
    int draw_calls = 0;
    int quad_drawn_count = 0;
};

namespace utd
{

    typedef class renderer2d : utility
    {
    public:
        static void init();
        static void shutdown() noexcept;
        static void begin(const multi_camera& camera, const glm::mat4& transform = glm::mat4(1.f));
        static void begin(const editor_camera& editor_camera);
        static void begin(const camera& camera);
        static void end();
        
        static void draw(const rectangle& rect);
        
        //temp
        // static void draw(const triangle& rect);
        // static void draw(const std::ref_ptr<texture>& texture);
        //temp
        
        static void draw(const entity& entity);
        static void draw(const transform& transform, const line& line);
        static void draw(const transform& transform, const circle& circle);
        static void draw(const transform& transform, const sprite& sprite);
        static void draw(const transform& transform, const render_string& rstring);
        
        //TODO: consider deleteting that sh*t
        template<typename t_primitive, typename... _args>
        static void draw(_args&&... args);
    private:
        // NOTE: think of moving these functions to the .cpp
        // as static functions within the only one translation unit
        static void start_batch();
        static void restart_batch();
        static void flush();

    } renderer2D, renderer_2d;

} /* namespace utd */

stats get_stats();// TODO: make as a part of Renderer2D