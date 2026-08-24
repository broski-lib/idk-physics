#pragma once

#include <array>
#include <cstddef>

namespace idk {

struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

struct SimInput {
    double speed = 1.0;
    bool paused = false;
};

// Minimal simulation core: no rendering, no windowing, no ImGui. The UI
// observes it through Snapshot and influences it only via SimInput.
class Sim {
public:
    static constexpr std::size_t kPointCount = 64;

    struct Snapshot {
        std::array<Vec3, kPointCount> points{};
        double time = 0.0;
        int stepCount = 0;
    };

    Sim();

    void step(const SimInput& input, double dt);
    const Snapshot& snapshot() const { return snapshot_; }

private:
    Snapshot snapshot_{};
    double phase_ = 0.0;
};

}  // namespace idk