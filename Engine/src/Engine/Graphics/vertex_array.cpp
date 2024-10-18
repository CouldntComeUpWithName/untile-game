#include <upch.h>
#include "vertex_array.h"

#include <glad/glad.h>

#include <Engine/Core/assert.h>

#include <Engine/Graphics/index_buffer.h>
#include <Engine/Graphics/vertex_buffer.h>
#include <Engine/Graphics/vertex_attrib.h>

static GLenum _cast_shader_base_type(utd::shader::base_type type)
{
    switch (type)
    {
    case utd::shader::base_type::MATRIX:         return GL_FLOAT;
    case utd::shader::base_type::FLOATING_POINT: return GL_FLOAT;
    case utd::shader::base_type::INTEGER:        return GL_INT;
    case utd::shader::base_type::BOOLEAN:        return GL_BOOL;
    }

    return 0;
}

void utd::vertex_array::_set_vertex_attribute_pointer(const utd::vertex_attrib& attribute)
{
    using utd::shader;

    auto type = static_cast<shader::base_type>(((static_cast<int>(attribute.datatype) - attribute.count()) / shader::BASE_TYPE_MAX_SIZE) + 1);
    auto vbi            = m_array.size() - 1;
    auto& vertex_buffer = *m_array[vbi].get();

    switch (type)
    {
    case shader::base_type::BOOLEAN:
    case shader::base_type::INTEGER:
    {
        glEnableVertexAttribArray(m_current_attrib_index);
        glVertexAttribIPointer(m_current_attrib_index, attribute.count(), _cast_shader_base_type(type), vertex_buffer.stride(), (const void*)attribute.offset);
        break;
    }
    case shader::base_type::FLOATING_POINT:
    {
        glEnableVertexAttribArray(m_current_attrib_index);
        glVertexAttribPointer(m_current_attrib_index, attribute.count(), _cast_shader_base_type(type), attribute.normalized, vertex_buffer.stride(), (const void*)attribute.offset);
        break;
    }
    case shader::base_type::MATRIX:
    {
        u8 count = attribute.count();

        for (u8 i = 0; i < count; i++)
        {
            glEnableVertexAttribArray(m_current_attrib_index);
            glVertexAttribPointer(m_current_attrib_index, count, _cast_shader_base_type(type), attribute.normalized, vertex_buffer.stride(), (const void*)(attribute.offset + sizeof(float) * count * i));
            glVertexAttribDivisor(m_current_attrib_index, 1);
        }
        break;
    }
    default: 
        UTD_ENGINE_ASSERT(shader::base_type::UNKNOWN, "UNKOWN datatype subset!");
    }
    m_current_attrib_index++;
}

utd::vertex_array::vertex_array()
{
    glGenVertexArrays(1, &m_id);
}

utd::vertex_array::~vertex_array()
{
    glDeleteVertexArrays(1, &m_id);
}

void utd::vertex_array::bind() const 
{
    glBindVertexArray(m_id);
}

void utd::vertex_array::unbind() const
{
    glBindVertexArray(0);
}

void utd::vertex_array::set_index_buffer(std::uptr<index_buffer>&& index_buffer)
{
    bind();
    index_buffer->bind();
    m_index_buffer = std::forward<decltype(index_buffer)>(index_buffer);
}

void utd::vertex_array::push_back(std::uptr<vertex_buffer>&& vbo)
{
    UTD_ENGINE_ASSERT(!vbo->empty(), "Vertex Buffer does NOT have layout!");
    auto& vbo_ref = *vbo.get();

    this->bind();
    vbo_ref.bind();

    m_array.push_back(std::forward<decltype(vbo)>(vbo));
    
    for (const auto& attribute : vbo_ref)
    {
        _set_vertex_attribute_pointer(attribute);
    }
}

utd::vertex_buffer& utd::vertex_array::operator[](int index)
{
    return *m_array[index].get();
}
