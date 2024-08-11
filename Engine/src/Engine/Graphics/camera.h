#pragma once 

#include <Engine/Core/base.h>

#include <glm/glm.hpp>

namespace utd
{
    class event;
    class mouse_moved_event;
    
    class camera
    {
    public:
        typedef enum class projection_type
        {
            PERSPECTIVE,
            ORTHOGRAPHIC,
            FRUSTUM

        } type;

    public:
        camera(const glm::vec3& start_position = glm::vec3(0.f), const glm::mat4& projection = glm::mat4(1.f));
    
    public:
        void position(const glm::vec3& pos) { m_position = pos; }
        const glm::vec3& position() { return m_position; }

        void projection(const glm::mat4& projection) { m_projection = projection; }
        const glm::mat4& projection() const { return m_projection; }

        const glm::mat4& view() const { return m_view; }
        void view(const glm::mat4& updated_view) { m_view = updated_view; }

        const glm::mat4& view_projection() const {return m_view_projection; }
        void view_projection(glm::mat4& vp) { m_view_projection = vp; }

    private:
        void _update_projection();
        void       _update_view();

        
        void on_event(event&);
        void on_update(float dt);
        
        bool mouse_move(mouse_moved_event& moved_event);

    private:
        float            m_rotation;
        glm::vec3        m_position;
        
        glm::mat4      m_projection;
        glm::mat4            m_view;
        glm::mat4 m_view_projection;
 
    };

    class multi_camera
    {
    public:
        multi_camera(const glm::vec3& position = glm::vec3(0.f), const glm::mat4& projection = glm::mat4(1.f))
            : m_type(camera::type::PERSPECTIVE), m_position(position), m_projection(projection)
        {          
        }

        ~multi_camera() = default;

    public:
        // temp 
        void position(const glm::vec3& pos) { m_position = pos; _update_projection_specific(); }
        const auto& position() const { return m_position; };
        
        void perspective(float _fov, float _near, float _far);
        void orthographic(float size, float _near, float _far);

        float ortho_size();
        void ortho_size(float size);

        const glm::mat4& projection() const;

        void viewport(u32 width, u32 height);
        void viewport(float aspect_ratio);
        
        camera::projection_type type() const;
        void type(camera::projection_type projection_type);

        void  fovy(float);
        float fovy() const;

        float  perspective_far() const;
        float perspective_near() const;

        float  orthographic_far() const;
        float orthographic_near() const;

        void  perspective_far(float);
        void perspective_near(float);

        void  orthographic_far(float);
        void orthographic_near(float);

    private:
        inline void _update_perspective();
        inline void _update_orthographic();
        inline void _update_projection_specific();
    private:
        camera::type m_type;
        glm::vec3    m_position;
        glm::mat4    m_projection = glm::mat4(1.f);

        // TODO: think about making unions of data for Shipping config
        float m_rotation = 0.f;
        float m_aspect_ratio = 16/9.f;
        float m_fov = glm::radians(45.f);
        float m_perspective_far = 0.01f; 
        float m_perspective_near = 100.f;
        
        float m_size = 1.f;
        float m_orthographic_far = 0.1f;
        float m_orthographic_near = 100.f;

    };
}