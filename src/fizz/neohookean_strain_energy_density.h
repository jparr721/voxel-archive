#pragma once

namespace vx::fizz {
    // I_1 = trace(R^T * F)
    // I_2 = ||F||^2
    // I_3 = J = det F
    auto neohookeanStrainEnergyDensity(double C, double D, double **mat) -> double;
}// namespace vx::fizz
