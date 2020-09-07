#ifndef VORONOI_HPP
#define VORONOI_HPP

namespace library
{
class Voronoi
{
public:
    typedef float vor_t;

    typedef vor_t (*distance_func)(vor_t x1, vor_t y1, vor_t x2, vor_t y2);

    // usable distance functions
    static vor_t vor_euclidian(vor_t, vor_t, vor_t, vor_t);
    static vor_t vor_quadratic(vor_t, vor_t, vor_t, vor_t);
    static vor_t vor_linsquare(vor_t, vor_t, vor_t, vor_t);
    static vor_t vor_minkowski(vor_t, vor_t, vor_t, vor_t);
    static vor_t vor_manhattan(vor_t, vor_t, vor_t, vor_t);
    static vor_t vor_chebyshev(vor_t, vor_t, vor_t, vor_t);

    static vor_t getdist(vor_t x, vor_t y, distance_func);
    static int getid(vor_t x, vor_t y, distance_func);
};

} // namespace library

#endif // VORONOI_HPP
