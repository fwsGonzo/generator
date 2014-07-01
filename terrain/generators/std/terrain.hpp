#ifndef NOISE_HPP
#define NOISE_HPP

namespace library
{
	class vec3;
}

extern float getnoise_caves(library::vec3 p);

extern float getnoise_icecap(library::vec3 p);
extern float getnoise_snow(library::vec3 p);
extern float getnoise_autumn(library::vec3 p);
extern float getnoise_islands(library::vec3 p);
extern float getnoise_grass(library::vec3 p);
extern float getnoise_marsh(library::vec3 p);
extern float getnoise_jungle(library::vec3 p);
extern float getnoise_desert(library::vec3 p);

#endif
