#include <upch.h>
#include "application.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <Engine/Core/Log.h>
#include <Engine/Core/Window.h>

#include <Engine/Events/application_event.h>
#include <Engine/Core/System/clock.h>

#include <Engine/Graphics/Renderer.h>
#include <Engine/Graphics/Shader.h>
#include <Engine/Graphics/Texture.h>

#include <Engine/Graphics/vertex_array.h>
#include <Engine/Graphics/index_buffer.h>
#include <Engine/Graphics/vertex_buffer.h>
#include <Engine/Graphics/vertex_attrib.h>

utd::application::application(const cmdline_args &)
    : m_running(true)
{ 
    UTD_ENGINE_ASSERT(!application::exists(), "Application already exists");
    singleton::init(this);

    m_window = window::create(1280, 720, "Untile");
    m_window->callback([this](event& e){ on_event(e); });
    
    UTD_ENGINE_ASSERT(m_window, "window is null");
    
    renderer::init();
    
    m_imgui_layer = new imgui_layer();
    m_layer_stack.push_overlay(m_imgui_layer);
    m_imgui_layer->on_attach();

    demo* dem = new demo();
    m_layer_stack.push_overlay(dem);
    dem->on_attach();
    
    auto* layer = new triangle_layer();
    m_layer_stack.push(layer);
    layer->on_attach();

}

utd::application::~application()
{
    if(s_instance == this)
    {    
        //release resources here
        singleton::destroy();
    }
}

void utd::application::run()
{
    using namespace utd::literals;
    //
    

    // auto triangle_shader = utd::shader::create(vertexShaderSource, fragmentShaderSource);
    // auto triangle_shader = utd::shader::load("E:/Programming/untile/Untile/assets/shaders/triangle.vert", "E:/Programming/untile/Untile/assets/shaders/triangle.frag");
    
    float vertices[] = {
        // positions          // colors                // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.f,    1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    auto texture_shader = utd::shader::load("E:/Programming/untile/Untile/assets/shaders/texture.vert", "E:/Programming/untile/Untile/assets/shaders/texture.frag");

    vertex_array va;
    
    std::uptr<vertex_buffer> vb = std::make_unique<vertex_buffer>(vertices, sizeof(vertices));
    vb->set_layout
    ({
        { shader::data_type::FLOAT3, "aPos" },
        { shader::data_type::FLOAT4, "aColor" },
        { shader::data_type::FLOAT2, "aTexCoord" },
    });

    va.push_back(std::move(vb));
    va.make_index_buffer(indices, sizeof(indices));

    //////float vertices[]
    //////{
    //////     //first
    //////    0.3f, -0.5f, 0.0f, // left  
    //////    0.3f, 0.5f, 0.0f, // right 
    //////    0.9f, -0.5f, 0.0f,  // top   
    //////};
    ////    //second

    ////    -0.7f, 0.7f, 0.f,
    ////    -0.3f, 0.7f, 0.f,
    ////    -0.35f, 0.9f, 0.f
    ////};

    ////u32 indices[]
    ////{
    ////    0, 1, 2,
    ////    3, 4, 5
    ////};
#if 0
    u32 VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
#endif // 0


    auto texture = utd::texture::load("E:/Programming/untile/Untile/assets/textures/sand_3.png");
    glBindTexture(GL_TEXTURE_2D, texture->get_id());
    
    /*glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

    //glPolygonMode(GL_ELEMENT_ARRAY_BUFFER, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glm::vec4 current_color(1.0f, 0.0f, 0.2f, 1.0f);

    renderer::set_clear_color({ 0.52f, 0.8f, 0.9f, 1.0f });
   
    texture_shader->bind();
    // texture_shader->set_float4("user_color", glm::vec4{1.f, 0.f, 0.f,1.f});
    utd::clock clock;
    while (m_running)
    {
        auto dt = clock.restart().sec();
        
        glBindTexture(GL_TEXTURE_2D, texture->get_id());
        texture_shader->bind();
        
        renderer::draw_indexed(va, 6);

        m_imgui_layer->begin();
        for(auto* layer : m_layer_stack)
        {
            layer->on_update(dt);
            layer->on_render();
        }
        m_imgui_layer->end();
        
        m_window->update();
        
        renderer::clear();
    }
}

bool utd::application::close(utd::event& event)
{
    return !(m_running = false);
}

bool utd::application::window_resize(window_resize_event& event)
{
    glViewport(0, 0, event.width(), event.height());
    return true;
}

void utd::application::on_event(event& event)
{
    event_dispatcher::dispatch<window_close_event>(event, UTD_BIND_EVENT(application::close));
    event_dispatcher::dispatch<window_resize_event>(event, UTD_BIND_EVENT(application::window_resize));

    UTD_ENGINE_INFO(event.str());
}

void utd::triangle()
{
    constexpr unsigned int SCR_WIDTH = 800;
    constexpr unsigned int SCR_HEIGHT = 600;

    const char* vertexShaderSource = /*vertex shader: */ "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

        auto window = utd::window::create(1280, 720, "Untile");

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // link shaders
        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left  
             0.5f, -0.5f, 0.0f, // right 
             0.0f,  0.5f, 0.0f  // top   
        };

        unsigned int VBO, VAO;
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(false);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        //glBindVertexArray(0);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // ImGui::StyleColorsDark();
        ImGui::StyleColorsClassic();
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->native_handle(), true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // uncomment this call to draw in wireframe polygons.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUseProgram(shaderProgram);
        // render loop
        // -----------
        while (window->opened())
        {
            // input
            // -----

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // draw our first triangle
            //glUseProgram(shaderProgram);
            //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            //glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            ImGui::Begin("ImGUI");
            ImGui::Text("Caption here");
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            window->title();
            window->update();
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
}
