#include "editor-layer.h"

#include <Engine/Core/input.h>
#include <Engine/Core/Application.h>
#include <Engine/Profiling/Profile.h>
#include <filesystem>

#include<Engine/Graphics/Renderer2D.h>
#include<Engine/Graphics/Texture.h>
#include<Engine/Graphics/Renderer.h>

#include <Engine/ImGui/imgui_layer.h>

#include<Engine/Scene/components.h>
#include<Engine/Scene/scene.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <string>
#include <chrono>
#include <thread>

void utd::editor_layer::on_attach()
{
    
    m_viewport_framebuffer = framebuffer::create(1920, 1080);
    m_viewport_framebuffer->attach(framebuffer::attachment::RGBA8, 
        framebuffer::attachment::RED_INT, framebuffer::attachment::DEPTH);

    m_active_scene = std::make_unique<utd::scene>();
    
    m_active_scene->build_scene();

    m_character_atlas = utd::atlas(64, 64, m_texture_manager.fetch("redhat", "E:/Programming/untile/Sandbox/assets/textures/redhat.png"));
    
    for(int i = 0; i < m_character_atlas.rows(); i++)
        m_character_atlas.add(i, 1);

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

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.01f, 0.01f));
    ImGui::Begin("Menu Bar", &dockspaceOpen, window_flags);
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
            if (ImGui::MenuItem("Open Project"))
            {

            }
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
    }
    ImGui::EndMenuBar();
    ImGui::End();

    ImGui::Begin("Project Inspector");                // Create a window called "Hello, world!" and append into it.

    ImGui::End();

    ImGui::Begin("Performance Statistics");
    {
        const auto& stats = renderer2d::stats();
        
        ImGui::Text((std::string("Quads: ") + std::to_string(stats.quad_drawn_count)).c_str());
        ImGui::Text("Draw Calls: ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(stats.draw_calls).c_str());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    }
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
    ImGui::Begin("Viewport");

    m_viewport_hovered = ImGui::IsWindowHovered();
    m_viewport_focused = ImGui::IsWindowFocused();
    
    m_viewport_framebuffer->bind();

    utd::renderer::command::clear_color(glm::vec4{ 0.5f, 0.9f, 1.f, 1.f });
    utd::renderer::command::clear();

    const auto viewport_region = ImGui::GetContentRegionAvail();
    m_editor_camera.viewport(viewport_region.x, viewport_region.y);
    
    static int i = 0;
    static time gt = utd::clock::now();
    
    renderer2d::begin(m_editor_camera);
    
    m_active_scene->on_render();
    // renderer2d::draw(m_character_atlas[i % m_character_atlas.count()], utd::transform{});
    // renderer2d::draw(utd::transform{{3.5f, 0.f, 0.f,}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}}, utd::sprite{m_texture_manager.get("redhat")});
    
    renderer2d::end();

    size_t tex_id = m_viewport_framebuffer->at(0);
    ImGui::Image((void*)tex_id, viewport_region, { 0, 1 }, { 1, 0 });
    m_viewport_framebuffer->unbind();
    ImGui::End();
    ImGui::PopStyleVar();



#if 0  
    ImGui::Image((void*)m_texture_manager.get("cobblestone").get_id(), size);

    ImGui::Text("T");               // Display some text (you can use a format strings too)

    ImGui::Checkbox("visible", &visible);
    auto stats = get_stats();
    //ImGui::SliderFloat("Camera FOV", &camera.fovy(), 30.f, 135.f);
    ImGui::Text((std::string("Quads: ") + std::to_string(stats.quad_drawn_count)).c_str());
    ImGui::Text("Draw Calls: ");
    ImGui::SameLine();
    ImGui::Text(std::to_string(stats.draw_calls).c_str());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    const char* projection_type_str[] = { "Perspective", "Orthographic" };
    const char* current_projection_type_str = projection_type_str[1];

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
#endif

}

void utd::editor_layer::_on_statistics_render()
{

}

void utd::editor_layer::on_render()
{
    UTD_IMGUI_SCOPE()
    {
        _on_imgui_render();
    }
    else
    {
        renderer2d::begin(m_editor_camera);

        m_active_scene->on_render();
        // renderer2d::draw(m_character_atlas[i % m_character_atlas.count()], utd::transform{});
        // renderer2d::draw(utd::transform{{3.5f, 0.f, 0.f,}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}}, utd::sprite{m_texture_manager.get("redhat")});

        renderer2d::end();
    }
}

void utd::editor_layer::on_update(float dt)
{
    UTD_IMGUI_SCOPE()
    {
        if(m_viewport_hovered && m_viewport_focused)
            m_editor_camera.on_update(dt);
    }
    else
    {
        m_editor_camera.on_update(dt);
    }
        
    m_active_scene->on_update(dt);

}

void utd::editor_layer::on_event(utd::event& event)
{
    UTD_IMGUI_SCOPE()
    {
        if (m_viewport_hovered)
            m_editor_camera.on_event(event);
    }
    else
    {
        m_editor_camera.on_event(event);
    }
}
