#pragma once

#include <glm/glm.hpp>
#include <Engine/Graphics/texture.h>
#include <Engine/Utils/memory.h>

namespace utd
{
    struct transform
    {
        glm::vec3 position = {0.f, 0.f, 0.f};
        glm::vec3 rotation = {0.f, 0.f, 0.f};
        glm::vec3 scale    = {1.f, 1.f, 1.f};
    public:
        //TODO: think about explicit or implicit cast to glm::mat4 instead...
        static glm::mat4 get(const transform&);

    };

    struct sprite
    {
        ref_ptr<texture> texture = nullptr;
        glm::vec4 color = {0.f, 0.f, 0.f, 1.f};
        float tiling_count = 1.f;
    };

    struct line
    {

    };

    class render_string
    {

    };

    struct rectangle
    {

    };

    struct circle
    {

    };

    struct triangle
    {

    };

}

