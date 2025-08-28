#include "MyApp.hpp"

#include <cstddef>
#include <format>
#include <memory>

#include "app_resources_default.h"
#include "app_resources_local.h"
#include "imaging/PixelariumImage.hpp"
#include "imgui.h"
#include "portable-file-dialogs.h"
#include "spdlog/fmt/bundled/base.h"
#include "utilities/ILog.hpp"

using namespace pixelarium::imaging;

void pixelarium::ui::MyApp::MenuBarOptionsColumn1()
{
    ImGui::MenuItem(SHOWIMGUIDEMOS, NULL, &this->demop_);
    ImGui::MenuItem(SHOWIMAGEGALLERY, NULL, &this->image_listp_);
}

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
    if (image_listp_) this->ImageGalleryRender();
}

void pixelarium::ui::MyApp::ImageGalleryRender()
{
    static size_t selected_index{0};
    int highlight_index{-1};

    if (ImGui::BeginListBox("ListBox"))
    {
        pool_.EnumerateResources(
            [&](size_t id, size_t idx, const imaging::PixelariumImage& img) -> void
            {
                const bool is_selected = selected_index == idx;
                if (ImGui::Selectable(std::format("{}", img.Name()).c_str(), is_selected))
                {
                    selected_index = idx;
                    this->view_ = this->image_view_factory_->RenderImage(this->selected_image_);
                    this->selected_image_ = id;
                }
                if (highlight_index && ImGui::IsItemHovered()) highlight_index = idx;

                if (is_selected) ImGui::SetItemDefaultFocus();
            });

        ImGui::EndListBox();
    }

    if (ImGui::Button("Open") && this->view_)
    {
        this->view_->ToggleView(true);
    }

    if (this->view_) {
        this->view_->ShowImage();
    }

}

void pixelarium::ui::MyApp::LoadImageProt()
{
    size_t last_id{};
    auto res{pfd::open_file("Load Inputs", pfd::path::home(), {"All Files", "*"}, pfd::opt::multiselect).result()};
    for (auto& p : res)
    {
        this->logger_.Debug(std::format("{}: Creating image {}", __FUNCTION__, p));

        last_id = image_view_factory_->AddImage(std::make_unique<PixelariumImage>(p));
    }
}
