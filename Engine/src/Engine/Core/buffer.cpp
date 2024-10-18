#include "buffer.h"

utd::buffer::buffer(byte *data, size_t size, const deleter& deleter)
    : m_data(data), m_size(size), m_deleter{deleter}
{ }

utd::buffer::buffer(size_t size)
    : m_data(new byte[size]), m_size(size)
{ }

utd::buffer::buffer(const buffer &other)
    : buffer(other.m_size)
{
    memcpy(m_data, other.m_data, other.m_size);
}

utd::buffer::buffer(buffer&& other) noexcept
    : m_data(other.m_data), m_size(other.m_size), m_deleter(other.m_deleter)
{
    other.m_data = nullptr;
    other.m_size = 0;
}

utd::buffer::~buffer()
{
    clear();
}

utd::buffer &utd::buffer::operator=(const buffer &other)
{
    if(this != &other)
    {
        clear();

        m_size = other.m_size;

        _allocate();
        m_deleter = buffer::default_deleter;

        memcpy(m_data, other.m_data, other.m_size);
    }

    return *this;
}

utd::buffer &utd::buffer::operator=(buffer &&other) noexcept
{
    if(this != &other)
    {
        m_data = other.m_data;
        m_size = other.m_size;
        m_deleter = other.m_deleter;

        other.m_data = nullptr;
        m_size = 0;
    }
    return *this;
}

void utd::buffer::clear()
{
    if (m_data)
    {
        m_deleter(m_data);
        m_size = 0;
    }
}

void utd::buffer::resize(size_t size)
{
    if(!empty())
    {
        buffer temp(size);
        _copy(temp, *this);
        
        clear();
        *this = std::move(temp);
    }
    else
    {
        m_size = size;
        _allocate();
    }
}

void utd::buffer::_copy(const buffer &from, buffer &to)
{
    memcpy(to.m_data, from.m_data, from.m_size);
}

void utd::buffer::_allocate()
{
    m_data = new byte[m_size];
}
