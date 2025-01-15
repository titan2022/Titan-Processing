#ifndef UNIT
#define UNIT

#define _USE_MATH_DEFINES
#include <cmath>

namespace titan
{
namespace Unit
{
// DEG to RAD
constexpr const double DEG = M_PI / 180.0;
// RAD to DEG
constexpr const double RAD = 180.0 / M_PI;
// IN to M
constexpr const double INCH = 0.0254;
}; // namespace Unit
} // namespace titan

#endif