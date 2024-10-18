#pragma once

#include "Event.h"
#include <Engine/Core/key.h>

namespace utd
{
    class key_event : public event
    {
    public:
        key_event(key::code keycode)
            : m_keycode(keycode)
        { }

        event::category_t get_category() const override { return event::KEYBOARD; }
        std::string str() const override
        {
            std::string string = debug_type() + ": " + std::to_string(m_keycode);

            return string;
        }
    protected:
        key::code m_keycode;
    };
    
    class key_pressed_event : public key_event
    {
    public:
        key_pressed_event(key::code keycode, bool is_repeated = false)
            : key_event(keycode), m_repeated(is_repeated)
        { }

        bool is_repeated() const { return m_repeated; }
        UTD_EVENT_TYPE(event::type::KEY_PRESSED)
        
    private:
        bool m_repeated;
    };

    class key_released_event : public key_event
    {
    public:
        key_released_event(key::code keycode)
            : key_event(keycode)
        { }

        UTD_EVENT_TYPE(event::type::KEY_RELEASED)
    private:
    };
}