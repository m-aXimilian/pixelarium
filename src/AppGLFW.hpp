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
#include "viewmodels/ImageViewFactory.hpp"
#include "views/PixelariumImageView.hpp"

namespace pixelarium::ui
{
class AppGLFW
{
   public:
    AppGLFW(std::unique_ptr<utils::log::ILog>& log, std::unique_ptr<pixelarium::resources::ImageResourcePool>& pool)
        : logger_(*log), pool_(*pool)
    {
        image_view_model_ = std::make_unique<ImageViewFactory>(pool_);

        this->InitMainWindow();
    }
    int Run();

   private:
    void InitMainWindow();
    void MenuBar();
    void LoadImageProt();

   private:
    utils::log::ILog& logger_;
    resources::ImageResourcePool& pool_;
    GLFWwindow* window = nullptr;
    ImGuiWindowFlags window_flags_ = 0;
    // std::shared_ptr<pixelarium::imaging::PixelariumImage> img_;
    // std::shared_ptr<pixelarium::ui::PixelariumImageView> image_view_;
    std::unique_ptr<ImageViewFactory> image_view_model_;
    // pixelarium::render::CvMatRender render_;
    bool imagep_{false};
    bool demop_{false};
    int log_level_{0};
    ImVec2 curr_dim_;
};

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
}  // namespace pixelarium::ui
