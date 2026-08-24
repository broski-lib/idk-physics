#include "ui/Ui.h"

#include <cstdio>
#include <cstdlib>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot3d.h>

#include "sim/Sim.h"

namespace idk {

struct Ui::Impl {
    GLFWwindow* window = nullptr;
    float speed = 1.0f;
};

Ui::Ui(int width, int height, const char* title) : impl_(new Impl) {
    glfwSetErrorCallback([](int code, const char* desc) {
        std::fprintf(stderr, "[glfw] error %d: %s\n", code, desc);
    });
    if (!glfwInit()) {
        std::fprintf(stderr, "Failed to initialize GLFW\n");
        std::exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    impl_->window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!impl_->window) {
        std::fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        std::exit(1);
    }
    glfwMakeContextCurrent(impl_->window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot3D::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(impl_->window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

Ui::~Ui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot3D::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(impl_->window);
    glfwTerminate();
    delete impl_;
}

bool Ui::beginFrame() {
    glfwPollEvents();
    if (glfwWindowShouldClose(impl_->window)) {
        return false;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    return true;
}

void Ui::draw(const Sim& sim, SimInput& input) {
    const Sim::Snapshot& snap = sim.snapshot();

    ImGui::Begin("Simulation");
    ImGui::Checkbox("Pause", &input.paused);
    ImGui::SliderFloat("Speed", &impl_->speed, 0.0f, 4.0f, "%.2fx");
    input.speed = impl_->speed;
    ImGui::Separator();
    ImGui::Text("time : %.3f s", snap.time);
    ImGui::End();

    ImGui::Begin("3D View");
    if (ImPlot3D::BeginPlot("Simulation", ImVec2(-1, -1), ImPlot3DFlags_Equal)) {
        ImPlot3D::SetupAxes("x", "y", "z");
        ImPlot3D::SetupAxesLimits(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        ImPlot3D::PlotLine("samples", &snap.points[0].x, &snap.points[0].y, &snap.points[0].z,
                           static_cast<int>(snap.points.size()),
                           {ImPlot3DProp_Stride, sizeof(Vec3)});
        ImPlot3D::EndPlot();
    }
    ImGui::End();
}

void Ui::endFrame() {
    ImGui::Render();
    const ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
    glClearColor(0.10f, 0.10f, 0.10f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(impl_->window);
}

}  // namespace idk