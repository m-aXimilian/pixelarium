#include <opencv2/core/hal/interface.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "DefaultApp.hpp"
#include "impl/PixelariumMem.hpp"
#include "portable-file-dialogs.h"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"
#include "utilities/SpdLogger.hpp"

using namespace pixelarium;
using namespace std;
using Log = utils::log::ILog;
using Pool = resources::ImageResourcePool;

// setup a logger
#ifdef _WIN32
unique_ptr<Log> logger{
    make_unique<utils::log::SpdLogger>(string(getenv("APPDATA")) + "/pixelarium/simple_app.log", "default")};
#else
unique_ptr<Log> logger{
    make_unique<utils::log::SpdLogger>(string(getenv("HOME")) + "/.cache/pixelarium/simple_app.log", "default")};
#endif

// instantiate an image pool for the application
resources::ImageResourcePool image_pool;

constexpr auto ToCVPixelType(size_t depth, size_t chans) -> int
{
    int tp{};
    switch (depth)
    {
        case 8:
            tp = CV_8U;
            break;
        case 16:
            tp = CV_16U;
        default:
            return -1;
    }

    if (chans > 1)
    {
        return CV_MAKETYPE(tp, chans);
    }

    return tp;
}

class BinarayReader
{
   private:
    filesystem::path bin_file{};
    vector<std::byte> buffer{};
    uintmax_t file_size;
    int width, height;

    struct ParsedImage
    {
        uint8_t depth;
        uint8_t channels;
        uint16_t width;
        uint16_t height;
        void* data;
    };

    auto RegisterImage(const ParsedImage& img, string& name) -> void
    {
        if (!img.data) return;
        auto mat = cv::Mat(img.height, img.width, ToCVPixelType(img.depth, img.channels), img.data);
        image_pool.SetResource(make_unique<imaging::PixelariumMem>(mat, name.c_str(), *logger));
    }

    auto ReadFile(const filesystem::path& file) -> ParsedImage
    {
        uint8_t depth{};
        uint8_t channels{};
        uint16_t width{};
        uint16_t height{};
        uint64_t pixel_count{};
        if (!filesystem::exists(file)) return {};

        auto sz = filesystem::file_size(file);
        if (!buffer.empty())
        {
            buffer.clear();
        }

        // buffer = static_cast<char*>(malloc(sz));
        ifstream inp_stream(file, ios::binary);
        if (inp_stream)
        {
            inp_stream.read(reinterpret_cast<char*>(&width), sizeof(width));
            inp_stream.read(reinterpret_cast<char*>(&height), sizeof(height));
            inp_stream.read(reinterpret_cast<char*>(&depth), sizeof(depth));
            inp_stream.read(reinterpret_cast<char*>(&channels), sizeof(channels));
            inp_stream.read(reinterpret_cast<char*>(&pixel_count), sizeof(pixel_count));
            logger->Info(format("{}(): Pixel count {}", __FUNCTION__, pixel_count));

            if (pixel_count < sz)
            {
                buffer.reserve(pixel_count);
                inp_stream.read(reinterpret_cast<char*>(buffer.data()), pixel_count);
            }
        }

        return {.depth = depth, .channels = channels, .width = width, .height = height, .data = buffer.data()};
    }

   public:
    auto Present() -> void
    {
        using namespace ImGui;
        Begin("Load Binary File");
        if (Button("Load File"))
        {
            auto res{pfd::open_file("Load Inputs", pfd::path::home(), {"Bin Files", "*.bin"}).result()};
            if (!res.empty()) bin_file = filesystem::path(res.at(0));
        }

        if (filesystem::exists(bin_file))
        {
            file_size = filesystem::file_size(bin_file);
            Text("File: %s (%lu)", bin_file.filename().c_str(), file_size);
            if (Button("Parse File"))
            {
                auto buff = ReadFile(bin_file);
                auto name = bin_file.filename().string();
                RegisterImage(buff, name);
            }
        }

        End();
    }
};

// create a custom app inheriting from the library's default app
class MyApp : public application::DefaultApp
{
   private:
    BinarayReader bin_read;

   public:
    MyApp(const Log& log, Pool& pool) : application::DefaultApp(log, pool) {}

    // override some of the defaults member functions
    void Run() override;
    void MenuBarOptionsColumn1() override {};
    void MenuBarOptionsColumn2() override {};
};

int main()
{
    // some initial log message
    logger->Info(std::format("{}: Starting Application {}", __FUNCTION__, "Pixelarium"));

    // create a custom application, inject its dependencies and start it
    auto app{MyApp(*logger, image_pool)};

    app.Start();
}

void MyApp::Run()
{
    this->gallery_.RenderGallery();
    this->gallery_.RenderImages();
    bin_read.Present();
}
