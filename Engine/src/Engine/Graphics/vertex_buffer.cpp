#include <upch.h>
#include "vertex_buffer.h"

#include <glad/glad.h>

#include <Engine/Graphics/vertex_attrib.h>
#include <Engine/Profiling/profile.h>

utd::vertex_buffer::vertex_buffer(u32 size)
    : m_id(0)
{
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

utd::vertex_buffer::vertex_buffer(float* vertices, u32 size)
    : m_id(0)
{
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

utd::vertex_buffer::~vertex_buffer()
{
    glDeleteBuffers(1, &m_id);
}

void utd::vertex_buffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void utd::vertex_buffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void utd::vertex_buffer::set_data(const void* data, u32 size)
{
    UTD_PROFILE_FUNC();

    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void utd::vertex_buffer::set_layout(const std::initializer_list<vertex_attrib>& layout_list)
{
    m_attributes = layout_list;
    _calculate_offset_and_stride();
}

void utd::vertex_buffer::_calculate_offset_and_stride()
{
    u16 current_offset = 0;

    m_stride = 0;

    for (auto& attribute : m_attributes)
    {
        attribute.offset = current_offset;
        current_offset  += attribute.size;
        m_stride += attribute.size;
    }
}
