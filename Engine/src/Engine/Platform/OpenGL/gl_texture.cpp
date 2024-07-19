#include <upch.h>
#include "gl_texture.h"

#include <stb_image.h>
#include <glad/glad.h>

#include <Engine/Core/Assert.h>
static inline utd::image_format enum_cast(int channels);

utd::gl_texture::gl_texture(const specs& specifications)
    : m_specifications(specifications), m_loaded(false)
{

}

utd::gl_texture::gl_texture(const std::string& path)
{
    m_path = std::make_unique<std::string>(path);

    i32 channels;
    byte* data = stbi_load(m_path.get()->c_str(), &m_specifications.width, &m_specifications.height, &channels, 0);
    
    UTD_ENGINE_ASSERT(data, "Can\'t load a texture from file");
    m_loaded = true;

    glGenTextures(1, &m_id);
    //glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    if(channels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specifications.width, m_specifications.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        m_specifications.format = utd::image_format::RGBA8;
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_specifications.height, m_specifications.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        m_specifications.format = utd::image_format::RGB8;
    }
   
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
}

const utd::texture::specs& utd::gl_texture::get_specs()
{
    return m_specifications;
}

utd::u32 utd::gl_texture::width() const
{
    return m_specifications.width;
}

utd::u32 utd::gl_texture::height() const
{
    return m_specifications.height;
}

glm::vec2 utd::gl_texture::size() const
{
    return glm::vec2(width(), height());
}

const std::string& utd::gl_texture::path() const
{
    return *m_path.get();
}

void utd::gl_texture::set_data(u32 slot)
{

}

bool utd::gl_texture::is_loaded() const
{
    return m_loaded;
}

bool utd::gl_texture::operator==(const texture& other) const
{
    return false;
}


static inline utd::image_format enum_cast(int channels)
{
    switch (channels)
    {
    case 3:
        return utd::image_format::RGB8;
    case 4:
        return utd::image_format::RGBA8;
    default:
        return utd::image_format::NONE;
    }
}


