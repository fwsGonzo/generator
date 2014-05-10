#ifndef COLORTABLE_HPP
#define COLORTABLE_HPP

struct cl_rgb;
#define GRAD_STONE  4
#define GRAD_GRASS  8

extern cl_rgb StonyColors[GRAD_STONE][GRAD_STONE];
extern cl_rgb GrassyColors[GRAD_GRASS][GRAD_GRASS];

extern cl_rgb getStoneColor(int terrain);
extern cl_rgb getGrassColor(int terrain);
extern cl_rgb getCrossColor(int terrain);
extern cl_rgb getLeafColor(int terrain);
extern cl_rgb getLeafColor(int terrain);

#endif
