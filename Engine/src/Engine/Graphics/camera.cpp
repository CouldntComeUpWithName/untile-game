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
    m_view(glm::lookAt(m_position, m_position + m_front, m_up)),
    m_speed(5.f), m_pitch(90.f), m_yaw(-90.f)
{ 
    m_current_mouse_x = 0;
    m_current_mouse_y = 0;
    //
    _update_vectors();
}

void utd::camera::on_event(event& event)
{
    event_dispatcher::dispatch<mouse_moved_event>(event, UTD_BIND_EVENT(camera::mouse_move));
}

void utd::camera::on_update(float dt)
{
    UTD_PROFILE_FUNC();
    //if (application::instance().get_window().cursor_disabled())
    {
        bool camera_moved = false;
        auto speed = m_speed * dt;
        
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
    UTD_PROFILE_FUNC();

    auto xpos = moved_event.get_x();
    auto ypos = moved_event.get_y();
    static bool f = true;

    if (f)
    {
        m_current_mouse_x = xpos;
        m_current_mouse_y = ypos;
        f = false;
    }

    auto xoffset = xpos - m_current_mouse_x;
    auto yoffset = m_current_mouse_y - ypos;

    m_current_mouse_x = xpos;
    m_current_mouse_y = ypos;

    m_yaw += xoffset * 0.1f;
    m_pitch += yoffset * 0.1f;

    if (m_pitch > 89.9f)
        m_pitch = 89.9f;
    else if (m_pitch < -89.9f)
        m_pitch = -89.9f;

    _update_vectors();

    return true;
}

void utd::camera::_update_vectors()
{
    UTD_PROFILE_FUNC();

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_up));

}
