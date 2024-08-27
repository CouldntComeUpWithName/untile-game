#pragma once

#include <Engine/Core/window.h>
#include <Engine/Core/base.h>
#include <Engine/Core/log.h>
#include <Engine/Graphics/graphics_context.h>

struct GLFWwindow;

namespace utd
{
    class glfw_window : public window
    {
    public:
        glfw_window() = default;
        glfw_window(u32 width, u32 height, const std::string& title, const event_callback_func& = default_callback);
        glfw_window(const properties&, const event_callback_func& = default_callback);

        void init(u32, u32, const std::string&, const event_callback_func& = default_callback);
        void init(const properties&, const event_callback_func& = default_callback);
    
    public:
        static void default_callback(event&);
        template<typename t_native = GLFWwindow>
        t_native* native_handle() { return static_cast<t_native*>(m_handle); }

    public:
        void show_cursor(bool state) override;
        bool cursor_disabled() override;

        bool opened() override;
        bool vsync() override;
        glm::vec2 size() override;
        const std::string& title() override;

        const event_callback_func& callback() override;
        void vsync(bool) override;
        void title(const std::string_view) override;
        void max_fps(u32) override;
        void callback(const event_callback_func&) override;
        
        void close() override;

        void update();

        ~glfw_window() override;

    private:
        bool m_vsync;
        std::uptr<graphics_context> m_context;
        properties m_properties;
    };
}