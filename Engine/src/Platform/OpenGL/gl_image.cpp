#include <upch.h>
#include "gl_image.h"

#include <stb_image.h>
#include <Engine/Profiling/profile.h>
#include <Platform/Common/iosystem.h>

utd::gl_image::gl_image(const std::filesystem::path& path)
{
    m_path = std::make_unique<std::string>(path.string());
    _filepath(path);
}

void utd::gl_image::filepath(const std::filesystem::path& path)
{
    m_path = std::make_unique<std::string>(path.string());
    _filepath(path);
}

void utd::gl_image::source(const buffer &)
{
    
}

void utd::gl_image::reload()
{
    buffer data = io::read_file(*m_path.get(), std::ios::in | std::ios::binary);
    
    //stbi__context s;
    //s.io = {};
    //s.buflen = data.size();
    //s.img_buffer = m_data.data();
    //s.img_buffer_end = m_data.data() + m_data.size();
    //s.img_buffer_original = data.data();
    //s.img_buffer_original_end = data.data() + data.size();
    //stbi__result_info ri;

}

void utd::gl_image::_filepath(const std::filesystem::path& path)
{
    stbi_set_flip_vertically_on_load(true);

    i32 width, height, channels;
    byte* data;
    UTD_PROFILE_BEGIN("stbi_load");
    data = stbi_load(m_path.get()->c_str(), &width, &height, &channels, 0);
    UTD_PROFILE_END();
    m_specs.format = image::retrive_format(channels);
    m_specs.width  = width;
    m_specs.height = height;

    std::function<void(void*)> image_free = stbi_image_free;
    m_data = utd::buffer((byte*)data, width * height * channels, image_free);
}
