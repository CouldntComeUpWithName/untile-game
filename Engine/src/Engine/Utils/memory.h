#pragma once

namespace utd
{
    template<typename T>
    class ref_ptr
    {
        static_assert(std::is_default_constructible_v<T>, "T must have a default constructor");
    public:

        ref_ptr() = default;
        ref_ptr(const ref_ptr<T>&) = default;
        ref_ptr(std::unique_ptr<T>& uptr) : m_ptr(uptr.get()) {}
        ref_ptr(T& _ref)
        : m_ptr(&_ref)
        { }
        ref_ptr(ref_ptr<T>&&) = default;
        
        ref_ptr<T>& operator = (const ref_ptr<T>&) = default;
        ref_ptr<T>& operator = (ref_ptr<T>&&)      = default;
        ref_ptr<T>& operator = (std::unique_ptr<T>& _uref)
        {
            m_ptr = _uref.get();
            return *this;
        }
        ref_ptr<T>& operator = (T& ref)
        {
            m_ptr = &ref;
            return *this;
        }
        
        operator bool() { return m_ptr; }

        T* operator->() const { return m_ptr; }
        inline T& operator*(){ return *m_ptr; }

        inline T* get() const { return m_ptr; }
    public:
        T* m_ptr;
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
