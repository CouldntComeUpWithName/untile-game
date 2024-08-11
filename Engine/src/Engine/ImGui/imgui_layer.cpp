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
    IMGUI_CHECKVERSION();
    
    UTD_PROFILE_FUNC(utd::profile::color::deepred);

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
    event.handled |= (event.get_category() == event::category::MOUSE) & io.WantCaptureMouse;
    event.handled |= (event.get_category() == event::category::KEYBOARD) & io.WantCaptureKeyboard;
}

void utd::imgui_layer::begin() const
{
    UTD_PROFILE_FUNC(profile::color::lightcyan);

#if !defined(UTD_CONFIG_SHIP) && UTD_IMGUI_DISABLE == 0
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    UTD_PROFILE_BEGIN("ImGui::NewFrame")
    ImGui::NewFrame();
    UTD_PROFILE_END();
#endif
    //ZoneNamedN(setupzone, "ImGui begin", true);
}

void utd::imgui_layer::end() const
{
    UTD_PROFILE_FUNC(profile::color::darkcyan);

#if !defined(UTD_CONFIG_SHIP) && UTD_IMGUI_DISABLE == 0
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

#endif /* !defined(UTD_CONFIG_SHIP) || UTD_IMGUI_DISABLE != 1 */

}

void utd::demo::on_render()
{
    UTD_PROFILE_FUNC();
    
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

void utd::triangle_layer::on_attach()
{
    float vertices[]
    {
        //first
       0.3f, -0.5f, 0.0f,  // left  
       0.3f, 0.5f, 0.0f,   // right 
       0.9f, -0.5f, 0.0f,  // top   
       //second
       -0.5f, 0.5f, 0.f,
       -0.3f, 0.5f, 0.f,
       -0.4f, 0.9f, 0.f
    };

    

    m_shader = utd::shader::load("E:/Programming/untile/Untile/assets/shaders/triangle.vert", "E:/Programming/untile/Untile/assets/shaders/triangle.frag");
    
    std::uptr<vertex_buffer> vb = std::make_unique<vertex_buffer>(vertices, sizeof(vertices));
    vb->set_layout
    ({
        { shader::data_type::FLOAT3, "aPos" },
    });

    m_vertex_array.push_back(std::move(vb));
    //m_vertex_array.set_index_buffer(std::make_unique<index_buffer>(indices, sizeof(indices)));
    
        u32 indices[]
        {
            0, 1, 2,
            3, 4, 5
        };

        m_vertex_array.make_index_buffer(indices);

    //auto& vert_buffer = m_vertex_array.emplace_back(vertices, sizeof(vertices));
    //vert_buffer.set_layout
    //({
    //    { shader::data_type::FLOAT3, "aPos" },
    //});

    //m_vertex_array.set_index_buffer(std::make_unique<index_buffer>(indices, sizeof(indices)));

}

void utd::triangle_layer::on_render()
{

    UTD_PROFILE_FUNC(); 
    //UTD_GET_RESPECTIVE_MACRO_IMPL("Hello", tracy::Color::Blue, ZoneScopedNC, ZoneScopedN)("Hello", tracy::Color::Blue);
    static float temp_color[4]{};
    static unsigned int VBO, VAO, EBO;

#if 0
    
    static float vertices[]
    {
        //first
       0.3f, -0.5f, 0.0f,  // left  
       0.3f, 0.5f, 0.0f,   // right 
       0.9f, -0.5f, 0.0f,  // top   
       //second
       -0.5f, 0.5f, 0.f,
       -0.3f, 0.5f, 0.f,
       -0.4f, 0.9f, 0.f
    };

    static u32 indices[]
    {
        0, 1, 2,
        3, 4, 5
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //// texture coord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);  
#endif // 0


    auto& io = ImGui::GetIO();
    ImGui::Begin("Hello, Triangle!");                          // Create a window called "Hello, world!" and append into it.

    //ImGui::Text("T");               // Display some text (you can use a format strings too)
    //ImGui::Checkbox("visible", &visible);      // Edit bools storing our window open/close state

    //ImGui::SameLine();

    ImGui::ColorEdit3("clear color", temp_color); // Edit 3 floats representing a color
    
    color.r = temp_color[0];
    color.g = temp_color[1];
    color.b = temp_color[2];
    color.a = temp_color[3];

    ImGui::End();

    if(visible)
    {
        utd::renderer::draw_indexed(m_vertex_array, 6);
        /*glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
    }
}

void utd::triangle_layer::on_update(float dt)
{
    static float accumulator = 0.0f;

    if(accumulator <= 1.f)
        accumulator += 0.25f * dt;
    
    m_shader->bind();
    UTD_PROFILE_SCOPE(UTD_CONCAT(__FUNCTION__, " shader values setting"), profile::color::lightred);
    m_shader->vec4("triangle_color", color);
    m_shader->real("offset", accumulator);

}
