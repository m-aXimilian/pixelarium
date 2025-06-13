#pragma once

#include <GLFW/glfw3.h>

#include <cstdio>
#include <format>
#include <memory>

#include "PixelariumImage.hpp"
#include "imgui.h"
#include "rendering/CvMatRender.hpp"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"

namespace pixelarium::ui
{
static bool dim_changed_p(const ImVec2& ref_rect, const ImVec2& new_rect);

static ImVec2 aspect_const_dimensions(const pixelarium::imaging::PixelariumImage& img, const ImVec2& curr_dim);

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
    AppGLFW() { this->InitMainWindow(); }
    AppGLFW(std::unique_ptr<utils::log::ILog>& log) : AppGLFW()
    {
        logger_ = log.get();
        if (logger_)
        {
            logger_->Debug(std::format("{}: Initiating a new window", __FUNCTION__).c_str());

            if (pool_)
            {
                logger_->Debug(std::format("{}: We have an image resource pool!", __FUNCTION__).c_str());
            }
        }
    }
    AppGLFW(std::unique_ptr<utils::log::ILog>& log, std::unique_ptr<pixelarium::resources::ImageResourcePool>& pool)
        : AppGLFW(log)
    {
        pool_ = pool.get();
    };
    int Run();

   private:
    void InitMainWindow();
    void MenuBar();
    void LoadImageProt();

   private:
    // LogLevelSelection log_level_ = static_cast<LogLevelSelection>(0);
    utils::log::ILog* logger_;
    resources::ImageResourcePool* pool_;
    GLFWwindow* window = nullptr;
    ImGuiWindowFlags window_flags_ = 0;
    std::shared_ptr<pixelarium::imaging::PixelariumImage> img_;
    pixelarium::render::CvMatRender render_;
    bool imagep_{false};
    ImVec2 curr_dim_;
};

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
}  // namespace pixelarium::ui
