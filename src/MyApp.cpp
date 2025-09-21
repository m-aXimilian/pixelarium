#include "MyApp.hpp"

#include <cstddef>
#include <format>
#include <memory>

#include "PixelariumImageFactory.hpp"
#include "app_resources_default.h"
#include "app_resources_local.h"
#include "imgui.h"
#include "portable-file-dialogs.h"
#include "rendering/RenderImageManager.hpp"
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

    this->RenderImages();
}

void pixelarium::ui::MyApp::RenderImages()
{
    this->render_manager_->Enumerate(
        [&](resources::ResourceKey key, render::RenderImageStateWrapper& render_state)
        {
            render_state.view->ShowImage();

            if (!*render_state.view->GetStatus())
            {
                this->render_manager_->MarkForDeletion(key);
            }
        });
}

void pixelarium::ui::MyApp::ImageGalleryRender()
{
    ImGui::SetNextWindowSize(ImVec2(300, 500));
    ImGui::Begin(SHOWIMAGEGALLERY);

    // this updates the render collection
    // essentially deleting render views that were
    // marked for deletion
    this->render_manager_->UpdateCollection();

    static size_t selected_index{0};
    int highlight_index{-1};

    if (ImGui::BeginListBox("Image List", ImVec2(200, 400)))
    {
        pool_.EnumerateResources(
            [&](size_t id, size_t idx, const imaging::IPixelariumImage& img) -> void
            {
                const bool is_selected = selected_index == idx;
                if (ImGui::Selectable(std::format("{}", img.Name()).c_str(), is_selected))
                {
                    selected_index = idx;
                    this->selected_image_ = id;
                }
                if (highlight_index && ImGui::IsItemHovered()) highlight_index = idx;

                if (is_selected) ImGui::SetItemDefaultFocus();
            });

        ImGui::EndListBox();
    }

    ImGui::Checkbox(AUTOSHOWSELECTED, &this->auto_show_selectd_image_);
    ImGui::SameLine();  // put the button next to the checkbox
    // note that the button will only show when the checkbox is toggled off
    // this is intended behavior as the selected image will render automatically
    // when the checkbox is toggled on
    if (this->auto_show_selectd_image_ || ImGui::Button(OPENIMAGE))
    {
        // Try add the selected index to the manager
        this->render_manager_->Add(this->selected_image_);
    }

    ImGui::End();  // end gallery window
}

void pixelarium::ui::MyApp::LoadImageProt()
{
    size_t last_id{};
    auto res{pfd::open_file("Load Inputs", pfd::path::home(), {"All Files", "*"}, pfd::opt::multiselect).result()};
    for (auto& p : res)
    {
        this->logger_.Debug(std::format("{}: Creating image {}", __FUNCTION__, p));

        pool_.SetResource(PixelariumImageFactory::CreateImage(p));
    }
}
