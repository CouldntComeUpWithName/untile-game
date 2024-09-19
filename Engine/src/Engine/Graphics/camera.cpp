#include <upch.h>
#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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
    UTD_PROFILE_FUNC();
    if (application::instance().get_window().cursor_disabled())
    {
        bool camera_moved = false;
        auto speed = 5.f * dt;
        
        camera_moved |= input::is_mouse_moved();
        
        if (input::is_key_pressed(key::W))
        {
            m_position += speed * m_front;

            camera_moved = true;
        }
        if (input::is_key_pressed(key::S))
        {
            m_position -= speed * m_front;

            camera_moved = true;
        }
        if (input::is_key_pressed(key::A))
        {
            m_position -= m_right * speed;
            //m_position -= glm::normalize(glm::cross(m_front, m_up)) * (m_speed * dt);

            camera_moved = true;
        }
        if (input::is_key_pressed(key::D))
        {
            m_position += m_right * speed;
            //m_position += glm::normalize(glm::cross(m_front, m_up)) * m_speed * dt;

            camera_moved = true;
        }

        UTD_PROFILE_SCOPE("LookAt");
        
        if (camera_moved)
            m_view = glm::lookAt(m_position, m_position + m_front, m_up);
    }

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

glm::quat utd::editor_camera::get_orientation() const
{
    return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.f));
}

glm::vec3 utd::editor_camera::up() const
{
    return glm::rotate(get_orientation(), glm::vec3(0.f, 1.f, 0.f));
}

glm::vec3 utd::editor_camera::front() const
{
    return glm::rotate(get_orientation(), glm::vec3(0.f, 0.f, -1.f));
}

glm::vec3 utd::editor_camera::right() const
{
    return glm::rotate(get_orientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

void utd::editor_camera::on_update(float dt)
{
    float speed = 5.f * dt;
    auto mouse = input::get_mouse_pos();
    auto delta = (mouse - m_inital_mouse_position) * 0.005f;
    m_inital_mouse_position = mouse;

    if (input::is_mouse_button_pressed(mouse::LEFT_BUTTON))
    {
       _mouse_pan(delta);
    }
    auto forward = front();
    if (input::is_key_pressed(key::W))
    {
        m_position += speed * front();

    }
    if (input::is_key_pressed(key::S))
    {
        m_position -= speed * front();
    }
    if (input::is_key_pressed(key::A))
    {
        m_position -= right() * speed;
    }
    if (input::is_key_pressed(key::D))
    {
        m_position += right() * speed;
    }
    _update_view();

    UTD_PROFILE_SCOPE("LookAt");
}

void utd::editor_camera::on_event(event& event)
{
    event_dispatcher::dispatch<mouse_scrolled_event>(event, UTD_BIND_EVENT(editor_camera::on_mouse_scroll));
    //event_dispatcher::dispatch<mouse_moved_event>(event, UTD_BIND_EVENT(editor_camera::on_mouse_move));
}

bool utd::editor_camera::on_mouse_scroll(mouse_scrolled_event& event)
{
    float delta = event.offset_y() * 0.1f;

    m_focal_length -= delta * 5.f;

    if (m_focal_length < 1.f)
    {
        m_focal_point += front();
        m_focal_length = 1.f;
    }
    _update_view();

    return false;
}

bool utd::editor_camera::on_mouse_move(mouse_moved_event& event)
{
    static float current_mouse_x = 0.f;
    static float current_mouse_y = 0.f;

    auto xpos = event.get_x();
    auto ypos = event.get_y();
    
    static bool f = true;

    if (f)
    {
        current_mouse_x = xpos;
        current_mouse_y = ypos;
        f = false;
    }

    auto xoffset = xpos - current_mouse_x;
    auto yoffset = current_mouse_y - ypos;

    current_mouse_x = xpos;
    current_mouse_y = ypos;

    m_yaw += xoffset * 0.1f;
    m_pitch += yoffset * 0.1f;

    return false;
}

void utd::editor_camera::_update_projection()
{
    m_aspect_ratio = m_viewport_width / m_viewport_height;
    m_projection = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_clip, m_far_clip);
}

void utd::editor_camera::_update_view()
{
    _calculate_position();
    auto orientation = get_orientation();
    //m_view = glm::lookAt(m_position, m_position + front(), up());

    m_view = glm::translate(glm::mat4(1.f), m_position) * glm::toMat4(orientation);
    m_view = glm::inverse(m_view);
}

void utd::editor_camera::_calculate_position()
{
    m_position = m_focal_point - front() * m_focal_length;
}

void utd::editor_camera::_mouse_pan(const glm::vec2& delta)
{
    float x = std::min(m_viewport_width / 1000.0f, 2.4f); // max = 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = std::min(m_viewport_width / 1000.0f, 2.4f); // max = 2.4f
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    m_focal_point += -right() * delta.x * xFactor * m_focal_length;
    m_focal_point += up() * delta.y * yFactor * m_focal_length;
}
