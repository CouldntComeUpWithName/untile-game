#pragma once
#include "base.h"

namespace utd
{
    class graphics_context
    {
    public:
        virtual void init(void*)         = 0;
        virtual void swap_buffers(void*) = 0;

        static std::uptr<graphics_context> create();
    };
}/* namespace utd */
