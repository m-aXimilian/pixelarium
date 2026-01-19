#include "IPixelariumImage.hpp"
#include <opencv2/imgcodecs.hpp>

bool pixelarium::imaging::IPixelariumImageCvMat::SaveImage(const std::string& uri)
{
    auto current_image = this->TryGetImage();

    if (!current_image.has_value())
    {
        return false;
    }

    return cv::imwrite(uri, current_image.value());
}
