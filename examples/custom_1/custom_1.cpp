#include <opencv2/core/hal/interface.h>

#include <ctime>
#include <memory>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

#include "DefaultApp.hpp"
#include "IPixelariumImage.hpp"
#include "imgui.h"
#include "impl/PixelariumMem.hpp"
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

class Selector
{
    using Pool = resources::ImageResourcePool;
    Pool& pool_;
    std::string preview_0_{"None"};
    std::string preview_1_{"None"};
    resources::ResourceKey selected_key_0;
    resources::ResourceKey selected_key_1;
    int idx_{0};

   public:
    Selector(resources::ImageResourcePool& pool) : pool_(pool) {}
    void SelectImage()
    {
        ImGui::Begin("Image Multiply");
        static int selected_idx_0{0};
        static int selected_idx_1{0};
        if (ImGui::BeginCombo("Select first image", preview_0_.c_str()))
        {
            pool_.Enumerate(
                [&](resources::ResourceKey key, size_t idx, const imaging::IPixelariumImage<cv::Mat>& img) -> void
                {
                    const bool is_selected = static_cast<int>(idx) == selected_idx_0;
                    if (ImGui::Selectable(img.Name().c_str(), is_selected))
                    {
                        selected_idx_0 = idx;
                        preview_0_ = img.Name();
                        selected_key_0 = key;
                    }

                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                });

            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo("Select second image", preview_1_.c_str()))
        {
            pool_.Enumerate(
                [&](resources::ResourceKey key, size_t idx, const imaging::IPixelariumImage<cv::Mat>& img) -> void
                {
                    const bool is_selected = static_cast<int>(idx) == selected_idx_1;
                    if (ImGui::Selectable(img.Name().c_str(), is_selected))
                    {
                        selected_idx_1 = idx;
                        preview_1_ = img.Name();
                        selected_key_1 = key;
                    }

                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                });

            ImGui::EndCombo();
        }

        if (ImGui::Button("Process"))
        {
            auto img0 = pool_.GetResource(selected_key_0);
            auto img_mat0 = img0.lock()->TryGetImage();
            auto img1 = pool_.GetResource(selected_key_1);
            auto img_mat1 = img1.lock()->TryGetImage();

            if (!img_mat0.has_value() || !img_mat1.has_value() || img_mat0.value().empty() || img_mat1.value().empty()) return;

            if (img_mat0->size != img_mat1->size) return;

            cv::multiply(img_mat0.value(), img_mat1.value(), img_mat0.value());

            std::string name{std::format("Multiply_{}", idx_)};
            pool_.SetResource(std::make_unique<imaging::PixelariumMem>(*img_mat0, name, *logger));

            ++idx_;
        }

        ImGui::End();
    }
};

// create a custom app inheriting from the library's default app
class MyApp : public application::DefaultApp
{
    Selector select_;

   public:
    MyApp(const Log& log, Pool& pool) : application::DefaultApp(log, pool), select_(pool) {}

    // override some of the defaults member functions
    void Run() override;
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
    application::DefaultApp::Run();
    select_.SelectImage();
}
