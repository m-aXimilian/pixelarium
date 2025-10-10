#pragma once

#include <cstddef>

#include "AppGLFW.hpp"
#include "app/PixelariumGallery.hpp"
#include "imgui.h"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"

namespace pixelarium::application
{
/// @brief Default implementation of AppGLFW.
/// This can either be used as is, as an example or as a base class
/// providing some defaults for a more custom implementation.
class DefaultApp : public AppGLFW
{
   public:
    DefaultApp(const utils::log::ILog& log, pixelarium::resources::ImageResourcePool& pool)
        : application::AppGLFW(log), pool_(pool), gallery(log, pool)
    {
        gallery.SetLoadFunction([&]() -> void { this->LoadImage(); });
    }

   protected:
    void MenuBarOptionsColumn1() override;
    void MenuBarOptionsColumn2() override;
    void Run() override;

   private:
    void LoadImage();

   private:
    resources::ImageResourcePool& pool_;
    bool image_listp_{true};
    bool demop_{false};
    ImVec2 curr_dim_;
    application::PixelrariumImageGallery gallery;
};
}  // namespace pixelarium::application
