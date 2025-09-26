#include "DefaultApp.hpp"

#include <cstddef>
#include <format>
#include <memory>

#include "PixelariumImageFactory.hpp"
#include "app_resources_default.h"
#include "imgui.h"
#include "portable-file-dialogs.h"
#include "rendering/RenderImageManager.hpp"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"

using namespace pixelarium::imaging;

void pixelarium::ui::DefaultApp::MenuBarOptionsColumn1()
{
    ImGui::MenuItem(SHOWIMGUIDEMOS, NULL, &this->demop_);
    ImGui::MenuItem(SHOWIMAGEGALLERY, NULL, &this->image_listp_);
}

void pixelarium::ui::DefaultApp::MenuBarOptionsColumn2()
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

void pixelarium::ui::DefaultApp::Run()
{
    if (demop_) ImGui::ShowDemoWindow(&this->demop_);
    if (image_listp_) this->ImageGalleryRender();

    this->RenderImages();
}

void pixelarium::ui::DefaultApp::RenderImages()
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

void pixelarium::ui::DefaultApp::ImageGalleryRender()
{
    ImGui::SetNextWindowSize(ImVec2(300, 550));
    ImGui::Begin(SHOWIMAGEGALLERY, &this->image_listp_);

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

    if (ImGui::Button(LOADIMAGE))
    {
        this->LoadImage();
    }

    if (ImGui::Button(REMOVEIMAGE))
    {
        this->render_manager_->MarkForDeletion(this->selected_image_);
        this->pool_.DeleteResource(this->selected_image_);
    }

    ImGui::SameLine();
    if (ImGui::Button(CLEARALL))
    {
        this->render_manager_->Clear();
        this->pool_.Clear();
    }

    ImGui::End();  // end gallery window
}

void pixelarium::ui::DefaultApp::LoadImage()
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
