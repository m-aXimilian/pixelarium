#include "PixelariumCzi.hpp"

#include <cstdint>
#include <filesystem>
#include <format>
#include <memory>
#include <opencv2/imgcodecs.hpp>
#include <optional>
#include <stdexcept>

#include "ILog.hpp"
#include "libCZI.h"
#include "libCZI_Pixels.h"

namespace
{
bool comp_blockinfo_params(const pixelarium::imaging::CziParams& params, const libCZI::SubBlockInfo& info)
{
    bool res{true};

    info.coordinate.EnumValidDimensions(
        [&](libCZI::DimensionIndex dim, int start) -> bool
        {
            if (params.dimension_map.at(dim) == start)
            {
                return true;
            }

            res &= false;
            return true;
        });

    return res;
}

constexpr int try_get_index_match(const pixelarium::imaging::CziParams& params, libCZI::ICZIReader& reader)
{
    int index{-1};
    reader.EnumerateSubBlocks(
        [&](int idx, const libCZI::SubBlockInfo& info) -> bool
        {
            if (comp_blockinfo_params(params, info))
            {
                index = idx;
                // returning false will stop the enumeration
                return false;
            }

            return true;
        });

    return index;
}

}  // namespace

std::optional<cv::Mat> CZISubBlockToCvMat(std::shared_ptr<libCZI::IBitmapData> bitmap, libCZI::PixelType pixeltype,
                                          const pixelarium::utils::log::ILog& log)
{
    const auto cv_pixel = GetCVPixelTypeAndSize(pixeltype);

    // ToDo(MAK): fix this makeshift pixel-type-catch
    if (cv_pixel.size < 0 || cv_pixel.size > 4) return std::nullopt;

    log.Info(std::format("{}: source pixel type {}, target cv pixel type {}, pixel size {}", __PRETTY_FUNCTION__,
                         static_cast<uint8_t>(pixeltype), cv_pixel.type, cv_pixel.size));

    size_t height{bitmap->GetHeight()};
    size_t width{bitmap->GetWidth()};
    auto fill_mat = cv::Mat(height, width, cv_pixel.type);

    if (const auto bitmap_info = bitmap->Lock(); bitmap_info.ptrDataRoi)
    {
        for (size_t h{0}; h < height; ++h)
        {
            unsigned char* source_row = ((unsigned char*)bitmap_info.ptrDataRoi) + bitmap_info.stride * h;
            unsigned char* target_row = fill_mat.ptr(h);

            for (size_t w{0}; w < width; ++w)
            {
                switch (cv_pixel.size)
                {
                    case 1:
                        target_row[w] = source_row[w];
                        break;
                    case 2:
                        reinterpret_cast<unsigned short*>(target_row)[w] =
                            reinterpret_cast<unsigned short*>(source_row)[w];
                        break;
                    case 3:
                        target_row[3 * w] = source_row[3 * w];
                        target_row[3 * w + 1] = source_row[3 * w + 1];
                        target_row[3 * w + 2] = source_row[3 * w + 2];
                        break;
                    case 4:
                        reinterpret_cast<std::int32_t*>(target_row)[w] = reinterpret_cast<std::int32_t*>(source_row)[w];
                        break;
                    default:
                        throw std::runtime_error("Unknown pixel type requested!");
                        break;
                }
            }
        }
    }

    // this is outside of the previous if-clause on purpose:
    // no matter if we reach the inner scope of this clause, we locked the bitmap,
    // so here it has to be unlocked.
    bitmap->Unlock();

    return fill_mat;
}

std::optional<cv::Mat> pixelarium::imaging::PixelariumCzi::SubblockToCvMat(int index)
{
    log_.Info(std::format("{}: constructing bitmap with index {}", __PRETTY_FUNCTION__, index));
    auto block = this->czi_reader_->ReadSubBlock(index);
    auto bitmap = block->CreateBitmap();
    return CZISubBlockToCvMat(bitmap, block->GetSubBlockInfo().pixelType, log_);
}

pixelarium::imaging::PixelariumCzi::PixelariumCzi(const std::string& uri, const Log& log) : log_(log)
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

    this->image_statistics_.dimBounds.EnumValidDimensions(
        [&](libCZI::DimensionIndex dim, int start, int) -> bool
        {
            this->dimension_map_[dim] = start;
            return true;
        });
}

std::optional<cv::Mat> pixelarium::imaging::PixelariumCzi::TryGetImage() { return SubblockToCvMat(0); }

std::optional<cv::Mat> pixelarium::imaging::PixelariumCzi::TryGetImage(const IImageQuery& query)
{
    const auto czi_query = static_cast<const CziParams&>(query);
    int index = try_get_index_match(czi_query, *this->czi_reader_);

    if (index < 0)
    {
        return SubblockToCvMat(0);
    }

    return SubblockToCvMat(index);
}
