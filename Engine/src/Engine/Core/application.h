#pragma once 
#include "base.h"
#include "window.h"
#include "graphics_context.h"
#include "assert.h"
#include "layer_stack.h"

#include <Engine/Events/event.h>
#include <Engine/Events/application_event.h>
#include <Engine/ImGui/imgui_layer.h>

namespace utd
{
    struct cmdline_args
    {
        int argc      = 0;
        char** argv = nullptr;
    };



    class application : public singleton<application>
    {
    public:
        application(const cmdline_args&);

        virtual ~application();

        void run();
        void on_event(event&);
        
        window& get_window() { return *m_window.get(); };
    protected:
        bool close(event&);
        bool window_resize(window_resize_event&);

    private:
        bool m_running;
        imgui_layer* m_imgui_layer;
        std::uptr<window> m_window;
        layer_stack m_layer_stack;

    };

    void triangle();
}