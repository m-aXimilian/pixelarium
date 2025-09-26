#pragma once

#include <cstddef>
#include <memory>

#include "AppGLFW.hpp"
#include "imgui.h"
#include "rendering/RenderImageManager.hpp"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"

namespace pixelarium::ui
{
/// @brief Default implementation of AppGLFW.
/// This can either be used as is, as an example or as a base class
/// providing some defaults for a more custom implementation.
class DefaultApp : public application::AppGLFW
{
   public:
    DefaultApp(const utils::log::ILog& log, pixelarium::resources::ImageResourcePool& pool)
        : application::AppGLFW(log),
          pool_(pool),
          render_manager_(std::make_unique<render::RenderImageManager>(pool, log))
    {
    }

   protected:
    void MenuBarOptionsColumn1() override;
    void MenuBarOptionsColumn2() override;
    void Run() override;

   private:
    void LoadImage();
    void ImageGalleryRender();
    void RenderImages();

   private:
    resources::ImageResourcePool& pool_;
    std::unique_ptr<render::RenderImageManager> render_manager_;
    bool image_listp_{true};
    bool auto_show_selectd_image_{true};
    bool demop_{false};
    ImVec2 curr_dim_;
    size_t selected_image_{0};
};
}  // namespace pixelarium::ui
