#include <upch.h>
#include "framebuffer.h"

#include <Engine/Core/Assert.h>

#include <Platform/OpenGL/gl_framebuffer.h>

utd::framebuffer::framebuffer(u32 width, u32 height)
    : m_width{ width }, m_height{ height }
{

}

void utd::framebuffer::resize(u32 width, u32 height)
{
    
    if (width == 0 || height == 0 || width > MAX_WIDTH || height > MAX_HEIGHT)
    {
        UTD_WARN("An attempt to resize framebuffer to {0}, {1}", width, height);
        return;
    }
    
    m_width = width;
    m_height = height;

    this->_attach();
}

utd::i32 utd::framebuffer::read_pixel(u32 attachment_index, i32 x, i32 y)
{
    UTD_ENGINE_ASSERT(attachment_index < m_formats.size());

    return this->_read_pixel(attachment_index, x, y);
}

void utd::framebuffer::erase(u32 attachment_index, int value)
{
    UTD_ENGINE_ASSERT(attachment_index < m_formats.size(), " attachment index is out of the range");
    
    this->_erase_attachment(attachment_index, value);
    
    m_formats.erase(m_formats.begin() + attachment_index, m_formats.begin() + (attachment_index + 1));
}

std::uptr<utd::framebuffer> utd::framebuffer::create()
{
    return std::make_unique<gl_framebuffer>();
}

std::uptr<utd::framebuffer> utd::framebuffer::create(u32 width, u32 height)
{
    return std::make_unique<gl_framebuffer>(width, height);
}
