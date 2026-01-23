#pragma once

#include <memory>

#include "imaging/IPixelariumImage.hpp"
#include "imgui.h"
#include "CvMatRender.hpp"
#include "IPixelariumImageView.hpp"

namespace pixelarium::application
{
/// @brief A default implementation of IPixelariumImageView.
/// This is sufficient for single dimension images like png or jpg.
class PixelariumImageViewDefault : public IPixelariumImageView
{
    using Image = imaging::IPixelariumImageCvMat;

   public:
    explicit PixelariumImageViewDefault(std::shared_ptr<Image> img) : render_(*img->TryGetImage())
    {
        img_ = img;
        // this->SetInitialSize();
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
    CvMatRender render_;

   private:
    void RefreshCachedImage();
};
}  // namespace pixelarium::render
