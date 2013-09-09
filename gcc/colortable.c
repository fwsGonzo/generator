// color tables for 2D gradients

#define GRAD_STONE  4

cl_rgb StonyColors[GRAD_STONE][GRAD_STONE] =
{
	{
		{  90,77,65 },  // rock
		{  88,88,100 }, // bastille
		{ 120,120,120 },
		{ 110,110,110 }
	},
	{
		{  85,96,97 }, // river bed
		{ 119,66,44 }, // copper canyon
		{ 115,115,115 },
		{ 105,105,105 }
	},
	{
		{ 105,96,97 },
		{ 109,86,64 },
		{ 68,71,58 },   // dark grey
		{ 95,105,95 }
	},
	{
		{ 90,103,96 },
		{ 90,87,74 },
		{ 136,120,88 },
		{ 116,87,74 } // more copper
	}
};

#define GRAD_GRASS  8

cl_rgb GrassyColors[GRAD_GRASS][GRAD_GRASS] =
{
	 99,186, 59,
	 83,156, 91,
	111,120, 52,
	 84, 88, 52,
	161,143,105,
	123,124, 85,
	 40,112, 45,
	131,101, 50,
	
	125,111, 62,
	 75,103, 65,
	 46, 76, 40,
	104,126, 54,
	 57,130, 61,
	136,176, 50,
	119,146, 45,
	192,189, 97,
	
	 46,114, 51,
	 68,138, 66,
	110,129, 57,
	 77,127, 46,
	 80, 73, 35,
	130,123, 66,
	 84,101, 47,
	
	166,146, 80,
	 66,122, 59,
	 55, 96, 36,
	 97,134, 65,
	126,126, 66,
	105,172, 79,
	 55,115, 55,
	119,129, 76,
	128,153, 50,
	
	 75,117, 54,
	 53, 85, 38,
	145,173, 79,
	 67,118, 65,
	206,188,125,
	106, 93, 59,
	 48, 75, 47,
	 65,113, 59,
	
	130,170, 56,
	117,169, 44,
	140,171, 62,
	128,167, 42,
	125,147, 53,
	102,121, 28,
	217,185,134,
	193,155, 96,
	
	206,203,121,
	175,196, 53,
	137,168, 58,
	126,155, 43,
	110,141, 28,
	 93,119, 36,
	 71,104, 22,
	 57, 73, 30,
	
	119, 91, 53,
	179,161,106,
	223,201,136,
	239,233,140,
	165,192,  6,
	144,177,  2,
	173,174, 12,
	 81,130,  4
};

// indirectly 2D gradients via weights
// can be made into arrays that match T_TERRAINS size

cl_rgb getStoneColor(int terrain)
{
	switch (terrain)
	{
	case T_ICECAP:
	case T_SNOW:    // 
		return (cl_rgb) { 216, 216, 216 };
	case T_AUTUMN:  // 
		return (cl_rgb) { 184, 168, 136 };
	case T_ISLANDS: // 
		return (cl_rgb) { 168, 168, 160 };
	case T_GRASS:   // 
		return (cl_rgb) { 232, 232, 200 };
	case T_MARSH:
	case T_JUNGLE:  // 
		return (cl_rgb) { 168, 168, 136 };  // { 168, 152, 104 }; cool green stone
	case T_DESERT:  // 
		return (cl_rgb) { 216, 112, 80 };
	}
	return (cl_rgb) { 255, 0, 255 };
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

cl_rgb getCrossColor(int terrain)
{
	switch (terrain)
	{
	case T_ICECAP:
	case T_SNOW:
		//return (cl_rgb) { 19, 136, 8 };
		return (cl_rgb) { 233, 233, 255 };
	case T_AUTUMN:
		return (cl_rgb) { 81, 105, 48 };
	case T_ISLANDS:  // grass island
		return (cl_rgb) {  76, 158,  0 };
	case T_GRASS:
		return (cl_rgb) { 72, 144,  16 };
	case T_MARSH:
	case T_JUNGLE:
		return (cl_rgb) { 44,93,7 }; //{ 133, 133, 54 };
	case T_DESERT:
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
