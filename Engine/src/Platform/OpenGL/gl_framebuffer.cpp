#include <upch.h>
#include "gl_framebuffer.h"

#include <glad/glad.h>


namespace detail
{
    static constexpr inline bool is_depth(utd::framebuffer::attachment attachment);

    static constexpr inline GLenum texture_target(bool multisampled);

    static void attach_color(utd::u32 id, int samples, GLenum internal_format, GLenum format, utd::u32 width, utd::u32 height, int index);

    static void attach_depth(utd::u32 id, int samples, GLenum format, GLenum attachment_type, utd::u32 width, utd::u32 height);

    static constexpr inline GLenum native_texture_format(utd::framebuffer::attachment attachment);
    
} /* namespace detail */

utd::gl_framebuffer::gl_framebuffer(u32 width, u32 height)
    : framebuffer(width, height)
{ }

void utd::gl_framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    glViewport(0, 0, m_width, m_height);
}

void utd::gl_framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

utd::gl_framebuffer::~gl_framebuffer()
{
    if (m_id)
        _release();
}

void utd::gl_framebuffer::_attach()
{
    if (m_id) 
        _release();

    glCreateFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    bool multisample = m_samples > 1;

    if (m_formats.size())
    {
        m_attachment_ids.resize(m_formats.size());
        
        glCreateTextures(detail::texture_target(multisample), m_attachment_ids.size(), m_attachment_ids.data());
        
        for (size_t i = 0; i < m_attachment_ids.size(); i++)
        {
            glBindTexture(detail::texture_target(multisample), m_attachment_ids[i]);
            switch (m_formats[i])
            {
            case framebuffer::attachment::RGBA8:
                detail::attach_color(m_attachment_ids[i], m_samples, GL_RGBA8, GL_RGBA, m_width, m_height, i);
                break;
            case framebuffer::attachment::RED_INT:
                detail::attach_color(m_attachment_ids[i], m_samples, GL_R32I, GL_RED_INTEGER, m_width, m_height, i);
                break;
            case framebuffer::attachment::DEPTH:
                detail::attach_depth(m_attachment_ids[i], m_samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_width, m_height);
                break;
            }
        }
    }

    // TODO: review this later
    if (m_attachment_ids.size() > 1)
    {
        UTD_ENGINE_ASSERT(m_attachment_ids.size() <= 4);
        GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(m_attachment_ids.size() - _has_depth(), buffers);
    }
    else if (_has_depth_only())
    {
        glDrawBuffer(GL_NONE);
    }
    UTD_ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
    
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void utd::gl_framebuffer::_resize()
{

}

utd::i32 utd::gl_framebuffer::_read_pixel(u32 attachment_index, i32 x, i32 y)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
    i32 pixel_data = 0;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);
    
    return pixel_data;
}

void utd::gl_framebuffer::_erase_attachment(u32 attachment_index, int value)
{
    //TODO: implement
}

utd::u32 utd::gl_framebuffer::_at(int index) const
{
    UTD_ENGINE_ASSERT(index < m_attachment_ids.size(), "out of the range");
    return m_attachment_ids[index];
}

void utd::gl_framebuffer::_release()
{
    glDeleteFramebuffers(1, &m_id);
    glDeleteTextures(m_attachment_ids.size(), m_attachment_ids.data());
   
    m_attachment_ids.clear();
    m_id = 0;
}

bool utd::gl_framebuffer::_has_depth()
{
    return detail::is_depth(m_formats.back());
}

bool utd::gl_framebuffer::_has_depth_only()
{
    return m_formats.size() == 1 && _has_depth();
}

namespace detail
{
    static constexpr inline bool is_depth(utd::framebuffer::attachment attachment)
    {
        return attachment >= utd::framebuffer::attachment::DEPTH;
    }

    static constexpr inline GLenum texture_target(bool multisampled)
    {
        return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void attach_color(utd::u32 id, int samples, GLenum internal_format, GLenum format, utd::u32 width, utd::u32 height, int index)
    {
        bool multisampled = samples > 1;
        if (multisampled)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, detail::texture_target(multisampled), id, 0);
    }

    static void attach_depth(utd::u32 id, int samples, GLenum format, GLenum attachment_type, utd::u32 width, utd::u32 height)
    {
        bool multisampled = samples > 1;
        if (multisampled)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
        }
        else
        {
            glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, detail::texture_target(multisampled), id, 0);
    }

    static constexpr inline GLenum native_texture_format(utd::framebuffer::attachment attachment)
    {
        switch (attachment)
        {
        case utd::framebuffer::attachment::RGBA8:
            return GL_RGBA;
        case utd::framebuffer::attachment::RED_INT:
            return GL_RED_INTEGER;
        case utd::framebuffer::attachment::DEPTH:
            return GL_DEPTH24_STENCIL8;
        }

        UTD_ENGINE_ASSERT(attachment != utd::framebuffer::attachment::NONE, 
            "Impossible native format conversion (not a texture nor a depth formats)");
        
        return GL_NONE;
    }

} /* namespace detail */
