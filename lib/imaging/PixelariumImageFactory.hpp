#pragma once

#include <memory>

#include "IPixelariumImage.hpp"
namespace pixelarium::imaging
{
class PixelariumImageFactory
{
   public:
    static std::unique_ptr<IPixelariumImage> CreateImage(const std::string& uri);
};
}  // namespace pixelarium::imaging
