#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <blocks.hpp>

extern int ofacing(int dx, int dz);
extern int ofacingNeg(int dx, int dz);

extern bool coretest(int x, int y, int z, int ground_rad, int air_rad, int height);
extern void downSpider(int x, int y, int z, block_t id, int tries);

extern void ocircleXZ(int x, int y, int z, int radius, block_t id);
extern void ocircleXZstencil(int x, int y, int z, int rad, block_t id, float chance);
extern void ocircleXZroots(int x, int y, int z, int radius, block_t id);
extern void oellipsoidXZ(int x, int y, int z, int radius, float radx, float radz, block_t id);
extern void oellipsoidXY(int x, int y, int z, int radius, float radx, float rady, float stencil, block_t id);

extern void obell(int x, int y, int z, block_t id, int lower, int height, int radius, int inner_rad, int midlevel, float midstrength, float understrength, float stencilchance);


#endif
