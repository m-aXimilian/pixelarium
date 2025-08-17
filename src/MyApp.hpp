#pragma once

#include <GLFW/glfw3.h>

#include <memory>

#include "AppGLFW.hpp"
#include "imgui.h"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"
#include "viewmodels/ImageViewFactory.hpp"

namespace pixelarium::ui
{
class MyApp : public application::AppGLFW
{
   public:
    MyApp(std::unique_ptr<utils::log::ILog>& log, std::unique_ptr<pixelarium::resources::ImageResourcePool>& pool)
        : application::AppGLFW(log), pool_(*pool)
    {
        image_view_model_ = std::make_unique<ImageViewFactory>(pool_);
    }

   protected:
    void MenuBarOptionsColumn1() override;
    void MenuBarOptionsColumn2() override;
    void Run() override;

   private:
    void LoadImageProt();

   private:
    resources::ImageResourcePool& pool_;
    std::unique_ptr<ImageViewFactory> image_view_model_;
    bool imagep_{false};
    bool demop_{false};
    int log_level_{0};
    ImVec2 curr_dim_;
};
}  // namespace pixelarium::ui
