#include <upch.h>
#include "imgui_layer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Engine/Core/Application.h>

#include <Engine/Graphics/vertex_buffer.h>
#include <Engine/Graphics/index_buffer.h>
#include <Engine/Graphics/vertex_attrib.h>
#include <Engine/Graphics/renderer.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Profiling/profile.h>

utd::imgui_layer::imgui_layer(const std::string &name)
    : layer(name)
{

}

void utd::imgui_layer::on_attach()
{
    UTD_PROFILE_FUNC(utd::profile::color::deepred);
    UTD_IMGUI_SCOPE()
    {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        auto& window = singleton<application>::instance().get_window();
        ImGui_ImplGlfw_InitForOpenGL(window.native_handle<GLFWwindow>(), true);
#ifdef __EMSCRIPTEN__
        ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif

        ImGui_ImplOpenGL3_Init("#version 450");
    }
}

void utd::imgui_layer::on_detach()
{
    UTD_IMGUI_SCOPE()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}

void utd::imgui_layer::on_event(event& event)
{
    UTD_IMGUI_SCOPE()
    {
        //ImGuiIO& io = ImGui::GetIO();
        //event.handled |= (event.get_category() == event::category::MOUSE) & io.WantCaptureMouse;
        //event.handled |= (event.get_category() == event::category::KEYBOARD) & io.WantCaptureKeyboard;
    }
}

void utd::imgui_layer::begin() const
{
    UTD_IMGUI_SCOPE()
    {
        UTD_PROFILE_FUNC(profile::color::lightcyan);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
}

void utd::imgui_layer::end() const
{
    UTD_PROFILE_FUNC();
    
    UTD_IMGUI_SCOPE()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        auto window_size = application::instance().get_window().size();
        io.DisplaySize = ImVec2(window_size.x, window_size.y);

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            //UTD_PROFILE_SCOPE_C("Check if viewport enable in ImGui", tracy::Color::Red);
            GLFWwindow* backup_current_context = glfwGetCurrentContext();

            UTD_PROFILE_SCOPE("Update and Render ImGui viewport", profile::color::deeprose);
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

}


