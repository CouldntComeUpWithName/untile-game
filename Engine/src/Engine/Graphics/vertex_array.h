#pragma once

#include <Engine/Core/Base.h>
#include <Engine/Core/assert.h>
#include <Engine/Graphics/index_buffer.h>
#include <Engine/Graphics/vertex_buffer.h>
#include <Engine/Graphics/vertex_attrib.h>

namespace utd
{
    class vertex_buffer; 
    class index_buffer;
    struct vertex_attrib;
    
    class vertex_array
    {
    public:
        enum primitive_type
        {
            POINTS,
            LINES,
            LINE_STRIP,
            TRIANGLES,
            TRIANGLE_STRIP,
            TRIANGLE_FAN,
            QUADS,
        };
    public:
        vertex_array();
        vertex_array(primitive_type primitive);
        ~vertex_array();

        void bind() const;
        void unbind() const;

        void set_index_buffer(std::uptr<index_buffer>&&);
        
        template<typename... t_ctor_params>
        void make_index_buffer(t_ctor_params&&... params);
        
        void push_back(std::uptr<vertex_buffer>&& vbo);
        
        template<typename... t_ctor_params>
        vertex_buffer& emplace_back(t_ctor_params&&... params);

        vertex_buffer& front() { return *m_array.front().get(); }
        vertex_buffer& back() { return *m_array.back().get(); } 
        
        vertex_buffer& front() const { return *m_array.front().get(); }
        vertex_buffer& back() const { return *m_array.back().get(); }
        
        size_t size() const { return m_array.size(); }

        vertex_buffer& operator[](int);
    private:
        void _set_vertex_attribute_pointer(const vertex_attrib& attribute);
    private:
        std::vector<std::uptr<vertex_buffer>> m_array;
        std::uptr<index_buffer> m_index_buffer;
        u32 m_current_attrib_index = 0;
        u32 m_id = 0;
    };

    template<typename ...t_ctor_params>
    inline void vertex_array::make_index_buffer(t_ctor_params&& ...params)
    {
        bind();
        m_index_buffer = std::make_unique<index_buffer>(std::forward<t_ctor_params>(params)...);
        m_index_buffer->bind();
    }

    //isn't working atm
    template<typename ...t_ctor_params>
    inline vertex_buffer& vertex_array::emplace_back(t_ctor_params&&... params)
    {
        auto vbo = std::make_unique<vertex_buffer>(std::forward<t_ctor_params>(params)...);
        
        auto& vbo_ref = *vbo.get();

        m_array.emplace_back(std::move(vbo));
        
        this->bind();
        vbo_ref.bind();

        UTD_ENGINE_ASSERT(!vbo_ref.empty());
        for (const auto& attribute : vbo_ref)
        {
            _set_vertex_attribute_pointer(attribute);
        }
        
        return vbo_ref;
    }

} /* namespace utd */