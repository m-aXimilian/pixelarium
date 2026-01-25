#pragma once

#include <memory>
#include <vector>

#include "CvMatRender.hpp"
#include "IPixelariumImageView.hpp"
#include "imaging/IPixelariumImage.hpp"
#include "imgui.h"

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
    }

    PixelariumImageViewDefault() = delete;
    PixelariumImageViewDefault(PixelariumImageViewDefault&) = delete;
    PixelariumImageViewDefault(const PixelariumImageViewDefault&) = delete;
    PixelariumImageViewDefault(PixelariumImageViewDefault&&) = delete;
    PixelariumImageViewDefault& operator=(PixelariumImageViewDefault&) = delete;
    PixelariumImageViewDefault& operator=(PixelariumImageViewDefault&&) = delete;

    void ShowImage() override;

    void ImageViewMenuBarAdditions() override;

    void GenerateHistogram();

   private:
    ImVec2 curr_dim_{};
    CvMatRender render_;
    bool show_hists_{false};
    bool hist_available_{false};
    std::vector<cv::Mat> bgr_planes_;
    std::vector<cv::Mat> hist_planes_;

   private:
    void RefreshCachedImage();
};
}  // namespace pixelarium::application
