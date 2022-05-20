#include "material.h"

namespace vx::fizz {
    auto lameParameterMu(double E, double v) -> double { return E / 2 * (1 + v); }
    auto lameParameterLambda(double E, double v) -> double { return (E * v) / ((1 + v) * (1 - 2 * v)); }
}// namespace vx::fizz
