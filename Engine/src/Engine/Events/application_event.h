#pragma once

#include "event.h"

namespace utd
{   
    class window_close_event : public event
    {
    public:
        window_close_event() = default;
        UTD_EVENT_TYPE(event::type::WINDOW_CLOSE)
        category_t get_category()const override { return event::category::APPLICATION; }
        
        std::string str() const override
        {
            return debug_type();
        }
    
    };

    class window_resize_event : public event
    {
    public:
        window_resize_event(u32 width, u32 height)
            : m_width(width), m_height(height)
        { }

        u32 width() { return m_width; }
        u32 height() { return m_height; }

        UTD_EVENT_TYPE(event::type::WINDOW_RESIZE)
        category_t get_category() const override { return event::category::APPLICATION; }
        std::string str() const override { return debug_type() + ": " + std::to_string(m_width) + ", " + std::to_string(m_height); }
    private:
        u32 m_width, m_height;
    };
}
