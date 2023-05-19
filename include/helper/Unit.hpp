#ifndef UNIT
#define UNIT

#define _USE_MATH_DEFINES
#include <math.h>

class Unit
{
    public:
        constexpr static const double DEG = M_PI / 180.0; // DEG to RAD
        constexpr static const double RAD = 180.0 / M_PI; // RAD to DEG
        constexpr static const double INCH = 0.0254; // IN to M
};

#endif