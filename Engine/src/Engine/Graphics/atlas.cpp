#include <upch.h>
#include "atlas.h"
#include <Engine/Graphics/texture.h>
#include <Engine/Core/Assert.h>

utd::atlas::atlas(int tile_size_w, int tile_size_h, const ref_ptr<texture> original)
    : m_tile_unit_size{tile_size_w, tile_size_h}, m_original_texture{original}
{
}

utd::sub_texture& utd::atlas::add(int origin_x, int origin_y, int width_multipier, int height_multipier)
{
    UTD_ENGINE_ASSERT(true);

    auto normalized_width = m_tile_unit_size.width / static_cast<float>(m_original_texture->width());
    auto normalized_height = m_tile_unit_size.height / static_cast<float>(m_original_texture->height());

    sub_texture::rect bounds {
        .u      = origin_x * normalized_width,
        .v      = origin_y * normalized_height,
        .width = (bounds.u + normalized_width) * width_multipier,
        .height = (bounds.v + normalized_height) * height_multipier
    };
    
    // potentially dangling pointer(return a copy??? handle pattern??? associate sub_texture with its atlas???)
    m_tile_data.push_back(sub_texture(bounds, m_original_texture));
    return m_tile_data.back();
}

size_t utd::atlas::rows() const
{
    return m_original_texture->width() / m_tile_unit_size.width;
}

size_t utd::atlas::columns() const
{
    return m_original_texture->height() / m_tile_unit_size.height;
}
