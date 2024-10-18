#pragma once

#include <Engine/Core/Base.h>
#include <functional>
namespace utd
{

    class buffer
    {
    public:
        // Manages proper deallocation of buffer
        // if allocation was done by means other than standard new[] operator.
        // copy/resize operations provoke deleter to become default
        // and further allocations/deallocations are accomplished with new[]/delete[] respectively.
        class deleter 
        {
        public:
            deleter(const std::function<void(void*)>& del)
            : m_deleter{del}
            { }

            void operator()(void* block) const
            {
                m_deleter(block);
            }
        private:
            std::function<void(void*)> m_deleter;
        };
        static const inline deleter default_deleter = deleter([](void* block) { delete[] block; });
    public:
        buffer() = default;
        buffer(byte* data, size_t size, const deleter& deleter = buffer::default_deleter);
        buffer(size_t size);
        buffer(const buffer& other);
        buffer(buffer&& other) noexcept;
        
        ~buffer();

        buffer& operator = (const buffer& other);
        buffer& operator = (buffer&& other) noexcept;

        template<typename T>
        T* as() { return (T*)(m_data); }

        template<typename T>
        explicit operator T*() { return static_cast<T*>(m_data); }
        
        void clear();
        void resize(size_t); 
        
        inline size_t size() const { return m_size; }

        bool empty() const { return m_size == 0; }
        byte* data() { return m_data; }
        const byte* data() const { return m_data; }
    private:
        static void _copy(const buffer& from, buffer& to);
        void _allocate();
    private:
        byte* m_data = nullptr;
        size_t m_size = 0ULL;
        deleter m_deleter = default_deleter;
    };

}