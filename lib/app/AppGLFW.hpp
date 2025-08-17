#pragma once

#include <GLFW/glfw3.h>

#include <memory>

#include "imgui.h"
#include "utilities/ILog.hpp"

namespace pixelarium::application
{
class AppGLFW
{
   public:
    explicit AppGLFW(const utils::log::ILog& log) : logger_(log) { this->InitMainWindow(); }

    void Start() { this->RunLoop(); }

   protected:
    virtual void MenuBarOptionsColumn1() {}
    virtual void MenuBarOptionsColumn2() {}
    virtual void MenuBarOptionsColumn3() {}
    virtual void MenuBarOptionsColumn4() {}
    virtual void MenuBarOptionsColumn5() {}
    virtual void Run() {}

    const utils::log::ILog& logger_;

   private:
    int RunLoop();
    void InitMainWindow();
    void MenuBar();
    void LogLevelSelect();
    int log_level_{0};
    GLFWwindow* window = nullptr;
    ImGuiWindowFlags window_flags_ = 0;
};
}  // namespace pixelarium::application
