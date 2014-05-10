#ifndef COSNOISE_HPP
#define COSNOISE_HPP
/**
 * cosnoise() noise function
 * as originally written by Gonzo
 * 
 * 
**/

#include "../math/vector.hpp"

namespace library
{

extern double cosnoise(vec3& v, double density, double scale_xz, double scale_y, 
						double curve, double fatness, double cuts);


}

#endif
