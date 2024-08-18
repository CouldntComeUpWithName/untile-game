#pragma once
#include <memory>

namespace utd
{
    /// @brief useful imho hybrid resource wrapper which's supposed to own a resource, NOT to manage it
    template<typename T>
    class ref_ptr
    {
    public:
        ref_ptr() = default;
        ref_ptr(const ref_ptr<T>& ref) = default;
        //     : m_ptr(ref.m_ptr)
        // { }

        ref_ptr(const std::unique_ptr<T>& uptr) : m_ptr(uptr.get()) {}
        
        ref_ptr(T& _ref)
        : m_ptr(&_ref)
        { }
        
        ref_ptr(ref_ptr<T>&&) = default;
        ref_ptr<T>& operator = (const ref_ptr<T>& ref) = default;
        // {
        //     // if(*this == ref)
        //     //     return *this;
            
        //     m_ptr = ref.m_ptr;
            
        //     return *this;
        // }
        ref_ptr<T>& operator = (ref_ptr<T>&&)      = default;
        ref_ptr<T>& operator = (const std::unique_ptr<T>& _uref)
        {
            m_ptr = _uref.get();
            return *this;
        }
        ref_ptr<T>& operator = (const T& ref)
        {
            static_assert(std::is_copy_assignable_v<T>, "You should never move resources to ref_ptr");
            *m_ptr = ref;
            return *this;
        }

        operator bool() const { return m_ptr; }
        bool valid() const { return m_ptr; }
    
        T* operator->() const { return m_ptr; }
        inline T& operator*() { return *m_ptr; }

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
};
