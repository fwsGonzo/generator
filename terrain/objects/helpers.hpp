#ifndef HELPERS_HPP
#define HELPERS_HPP

typedef unsigned short block_t;

extern int ofacing(int dx, int dz);
extern int ofacingNeg(int dx, int dz);

extern int coretest(int x, int y, int z, int rad);
extern void downSpider(int x, int y, int z, block_t id, int tries);

extern void ocircleXZ(int x, int y, int z, float radius, block_t id);
extern void oellipsoidXZ(int x, int y, int z, int radius, float radx, float radz, block_t id);
extern void oellipsoidXY(int x, int y, int z, int radius, float radx, float rady, float stencil, block_t id);
extern void ocircleXZroots(int x, int y, int z, float radius, block_t id);

extern void obell(int x, int y, int z, block_t id, int lower, int height, int radius, int inner_rad, int midlevel, float midstrength, float understrength, float stencilchance);


#endif
