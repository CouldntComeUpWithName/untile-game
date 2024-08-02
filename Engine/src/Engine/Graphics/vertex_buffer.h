#pragma once
#include <vector>

#include <Engine/Core/Base.h>

namespace utd
{
    struct vertex_attrib;
    
    class vertex_buffer
    {
    public:
        using iterator       = std::vector<vertex_attrib>::iterator;
        using const_iterator = std::vector<vertex_attrib>::const_iterator;
    public:
        vertex_buffer() = default;
        vertex_buffer(u32 size);
        vertex_buffer(float* vertices, u32 size);

        ~vertex_buffer();
        
        void bind() const;
        void unbind() const;
        
        void set_data(const void* data, u32 size);
        void set_layout(const std::initializer_list<vertex_attrib>& layout_list);

        inline u32 stride() const noexcept { return m_stride; }
        
        vertex_attrib& operator[](int attribute) { return m_attributes[attribute]; }
        const vertex_attrib& operator[](int attribute) const { return m_attributes[attribute]; }

        iterator begin() { return m_attributes.begin(); }
        iterator end() { return m_attributes.end(); }
        
        const_iterator begin() const { return m_attributes.begin(); }
        const_iterator end() const { return m_attributes.end(); }
    public:
        inline size_t size() const noexcept { return m_attributes.size(); }
        
        inline size_t capacity() const noexcept { return m_attributes.capacity(); }
        
        inline vertex_attrib* data() { return m_attributes.data(); }
        
        inline bool empty() { return m_attributes.empty(); }
        
        inline void clear() { m_attributes.clear(); }
        
    public:
        template<typename t_vertex>
        static std::uptr<vertex_buffer> create(u32 size);

    private:
        void _calculate_offset_and_stride();

    private:
        std::vector<vertex_attrib> m_attributes;
        u32 m_stride = 0;
        u32 m_id;
    };

    template<typename t_vertex>
    inline std::uptr<vertex_buffer> vertex_buffer::create(u32 size)
    {
        return std::make_unique<vertex_buffer>(size * sizeof(t_vertex));
    }

}