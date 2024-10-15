#include <upch.h>
#include "memory.h"

#include <Engine/Graphics/texture.h>
#include <Engine/Core/log.h>

void* operator new(std::size_t count)
{
    static size_t alloc_count = 0;
    auto* mem = malloc(count);
    
    std::cout << "Allocation " << count << std::endl;

    //UTD_INFO("MEMORY ALLOCATION DETECTED:");

    return mem;
}

void operator delete(void* block)
{
    static size_t dealloc_count = 0;
    //UTD_WARN("MEMORY DEALLOCATION:");
    free(block);
}

utd::bytes_t utd::vmemsize_of(const utd::texture& texture)
{
    auto size = texture.size();
    // return size.x * size.y * specs.channels;
    return 0;
}