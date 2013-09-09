#include <math.h>
#include "multifractal.h"
#include "simplex1234.h"

float Multifractal2(double x, double y)
{
	const double H = 0.5;
	const double lacunarity = 2.3;
	const double gain = 2.0;
	
	double frequency = 0.5;
	double signal = snoise2(x, y);
	double offset = 0.0;
	signal = offset - signal;
	signal *= signal;
	double result = signal;
	double weight = 1.0;
	
	result += 0.2 * snoise2(x*0.25, y*0.25);
	
	int i = 1;
	for (; i<4; i++)
	{
		x *= lacunarity;
		y *= lacunarity;
		weight = signal * gain;
		if (weight < 0.0) weight = 0.0; else if (weight > 1.0) weight = 1.0;
		signal = snoise2(x, y);
		signal = offset - signal;
		signal *= signal;
		signal = signal*weight;

		result += signal * pow( frequency, -H );
		frequency *= lacunarity;
	}

	result = tanh(result * 1.2);

	return result*0.9;
}
