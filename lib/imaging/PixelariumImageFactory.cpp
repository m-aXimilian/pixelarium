#include "PixelariumImageFactory.hpp"

#include <cctype>
#include <memory>

#include "imaging/IPixelariumImage.hpp"
#include "imaging/impl/PixelariumMem.hpp"
#include "impl/PixelariumCzi.hpp"
#include "impl/PixelariumJpg.hpp"
#include "impl/PixelariumPng.hpp"
#include "impl/PixelariumTiff.hpp"

/*static*/ std::unique_ptr<pixelarium::imaging::IPixelariumImageCvMat>
pixelarium::imaging::PixelariumImageFactory::CreateImage(const std::string& uri, const Log& log)
{
    const auto res{std::filesystem::path(uri)};
    const auto target_type{ExtensionToType(res.extension().string())};

    switch (target_type)
    {
        case ImageFileType::kUnknown:
            return {};
        case ImageFileType::kAbstract:
            return {};
        case ImageFileType::kPng:
            return std::make_unique<PixelariumPng>(uri);
        case ImageFileType::kJpg:
            return std::make_unique<PixelariumJpg>(uri);
        case ImageFileType::kCzi:
            return std::make_unique<PixelariumCzi>(uri, log);
        case ImageFileType::kTiff:
            return std::make_unique<PixelariumTiff>(uri, log);
        case ImageFileType::kMemory:
            return std::make_unique<PixelariumMem>(cv::Mat(), uri, log);
        default:
            return {};
    }
}
