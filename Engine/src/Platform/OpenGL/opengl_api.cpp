#include <upch.h>
#include "opengl_api.h"

#include <glad/glad.h>
#include <Engine/Graphics/vertex_array.h>

static void opengl_message_callback(utd::u32 source, utd::u32 type, utd::u32 id, utd::u32 severity, utd::i32 length, utd::cstring message, const void* user_program)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         UTD_ENGINE_FATAL(message); return;
    case GL_DEBUG_SEVERITY_MEDIUM:       UTD_ENGINE_ERROR(message); return;
    case GL_DEBUG_SEVERITY_LOW:          UTD_ENGINE_WARN(message); return;
    case GL_DEBUG_SEVERITY_NOTIFICATION: UTD_ENGINE_INFO(message); return;
    }

    UTD_ENGINE_ASSERT(false, "Unknown severity level!");
}

void utd::opengl_api::init()
{
#ifdef UTD_CONFIG_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(opengl_message_callback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
}

void utd::opengl_api::viewport(u32 x, u32 y, u32 width, u32 height)
{
    glViewport(x, y, width, height);
}

void utd::opengl_api::line_width(float width)
{
    glLineWidth(width);
}

void utd::opengl_api::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void utd::opengl_api::clear_color(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void utd::opengl_api::draw_indexed(const vertex_array& vertex_array, u32 count)
{
    vertex_array.bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

void utd::opengl_api::draw_line(const vertex_array& vertex_array, u32 count)
{
}

void utd::opengl_api::draw_arrays(const vertex_array& vertex_array, u32 first, u32 count)
{
    vertex_array.bind();
    glDrawArrays(GL_TRIANGLES, first, count);
}

void utd::opengl_api::depth_buffer(bool enabled)
{
    if (enabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}
