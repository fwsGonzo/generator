#pragma once

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace library
{
extern glm::mat4 biasMatrix();
extern glm::mat4 rotationMatrix(float ax, float ay, float az);
inline glm::mat4 extractRotation(glm::mat4& matrix) { return glm::mat4(glm::mat3(matrix)); }

extern glm::mat4 ortho2dMatrix(float width, float height, float znear, float zfar);
extern glm::mat4 orthoMatrix(float width, float height, float znear, float zfar);
extern glm::mat4 perspectiveMatrix(float fov, float aspect, float znear, float zfar);
} // namespace library
