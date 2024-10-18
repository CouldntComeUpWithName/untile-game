#include <upch.h>
#include "index_buffer.h"

#include <glad/glad.h>

utd::index_buffer::index_buffer(u32* indices_source, u32 count)
    : m_id(0), m_count(count)
{
    _init(indices_source, count);
}


utd::index_buffer::index_buffer(const std::initializer_list<u32>& indices)
    : m_id(0), m_count(static_cast<u32>(indices.size()))
{
    const std::vector<u32> arr = indices;
    
    glGenBuffers(1, &m_id);

    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(u32), arr.data(), GL_STATIC_DRAW);

    //_init(&arr[0], m_count);
}

utd::index_buffer::~index_buffer()
{
    glDeleteBuffers(1, &m_id);
}

void utd::index_buffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void utd::index_buffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void utd::index_buffer::_init(u32* indices_source, u32 count)
{
    glGenBuffers(1, &m_id);

    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(count), indices_source, GL_STATIC_DRAW);
}
