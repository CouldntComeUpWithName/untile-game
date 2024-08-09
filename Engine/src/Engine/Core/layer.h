#pragma once

#include <string>

#include <Engine/Core/Base.h>
#include <Engine/Events/event.h>

#define UTD_DEBUG_NAME(m_debug_name) std::string m_debug_name
#define UTD_DEBUG_NAME_FN(debug_name_func, m_debug_name) const char* debug_name(){ return #m_debug_name; }

namespace utd
{
    class layer
    {
    public:
        layer() = default;
        layer(const std::string& name)
        { }

        virtual void on_attach(){}
        virtual void on_detach(){}
        virtual void on_render() {}
        virtual void on_update(float){}
        virtual void on_event(event&){}
        virtual ~layer() = default;
    
        UTD_DEBUG_NAME_FN(debug_name, "basic layer");

    protected:
    };

}