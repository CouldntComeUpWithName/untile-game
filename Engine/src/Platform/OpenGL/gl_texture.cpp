#include <upch.h>
#include "gl_texture.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <Engine/Core/Assert.h>
#include <Engine/Graphics/image.h>

utd::gl_texture::gl_texture(const specs& specifications)
    : m_specifications(specifications), m_loaded(false)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureStorage2D(m_id, 1, GL_RGBA8, m_specifications.width, m_specifications.height); // hard coded

    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

utd::gl_texture::gl_texture(const std::string& path)
{
    auto image = utd::image::load(path);

    UTD_ENGINE_ASSERT(image, "Can\'t load a texture from file");
    
    m_specifications.width  = image->width();
    m_specifications.height = image->height();
    m_loaded = true;
    //glGenTextures(1, &m_id);
    
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    switch (image->get_format())
    {
    case image::format::RGBA8:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specifications.width, m_specifications.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->raw_data());
        break;
    case image::format::RGB8:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_specifications.height, m_specifications.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->raw_data());
        break;
    default:
        UTD_ENGINE_ASSERT(false, "image format is unsupported!");
    }

    m_path = std::make_unique<std::string>(image->path());

    glGenerateMipmap(GL_TEXTURE_2D);
}

utd::gl_texture::~gl_texture()
{
    glDeleteTextures(1, &m_id);
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

void utd::gl_texture::set_data(void* data, u32 size)
{
    glTextureSubImage2D(m_id, 0, 0, 0, m_specifications.width, m_specifications.height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void utd::gl_texture::set_data(const image& image)
{
    auto format = image.get_format() == utd::image::format::RGBA8 ? GL_RGBA : GL_RGB;
    glTextureSubImage2D(m_id, 0, 0, 0, image.width(), image.height(), format, GL_UNSIGNED_BYTE, image.raw_data());
}

bool utd::gl_texture::is_loaded() const
{
    return m_loaded;
}

bool utd::gl_texture::operator==(const texture& other) const
{
    return m_id == other.get_id();
}

void utd::gl_texture::bind(u32 slot)
{
    glBindTextureUnit(slot, m_id);
}

