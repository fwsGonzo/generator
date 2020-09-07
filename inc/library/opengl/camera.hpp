#ifndef LIBRARY_CAMERA_HPP
#define LIBRARY_CAMERA_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <library/math/frustum.hpp>

namespace library
{
class WindowClass;

class Camera
{
public:
    Camera() {}
    Camera(float FOV, float aspect, float znear, float zfar);

    void calculateFrustum();
    const library::Frustum& getFrustum() const;

    //! \brief returns the perspective field of view in degrees
    inline float getFOV() const { return this->FOV; }
    //! \brief returns the distance from eye to znear plane
    inline float getZNear() const { return this->znear; }
    //! \brief returns the distance from eye to zfar plane
    inline float getZFar() const { return this->zfar; }

    //! \brief set perspective projection matrix
    void setProjection(float FOV, float aspect, float znear, float zfar);
    //! \brief set rotation and translation (unstacked)
    void setRotation(float rx, float ry, float rz);
    void setTranslation(float tx, float ty, float tz);

    //! \brief directly set rotation matrix
    void setRotationMatrix(const glm::mat4& rotation_matrix);
    //! \brief directly set modelview matrix
    void setViewMatrix(const glm::mat4& view_matrix);

    const glm::mat4& getProjection() const { return matproj; }
    const glm::mat4& getViewMatrix() const { return matview; }
    const glm::mat4& getRotationMatrix() const { return matrot; }
    const glm::mat4& getMVP() const { return matviewproj; }

    const glm::vec2& getNearPlaneHalfSize() const { return nearPlaneHalfSize; }

protected:
    float FOV;         //! \brief field of view (angle in degrees)
    float znear, zfar; //! \brief znear and zfar planes

    // viewing frustum representation
    library::Frustum frustum;

    // camera matrices
    glm::mat4 matproj;     //! \brief projection matrix (P)
    glm::mat4 matrot;      //! \brief camera rotation matrix (R)
    glm::mat4 matview;     //! \brief modelview matrix (MV)
    glm::mat4 matviewproj; //! \brief modelview-projection matrix (MVP)
    // used to reconstruct viewpos from windowspace depth
    glm::vec2 nearPlaneHalfSize;
};

} // namespace library

#endif
