#include "Engine/Core/graphics_context.h"

namespace utd
{
    class opengl_context : public graphics_context
    {
    public:
        void init(void*) override;
        void swap_buffers(void*) override;
    };
}