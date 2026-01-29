#pragma once

#include <GLFW/glfw3.h>

#include <format>

#include "implot.h"
#include "ILog.hpp"

namespace pixelarium::application
{
/// @brief Base class providing scaffolding with GLFW and some default
/// implementations that can be extended and/or overridden by consumers
class AppGLFW
{
   public:
    explicit AppGLFW(const utils::log::ILog& log) : logger_(log), plot_context_(ImPlot::CreateContext())
    {
        this->InitMainWindow();
    }

    ~AppGLFW() { ImPlot::DestroyContext(plot_context_); }

    AppGLFW(AppGLFW&) = delete;
    AppGLFW(const AppGLFW&) = delete;
    AppGLFW(AppGLFW&&) = delete;
    AppGLFW& operator=(AppGLFW&) = delete;
    AppGLFW& operator=(AppGLFW&&) = delete;

    /// @brief Start the main render loop
    void Start() { this->RunLoop(); }

    void SetStatusTimed(const std::string& status, size_t second);

    void SetStatus(const std::string& status)
    {
        logger_.Info(std::format("{}(): {}", __PRETTY_FUNCTION__, status));
        status_message_ = status;
        show_status_ = true;
    }

    void ResetStatus()
    {
        status_message_.clear();
        show_status_ = false;
    }

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
    ImPlotContext* plot_context_ = nullptr;
    bool show_status_{false};
    std::string status_message_{};
};
}  // namespace pixelarium::application
