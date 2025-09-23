#pragma once

#include <memory>

#include "imaging/IPixelariumImage.hpp"
#include "imgui.h"
#include "rendering/CvMatRender.hpp"
#include "rendering/IPixelariumImageView.hpp"

namespace pixelarium::render
{
class PixelariumImageViewDefault : public IPixelariumImageView
{
    using Image = imaging::IPixelariumImage;
    using Render = render::CvMatRender;

   public:
    explicit PixelariumImageViewDefault(std::shared_ptr<Image> img)
    {
        img_ = img;
        render_ = Render(img_);
    }
    PixelariumImageViewDefault() = delete;
    PixelariumImageViewDefault(PixelariumImageViewDefault&) = delete;
    PixelariumImageViewDefault(const PixelariumImageViewDefault&) = delete;
    PixelariumImageViewDefault(PixelariumImageViewDefault&&) = delete;
    PixelariumImageViewDefault& operator=(PixelariumImageViewDefault&) = delete;
    PixelariumImageViewDefault& operator=(PixelariumImageViewDefault&&) = delete;

    void ShowImage() override;

   private:
    ImVec2 curr_dim_{};
};
}  // namespace pixelarium::render
