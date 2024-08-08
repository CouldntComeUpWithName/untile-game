#include <upch.h>
#include "input.h"

#include <Engine/Core/application.h>
#include <Engine/Core/application.h>
#include <Engine/Core/assert.h>

#include <GLFW/glfw3.h>
#include <Engine/Profiling/profile.h>

bool utd::input::is_key_pressed(const key::code keycode)
{
    UTD_ENGINE_ASSERT(application::instance().exists(), "Application doesn't exist yet!");
    UTD_PROFILE_FUNC();

    auto* window = static_cast<GLFWwindow*>(application::instance().get_window().native_handle());
    
    auto state = glfwGetKey(window, keycode);
    
    return state == GLFW_PRESS;
}

bool utd::input::is_key_released(const key::code keycode)
{
    auto* window = static_cast<GLFWwindow*>(application::instance().get_window().native_handle());
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    auto state = glfwGetKey(window, keycode);

    return state == GLFW_RELEASE;
}

void utd::input::get_mouse_pos(double* x, double* y)
{
    glfwGetCursorPos((GLFWwindow*)application::instance().get_window().native_handle(), x, y);
}

bool utd::input::is_mouse_moved()
{
    double x = {}, y = {};
    glfwGetCursorPos((GLFWwindow*)application::instance().get_window().native_handle(), &x, &y);
    
    static float mouse_last_x = x;
    static float mouse_last_y = y;

    bool result = (mouse_last_x != x) || (mouse_last_y != y);
    
    mouse_last_x = x;
    mouse_last_y = y;

    return result;
}
