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
        // friend T;
    public:
        static bool exists() { return m_instance; }
        static T& instance() { return *m_instance; }

    protected:
        static void init(T* other) { m_instance = other; }
        static void destroy() { m_instance = nullptr; }
    
    private:
        static inline T* m_instance = nullptr;
    };

    /// @brief "utility objects" have no state, provide no interface, and hold only static methods or data
    /// for performing specific useful operations 
    class utility
    {
    private:
        utility() = delete;
    };

} /* namespace utd */