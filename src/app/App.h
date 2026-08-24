#pragma once

#include "sim/Sim.h"

namespace idk {

class Ui;

// Owns the main flow of the app: the simulation always runs, and the UI
// (if built) is a secondary consumer that renders the sim's state and
// produces input for the next sim step.
class App {
public:
    App();
    ~App();

    int run();

private:
    Sim sim_;
    Ui* ui_ = nullptr;
};

}  // namespace idk