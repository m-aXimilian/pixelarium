#include "ImageViewFactory.hpp"

#include <format>
#include <memory>

#include "IPixelariumImageView.hpp"
#include "PixelariumImageViewCzi.hpp"
#include "PixelariumImageViewDefault.hpp"
#include "IPixelariumImage.hpp"
#include "PixelariumImageFactory.hpp"

/// @brief Creates a PixelariumImageView from a resource image.
/// @param image_id The ID of the image resource to render.
/// @return A unique pointer to the PixelariumImageView, or nullptr if the image resource is not found or is empty.  The
/// image data is copied.
std::unique_ptr<pixelarium::application::IPixelariumImageView> pixelarium::application::ImageViewFactory::RenderImage(
    resources::ResourceKey image_id)
{
    using ImageType = imaging::ImageFileType;
    auto res{this->image_pool_.GetResource(image_id)};

    const auto img{res.lock()};

    if (img == nullptr)
    {
        return {};
    }

    if (img->Empty())
    {
        return {};
    }

    auto type = imaging::ExtensionToType(img->Uri().extension().string());
    if (img->Uri().empty())
    {
        log_.Info(std::format("{}: empty Uri for {}.", __PRETTY_FUNCTION__, img->Name()));
        type = ImageType::kMemory;
    }

    switch (type)
    {
        case ImageType::kUnknown:
        case ImageType::kAbstract:
        case ImageType::kPng:
        case ImageType::kJpg:
        case ImageType::kTiff:
        case ImageType::kMemory:
            log_.Info(std::format("{}: Creating a Default View", __PRETTY_FUNCTION__));
            // beware: here we copy the actual image resource over to the new image
            try
            {
                auto view{std::make_unique<PixelariumImageViewDefault>(img)};
                return view;
            }
            catch (const std::exception& ex)
            {
                log_.Error(std::format("{}: Creating view failed: {}", __PRETTY_FUNCTION__, ex.what()));
            }
        case ImageType::kCzi:
            log_.Info(std::format("{}: Creating a CZI View", __PRETTY_FUNCTION__));
            try
            {
                auto view{std::make_unique<PixelariumImageViewCzi>(img, log_)};
                return view;
            }
            catch (const std::exception& ex)
            {
                log_.Error(std::format("{}: Creating view failed: {}", __PRETTY_FUNCTION__, ex.what()));
            }
        default:
            return {};
    }
}
