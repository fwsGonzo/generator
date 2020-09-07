#ifndef VOXELIZER_HPP
#define VOXELIZER_HPP

#include <library/math/vector.hpp>

namespace library
{
class Bitmap;

class XModel
{
public:
    typedef unsigned int xcolor_t;

    struct xvertex_t
    {
        float x, y, z;
        char nx, ny, nz, p1;
        xcolor_t c;
    };

    XModel();
    XModel(XModel& mod);
    ~XModel();

    xvertex_t* data();
    int vertices();

    void extrude(const Bitmap& fromImage, const glm::vec3& offset, const glm::vec3& scale);

    static void initVoxelizer();

private:
    static const int VOXELIZER_MAX_VERTICES = 32768;
    static xvertex_t* xv_dump;

    short cull2D(const Bitmap& img, int x, int y);
    void putv2D(const glm::vec3& offset, const glm::vec3& scale, int x, int y, int fid, int vid,
                xcolor_t vcolor);

    xvertex_t* vdata;
    int verts;
};
} // namespace library
#endif
