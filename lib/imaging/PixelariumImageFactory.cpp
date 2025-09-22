#include "PixelariumImageFactory.hpp"

#include <algorithm>
#include <cctype>
#include <memory>

#include "impl/PixelariumJpg.hpp"
#include "impl/PixelariumPng.hpp"

constexpr pixelarium::imaging::ImageFileType ExtensionToType(const std::string& extension)
{
    std::string lower_ext{extension};
    std::ranges::transform(extension, lower_ext.begin(),
                           [](const char c) -> char { return std::tolower(c); });

    if (lower_ext == ".jpg" || lower_ext == ".jpeg")
    {
        return pixelarium::imaging::ImageFileType::JPG;
    }
    if (lower_ext == ".png")
    {
        return pixelarium::imaging::ImageFileType::PNG;
    }

    return pixelarium::imaging::ImageFileType::UNKONWN;
}

/*static*/ std::unique_ptr<pixelarium::imaging::IPixelariumImage>
pixelarium::imaging::PixelariumImageFactory::CreateImage(const std::string& uri)
{
    const auto res{std::filesystem::path(uri)};
    const auto target_type{ExtensionToType(res.extension().string())};

    switch (target_type)
    {
        case ImageFileType::UNKONWN:
            return {};
            break;
        case ImageFileType::ABSRACT:
            return {};
            break;
        case ImageFileType::PNG:
            return std::make_unique<PixelariumPng>(uri);
            break;
        case ImageFileType::JPG:
            return std::make_unique<PixelariumJpg>(uri);
            break;
        case ImageFileType::CZI:
            return {};
            break;
    }
}
