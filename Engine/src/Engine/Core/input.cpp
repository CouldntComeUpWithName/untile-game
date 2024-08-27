#include <upch.h>
#include "input.h"

#include <Engine/Core/application.h>
#include <Engine/Core/assert.h>
#include <Engine/Profiling/profile.h>

#include <GLFW/glfw3.h>

bool utd::input::is_key_pressed(const key::code keycode)
{
    UTD_PROFILE_FUNC();
    UTD_ENGINE_ASSERT(application::exists(), "Application doesn't exist yet!");

    auto* window = application::instance().get_window().native_handle<GLFWwindow>();
    
    auto state = glfwGetKey(window, keycode);
    
    return state == GLFW_PRESS;
}

bool utd::input::is_key_released(const key::code keycode)
{
    UTD_ENGINE_ASSERT(application::exists(), "Application doesn't exist yet!");

    auto* window = application::instance().get_window().native_handle<GLFWwindow>();

    auto state = glfwGetKey(window, keycode);

    return state == GLFW_RELEASE;
}

glm::vec2 utd::input::get_mouse_pos()
{
    UTD_ENGINE_ASSERT(application::exists(), "Application doesn't exist yet!");

    double x, y;
    glfwGetCursorPos(application::instance().get_window().native_handle<GLFWwindow>(), &x, &y);
    
    return glm::vec2{ x, y };
}

float utd::input::get_mouse_x()
{
    UTD_ENGINE_ASSERT(application::exists(), "Application doesn't exist yet!");
    
    double x, y;
    glfwGetCursorPos(application::instance().get_window().native_handle<GLFWwindow>(), &x, &y);
    
    return static_cast<float>(x);
}

float utd::input::get_mouse_y()
{
    UTD_ENGINE_ASSERT(application::exists(), "Application doesn't exist yet!");

    double x, y;
    glfwGetCursorPos(application::instance().get_window().native_handle<GLFWwindow>(), &x, &y);

    return static_cast<float>(y);
}

bool utd::input::is_mouse_moved()
{
    UTD_ENGINE_ASSERT(application::exists(), "Application doesn't exist yet!");
    
    double x = {}, y = {};
    glfwGetCursorPos(application::instance().get_window().native_handle<GLFWwindow>(), &x, &y);
    
    static float mouse_last_x = static_cast<float>(x);
    static float mouse_last_y = static_cast<float>(y);

    bool result = (mouse_last_x != x) || (mouse_last_y != y);
    
    mouse_last_x = static_cast<float>(x);
    mouse_last_y = static_cast<float>(y);

    return result;
}

bool utd::input::is_mouse_button_pressed(const mouse::code button)
{
    UTD_ENGINE_ASSERT(application::exists(), "Application doesn't exist yet!");
    
    auto* window = application::instance().get_window().native_handle<GLFWwindow>();

    bool state = glfwGetMouseButton(window, static_cast<i32>(button));
    
    return state == GLFW_PRESS;
}

bool utd::input::is_mouse_button_released(const mouse::code button)
{
    UTD_ENGINE_ASSERT(application::exists(), "Application doesn't exist yet!");
    
    auto* window = application::instance().get_window().native_handle<GLFWwindow>();

    bool state = glfwGetMouseButton(window, static_cast<i32>(button));

    return state == GLFW_RELEASE;
}
