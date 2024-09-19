#pragma once

#include <Engine/Core/Base.h>
#include <Engine/Core/layer.h>
#include <Engine/Graphics/vertex.h>
#include <Engine/Graphics/texture.h>
#include <Engine/Graphics/camera.h>

#include <glm/glm.hpp>
#include <Engine/Scene/components.h>

#include <Engine/Core/resource_manager.h>

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
    utd::ref_ptr<utd::texture> m_cobblestone;
    utd::ref_ptr<utd::texture> m_grass;
    utd::ref_ptr<utd::texture> m_sand;
    utd::ref_ptr<utd::shader> m_texture_shader;
    utd::editor_camera m_editor_camera;
    utd::multi_camera m_camera;
    utd::ref_ptr<utd::texture> texture_map[3];

    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_right;
    
    utd::sprite m_sprites[100][100];

    float m_yaw   = 45.f;
    float m_pitch = 89.f;

    utd::texture_pack m_texture_manager;
    
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