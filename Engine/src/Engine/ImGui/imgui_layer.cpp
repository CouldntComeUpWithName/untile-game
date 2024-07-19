#include <upch.h>
#include "imgui_layer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Engine/Core/Application.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include<glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

utd::imgui_layer::imgui_layer(const std::string &name)
    : layer(name)
{

}

void utd::imgui_layer::on_attach()
{
    IMGUI_CHECKVERSION();
    
    ImGui::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
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
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window.native_handle(), true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
  
    ImGui_ImplOpenGL3_Init("#version 450");
}

void utd::imgui_layer::on_detach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();   
}

void utd::imgui_layer::on_event(event& event)
{
    ImGuiIO& io = ImGui::GetIO();
    event.handled |= event.get_category() == event::category::MOUSE & io.WantCaptureMouse;
    event.handled |= event.get_category() == event::category::KEYBOARD & io.WantCaptureKeyboard;
}

void utd::imgui_layer::begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void utd::imgui_layer::end()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    ImGuiIO& io = ImGui::GetIO();
    auto window_size = application::instance().get_window().size();
    io.DisplaySize = ImVec2(window_size.x, window_size.y);

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

}

void utd::demo::on_render()
{
    static float f = 0.0f;
    static int counter = 0;
    static bool show_demo_window = true;
    static bool show_another_window = true;
    static float clear_color[4]{};

    auto& io = ImGui::GetIO();
    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", clear_color); // Edit 3 floats representing a color

    auto& col = get_color();

    col.r = clear_color[0];
    col.g = clear_color[1];
    col.b = clear_color[2];
    col.a = clear_color[3];

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}
