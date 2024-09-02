#include "editor-layer.h"

#include <Engine/Core/input.h>
#include <Engine/Core/Application.h>
#include <Engine/Profiling/Profile.h>
#include <filesystem>

#include<Engine/Graphics/Renderer2D.h>
#include<Engine/Graphics/Texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <string>

void utd::editor_layer::on_attach()
{
    m_sand = m_texture_manager.fetch("sand", "E:/Programming/untile/Untile/assets/textures/sand_3.png");
    m_grass = m_texture_manager.fetch("grass", "E:/Programming/untile/Untile/assets/textures/grass_2.png");
    m_cobblestone = m_texture_manager.fetch("cobblestone", "E:/Programming/untile/Untile/assets/textures/cobblestone_1.png");
}


void utd::editor_layer::on_detach()
{

}

void utd::editor_layer::_on_imgui_render()
{
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 100.f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open Project"));
            if (ImGui::MenuItem("Exit"))
            {
                application::instance().close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Begin("Cube Scene");                // Create a window called "Hello, world!" and append into it.

    //ImGui::Text("T");               // Display some text (you can use a format strings too)

    //ImGui::Checkbox("visible", &visible);
    //auto stats = get_stats();
    ////ImGui::SliderFloat("Camera FOV", &camera.fovy(), 30.f, 135.f);
    //ImGui::Text((std::string("Quads: ") + std::to_string(stats.quad_drawn_count)).c_str());
    //ImGui::Text("Draw Calls: ");
    //ImGui::SameLine();
    //ImGui::Text(std::to_string(stats.draw_calls).c_str());
    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    //const char* projection_type_str[] = { "Perspective", "Orthographic" };
    //const char* current_projection_type_str = projection_type_str[1];

    // if (ImGui::BeginCombo("Projection", current_projection_type_str))
    // {
    //     for (int i = 0; i < 2; i++)
    //     {
    //         bool selected = current_projection_type_str == projection_type_str[i];
    //         if (ImGui::Selectable(projection_type_str[i], selected))
    //         {
    //             current_projection_type_str = projection_type_str[i];
    //             m_camera.type((utd::camera::type)i);
    //         }

    //         if (selected) ImGui::SetItemDefaultFocus();
    //     }
    //     ImGui::EndCombo();
    // }

    // ImGui::BeginGroup();

    // if (m_camera.type() == utd::camera::type::ORTHOGRAPHIC)
    // {
    //     float curr_size = m_camera.ortho_size();
    //     if (ImGui::SliderFloat("ortho size", &curr_size, 0.1f, 100.f))
    //     {
    //         m_camera.ortho_size(curr_size);
    //     }
    // }
    // else
    // {
    //     float curr_fov = m_camera.fovy();
    //     if (ImGui::SliderFloat("FOV", &curr_fov, 0.1f, 100.f))
    //     {
    //         m_camera.fovy(curr_fov);
    //     }
    // }

    //ImGui::EndGroup();

    ImGui::End();
}

void utd::editor_layer::on_render()
{
    _on_imgui_render();
}

void utd::editor_layer::on_update(float dt)
{
    m_editor_camera.on_update(dt);
}

void utd::editor_layer::on_event(utd::event& event)
{
    m_editor_camera.on_event(event);
}
