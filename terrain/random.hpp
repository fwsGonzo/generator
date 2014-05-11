///
/// hash algorithm for randomness that isn't random at all
///
#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <cstdint>
#include <cmath>

inline unsigned int hash(unsigned int x)
{
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}
inline int ihash(int x)
{
	return hash(x) & INT32_MAX;
}

inline float fhash(int x)
{
	return ihash(x) / (float)INT32_MAX;
}
inline float fhash(int x, int y)
{
	return fhash(x xor ihash(y));
}
inline float fhash(int x, int y, int z)
{
	return fhash(x xor ihash(y) xor ihash(z));
}

#endif
