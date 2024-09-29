#pragma once
#include <Engine/Core/Base.h>
#include <Engine/Utils/memory.h>

namespace utd
{
    class sub_texture
    {
        friend class atlas;
    
    public:
        //NOTE: come up with a better name
        struct rect
        {
           float u, v, width, height;
        };
        
        inline const ref_ptr<texture> get_texture() const { return m_texture; }
        inline rect get_rect() const { return m_rect; }
        
        sub_texture() = default;
    private:
        sub_texture(const rect& bounds, const ref_ptr<texture> texture)
            : m_rect{bounds}, m_texture(texture)
        { }

    private:
        rect m_rect;
        ref_ptr<texture> m_texture;
    };

    class atlas
    {
    public:
        using atlas_storage = std::vector<sub_texture>;
    
    public:
        atlas() = default;
        atlas(int tile_size_w, int tile_size_h, const ref_ptr<texture> original = nullptr);

        sub_texture& add(int origin_x, int origin_y, int width_multipier = 1, int height_multipier = 1);
        
        inline size_t count() { return m_tile_data.size(); };
        
        size_t rows() const;
        size_t columns() const;

        inline sub_texture& operator[](int index) {return m_tile_data[index]; }
        inline const sub_texture& operator[](int index) const {return m_tile_data[index]; }
    public:

    private:
        struct
        {
            int width  = 0;
            int height = 0;
        }m_tile_unit_size;
        
        ref_ptr<texture> m_original_texture;
        atlas_storage m_tile_data;
    };
}