#include <upch.h>
#include "uniform_buffer.h"
#include <glad/glad.h>

utd::uniform_buffer::uniform_buffer(u32 size, u32 binding)
{
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_id);
}

utd::uniform_buffer::~uniform_buffer()
{
    glDeleteBuffers(1, &m_id);
}

void utd::uniform_buffer::set_data(const void *data, u32 size, u32 offset)
{
    glNamedBufferSubData(m_id, offset, size, data);
}
