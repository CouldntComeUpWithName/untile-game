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
    struct transform;
    struct line;
    struct circle;
    struct rectangle;
    struct triangle;
    struct sprite;
    class render_string;
    class multi_camera;
    class camera;
    class editor_camera;

    typedef class renderer2d : utility
    {
    public:
        enum class policy
        {
            //TODO: choose between lower case and upper one
            standard = 0,
            STANDARD = 0,

            norotate = 1,
            NOROTATE = 1
        };
    public:
        static void init();
        static void begin(const multi_camera& camera, const glm::mat4& transform = glm::mat4(1.f));
        static void begin(const editor_camera& editor_camera);
        static void begin(const camera& camera);
        static void end();
        
        static void draw(const rectangle& rect);
        // static void draw(const triangle& rect);
        //temp
        // static void draw(const std::ref_ptr<texture>& texture);
        //temp
        static void draw(const transform& transform, const line& line);
        static void draw(const transform& transform, const circle& circle);
        static void draw(const transform& transform, const sprite& sprite);
        static void draw(const transform& transform, const render_string& font);
        
        //TODO: consider deleteting that sh*t
        template<typename t_primitive, typename... _args>
        static void draw(_args&&... args);
    private:
        static void start_batch();
        static void restart_batch();
        static void flush();

    } renderer2D, renderer_2d;

} /* namespace utd */



stats get_stats();