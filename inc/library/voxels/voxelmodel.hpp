#ifndef VOXELMODEL_HPP
#define VOXELMODEL_HPP

#include <library/opengl/vao.hpp>

/**
 * Create, check that its "good", then render!
 * This model is specially made to be used with the XModel class
 *
 **/

namespace library
{
class VoxelModel
{
public:
    VoxelModel(GLsizei vertices, GLvoid* data);
    VoxelModel(const VoxelModel& vx);

    bool isGood() const { return vao.good(); }

    void render();
    void render(GLint mode);

private:
    VAO vao;
};
} // namespace library

#endif
