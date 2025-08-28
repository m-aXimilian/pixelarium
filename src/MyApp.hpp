#pragma once

#include <cstddef>
#include <memory>

#include "AppGLFW.hpp"
#include "imgui.h"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"
#include "viewmodels/ImageViewFactory.hpp"
#include "views/PixelariumImageView.hpp"

namespace pixelarium::ui
{
class MyApp : public application::AppGLFW
{
   public:
    MyApp(const utils::log::ILog& log, pixelarium::resources::ImageResourcePool& pool)
        : application::AppGLFW(log), pool_(pool)
    {
        image_view_factory_ = std::make_unique<ImageViewFactory>(pool_);
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
    std::unique_ptr<ImageViewFactory> image_view_factory_;
    std::unique_ptr<PixelariumImageView> view_;
    bool image_listp_{false};
    bool demop_{false};
    ImVec2 curr_dim_;
    size_t selected_image_{0};
};
}  // namespace pixelarium::ui
