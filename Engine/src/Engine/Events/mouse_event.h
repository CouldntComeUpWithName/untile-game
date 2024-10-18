
#include <glm/vec2.hpp>

#include "event.h"
#include "Engine/Core/mouse.h"

namespace utd
{
    class mouse_moved_event : public event
    {
    public:
        mouse_moved_event(float x, float y)
            : m_x(x), m_y(y)
        { }
        
        glm::vec2 get_position(){ return glm::vec2{ m_x, m_y }; }
        
        float get_x(){ return m_x; } 
        float get_y(){ return m_y; }
        UTD_EVENT_TYPE(event::type::MOUSE_MOVE);
        virtual std::string str() const override { return debug_type() + ": " + std::to_string(m_x) + ", " + std::to_string(m_y); }

        virtual category_t get_category() const override { return event::category::MOUSE; }
    private:
        float m_x, m_y;
    };


    class mouse_event : public event
    {
    public:
        mouse_event(const mouse::code button)
            : m_code(button)
        { }

        mouse::code get_code() const { return m_code; }
        virtual category_t get_category() const override { return event::category::MOUSE; }
        virtual std::string str() const override { return debug_type() + ": " + std::to_string(m_code); }
    protected:
        mouse::code m_code;
    };

    class mouse_pressed_event : public mouse_event
    {
    public:
        mouse_pressed_event(const mouse::code button)
            : mouse_event(button)
        { }
        UTD_EVENT_TYPE(event::type::MOUSE_BUTTON_PRESSED)
    private:
    };

    class mouse_released_event : public mouse_event
    {
    public:
        mouse_released_event(const mouse::code button)
            : mouse_event(button)
        { }

        UTD_EVENT_TYPE(event::type::MOUSE_BUTTON_RELEASED)
    private:
    };

    class mouse_scrolled_event : public event
    {
    public:
        mouse_scrolled_event(const float offset_x, const float offset_y)
            : m_offset_x(offset_x), m_offset_y(offset_y)
        { }
        
        UTD_EVENT_TYPE(event::type::MOUSE_SCROLLED);

        float offset_x() { return m_offset_x; }
        float offset_y() { return m_offset_y; }
        virtual category_t get_category() const override { return event::category::MOUSE; }
        std::string str() const override
        {
            return debug_type();
        }
    private:
        float m_offset_x, m_offset_y;
    };

} /* namespace utd */
