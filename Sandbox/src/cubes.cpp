#include "cubes.h"

#include <Engine/Core/input.h>
#include <Engine/Core/Application.h>
#include <Engine/Profiling/Profile.h>

#include <Engine/Graphics/Renderer.h>
#include <Engine/Graphics/Renderer2D.h>
#include <Engine/Graphics/Shader.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Image.h>

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

static std::array<quad, 4> make_quad(float x, float y, float idx)
{
    quad q0 = { {x, y, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f}, 1.f };
    quad q1 = { {x, y, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f}, 1.f };
    quad q2 = { {x, y, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f}, 1.f };
    quad q3 = { {x, y, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f}, 1.f };
    
    return { q0, q1, q2, q3 };
}

utd::cstring vertex_cube
{
    R"(
        #version 450 core

        layout (location = 0) in vec3 a_Position;
        layout (location = 1) in vec4 a_Color;
        layout (location = 2) in vec2 a_TexCoord;
        layout(location = 3) in float a_TexIndex;

        layout(std140, binding = 0) uniform Camera
        {
	        mat4 u_ViewProjection;
        };

        out vec4 v_Color;
        out float v_TexIndex;
        out vec2 v_TexCoord;

        void main()
        {
	        v_Color    = a_Color;
	        v_TexCoord = a_TexCoord;
	        v_TexIndex = a_TexIndex;

	        gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
        }
    )"
};

utd::cstring fragment_shader
{
    R"(
        #version 450
        layout(location = 0) out vec4 o_Color;

        in vec4 v_Color;
        in vec2 v_TexCoord;
        in float v_TexIndex;

        layout(binding = 0) uniform sampler2D u_Textures[32];

        void main()
        {
            int index = int(v_TexIndex);
            o_Color = texture(u_Textures[index], v_TexCoord);
        }
    )"
};

