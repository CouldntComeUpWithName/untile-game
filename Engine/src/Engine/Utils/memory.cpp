#include <upch.h>
#include "memory.h"

#include <Engine/Graphics/texture.h>

utd::bytes_t utd::vmemsize_of(const utd::texture& texture)
{
    auto size = texture.size();
    // return size.x * size.y * specs.channels;
    return 0;
}