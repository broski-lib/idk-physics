#include "app/App.h"

#include <chrono>

#include "sim/Sim.h"

#ifdef BUILD_UI
#include "ui/Ui.h"
#endif

namespace idk {

namespace {

constexpr double kStepSeconds = 1.0 / 120.0;
constexpr double kMaxFrameSeconds = 0.25;

using Clock = std::chrono::steady_clock;

}  // namespace

App::App()
#ifdef BUILD_UI
    : ui_(new Ui(1280, 720, "idk-physics"))
#endif
{
}

App::~App() {
#ifdef BUILD_UI
    delete ui_;
#endif
}

int App::run() {
    auto last = Clock::now();
    double accumulator = 0.0;
    SimInput input;

    for (;;) {
        const auto now = Clock::now();
        double frameSeconds = std::chrono::duration<double>(now - last).count();
        last = now;
        if (frameSeconds > kMaxFrameSeconds) {
            frameSeconds = kMaxFrameSeconds;
        }

#ifdef BUILD_UI
        if (ui_ && !ui_->beginFrame()) {
            break;
        }
#endif

        accumulator += frameSeconds;
        while (accumulator >= kStepSeconds) {
            sim_.step(input, kStepSeconds);
            accumulator -= kStepSeconds;
        }

#ifdef BUILD_UI
        if (ui_) {
            ui_->draw(sim_, input);
            ui_->endFrame();
        }
#endif
    }

    return 0;
}

}  // namespace idk