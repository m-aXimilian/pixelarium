#pragma once

#include <unordered_map>

#include "imgui.h"
#include "libCZI_DimCoordinate.h"
#include "rendering/IPixelariumImageView.hpp"
#include "utilities/ILog.hpp"

namespace pixelarium::render
{
class PixelariumImageViewCzi : public IPixelariumImageView
{
    using Image = imaging::IPixelariumImage;
    using Render = render::CvMatRender;
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
};
}  // namespace pixelarium::render
