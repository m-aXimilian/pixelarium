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
        return pixelarium::imaging::ImageFileType::JPG;
    }
    if (lower_ext == ".png")
    {
        return pixelarium::imaging::ImageFileType::PNG;
    }
    if (lower_ext == ".czi")
    {
        return pixelarium::imaging::ImageFileType::CZI;
    }

    return pixelarium::imaging::ImageFileType::UNKNOWN;
}

class PixelariumImageFactory
{
    using Log = utils::log::ILog;
   public:
    static std::unique_ptr<IPixelariumImage> CreateImage(const std::string& uri, const Log& log);
};
}  // namespace pixelarium::imaging
