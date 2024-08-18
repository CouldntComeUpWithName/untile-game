#include <upch.h>
#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Engine/Core/base.h>
#include <Engine/Core/input.h>
#include <Engine/Core/application.h>

#include <Engine/Events/mouse_event.h>
#include <Engine/Profiling/profile.h>

utd::camera::camera(const glm::vec3 &start_position, const glm::mat4 &projection)
    : m_position(start_position), m_projection(projection),
      m_view(1.f), m_view_projection(m_projection * m_view)
{ }

// void utd::camera::on_event(event& event)
// {
//     event_dispatcher::dispatch<mouse_moved_event>(event, UTD_BIND_EVENT(camera::mouse_move));
// }

void utd::camera::on_update(float dt)
{
    //UTD_PROFILE_FUNC();
    //if (application::instance().get_window().cursor_disabled())
    //{
    //    bool camera_moved = false;
    //    auto speed = m_speed * dt;
    //    
    //    camera_moved |= input::is_mouse_moved();
    //    
    //    if (input::is_key_pressed(key::W))
    //    {
    //        m_position += speed * m_front;

    //        camera_moved = true;
    //    }
    //    if (input::is_key_pressed(key::S))
    //    {
    //        m_position -= speed * m_front;

    //        camera_moved = true;
    //    }
    //    if (input::is_key_pressed(key::A))
    //    {
    //        m_position -= m_right * speed;
    //        //m_position -= glm::normalize(glm::cross(m_front, m_up)) * (m_speed * dt);

    //        camera_moved = true;
    //    }
    //    if (input::is_key_pressed(key::D))
    //    {
    //        m_position += m_right * speed;
    //        //m_position += glm::normalize(glm::cross(m_front, m_up)) * m_speed * dt;

    //        camera_moved = true;
    //    }

    //    UTD_PROFILE_SCOPE("LookAt");
    //    
    //    if (camera_moved)
    //        m_view = glm::lookAt(m_position, m_position + m_front, m_up);
    //}

}

bool utd::camera::mouse_move(mouse_moved_event& moved_event)
{
    return false;
}

void utd::camera::_update_projection()
{
    UTD_PROFILE_FUNC();

}

void utd::camera::_update_view()
{
    UTD_PROFILE_FUNC();

}

void utd::multi_camera::perspective(float _fov, float _near, float _far)
{
    m_type             = camera::projection_type::PERSPECTIVE;
    m_fov              = _fov;
    m_perspective_far  = _far;
    m_perspective_near = _near;
    
    _update_perspective();
}

void utd::multi_camera::orthographic(float _size, float _near, float _far)
{
    m_type             = camera::projection_type::ORTHOGRAPHIC;
    m_size             = _size;
    m_perspective_far  = _far;
    m_perspective_near = _near;

    _update_orthographic();
}

float utd::multi_camera::ortho_size()
{
    return m_size;
}

void utd::multi_camera::ortho_size(float size)
{
    m_size = size;

    _update_orthographic();
}

const glm::mat4 &utd::multi_camera::projection() const
{
    return m_projection;
}

void utd::multi_camera::viewport(u32 width, u32 height)
{
    m_aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
   
    _update_projection_specific();
}

void utd::multi_camera::viewport(float aspect_ratio)
{
    m_aspect_ratio = aspect_ratio;

    _update_projection_specific();
}

utd::camera::projection_type utd::multi_camera::type() const
{
    return m_type;
}

void utd::multi_camera::type(camera::projection_type projection_type)
{
    m_type = projection_type;

    _update_projection_specific();
}

void utd::multi_camera::fovy(float fovy)
{
    m_fov = fovy;
    
    _update_perspective();
}

float utd::multi_camera::fovy() const
{
    return m_fov;
}

float utd::multi_camera::perspective_far() const
{
    return m_perspective_far;
}

float utd::multi_camera::perspective_near() const
{
    return m_perspective_near;
}

float utd::multi_camera::orthographic_far() const
{
    return m_orthographic_far;
}

float utd::multi_camera::orthographic_near() const
{
    return m_orthographic_near;
}

void utd::multi_camera::perspective_far(float _far)
{
    m_perspective_far = _far;
    
    _update_perspective();

}

void utd::multi_camera::perspective_near(float _near)
{
    m_perspective_near = _near;
    
    _update_perspective();
}

void utd::multi_camera::orthographic_far(float _far)
{
    m_orthographic_far = _far;

    _update_orthographic();
}

void utd::multi_camera::orthographic_near(float _near)
{
    m_orthographic_near = _near;
    
    _update_orthographic();
}

void utd::multi_camera::position(const glm::vec3& pos)
{
    m_position = pos; 
    _update_projection_specific();
}

inline void utd::multi_camera::_update_perspective()
{
    UTD_PROFILE_FUNC();

    m_projection = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_perspective_near, m_perspective_far);
}

inline void utd::multi_camera::_update_orthographic()
{
    UTD_PROFILE_FUNC();

    float left   = -m_size * m_aspect_ratio;
    float right  = m_size * m_aspect_ratio;
    float bottom = -m_size;
    float top    = m_size;

    m_projection = glm::ortho(left, right, bottom, top, m_orthographic_far, m_orthographic_near);
}

inline void utd::multi_camera::_update_projection_specific()
{
    UTD_PROFILE_FUNC();

    if (m_type == camera::type::PERSPECTIVE) 
        _update_perspective();
    else 
        _update_orthographic();
}
