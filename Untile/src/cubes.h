#pragma once

#include <Engine/Core/Base.h>
#include <Engine/Core/layer.h>
#include <Engine/Graphics/vertex.h>
#include <Engine/Graphics/texture.h>
#include <Engine/Graphics/camera.h>

#include <glm/glm.hpp>

class utd::event;
class utd::shader;

class cubes : public utd::layer
{
public:
    cubes() = default;
    void on_attach() override;
    void on_detach() override;
    void on_render() override;
    void on_update(float) override;
    void on_event(utd::event&) override;

     ~cubes();
private:

private:
    bool visible = true;
    bool cursor_hidden = false;
    utd::vertex_array m_vertex_array;
    std::uptr<utd::texture> m_cobblestone;
    std::uptr<utd::shader> m_texture_shader;
    utd::camera m_camera;


    glm::vec3 cubePositions[10] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };


};