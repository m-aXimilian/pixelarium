#pragma once

#include <memory>
#include <unordered_map>

#include "imaging/IPixelariumImage.hpp"
#include "imgui.h"
#include "libCZI_DimCoordinate.h"
#include "CvMatRender.hpp"
#include "IPixelariumImageView.hpp"
#include "utilities/ILog.hpp"

namespace pixelarium::application
{
/// @brief A CZI-specific implementation of IPixelariumImageView.
class PixelariumImageViewCzi : public IPixelariumImageView
{
    using Image = imaging::IPixelariumImageCvMat;
    using Log = utils::log::ILog;

   public:
    explicit PixelariumImageViewCzi(std::shared_ptr<Image> img, const Log& log);
    PixelariumImageViewCzi() = delete;
    PixelariumImageViewCzi(PixelariumImageViewCzi&) = delete;
    PixelariumImageViewCzi(const PixelariumImageViewCzi&) = delete;
    PixelariumImageViewCzi(PixelariumImageViewCzi&&) = delete;
    PixelariumImageViewCzi& operator=(PixelariumImageViewCzi&) = delete;
    PixelariumImageViewCzi& operator=(PixelariumImageViewCzi&&) = delete;

    void ShowImage() override;

   private:
    ImVec2 curr_dim_{};
    const Log& log_;
    std::unordered_map<libCZI::DimensionIndex, int> dimension_map_;
    std::unique_ptr<CvMatRender> render_;

   private:
        void RefreshCachedImage();
};
}  // namespace pixelarium::render
