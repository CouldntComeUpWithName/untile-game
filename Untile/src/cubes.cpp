#include "cubes.h"

#include <Engine/Core/input.h>
#include <Engine/Core/application.h>
#include <Engine/Profiling/Profile.h>

#include <Engine/Graphics/Renderer.h>
#include <Engine/Graphics/Shader.h>
#include <Engine/Graphics/Texture.h>

#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

void cubes::on_attach()
{
    float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,  0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    std::uptr<utd::vertex_buffer> vb = std::make_unique<utd::vertex_buffer>(vertices, sizeof(vertices));
    vb->set_layout
    ({
        { utd::shader::data_type::FLOAT3, "aPos" },
        { utd::shader::data_type::FLOAT2, "aTexCoord" },
    });

    m_vertex_array.push_back(std::move(vb));
    m_cobblestone = utd::texture::load("E:/Programming/untile/Untile/assets/textures/cobblestone_2.png");
    
    m_texture_shader = utd::shader::load("E:/Programming/untile/Untile/assets/shaders/texture.vert", 
        "E:/Programming/untile/Untile/assets/shaders/texture.frag");

}

void cubes::on_detach()
{

}

void cubes::on_render()
{
    
    auto& io = ImGui::GetIO();
    ImGui::Begin("Cube Scene");                          // Create a window called "Hello, world!" and append into it.

    //ImGui::Text("T");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("visible", &visible);
    
    ImGui::SliderFloat("Camera FOV", &m_camera.m_fov, 30.f, 135.f);

    ImGui::End();

    if(visible)
    {
        utd::renderer::depth_buffer(true);

        m_cobblestone->bind();
        for (auto i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            m_texture_shader->set_mat4("model", model);

            utd::renderer::draw_arrays(m_vertex_array, 0, 36);

        }
    }
}

void cubes::on_update(float dt)
{
    if (utd::input::is_key_pressed(utd::key::ESCAPE))
    {
        if(utd::input::is_key_released(utd::key::ESCAPE))
        {
            cursor_hidden = !cursor_hidden;
            utd::application::instance().get_window().show_cursor(!cursor_hidden);
        }
    }

    m_texture_shader->bind();
    m_camera.projection(glm::perspective(glm::radians(m_camera.m_fov), 16 / 9.f, 0.1f, 100.f));
    m_texture_shader->set_mat4("projection", m_camera.projection());
    
    if(cursor_hidden)
    {

        UTD_PROFILE_BEGIN("Setting projection and view to m_texture_shader");

        m_texture_shader->set_mat4("view", m_camera.view());

        UTD_PROFILE_END();

        m_camera.on_update(dt);
    }
}

void cubes::on_event(utd::event& event)
{
    //utd::event_dispatcher::dispatch<utd::key_pressed_event>()
    m_camera.on_event(event);
}

cubes::~cubes()
{
}
