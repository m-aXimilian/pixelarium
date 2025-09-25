#pragma once

#include "imgui.h"
#include "rendering/IPixelariumImageView.hpp"

namespace pixelarium::render
{
class PixelariumImageViewCzi : public IPixelariumImageView
{
    using Image = imaging::IPixelariumImage;
    using Render = render::CvMatRender;

   public:
    explicit PixelariumImageViewCzi(std::shared_ptr<Image> img)
    {
        img_ = img;
        render_ = Render(img_);
    }
    PixelariumImageViewCzi() = delete;
    PixelariumImageViewCzi(PixelariumImageViewCzi&) = delete;
    PixelariumImageViewCzi(const PixelariumImageViewCzi&) = delete;
    PixelariumImageViewCzi(PixelariumImageViewCzi&&) = delete;
    PixelariumImageViewCzi& operator=(PixelariumImageViewCzi&) = delete;
    PixelariumImageViewCzi& operator=(PixelariumImageViewCzi&&) = delete;

    void ShowImage() override;

   private:
    ImVec2 curr_dim_{};
};
}  // namespace pixelarium::render