int map[]
{
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0,
    0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
void cubes::on_attach()
{
    static float vertices[] = {
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

    quad quads[4]
    {
        { { -0.5f, -0.5f, 0.0f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f}, 1.f},
        { {  0.5f, -0.5f, 0.0f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f}, 1.f},
        { {  0.5f,  0.5f, 0.0f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f}, 1.f},
        { { -0.5f, 0.5f, 0.0f},  {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f}, 1.f}
    };

    //std::uptr<utd::vertex_buffer> vb = std::make_unique<utd::vertex_buffer>(quads, sizeof(quads) / sizeof(quad));
    //vb->set_layout
    //(
    //    {
    //       { utd::shader::datatype::FLOAT3, "aPos"     },
    //       { utd::shader::datatype::FLOAT4, "aColor"        },
    //       { utd::shader::datatype::FLOAT2, "aTexCoord"     },
    //       { utd::shader::datatype::FLOAT,  "a_TexIndex"     },
    //       //{ utd::shader::datatype::FLOAT,  "a_TilingFactor" }, 
    //    }
    //);

    //m_vertex_array.push_back(std::move(vb));
   
    
    //m_texture_manager.load("cobblestone", "E:/Programming/untile/Sandbox/assets/textures/cobblestone_2.png");
    //m_texture_manager.load("sand", "E:/Programming/untile/Sandbox/assets/textures/sand_3.png");
    //m_texture_manager.load("grass", "E:/Programming/untile/Sandbox/assets/textures/grass_3.png");
    
    /*glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));*/
    m_sand = m_texture_manager.fetch("sand", "E:/Programming/untile/Sandbox/assets/textures/sand_3.png");
    m_grass = m_texture_manager.fetch("grass", "E:/Programming/untile/Sandbox/assets/textures/grass_2.png");
    m_cobblestone = m_texture_manager.fetch("cobblestone", "E:/Programming/untile/Sandbox/assets/textures/cobblestone_1.png");
    
    //m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_up));

    m_camera.type(utd::camera::type::PERSPECTIVE);
    m_camera.position(glm::vec3(0.f, 0.f, 3.f));
    m_camera.perspective(90.f, 0.1f, 100.f);
    m_camera.orthographic(2.f, 100.f, 100.f);
    m_camera.viewport(16 / 9.f);
    static utd::ref_ptr<utd::texture> texture_map[]
    {
        utd::ref_ptr<utd::texture>(), m_grass, m_cobblestone, m_sand
    };
    
    for (auto& row : m_sprites)
    {
        for (auto& sprite : row)
        {
            sprite = { texture_map[rand() % 4], {(std::rand() % 255) / 255.f, (std::rand() % 255) / 255.f, (std::rand() % 255) / 255.f, 1.f}, 1.f};
        }
    }
     
    //m_sprites[1] = { {0.f, 0.f, 0.f, 1.f}, m_grass, 1.f };
    //m_sprites[0] = { {0.f, 0.f, 0.f, 1.f}, m_cobblestone, 1.f };
    //m_sprites[2] = { {0.f, 0.f, 0.f, 1.f}, m_sand, 1.f };
    //m_sprites[3] = { {0.f, 0.f, 0.f, 1.f}, m_cobblestone, 1.f };

    //for (int x = 0; x < 100; x++)
    //{
    //    for (int y = 0; y < 100; y++)
    //    {
    //        //UTD_ENGINE_ERROR("map[current]: {0}", map[x * 26 + y]);
    //        m_sprites[x][y] = { {0.f, 0.f, 0.f, 1.f}, texture_map[(y % 3) + 1], 1.f };
    //    }
    //}
}


void cubes::on_detach()
{

}

void cubes::on_render()
{
    static auto drawing = utd::sprite{m_texture_manager.fetch("drawing", "E:/Programming/untile/Sandbox/assets/textures/drawing.png")};
    static auto image = utd::image::load(m_texture_manager.get("drawing").path());
    
    auto& io = ImGui::GetIO();
    ImGui::Begin("Cube Scene");                          // Create a window called "Hello, world!" and append into it.

    if(ImGui::Button("Reload"))
    {
        UTD_PROFILE_SCOPE("Texture Reloading");
        image = utd::image::load(m_texture_manager.get("drawing").path());
        drawing.texture->set_data(*image.get());
    }
    
    ImGui::Text("Texture Manager ");
    ImGui::SameLine();
    ImGui::Text(std::to_string(m_texture_manager.get("drawing").get_id()).c_str());

    ImGui::Text("drawing ");
    ImGui::SameLine();
    ImGui::Text(std::to_string(drawing.texture->get_id()).c_str());

    //ImGui::Text("T");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("visible", &visible);
    auto stats = get_stats();
    //ImGui::SliderFloat("Camera FOV", &camera.fovy(), 30.f, 135.f);
    ImGui::Text((std::string("Quads: ") + std::to_string(stats.quad_drawn_count)).c_str());
    ImGui::Text("Draw Calls: ");
    ImGui::SameLine();
    ImGui::Text(std::to_string(stats.draw_calls).c_str());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

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
    
    if (visible)
    {
        //utd::renderer::command::depth_buffer(true);
        static utd::circle circle = { {0.f, 1.f, 1.f, 1.f}, m_cobblestone, glm::vec4{1.f, 0.f, 1.f, 1.f}, 0.0f };
        
        //m_texture_shader->bind();

        //utd::renderer::command::draw_indexed(m_vertex_array, 12);


        //utd::renderer2D::begin(m_camera, glm::lookAt(m_camera.position(), { -0.5f, -1.f, -10.f }, {0.f, 1.f, 0.f}));
        //int i = 0;
        //for (auto& sprite : m_sprites)
        //{
        //    utd::renderer2d::draw({ { -0.5f + i, -0.5f - i, -30.f }, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f} }, sprite);
        //    i++;
        //}

         //utd::renderer2d::draw({{-0.5f, 0.5f, -30.f}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}}, {{0.f, 0.f, 0.f, 1.f}, m_cobblestone, 1.f});
         //utd::renderer2d::draw({ { 0.5f, 0.5f, -30.f }, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f} }, { {0.f, 0.f, 0.f, 1.f}, m_sand, 1.f });
         //utd::renderer2d::draw({ { 0.5f,-0.5f, -30.f }, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f} }, { {0.f, 0.f, 0.f, 1.f}, m_cobblestone, 1.f });
         //utd::renderer2d::draw({ { -0.5f, -0.5f, -30.f }, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f} }, { {0.f, 0.f, 0.f, 1.f}, m_grass, 1.f});

        //utd::renderer2d::begin(m_camera, glm::translate(glm::mat4(1.0f), glm::vec3(-0.f, -0.f, -3.f)));


        utd::renderer2D::begin(m_editor_camera);
         //for (auto x = 0; x < 100; x++)
         //{
         //    for (auto y = 0; y < 100; y++)
         //    {
         //        utd::renderer2d::draw({ {  y,  x, -30.f }, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f} }, m_sprites[x][y]);
         //    }
         //}

        utd::renderer2d::draw({ {  0.f,  0.f, -10.f }, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f} }, drawing);
        utd::renderer2d::draw({ {  10.f,  0.f, -10.f }, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f} }, circle);

        /*for (int y = 0; y < 32; y++)
        {
            for (int x = 0; x < 26; x++)
            {
                utd::renderer2d::draw({ {  y,  x, -30.f }, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f} }, { { 0.f, 0.f, 0.f, 1.f }, texture_map[map[y * 26 + x] + 1], 4.f });
            }
        }*/

       utd::renderer2d::end();
        /*m_cobblestone->bind();
        for (auto i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), { 1.0f, 0.3f, 0.5f });
            m_texture_shader->mat4("model", model);

            utd::renderer::command::draw_arrays(m_vertex_array, 0, 36);

        }*/

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
        m_editor_camera.on_update(dt);
        ////m_camera.projection(glm::perspective(glm::radians(m_camera.fovy()), 16 / 9.f, 0.1f, 1000.f));
        ////m_camera.perspective(m_camera.fovy(), 0.1f, 1000.f);

        bool camera_moved = false;
        auto speed = 5.f * dt * 2;

        ////m_texture_shader->set_mat4("view", glm::lookAt(m_camera.position(), m_camera.position() + m_front, m_up));

        //if (m_pitch > 89.9f)
        //    m_pitch = 89.9f;
        //else if (m_pitch < -89.9f)
        //    m_pitch = -89.9f;

        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

        m_right = glm::normalize(glm::cross(m_front, m_up));

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

            pos.x += cos(glm::radians(0.f)) * speed;
            pos.y += sin(glm::radians(0.f)) * speed;
            m_camera.position(pos);
          /*  pos -= glm::normalize(glm::cross(m_front, m_up)) * speed;
            m_camera.position(pos);*/
            
            camera_moved = true;

        }
        if (utd::input::is_key_pressed(utd::key::D))
        {
            auto pos = m_camera.position();
            pos.x -= cos(glm::radians(0.f)) * speed;
            pos.y -= sin(glm::radians(0.f)) * speed;
            m_camera.position(pos);
            
            camera_moved = true;
        }

        ////UTD_PROFILE_SCOPE("LookAt");
        //
        //m_texture_shader->bind();
        //auto view = glm::lookAt(m_camera.position(), m_camera.position() + m_front, m_up);
        //m_texture_shader->mat4("u_viewprojection", m_camera.projection() * view);

        //if (camera_moved)
        {

            //m_texture_shader->mat4("u_view", view_projection);
        }

        UTD_PROFILE_END();

    }
}

void cubes::on_event(utd::event& event)
{
    m_editor_camera.on_event(event);
    //utd::event_dispatcher::dispatch<mouse_scroll_event>(UTD_BIND_EVENT(event, ));
}

cubes::~cubes()
{
}
