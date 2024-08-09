#pragma once 

#include <glm/glm.hpp>

namespace utd
{
    class event;
    class mouse_moved_event;
    
    class camera
    {
    public:
        enum class type
        {
            PERSPECTIVE,
            ORTHOGRAPHIC,
            FRUSTUM
        };

    public:
        camera(const glm::vec3& start_position = glm::vec3(0.f), const glm::mat4& projection = glm::mat4(1.f));
    
    public:
        float rotation() const { return m_rotation; }
        void rotation(float rotation) { m_rotation = rotation; }
        
        void position(const glm::vec3& pos) { m_position = pos; }
        const glm::vec3& position() { return m_position; }

        void projection(const glm::mat4& projection) { m_projection = projection; }
        const glm::mat4& projection() const { return m_projection; }

        const glm::mat4& view() const { return m_view; }
        const glm::mat4& view_projection() const {return m_view_projection; }
        
        void on_event(event&);
        void on_update(float dt);
        
    private:
        void _update_projection();

    public:
        
        type m_type;
        glm::vec3 m_position;
        glm::mat4 m_projection = glm::mat4(1.f);
        
        glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_up    = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 m_right;

        glm::mat4            m_view;
        glm::mat4 m_view_projection;
        
        float m_speed = 5.f;
        float m_rotation = 0.f;
        float m_fov = 45.f;

        float m_current_mouse_x;
        float m_current_mouse_y;
        
        float m_yaw = 0.f;
        float m_pitch = -90.0f;

        float m_camera_sensitivity = 1.f;
    };
}