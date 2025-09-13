#pragma once

#include <cstddef>
#include <memory>

#include "AppGLFW.hpp"
#include "RenderImageManager.hpp"
#include "imgui.h"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"

namespace pixelarium::ui
{
class MyApp : public application::AppGLFW
{
   public:
    MyApp(const utils::log::ILog& log, pixelarium::resources::ImageResourcePool& pool)
        : application::AppGLFW(log), pool_(pool), render_manager_(std::make_unique<RenderImageManager>(pool, log))
    {
    }

   protected:
    void MenuBarOptionsColumn1() override;
    void MenuBarOptionsColumn2() override;
    void Run() override;

   private:
    void LoadImageProt();
    void ImageGalleryRender();

   private:
    resources::ImageResourcePool& pool_;
    std::unique_ptr<RenderImageManager> render_manager_;
    bool image_listp_{true};
    bool auto_show_selectd_image_{true};
    bool demop_{false};
    ImVec2 curr_dim_;
    size_t selected_image_{0};
};
}  // namespace pixelarium::ui
