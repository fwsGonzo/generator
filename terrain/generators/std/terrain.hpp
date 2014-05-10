#ifndef NOISE_HPP
#define NOISE_HPP

#include "../../vec.h"

extern f32_t getnoise_caves(vec3 p);

extern f32_t getnoise_icecap(vec3 p);
extern f32_t getnoise_snow(vec3 p);
extern f32_t getnoise_autumn(vec3 p);
extern f32_t getnoise_islands(vec3 p);
extern f32_t getnoise_grass(vec3 p);
extern f32_t getnoise_marsh(vec3 p);
extern f32_t getnoise_jungle(vec3 p);
extern f32_t getnoise_desert(vec3 p);

#endif
