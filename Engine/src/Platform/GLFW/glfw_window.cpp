#include <upch.h>
#include "glfw_window.h"
#include <glfw/glfw3.h>

#include <Engine/Core/Assert.h>
#include <Engine/Events/key_event.h>
#include <Engine/Events/mouse_event.h>
#include <Engine/Events/application_event.h>

static utd::u32 s_glfw_window_counter = {0};

utd::glfw_window::glfw_window(u32 width, u32 height, const std::string &title, const event_callback_func& callback)
{
    init(width, height, title, callback);
}

utd::glfw_window::glfw_window(const properties& props, const event_callback_func& callback)
{
    init(props.width, props.height, props.title, props.event_callback);
}

void utd::glfw_window::init(u32 width, u32 height, const std::string& title, const event_callback_func& callback)
{
    m_properties = { width, height, title };

    if (s_glfw_window_counter == 0)
    {
        auto status = glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
        UTD_ENGINE_ASSERT(status, "Can\'t initialize GLFW");
    }
    UTD_ENGINE_INFO("glfw is initialized");

    m_handle = glfwCreateWindow(width, height, m_properties.title.c_str(), nullptr, nullptr);
    s_glfw_window_counter++;
    UTD_ENGINE_INFO("Window is created");

    m_context = graphics_context::create();
    m_context->init(m_handle);
    UTD_ENGINE_INFO("Graphics context has been initialized");

    glfwSetWindowUserPointer(native_handle(), &m_properties);
    glfwSetInputMode(native_handle(), GLFW_STICKY_KEYS, GLFW_TRUE);
    
    glfwSetKeyCallback(native_handle(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto& props = *(properties*)glfwGetWindowUserPointer(window);
        
        switch (action)
        {
            case GLFW_PRESS:
            {
                key_pressed_event event(key);
                props.event_callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                key_released_event event(key);
                props.event_callback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                key_pressed_event event(key);
                props.event_callback(event);
                break;
            }
        }
    });

    glfwSetWindowSizeCallback(native_handle(), [](GLFWwindow* window, int width, int height)
    {
        auto& props  = *(properties*)glfwGetWindowUserPointer(window);
        props.width  = width;
        props.height = height;

        window_resize_event event(width, height);
        props.event_callback(event);
    });
   
    glfwSetFramebufferSizeCallback(native_handle(), [](GLFWwindow* hwnd, int w, int h)
    {
        glViewport(0, 0, w, h);
    });

    glfwSetWindowCloseCallback(native_handle(), [](GLFWwindow* window)
    {
        auto& props = *(properties*)glfwGetWindowUserPointer(window);

        window_close_event event;
        props.event_callback(event);
    });

    glfwSetMouseButtonCallback(native_handle(), [](GLFWwindow* window, int button, int action, int mods)
    {
        auto& props = *(properties*)glfwGetWindowUserPointer(window);

        switch(action)
        {
            case GLFW_PRESS:
            {
                mouse_pressed_event event(button);
                props.event_callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                mouse_released_event event(button);
                props.event_callback(event);
                break;
            }

        }
    });

    glfwSetCursorPosCallback(native_handle(), [](GLFWwindow* window, double x, double y)
    {
        auto& props = *(properties*)glfwGetWindowUserPointer(window);
        
        mouse_moved_event event((float)x, (float)y);
        props.event_callback(event);
    });

    glfwSetScrollCallback(native_handle(), [](GLFWwindow* window, double xoffset, double yoffset)
    {
        auto& props = *(properties*)glfwGetWindowUserPointer(window);

        mouse_scrolled_event event(static_cast<float>(xoffset), static_cast<float>(yoffset));
        props.event_callback(event);

    });

    UTD_ENGINE_INFO("Callbacks are set");
}

void utd::glfw_window::init(const properties& props, const event_callback_func&)
{
#if 0
    m_properties = props;

    if (s_glfw_window_counter == 0)
    {
        auto status = glfwInit();
        UTD_ENGINE_ASSERT(status, "Can\'t initialize GLFW");
    }
    UTD_ENGINE_INFO("{0}", "glfw is initialized");

    m_handle = glfwCreateWindow(props.width, props.height, m_properties.title.c_str(), nullptr, nullptr);
    s_glfw_window_counter++;
    UTD_ENGINE_INFO("Window is created");

    m_context = graphics_context::create();
    m_context->init(m_handle);

    UTD_ENGINE_INFO("Graphics context has been created");

    glfwSetFramebufferSizeCallback(native_handle(), [](GLFWwindow* hwnd, int w, int h) { glViewport(0, 0, w, h); });

    UTD_ENGINE_INFO("Callbacks are set");
#endif // 0


}

void utd::glfw_window::default_callback(utd::event&)
{

}

void utd::glfw_window::show_cursor(bool state)
{
    if(state)
        glfwSetInputMode(native_handle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
        glfwSetInputMode(native_handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool utd::glfw_window::cursor_disabled()
{
    return glfwGetInputMode(native_handle(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}

bool utd::glfw_window::opened()
{
    return !glfwWindowShouldClose(native_handle());
}

glm::vec2 utd::glfw_window::size()
{
    return glm::vec2{ m_properties.width, m_properties.height };
}

bool utd::glfw_window::vsync()
{
	return m_vsync;
}

const std::string& utd::glfw_window::title()
{
    return m_properties.title;
}

const utd::window::event_callback_func& utd::glfw_window::callback()
{
    return m_properties.event_callback;
}

void utd::glfw_window::vsync(bool enable)
{
    glfwSwapInterval(enable);
}

void utd::glfw_window::title(const std::string_view title)
{
    glfwSetWindowTitle(native_handle(), title.data());
}

void utd::glfw_window::max_fps(u32)
{
}

void utd::glfw_window::callback(const event_callback_func& callback)
{
    m_properties.event_callback = callback;
}

void utd::glfw_window::close()
{
    glfwSetWindowShouldClose(native_handle(), true);
}

void utd::glfw_window::update()
{
    m_context->swap_buffers(m_handle);
    glfwPollEvents();
}

utd::glfw_window::~glfw_window()
{
    if(m_handle)
    {
        glfwDestroyWindow(native_handle());
        m_handle = nullptr;
    }

    if(--s_glfw_window_counter == 0)
        glfwTerminate();
}
