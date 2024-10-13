#include <upch.h>
#include "components.h"
#include <Engine/Profiling/profile.h>

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

glm::mat4 utd::transform::get(const transform& transform)
{
    UTD_PROFILE_FUNC();
    glm::mat4 rotation = glm::toMat4(glm::quat(transform.rotation));

    return glm::translate(glm::mat4(1.0f), transform.position)
        * rotation
        * glm::scale(glm::mat4(1.0f), transform.scale);
}