#include "upch.h"
#include "opengl_context.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Engine/Core/log.h"
#include "Engine/Core/Assert.h"

void utd::opengl_context::init(void* window_handle)
{
    glfwMakeContextCurrent((GLFWwindow*)window_handle);
    
    auto glad_loader_status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    
    UTD_ENGINE_ASSERT(glad_loader_status, "GLAD\'s fucked up");
}

void utd::opengl_context::swap_buffers(void* window_handle)
{
    glfwSwapBuffers((GLFWwindow*)window_handle);
}
