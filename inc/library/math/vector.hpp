#pragma once

#include <cmath>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace library
{
inline glm::vec3 lookVector(const glm::vec2& rot)
{
    return glm::vec3(std::sin(rot.y) * std::cos(rot.x), -std::sin(rot.x),
                     -std::cos(rot.y) * std::cos(rot.x));
}
} // namespace library
