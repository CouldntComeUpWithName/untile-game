#pragma once

#include<Engine/Core/Base.h>

namespace utd
{
    class index_buffer : noncopyable
    {
    public:
        index_buffer() = default;
        index_buffer(u32* indices_source, u32 count);
        
        template<typename std::size_t SIZE>
        index_buffer(u32 (&indices)[SIZE])
            : m_id(0), m_count(SIZE)
        {
            _init(indices, SIZE);
        }

        index_buffer(index_buffer&& other) noexcept
        {
            m_id = other.m_id;
            other.m_id = 0;

            m_count = other.m_count;
            other.m_count = 0;
        }

        index_buffer& operator=(index_buffer&& other) noexcept
        {
            if (this == &other)
                return *this;

            m_id = other.m_id;
            other.m_id = 0;

            m_count = other.m_count;
            other.m_count = 0;
            
            return *this;
        }

        index_buffer(const std::initializer_list<u32>& indices);

        ~index_buffer();
        
        void bind() const;
        void unbind() const;
        
        inline u32 count() const { return m_count; };
    private:
        void _init(u32* indices_source, u32 count);
    private:
        u32 m_id;
        u32 m_count;
    };

} /* namespace utd */