#pragma once

namespace utd
{
    /// @brief an object cannot be copied
    class noncopyable
    {
    protected:
        noncopyable()  = default;
        ~noncopyable() = default;
    private:
        noncopyable(const noncopyable&)              = delete;
        noncopyable& operator = (const noncopyable&) = delete; 
    }; 
    
    template<typename T>
    class singleton : protected noncopyable
    {
        friend T;
    public:
        static bool exists() { return s_instance; }
        static T& instance() { return *s_instance; }

    protected:
        static void init(T* other) { s_instance = other; }
        static void destroy() { s_instance = nullptr; }
    
    private:
        singleton() = default;
    private:
        static inline T* s_instance = nullptr;
    };

    /// @brief "utility objects" have no state, provide no interface, and hold only static methods or data
    /// for performing specific useful operations 
    class utility
    {
    private:
        utility() = delete;
    };

} /* namespace utd */