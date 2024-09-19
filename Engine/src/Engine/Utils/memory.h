#pragma once
#include <memory>
#include "windows.devices.input.preview.h"

namespace utd::filesystem
{
    
}

namespace utd
{
    class texture;
    class shader;
    enum class image_format;

    typedef size_t bytes_t;
    
    // these evaluate the size in bytes for the value, not the data type which represents them
    bytes_t vmemsize_of(const texture&);
    bytes_t vmemsize_of(const shader&);
    bytes_t memsize_of(const image_format&);
    
    template<typename t_resource>
    inline bytes_t vmemsize_of(t_resource&&)
    {
        return {};
    }

    /// @brief an useful imho hybrid resource wrapper which's supposed to own a resource, NOT to manage it
    template<typename T>
    class ref_ptr
    {
    public:
        ref_ptr() = default;
        ref_ptr(const ref_ptr<T>& ref)
             : m_ptr(ref.m_ptr)
        { }

        ref_ptr(const std::unique_ptr<T>& uptr) : m_ptr(uptr.get())
        {}
        
        ref_ptr(std::nullptr_t null)
            : m_ptr{null}
        {
        }
        
        ref_ptr(T& _ref)
        : m_ptr(&_ref)
        { }
        
        ref_ptr(ref_ptr<T>&&) = default;
        ref_ptr<T>& operator = (const ref_ptr<T>& ref)
        {
            if(this == &ref)
                return *this;
            
            m_ptr = ref.m_ptr;
            
            return *this;
        }
        
        ref_ptr<T>& operator = (ref_ptr<T>&&)      = default;
        ref_ptr<T>& operator = (const std::unique_ptr<T>& _uref)
        {
            m_ptr = _uref.get();
            return *this;
        }
        ref_ptr<T>& operator = (const T& ref)
        {
            static_assert(std::is_copy_assignable_v<T>, "resource can\'t be copied, the object is non-copyable");
            *m_ptr = ref;
            return *this;
        }
        ref_ptr<T>& operator = (std::nullptr_t null)
        {
            m_ptr = null;
        }
        operator bool() const { return m_ptr; }
        bool valid() const { return m_ptr; }
    
        T* operator->() const { return m_ptr; }
        inline T& operator*() { return *m_ptr; }
        
        T& operator[](int index) { return m_ptr[index]; }

        inline T* get() const { return m_ptr; }
        inline T& ref() const { return *m_ptr; }
        
    public:
        T* m_ptr = nullptr;
    };

    template<typename T>
    typename ref_ptr<T> make_ref(T& _ref)
    {
        return ref_ptr<T>(_ref);
    }

    template<typename T>
    typename ref_ptr<T> make_ref(std::unique_ptr<T>& _ref)
    {
        return ref_ptr<T>(_ref.get());
    }
    
    namespace diu // digital information unit
    {
        consteval size_t operator""_b(size_t val) noexcept
        {
            return val;
        }
        
        consteval size_t operator""_kb(long double val) noexcept
        {
            return static_cast<size_t>(val * 1024_b);
        }

        consteval size_t operator""_kb(size_t val) noexcept
        {
            return val * 1024_b;
        }

        consteval size_t operator""_mb(long double val) noexcept
        {
            return static_cast<size_t>(val * 1024_kb);
        }

        consteval size_t operator""_mb(size_t val) noexcept
        {
            return val * 1024_kb;
        }
    }
};
