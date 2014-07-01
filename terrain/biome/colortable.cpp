#include "colortable.hpp"

/// color tables for 2D gradients ///

#include "generator.h"
#include "biome.hpp"

cl_rgb StonyColors[GRAD_STONE][GRAD_STONE] =
{
	{
		{  90, 77, 65 },  // rock
		{  88, 88,100 }, // bastille
		{ 120,120,120 },
		{ 110,110,110 }
	},
	{
		{  85, 96, 97 }, // river bed
		{ 119, 66, 44 }, // copper canyon
		{ 115,115,115 },
		{ 105,105,105 }
	},
	{
		{ 105, 96, 97 },
		{ 109, 86, 64 },
		{  68, 71, 58 }, // dark grey
		{  95,105, 95 }
	},
	{
		{  90,103, 96 },
		{  90, 87, 74 },
		{ 136,120, 88 },
		{ 116, 87, 74 }  // more copper
	}
};

cl_rgb GrassyColors[GRAD_GRASS][GRAD_GRASS] =
{
	{
		{   0,186, 0 },
		{   8,156, 0 },
		{  16,120, 0 },
		{  24, 88, 0 },
	},
	{
		{  32,111, 0 },
		{  40,103, 0 },
		{  48, 76, 0 },
		{  56,130, 0 },
	},
	{
		{  64,114, 0 },
		{  72,138, 0 },
		{  80,127, 0 },
		{  88, 73, 0 },
	},
	{
		{  96,122, 0 },
		{ 104, 96, 0 },
		{ 112,134, 0 },
		{ 120,115, 0 },
	},
};

// indirectly 2D gradients via weights
// can be made into arrays that match T_TERRAINS size

cl_rgb getStoneColor(int terrain)
{
	switch (terrain)
	{
	case T_ICECAP:
	case T_SNOW:    // 
		return cl_rgb(216, 216, 216);
	case T_AUTUMN:  // 
		return cl_rgb(184, 168, 136);
	case T_ISLANDS: // 
		return cl_rgb(168, 168, 160);
	case T_GRASS:   // 
		return cl_rgb(232, 232, 200);
	case T_MARSH:
	case T_JUNGLE:  // 
		return cl_rgb(168, 168, 136);  // { 168, 152, 104 }; cool green stone
	case T_DESERT:  // 
		return cl_rgb(216, 112, 80);
	}
	return cl_rgb(255, 0, 255);
}

cl_rgb getGrassColor(int terrain)
{
	switch (terrain)
	{
	case T_ICECAP:
	case T_SNOW:   // 
		return (cl_rgb) { 222, 233, 238 };
	case T_AUTUMN: // autumn brown
		return (cl_rgb) { 113, 121, 64 };
	case T_ISLANDS:  // grass island
		return (cl_rgb) {  76, 158,  0 };
	case T_GRASS:  // grass
		return (cl_rgb) { 72, 144,  16 };
	case T_MARSH:
	case T_JUNGLE: // jungle grass
		return (cl_rgb) {  54, 112, 10 };
	case T_DESERT: // android green
		return (cl_rgb) { 164, 198, 57 };
	}
	return (cl_rgb) { 255, 0, 255 };
}

cl_rgb getLeafColor(int terrain)
{
	switch (terrain)
	{
	case T_ICECAP:
	case T_SNOW:   // ALW needle green
		return (cl_rgb) { 54, 107, 59 };
	case T_AUTUMN: // Autumn brown
		return (cl_rgb) { 97, 121, 64 };
	case T_ISLANDS:  // Island trees
		return (cl_rgb) { 87, 156,  0 };
	case T_GRASS:  // Dark green trees
		return (cl_rgb) { 51, 136,  8 };
	case T_MARSH:
	case T_JUNGLE: // Jungle green
		return (cl_rgb) { 26,  88,  0 };
	case T_DESERT: // India Green
		return (cl_rgb) { 84, 166, 17 };
	}
	return (cl_rgb) { 255, 0, 255 };
}
