#pragma once

#include <GLFW/glfw3.h>

#include <memory>

#include "imgui.h"
#include "utilities/ILog.hpp"

namespace pixelarium::application
{
/// @brief Base class providing scaffolding with GLFW and some default
/// implementations that can be extended and/or overridden by consumers
class AppGLFW
{
   public:
    explicit AppGLFW(const utils::log::ILog& log) : logger_(log) { this->InitMainWindow(); }

    /// @brief Start the main render loop
    void Start() { this->RunLoop(); }

   protected:
    /// @brief Function implementing the first column of the menu bar (e.g. "Menu")
    virtual void MenuBarOptionsColumn1() {}

    /// @brief Function implementing the second column of the menu bar (e.g. "File")
    virtual void MenuBarOptionsColumn2() {}

    /// @brief Function implementing the third column of the menu bar (e.g. "Options")
    virtual void MenuBarOptionsColumn3() {}

    /// @brief Function implementing the fourth column of the menu bar (e.g. "More")
    virtual void MenuBarOptionsColumn4() {}

    /// @brief Function implementing the first column of the menu bar (e.g. "Help")
    virtual void MenuBarOptionsColumn5() {}

    /// @brief Main function that gets called within the render loop.
    virtual void Run() {}

    const utils::log::ILog& logger_;

   private:
    int RunLoop();
    void InitMainWindow();
    void MenuBar();
    void LogLevelSelect();
    int log_level_{0};
    GLFWwindow* window = nullptr;
};
}  // namespace pixelarium::application
