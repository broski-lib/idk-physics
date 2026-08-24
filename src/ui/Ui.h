#pragma once

namespace idk {

struct SimInput;
class Sim;

// Secondary UI layer: owns the window, the ImGui and ImPlot3D contexts, and
// renders the simulation's state. It never mutates the Sim; it only reads
// the snapshot and fills a SimInput struct for the app to apply.
class Ui {
public:
    Ui(int width, int height, const char* title);
    ~Ui();

    // Polls window events and starts an ImGui frame. Returns false when the
    // window has been closed (the app should shut down).
    bool beginFrame();

    // Draws the current sim state and writes control input for the next sim
    // step. The sim is read-only here.
    void draw(const Sim& sim, SimInput& input);

    void endFrame();

private:
    struct Impl;
    Impl* impl_;
};

}  // namespace idk