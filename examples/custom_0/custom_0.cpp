#include <opencv2/core/hal/interface.h>

#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <memory>
#include <string>

#include "DefaultApp.hpp"
#include "ILog.hpp"
#include "PixelariumLogger.hpp"
#include "PixelariumMem.hpp"
#include "imgui.h"
#include "implot.h"
#include "portable-file-dialogs.h"
#include "resource.hpp"

using namespace pixelarium;
using namespace std;
using Log = utils::log::ILog;
using Pool = resources::ImageResourcePool;

// setup a logger
#ifdef _WIN32
auto logger{utils::log::PixelariumLogger("pixellog", string(getenv("APPDATA")) + "/pixelarium/simple_app.log")};
#else
auto logger{utils::log::PixelariumLogger("pixellog", string(getenv("HOME")) + "/.cache/pixelarium/simple_app.log")};
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
            break;
        default:
            return -1;
    }

    if (chans > 1)
    {
        return CV_MAKETYPE(tp, chans);
    }

    return tp;
}

struct StatusReport
{
    const std::function<void(const std::string&)> report_status;
    const std::function<void()> reset_status;
};

class BinaryReader
{
   private:
    filesystem::path bin_file{};
    vector<std::byte> buffer{};
    uintmax_t file_size;

#pragma pack(push, 1)
    struct ParsedImage
    {
        uint8_t depth;
        uint8_t channels;
        uint16_t width;
        uint16_t height;
        void* data;
    };
#pragma pack(pop)

    auto RegisterImage(const ParsedImage& img, string& name, const StatusReport& report) -> void
    {
        if (img.width == 0 || img.height == 0 || img.channels == 0 || img.depth == 0 || !img.data)
        {
            report.reset_status();
            report.report_status(
                format("Parsing {} failed: Dimensions (w: {}, h: {}, d: {}, c: {}) cannot be parsed! The provided "
                       "bin-file is probably corrupted.",
                       name, img.width, img.height, img.depth, img.channels));
            return;
        }

        auto tmp_mat =
            cv::Mat(img.height, img.width, ToCVPixelType(img.depth, img.channels), const_cast<void*>(img.data));
        // not cloning is a dangling reference once the externally managed data pointer is freed
        auto mat{tmp_mat.clone()};

        image_pool.SetResource(make_unique<imaging::PixelariumMem>(mat, name.c_str(), logger));
    }

    auto ReadFile(const filesystem::path& file, const StatusReport& report) -> ParsedImage
    {
        uint8_t depth{};
        uint8_t channels{};
        uint16_t width{};
        uint16_t height{};
        uint64_t pixel_count{};

        if (!filesystem::exists(file)) return {};

        auto sz = filesystem::file_size(file);
        // header layout of binary file
        // | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 |
        // | ^ | ^ |     ^ |   ^   |        pixel count in byte        |
        //   |   |______ |__   |
        // pixel depth |   |  height in px
        //             |  width in px
        //       channel count
        constexpr auto header_size{14};

        bool can_read = sz >= header_size;

        if (!can_read) return {};

        if (!buffer.empty())
        {
            buffer.clear();
        }

        ifstream inp_stream(file, ios::binary);
        if (inp_stream)
        {
            inp_stream.read(reinterpret_cast<char*>(&width), sizeof(width));
            inp_stream.read(reinterpret_cast<char*>(&height), sizeof(height));
            inp_stream.read(reinterpret_cast<char*>(&depth), sizeof(depth));
            inp_stream.read(reinterpret_cast<char*>(&channels), sizeof(channels));
            inp_stream.read(reinterpret_cast<char*>(&pixel_count), sizeof(pixel_count));
            logger.Info(format("{}(): Pixel count {}", __FUNCTION__, pixel_count));

            if (pixel_count <= sz - header_size)
            {
                buffer.resize(pixel_count);
                inp_stream.read(reinterpret_cast<char*>(buffer.data()), pixel_count);
            }
        }

        logger.Info(format("{}: Parsed image with width: {}, height: {}, depth: {}, channels: {}", __PRETTY_FUNCTION__,
                           width, height, depth, channels));
        report.report_status(
            format("Parsed image with width: {}, height: {}, depth: {}, channels: {}", width, height, depth, channels));

        return {.depth = depth, .channels = channels, .width = width, .height = height, .data = buffer.data()};
    }

   public:
    auto Present(const StatusReport& report) -> void
    {
        using namespace ImGui;
        SetNextWindowSize({256, 124});
        Begin("Load Binary File");
        if (Button("Load File"))
        {
            auto res{pfd::open_file("Load Inputs", pfd::path::home(), {"Bin Files", "*.bin"}).result()};
            if (!res.empty()) bin_file = filesystem::path(res.at(0));
        }

        if (filesystem::exists(bin_file))
        {
            file_size = filesystem::file_size(bin_file);
            Text("File: %s (%ju)", bin_file.filename().c_str(), file_size);
            if (Button("Parse File"))
            {
                auto buff = ReadFile(bin_file, report);
                auto name = bin_file.filename().string();
                RegisterImage(buff, name, report);
            }
        }

        End();
    }
};

// create a custom app inheriting from the library's default app
class MyApp : public application::DefaultApp
{
   private:
    BinaryReader bin_read;
    bool plot_demop_{false};

   public:
    MyApp(const Log& log, Pool& pool) : application::DefaultApp(log, pool) {}
    ~MyApp() {}

    // override some of the defaults member functions
    void Run() override;
    void MenuBarOptionsColumn1() override;
    void MenuBarOptionsColumn2() override {};
};

int main()
{
    // some initial log message
    logger.Info(std::format("{}: Starting Application {}", __FUNCTION__, "Pixelarium"));

    // create a custom application, inject its dependencies and start it
    auto app{MyApp(logger, image_pool)};

    app.Start();
}

void MyApp::Run()
{
    this->gallery_.RenderGallery();
    this->gallery_.RenderImages();
    const auto reporter =
        StatusReport{.report_status = [this](const std::string& msg) { this->SetStatusTimed(msg, 5); },
                     .reset_status = [this]() { this->ResetStatus(); }};
    bin_read.Present(reporter);

    if (plot_demop_)
    {
        ImPlot::ShowDemoWindow(&plot_demop_);
    }
}

void MyApp::MenuBarOptionsColumn1() { ImGui::MenuItem("Show Plotdemos", NULL, &this->plot_demop_); }
