#pragma once
#include<Engine/Core/Base.h>

namespace utd
{
    class compute_shader
    {
    public:
        void load(const std::filesystem::path& file);
        void bind() const;
        void dispatch(u32 x, u32 y, u32 z);
    private:
        u32 m_id;
    };
}