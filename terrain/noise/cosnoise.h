/*
	cosnoise noise function
*/
//#include "vec.h"
#include <glm/glm.hpp>

extern float cosnoise(const glm::vec3& v, float density, float scale_xz, float scale_y, 
											float curve, float fatness, float cuts);
