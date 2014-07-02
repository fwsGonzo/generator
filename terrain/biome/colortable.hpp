#ifndef COLORTABLE_HPP
#define COLORTABLE_HPP

struct cl_rgb;
#define GRAD_4    4
#define GRAD_4x4  GRAD_4][GRAD_4

extern cl_rgb StonyColors[GRAD_4x4];

extern cl_rgb clAutumnColors[GRAD_4x4];
extern cl_rgb clIslandColors[GRAD_4x4];
extern cl_rgb clGrassyColors[GRAD_4x4];
extern cl_rgb clJungleColors[GRAD_4x4];

extern cl_rgb getStoneColor(int terrain);
extern cl_rgb getGrassColor(int terrain);
extern cl_rgb getLeafColor(int terrain);
extern cl_rgb getLeafColor(int terrain);

#endif
