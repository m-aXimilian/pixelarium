#pragma once

#include <GLFW/glfw3.h>

#include <cstdio>

#include "Image.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "rendering/CvMatRender.hpp"

namespace pixelarium::ui
{
enum LogLevelSelection
{
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3
};

class AppGLFW
{
   public:
    AppGLFW();
    int Run();

   private:
    void MenuBar();
    void LoadImageProt();

   private:
    LogLevelSelection log_level_ = static_cast<LogLevelSelection>(0);
    GLFWwindow* window = nullptr;
    ImGuiWindowFlags window_flags = 0;
    std::shared_ptr<pixelarium::imaging::Image> _img;
    pixelarium::render::CvMatRender _render;
    bool _imagep { false };
};

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
}  // namespace ui