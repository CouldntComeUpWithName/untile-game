#pragma once
#include <Engine/Core/Base.h>

namespace utd
{
    class uniform_buffer : noncopyable
    {
        using id = u32;
    public:
        uniform_buffer(u32 size, u32 binding);
        uniform_buffer(uniform_buffer&& ub) = default;
        ~uniform_buffer();

        uniform_buffer& operator = (uniform_buffer&& ub) = default;
        
        void set_data(const void* data, u32 size, u32 offset = 0u);
    private:
        id m_id;
    };
} /* namespace utd */