#include "sim/Sim.h"

#include <algorithm>
#include <cmath>

namespace idk {

Sim::Sim() = default;

void Sim::step(const SimInput& input, double dt) {
    if (input.paused) {
        return;
    }

    const double h = dt * std::clamp(input.speed, 0.0, 100.0);
    phase_ += h;

    for (std::size_t i = 0; i < kPointCount; ++i) {
        const double t = static_cast<double>(i) / (kPointCount - 1);
        snapshot_.points[i] = {
            std::cos(phase_ + t * 8.0) * t,
            std::sin(phase_ + t * 8.0) * t,
            t * 2.0 - 1.0,
        };
    }

    snapshot_.time += h;
    snapshot_.stepCount++;
}

}  // namespace idk