#include "MyApp.hpp"

#include <format>
#include <memory>

#include "imaging/PixelariumImage.hpp"
#include "imgui.h"
#include "portable-file-dialogs.h"
#include "uiresources_app.h"
#include "uiresources_local.h"
#include "utilities/ILog.hpp"

using namespace pixelarium::imaging;

void pixelarium::ui::MyApp::MenuBarOptionsColumn1() { ImGui::MenuItem(SHOWIMGUIDEMOS, NULL, &this->demop_); }

void pixelarium::ui::MyApp::MenuBarOptionsColumn2()
{
    if (ImGui::BeginMenu(FILEMENUNAME))
    {
        if (ImGui::MenuItem("Load File"))
        {
            this->LoadImageProt();
        }

        ImGui::EndMenu();
    }
}

void pixelarium::ui::MyApp::Run()
{
    if (demop_) ImGui::ShowDemoWindow(&this->demop_);

    // if (this->image_view_)
    // {
    //     this->image_view_->ShowImage();
    // }

    if (ImGui::BeginListBox("ListBox"))
    {
        pool_.EnumerateResources([](size_t id, const imaging::PixelariumImage&) -> void
                                 { ImGui::Selectable(std::format("Image {}", id).c_str()); });

        ImGui::EndListBox();
    }
}

void pixelarium::ui::MyApp::LoadImageProt()
{
    size_t last_id{};
    auto res{pfd::open_file("Load Inputs", pfd::path::home(), {"All Files", "*"}, pfd::opt::multiselect).result()};
    for (auto& p : res)
    {
        this->logger_.Debug(std::format("{}: Creating image {}", __FUNCTION__, p));

        last_id = image_view_model_->AddImage(std::make_unique<PixelariumImage>(p));
    }
}
