#include "cubes.h"

#include <Engine/Core/input.h>
#include <Engine/Core/application.h>
#include <Engine/Profiling/Profile.h>

#include <Engine/Graphics/Renderer.h>
#include <Engine/Graphics/Renderer2D.h>
#include <Engine/Graphics/Shader.h>
#include <Engine/Graphics/Texture.h>

#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <string>
struct quad
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coord;
    float texture_index;
};

std::array<quad, 4> make_quad(float x, float y, float idx)
{
    quad q0 = { {x, y, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f}, 1.f };
    quad q1 = { {x, y, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f}, 1.f };
    quad q2 = { {x, y, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f}, 1.f };
    quad q3 = { {x, y, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f}, 1.f };
    
    return { q0, q1, q2, q3 };
}

void cubes::on_attach()
{
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.f, 0.f, 0.f, 1.f, 0.0f, 0.0f,  1.f,
     0.5f, -0.5f, -0.5f,  0.f, 0.f, 0.f, 1.f, 1.0f, 0.0f, 1.f,
     0.5f,  0.5f, -0.5f,  0.f, 0.f, 0.f, 1.f,  1.0f, 1.0f, 1.f,
     0.5f,  0.5f, -0.5f,  0.f, 0.f, 0.f, 1.f,  1.0f, 1.0f, 1.f,
    -0.5f,  0.5f, -0.5f,  0.f, 0.f, 0.f, 1.f,  0.0f, 1.0f, 1.f,
    -0.5f, -0.5f, -0.5f,  0.f, 0.f, 0.f, 1.f,  0.0f, 0.0f, 1.f,
                                                          1.f,
    -0.5f, -0.5f,  0.5f,  0.f, 0.f, 0.f, 1.f, 0.0f, 0.0f, 1.f,
     0.5f, -0.5f,  0.5f,  0.f, 0.f, 0.f, 1.f, 1.0f, 0.0f, 1.f,
     0.5f,  0.5f,  0.5f,  0.f, 0.f, 0.f, 1.f, 1.0f, 1.0f, 1.f,
     0.5f,  0.5f,  0.5f,  0.f, 0.f, 0.f, 1.f, 1.0f, 1.0f, 1.f,
    -0.5f,  0.5f,  0.5f,  0.f, 0.f, 0.f, 1.f, 0.0f, 1.0f, 1.f,
    -0.5f, -0.5f,  0.5f,  0.f, 0.f, 0.f, 1.f, 0.0f, 0.0f, 1.f,
                                                          1.f,
    -0.5f,  0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f,  1.0f, 0.0f, 1.f,
    -0.5f,  0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,  1.0f, 1.0f, 1.f,
    -0.5f, -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,  0.0f, 1.0f, 1.f,
    -0.5f, -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,  0.0f, 1.0f, 1.f,
    -0.5f, -0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f,  0.0f, 0.0f, 1.f,
    -0.5f,  0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f,  1.0f, 0.0f, 1.f,
                                                          1.f,
     0.5f,  0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f,  1.0f, 0.0f, 1.f,
     0.5f,  0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,  1.0f, 1.0f, 1.f,
     0.5f, -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,  0.0f, 1.0f, 1.f,
     0.5f, -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,  0.0f, 1.0f, 1.f,
     0.5f, -0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f,  0.0f, 0.0f, 1.f,
     0.5f,  0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f,  1.0f, 0.0f, 1.f,
                                                          1.f,
    -0.5f, -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,  0.0f, 1.0f, 1.f,
     0.5f, -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,  1.0f, 1.0f, 1.f,
     0.5f, -0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f,  1.0f, 0.0f, 1.f,
     0.5f, -0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f,  1.0f, 0.0f, 1.f,
    -0.5f, -0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f,  0.0f, 0.0f, 1.f,
    -0.5f, -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,  0.0f, 1.0f, 1.f,
                                                          1.f,
    -0.5f,  0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,  0.0f, 1.0f, 1.f,
     0.5f,  0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,  1.0f, 1.0f, 1.f,
     0.5f,  0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f,  1.0f, 0.0f, 1.f,
     0.5f,  0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f,  1.0f, 0.0f, 1.f,
    -0.5f,  0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f,  0.0f, 0.0f, 1.f,
    -0.5f,  0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,  0.0f, 1.0f,  1.f
    };

   /* float vertices[] = {
        -1.5f, -0.5f, 0.f,   0.18f, 0.6f, 0.96f, 1.f,  0.f, 0.f,   0.f,
        -0.5f, -0.5f, 0.f,   0.18f, 0.6f, 0.96f, 1.f,  1.f, 0.f,   1.f,
        -0.5f, 0.5f, 0.f,   0.18f, 0.6f, 0.96f, 1.f,   1.f, 1.f,   0.f,
       -1.5f, 0.5f, 0.f,   0.18f, 0.6f, 0.96f, 1.f,    0.f, 1.f,   1.f,

       0.5f, -0.5f, 0.0f,  1.0f, 0.93f, 0.24f, 1.0f,  0.0f, 0.0f, 1.f,
       1.5f, -0.5f, 0.0f,  1.0f, 0.93f, 0.24f, 1.0f,  1.0f, 0.0f, 1.f,
       1.5f, 0.5f, 0.0f,  1.0f, 0.93f, 0.24f, 1.0f,  1.0f, 1.0f,  1.f,
       0.5f, 0.5f, 0.0f,  1.0f, 0.93f, 0.24f, 1.0f,  0.0f, 1.0f,  1.f,

    };*/

    std::uptr<utd::vertex_buffer> vb = std::make_unique<utd::vertex_buffer>(vertices, sizeof(vertices));

    vb->set_layout
    ({
        { utd::shader::datatype::FLOAT3, "a_Position" },
        { utd::shader::datatype::FLOAT4, "a_Color"}, 
        { utd::shader::datatype::FLOAT2, "a_TexCoord" },
        { utd::shader::datatype::FLOAT, "a_TexIndex" }
        //{ utd::shader::datatype::FLOAT, "a_TilingFactor" }
    });

    uint32_t quadIndices[6]{};
    utd::u32 qindex[]
    {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4 

    };

    uint32_t offset = 0;
    for (uint32_t i = 0; i < 6; i += 6)
    {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    m_vertex_array.push_back(std::move(vb));

    m_vertex_array.make_index_buffer(qindex);

    m_cobblestone = utd::texture::load("E:/Programming/untile/Untile/assets/textures/cobblestone_2.png");
    m_grass       = utd::texture::load("E:/Programming/untile/Untile/assets/textures/grass_3.png");
    m_sand = utd::texture::load("E:/Programming/untile/Untile/assets/textures/sand_3.png");
     m_texture_shader = utd::shader::load("E:/Programming/untile/Untile/assets/shaders/texture.vert", 
     "E:/Programming/untile/Untile/assets/shaders/texture.frag");
    
    m_sprite  = { {0.f, 0.f, 0.f, 1.f}, m_cobblestone, 1.f };
    m_sprite1 = { {0.f, 0.f, 0.f, 1.f}, m_cobblestone, 1.f };

    m_texture_shader->bind();
    int samplers[] = { 0, 1, 2 };
    m_texture_shader->array("u_textures", samplers, sizeof(samplers) / sizeof(int));

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_up));

    m_camera.type(utd::camera::type::PERSPECTIVE);
    m_camera.position(glm::vec3(0.f, 0.f, 3.f));
    m_camera.perspective(90.f, 0.1f, 100.f);
    m_camera.orthographic(2.f, 100.f, 100.f);
    m_camera.viewport(16 / 9.f);

    texture_map[0] = m_grass;
    texture_map[1] = m_cobblestone;
    texture_map[2] = m_sand;
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
    
    //ImGui::SliderFloat("Camera FOV", &camera.fovy(), 30.f, 135.f);
    auto stats = get_stats();
    
    ImGui::Text("Q");
    ImGui::Text(std::to_string(stats.quad_drawn_count).c_str());
    const char* projection_type_str[] = { "Perspective", "Orthographic" };
    const char* current_projection_type_str = projection_type_str[(int)m_camera.type()];

    if (ImGui::BeginCombo("Projection", current_projection_type_str))
    {
        for (int i = 0; i < 2; i++)
        {
            bool selected = current_projection_type_str == projection_type_str[i];
            if (ImGui::Selectable(projection_type_str[i], selected))
            {
                current_projection_type_str = projection_type_str[i];
                m_camera.type((utd::camera::type)i);
            }

            if (selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::BeginGroup();
        
    if (m_camera.type() == utd::camera::type::ORTHOGRAPHIC)
    {
        float curr_size = m_camera.ortho_size();
        if (ImGui::SliderFloat("ortho size", &curr_size, 0.1f, 100.f))
        {
            m_camera.ortho_size(curr_size);
        }
    }
    else
    {
        float curr_fov = m_camera.fovy();
        if (ImGui::SliderFloat("FOV", &curr_fov, 0.1f, 100.f))
        {
            m_camera.fovy(curr_fov);
        }
    }
        
    ImGui::EndGroup();

    ImGui::End();

    if(visible)
    {
        UTD_PROFILE_SCOPE(__FUNCTION__ " - if (visible)");
        
        static constexpr glm::vec2 TEXTURE_COORDS[4] 
        {
            {0.f, 0.f}, {1.f, 0.f}, 
            
            {1.f, 1.f}, {0.f, 1.f}
        };

       //m_grass->bind(0);
       //m_sprite.texture->bind(1);
        
       /* m_texture_shader->bind();
        m_texture_shader->mat4("u_projection", m_camera.projection());*/
        //utd::renderer::command::draw_indexed(m_vertex_array, 12);

        utd::renderer2d::begin(m_camera);
        int i = 0;
        for (float x = -1.0f; x < 1.f; x += 0.02f)
        {
            for(float j = 1.f; j > -1.f; j -= 0.02f)
            {
                utd::renderer2D::draw({ {x / 0.02f, j  / 0.02f, 0.f}, {glm::radians(30.f), glm::radians(55.f), 0.f}, {1.0f, 1.0f, 1.f}}, {{0.f, 0.f, 0.f, 1.f}, texture_map[i % 3], 1.f});
                i++;
            }
        }
        //utd::renderer2D::draw({ {-0.5f, -0.5f, 0.f}, {0.f, 0.f, 0.f}, { 1.f, 1.f, 1.f} }, m_sprite);
        //utd::renderer2D::draw({ {-0.5f, 0.5f, 0.f}, {0.f, 0.f, 0.f}, { 1.f, 1.f, 1.f} }, { {0.f, 0.f, 0.f, 1.f}, m_grass, 1.f });
        utd::renderer2d::end();
        
        //utd::renderer::command::draw_arrays(m_vertex_array, 0, 36);
        
         //for (auto i = 0; i < 10; i++)
         //{
         //    // glm::mat4 model = glm::mat4(1.0f);
         //    // model = glm::translate(model, cubePositions[i]);
         //    // float angle = 20.0f * i;
         //    // model = glm::rotate(model, glm::radians(angle), { 1.0f, 0.3f, 0.5f });
         //    // m_texture_shader->mat4("model", model);
         //}

    }
}

void cubes::on_update(float dt)
{
    // UTD_ENGINE_INFO("camera position: {0}, {1}", m_camera.position().x, m_camera.position().y);
    //if (utd::input::is_key_pressed(utd::key::ESCAPE))
    //{
    //    if(utd::input::is_key_released(utd::key::ESCAPE))
    //    {
    //        cursor_hidden = !cursor_hidden;
    //        utd::singleton<utd::application>::instance().get_window().show_cursor(!cursor_hidden);
    //    }
    //}
   
   

    //if(cursor_hidden)
    {
        UTD_PROFILE_BEGIN("Setting projection and view to m_texture_shader");

        //UTD_ENGINE_INFO("Camera position: {0}, {1}, {2}", m_camera.position().x, m_camera.position().y, m_camera.position().z);

        //m_camera.projection(glm::perspective(glm::radians(m_camera.fovy()), 16 / 9.f, 0.1f, 1000.f));
        //m_camera.perspective(m_camera.fovy(), 0.1f, 1000.f);

        bool camera_moved = false;
        auto speed = 5.f * dt;

        //m_texture_shader->set_mat4("view", glm::lookAt(m_camera.position(), m_camera.position() + m_front, m_up));

        //if (m_pitch > 89.9f)
        //    m_pitch = 89.9f;
        //else if (m_pitch < -89.9f)
        //    m_pitch = -89.9f;

       /* glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));*/

        //m_right = glm::normalize(glm::cross(m_front, m_up));

        camera_moved |= utd::input::is_mouse_moved();

        if (utd::input::is_key_pressed(utd::key::W))
        {
            if (m_camera.type() == utd::camera::type::ORTHOGRAPHIC)
            {
                m_camera.ortho_size(m_camera.ortho_size() - speed);
            }
            else
            {
                auto pos = m_camera.position();
                pos += speed * m_front;
                m_camera.position(pos);
            }
            camera_moved = true;
        }
        if (utd::input::is_key_pressed(utd::key::S))
        {
            if (m_camera.type() == utd::camera::type::ORTHOGRAPHIC)
            {
                m_camera.ortho_size(m_camera.ortho_size() + speed);
            }
            else
            {
                auto pos = m_camera.position();
                pos -= speed * m_front;
                m_camera.position(pos);
            }
            camera_moved = true;
            
        }
        if (utd::input::is_key_pressed(utd::key::A))
        {
            //m_position -= glm::normalize(glm::cross(m_front, m_up)) * (m_speed * dt);
            auto pos = m_camera.position();
            pos -= glm::normalize(glm::cross(m_front, m_up)) * speed;
            m_camera.position(pos);
            
            camera_moved = true;

        }
        if (utd::input::is_key_pressed(utd::key::D))
        {
            auto pos = m_camera.position();
            pos += glm::normalize(glm::cross(m_front, m_up)) * speed;
            m_camera.position(pos);
            
            camera_moved = true;
        }

        //UTD_PROFILE_SCOPE("LookAt");
        
        /*m_texture_shader->bind();
        m_texture_shader->mat4("u_projection", m_camera.projection());*/

        if (camera_moved)
        {
            auto view = glm::lookAt(m_camera.position(), m_camera.position() + m_front, m_up);

            //m_texture_shader->mat4("u_view", view_projection);
        }

        UTD_PROFILE_END();

    }
}

void cubes::on_event(utd::event& event)
{

    //m_camera.on_event(event);
}

cubes::~cubes()
{
}
