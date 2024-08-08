#pragma once
#include "base.h"
#include <glm/vec2.hpp>
#include <functional>
#include <string>

#include <Engine/Utils/object_traits.h>
#include <Engine/Events/event.h>

namespace utd
{
    class window : noncopyable
    {
    public:
        using event_callback_func = std::function<void(event&)>;

        struct properties
        {
            u32         width = {};
            u32        height = {};
            std::string title = {};
            std::function<void(event&)> event_callback = nullptr;
        };

    public:
        window() = default;

        virtual void show_cursor(bool = true) = 0;
        virtual bool cursor_disabled()      = 0;
        virtual void* native_handle()       = 0;
        virtual bool opened()               = 0; 
        virtual bool vsync()                = 0;
        virtual glm::vec2 size()            = 0;
        virtual const std::string& title()  = 0;
        virtual const event_callback_func& callback() = 0;
        
        virtual void vsync(bool)               = 0;
        virtual void title(const std::string_view) = 0;
        virtual void max_fps(u32)              = 0; 
        virtual void callback(const event_callback_func&) = 0;

        virtual void close() = 0;
        virtual void update() = 0;
        virtual ~window() = default;
    //temp methods
        //virtual void* poll() = 0;
    public:
        static std::uptr<window> create(u32, u32, const std::string&);
        static std::uptr<window> create(const properties&);
    };

} /* namespace utd */

// int App(std::string_view title);

void Triangle();