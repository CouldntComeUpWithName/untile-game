#pragma once

#include <Engine/Core/Base.h>
#include <Engine/Core/key.h>
#include <Engine/Core/mouse.h>

namespace utd
{
    class window;
    struct input : utility
    {
        static bool is_key_pressed(const key::code);
        static bool is_key_released(const key::code);
        static void get_mouse_pos(double* x, double* y);
        static bool is_mouse_moved();
        static bool is_mouse_button_pressed(const mouse::code);
        static bool is_mouse_wheel_moved();
        
        static bool is_mouse_off(const utd::window&);
        static bool is_mouse_on(const utd::window&);

        static bool lost_focus(const utd::window&);
        static bool gained_focus(const utd::window&);
    };

} /* namespace utd */