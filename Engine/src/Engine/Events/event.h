#pragma once

#include "Engine/Core/base.h"

#include <string>
//TODO: probably generalize UTD_BIND_EVENT as UTD_BIND for everything
#define UTD_BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) {return this->fn(std::forward<decltype(args)>(args)...);}

#define UTD_EVENT_TYPE(TYPE) static consteval type static_type(){ return TYPE; }               \
                             type get_type() const override { return static_type(); }          \
                             virtual std::string debug_type() const override { return #TYPE; } \

namespace utd
{
    class event
    {
    public:
        using category_t = int;

    public:
        enum class type
        {
            NONE = 0,
            WINDOW_RESIZE,
            WINDOW_CLOSE,
            WINDOW_MOVE,
            MOUSE_MOVE,
            MOUSE_BUTTON_PRESSED,
            MOUSE_BUTTON_RELEASED,
            MOUSE_SCROLLED,
            MOUSE_SCROLLED_UP,
            MOUSE_SCROLLED_DOWN,
            KEY_PRESSED,
            KEY_RELEASED,
            APP_UPDATE,
            APP_RENDER,
        };

        enum category
        {
            NONE         = 0,
            INPUT        = BIT(0),
            KEYBOARD     = BIT(1),
            MOUSE        = BIT(2),
            MOUSE_BUTTON = BIT(3),
            APPLICATION  = BIT(4) 
        };

        event()          = default;
        virtual ~event() = default;

        virtual type get_type() const = 0;
        virtual category_t get_category() const = 0;

        // Debug part of the interface
       /* virtual const char* type_to_string() const     = 0;
        virtual const char* category_to_string() const = 0;
        virtual const char* debug_category() const     = 0;*/
        virtual std::string debug_type() const         = 0;
        virtual std::string str() const                = 0; 
    
    public:
        bool handled = false;
    protected:
        category_t m_category = NONE;
    };

    class event_dispatcher
    {
    public:
        template<typename t_event_type, typename t_func,
        std::enable_if_t<std::is_base_of_v<event, t_event_type>, int> = 0>
        static bool dispatch(event& event, const t_func& func)
        {
            if(event.get_type() == t_event_type::static_type())
            {
                event.handled |= func(static_cast<t_event_type&>(event));
                return true;
            }
            return false;
        }
    
    };

    inline std::ostream& operator << (std::ostream& ostream, const event& event)
    {
        return ostream << event.str();
    }
}