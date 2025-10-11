#include "DefaultApp.hpp"

#include <cstddef>
#include <format>

#include "PixelariumImageFactory.hpp"
#include "app_resources_default.h"
#include "imgui.h"
#include "portable-file-dialogs.h"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"

using namespace pixelarium::imaging;
using namespace pixelarium::application;

void DefaultApp::MenuBarOptionsColumn1()
{
    ImGui::MenuItem(SHOWIMGUIDEMOS, NULL, &this->demop_);
    ImGui::MenuItem(SHOWIMAGEGALLERY, NULL, &this->image_listp_);
}

void DefaultApp::MenuBarOptionsColumn2()
{
    if (ImGui::BeginMenu(FILEMENUNAME))
    {
        if (ImGui::MenuItem(LOADIMAGE))
        {
            this->LoadImage();
        }

        ImGui::EndMenu();
    }
}

void DefaultApp::Run()
{
    if (demop_) ImGui::ShowDemoWindow(&this->demop_);
    if (image_listp_) this->gallery_.RenderGallery();

    this->gallery_.RenderImages();
}

void DefaultApp::LoadImage()
{
    auto res{pfd::open_file("Load Inputs", pfd::path::home(), {"All Files", "*"}, pfd::opt::multiselect).result()};
    for (auto& p : res)
    {
        this->logger_.Debug(std::format("{}: Creating image {}", __FUNCTION__, p));

        try
        {
            pool_.SetResource(PixelariumImageFactory::CreateImage(p, logger_));
        }
        catch (pixelarium::resources::empty_resource_exception& e)
        {
            logger_.Error(std::format("{}: Failed to load file '{}' with '{}'", __PRETTY_FUNCTION__, p, e.what()));
        }
    }
}
