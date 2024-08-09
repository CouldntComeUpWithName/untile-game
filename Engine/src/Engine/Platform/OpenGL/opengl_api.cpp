#include <upch.h>
#include "opengl_api.h"

#include <glad/glad.h>
#include <Engine/Graphics/vertex_array.h>

void utd::opengl_api::set_viewport(u32 x, u32 y, u32 width, u32 height)
{
    glViewport(x, y, width, height);
}

void utd::opengl_api::set_line_width(float width)
{
    glLineWidth(width);
}

void utd::opengl_api::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void utd::opengl_api::set_clear_color(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void utd::opengl_api::draw_indexed(const vertex_array& vertex_array, u32 count)
{
    vertex_array.bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

void utd::opengl_api::draw_lines(/*TODO: add parameters*/)
{
}

void utd::opengl_api::draw_arrays(const vertex_array& vertex_array, u32 first, u32 count)
{
    vertex_array.bind();
    glDrawArrays(GL_TRIANGLES, first, count);
}

void utd::opengl_api::depth_buffer(bool enabled)
{
    glEnable(GL_DEPTH_TEST);
}
