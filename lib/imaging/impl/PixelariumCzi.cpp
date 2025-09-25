#include "PixelariumCzi.hpp"

#include <filesystem>
#include <stdexcept>

#include "libCZI.h"

pixelarium::imaging::PixelariumCzi::PixelariumCzi(const std::string& uri)
{
    if (!std::filesystem::exists(uri))
    {
        throw std::runtime_error("Render file not found.");
    }

    this->is_empty_ = false;
    this->uri_ = std::filesystem::path(uri);

    auto stream = libCZI::CreateStreamFromFile(this->uri_.wstring().c_str());
    this->czi_reader_ = libCZI::CreateCZIReader();
    this->czi_reader_->Open(stream);
    this->image_statistics_ = this->czi_reader_->GetStatistics();
}

std::unique_ptr<cv::Mat> CZISubBlockToCvMat(std::shared_ptr<libCZI::IBitmapData> bitmap, libCZI::PixelType pixeltype)
{
    size_t pixel_size{0};
    int target_type;

    switch (pixeltype)
    {
        case libCZI::PixelType::Gray8:
            pixel_size = 1;
            target_type = CV_8U;
            break;
        case libCZI::PixelType::Gray16:
            pixel_size = 2;
            target_type = CV_16U;
            break;
        case libCZI::PixelType::Bgr24:
            pixel_size = 3;
            target_type = CV_8UC3;
            break;
        case libCZI::PixelType::Bgra32:
            target_type = CV_8UC4;
        case libCZI::PixelType::Gray32:
            target_type = CV_32S;
        case libCZI::PixelType::Gray32Float:
            target_type = CV_32F;
            pixel_size = 4;
            break;
        // case libCZI::PixelType::Gray64ComplexFloat:
        // case libCZI::PixelType::Gray64Float:
        //    target_type =
        //    pixel_size = 8;
        //    break;
        default:
            pixel_size = -1;
            break;
    }

    if (pixel_size < 0) return nullptr;

    size_t height{bitmap->GetHeight()};
    size_t width{bitmap->GetWidth()};
    auto fill_mat = std::make_unique<cv::Mat>(height, width, target_type);

    auto bitmap_info = bitmap->Lock();

    for (size_t h{0}; h < height; ++h)
    {
        unsigned char* source_row = ((unsigned char*)bitmap_info.ptrDataRoi) + bitmap_info.stride * h;
        unsigned char* target_row = fill_mat->ptr(h);

        for (size_t w{0}; w < width; ++w)
        {
            switch (pixel_size)
            {
                case 1:
                    target_row[w] = source_row[w];
                    break;
                case 2:
                    reinterpret_cast<unsigned short*>(target_row)[w] = reinterpret_cast<unsigned short*>(source_row)[w];
                    break;
                case 3:
                    target_row[3 * w] = source_row[3 * w];
                    target_row[3 * w + 1] = source_row[3 * w + 1];
                    target_row[3 * w + 2] = source_row[3 * w + 2];
                    break;
                default:
                    throw std::runtime_error("Unknown pixel type requested!");
                    break;
            }
        }
    }

    bitmap->Unlock();
    return fill_mat;
}

std::unique_ptr<cv::Mat> pixelarium::imaging::PixelariumCzi::TryGetImage()
{
    auto block = this->czi_reader_->ReadSubBlock(0);
    auto bitmap = block->CreateBitmap();
    auto res = CZISubBlockToCvMat(bitmap, block->GetSubBlockInfo().pixelType);

    return res;
}
