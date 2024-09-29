#pragma once 

#include <Engine/Core/base.h>

#include <glm/glm.hpp>

namespace utd
{
    class camera
    {
    public:
        typedef enum class projection_type
        {
            PERSPECTIVE,
            ORTHOGRAPHIC,

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

        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;

        float m_pitch;
        float m_yaw;
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
        void position(const glm::vec3& pos);
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
        glm::vec3    m_position = {0.f, 0.f, -45.f};
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

    class editor_camera
    {
    public:
        editor_camera()
        {
            _update_projection();
            _update_view();
        }

        void position(const glm::vec3& pos) { m_position = pos; }
        void fovy(float fovy) { m_fov = fovy; _update_projection(); }
        void near_clip(float near_clip) { m_near_clip = near_clip;  _update_projection(); }
        void far_clip(float far_clip) { m_far_clip = far_clip; }
        void viewport(float width, float height) { m_viewport_width = width; m_viewport_height = height; _update_projection(); }
        void viewport(const glm::vec2& viewport) { m_viewport_width = viewport.x; m_viewport_height = viewport.y; _update_projection(); }
        glm::quat get_orientation() const;

        const glm::vec2& viewport() const { return glm::vec2{ m_viewport_width, m_viewport_height }; }
        const glm::mat4& projection() const { return m_projection; }
        const glm::mat4& view() const { return m_view; }
        glm::mat4 view_projection() const { return m_projection * m_view; }

        glm::vec3 up() const;
        glm::vec3 front() const;
        glm::vec3 right() const;
    public:
        void on_update(float dt);
        void on_event(event& event);

        bool on_mouse_scroll(mouse_scrolled_event& event);
        bool on_mouse_move(mouse_moved_event& event);
    private:
        void _update_projection();
        void _update_view();
        void _calculate_position();
        void _mouse_pan(const glm::vec2& delta);
    private:
        camera::projection_type m_type;
        
        float m_fov = 45.f;
        float m_aspect_ratio = 16/9.f;
        
        float m_yaw, m_pitch;

        float m_near_clip = 0.1f;
        float m_far_clip = 1000.f;

        glm::vec3 m_position    = { 0.f, 0.f, -2.f };
        glm::vec2 m_inital_mouse_position = { 0.f, 0.f };
        glm::vec3 m_focal_point = { 0.f, 0.f, 0.f };
        float m_focal_length = 10.f;

        glm::mat4 m_projection = glm::mat4(1.f);
        glm::mat4 m_view = glm::mat4(1.f);

        float m_viewport_width = 1280.f;
        float m_viewport_height = 720.f;

    };
}