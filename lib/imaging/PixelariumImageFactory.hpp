#pragma once

#include <memory>

#include "IPixelariumImage.hpp"
#include "utilities/ILog.hpp"
namespace pixelarium::imaging
{
constexpr pixelarium::imaging::ImageFileType ExtensionToType(const std::string& extension)
{
    std::string lower_ext{extension};
    std::ranges::transform(extension, lower_ext.begin(), [](const char c) -> char { return std::tolower(c); });

    if (lower_ext == ".jpg" || lower_ext == ".jpeg")
    {
        return pixelarium::imaging::ImageFileType::kJpg;
    }
    if (lower_ext == ".png")
    {
        return pixelarium::imaging::ImageFileType::kPng;
    }
    if (lower_ext == ".czi")
    {
        return pixelarium::imaging::ImageFileType::kCzi;
    }
    if (lower_ext == ".tiff" || lower_ext == ".tif")
    {
        return pixelarium::imaging::ImageFileType::kTiff;
    }

    return pixelarium::imaging::ImageFileType::kUnknown;
}

/// @brief Factory for instantiating implementations of IPixelariumImage based on the given file type.
class PixelariumImageFactory
{
    using Log = utils::log::ILog;

   public:
    static std::unique_ptr<IPixelariumImage> CreateImage(const std::string& uri, const Log& log);
};
}  // namespace pixelarium::imaging
